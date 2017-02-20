#include "GlDebug.h"

#include "GlLibPrerequisites.h"

#include <iomanip>
#include <algorithm>

#if defined( PLATFORM_DESKTOP )
#	if defined( PLATFORM_WINDOWS )
#		include <Windows.h>
#	elif defined( PLATFORM_LINUX )
#		include <X11/Xlib.h>
#		include <GL/glx.h>
#	elif defined( PLATFORM_APPLE )
#		include <mach-o/dyld.h>
#	else
#		error "Please implement for this platform."
#	endif
#endif

#ifndef GL_INVALID_FRAMEBUFFER_OPERATION
#	define GL_INVALID_FRAMEBUFFER_OPERATION 0x506
#endif

namespace gl
{
	//*************************************************************************

#if defined( PLATFORM_DESKTOP )
#	define MAKE_GL_EXTENSION( x )	static const std::string x = "GL_"#x

	namespace
	{
		MAKE_GL_EXTENSION( AMD_debug_output );
		MAKE_GL_EXTENSION( ARB_debug_output );
		MAKE_GL_EXTENSION( KHR_debug );
		
		std::set< std::string > doGetExtensionsList()
		{
			std::set< std::string > ret;
			auto extensions = reinterpret_cast< char const * >
				( glGetString( GL_EXTENSIONS ) );

			if ( extensions )
			{
				std::string exts{ extensions };
				std::stringstream stream{ exts };
				std::string extension;
				auto count = std::count( exts.begin(), exts.end(), ' ' );

				for ( auto i = 0; i <= count; ++i )
				{
					stream >> extension;

					if ( !extension.empty() )
					{
						ret.insert( extension );
					}
				}
			}

			return ret;
		}

		bool hasExtension( std::string const & name )
		{
			static auto const extensions = doGetExtensionsList();
			return extensions.find( name ) != extensions.end();
		}

		template< typename Func >
		bool getFunction( std::string const & name, Func & func )
		{
#if defined( PLATFORM_WINDOWS )
			func = reinterpret_cast< Func >( wglGetProcAddress
				( name.c_str() ) );
#elif defined( PLATFORM_LINUX )
			func = reinterpret_cast< Func >( glXGetProcAddressARB
				( reinterpret_cast< GLubyte const * >( name.c_str() ) ) );
#elif defined( PLATFORM_APPLE )
			auto const symbolName = "_" + name;
			NSSymbol symbol = nullptr;

			if ( NSIsSymbolNameDefined( symbolName.c_str() ) )
			{
				symbol = NSLookupAndBindSymbol( symbolName.c_str() );
			}

			func = reinterpret_cast< Func >( NSAddressOfSymbol( symbol ) );
#else
#	error "Please implement for this platform."
#endif
			return func != nullptr;
		}

		template< typename Ret, typename ... Arguments >
		bool getFunction( std::string const & name
			, std::function< Ret( Arguments... ) > & func )
		{
			typedef Ret( CALLBACK * PFNType )( Arguments... );
			PFNType result = nullptr;

			if ( getFunction( name, result ) )
			{
				func = result;
			}

			return result != nullptr;
		}

		template< typename T >
		inline void getFunction( T & function
			, std::string const & name
			, std::string const & extension )
		{
			if ( !getFunction( name, function ) )
			{
				if ( !getFunction( name + extension, function ) )
				{
					std::cerr << "Unable to retrieve function "
						<< name << std::endl;
				}
			}
		}
	}
#endif
	//*************************************************************************

	bool Debug::m_hasExtension = false;

	Debug::Debug()
	{
	}

	Debug::~Debug()
	{
	}

	void Debug::initialise()
	{
#if defined( PLATFORM_DESKTOP )
		using PfnGlDebugProc = void( CALLBACK * )( uint32_t
			, uint32_t
			, uint32_t
			, uint32_t
			, int
			, const char *
			, void * );
		using PfnGlDebugAmdProc = void( CALLBACK * )( uint32_t
			, uint32_t
			, uint32_t
			, int
			, const char *
			, void * );
		using GlDebugFunc = std::function< void( PfnGlDebugProc
			, void * ) >;
		using GlDebugAmdFunc = std::function< void( PfnGlDebugAmdProc
			, void * ) >;
		GlDebugFunc pfnDebugMessageCallback;
		GlDebugAmdFunc pfnDebugMessageCallbackAMD;

		if ( hasExtension( KHR_debug ) )
		{
			getFunction( pfnDebugMessageCallback
				, "glDebugMessageCallback"
				, "KHR" );
		}
		else if ( hasExtension( ARB_debug_output ) )
		{
			getFunction( pfnDebugMessageCallback
				, "glDebugMessageCallback"
				, "ARB" );
		}
		else if ( hasExtension( AMD_debug_output ) )
		{
			getFunction( pfnDebugMessageCallbackAMD
				, "glDebugMessageCallbackAMD"
				, "" );
		}

		if ( pfnDebugMessageCallback )
		{
			pfnDebugMessageCallback
				( PfnGlDebugProc( &Debug::debugLog )
				, this );
		}

		if ( pfnDebugMessageCallbackAMD )
		{
			pfnDebugMessageCallbackAMD
				( PfnGlDebugAmdProc( &Debug::debugLogAMD )
				, this );
		}

		if ( pfnDebugMessageCallback || pfnDebugMessageCallbackAMD )
		{
			m_hasExtension = true;

#if defined( GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB )
			glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB );
#endif
		}

		filterMessage( 0x00020071 );
		filterMessage( 0x00020072 );
		// TODO: Investigate on why I get this noisy message.
		filterMessage( 0x00020096 );
#endif
	}

	bool Debug::checkError( std::string const & file
		, uint32_t line
		, std::string const & text )
	{
		bool hasError = false;

		if ( !m_hasExtension )
		{
			static std::string const Errors[]
			{
				"Invalid Enum",
				"Invalid Value",
				"Invalid Operation",
				"Stack Overflow",
				"Stack Underflow",
				"Out of memory",
				"Invalid frame buffer operation",
			};

			uint32_t error = GL_NO_ERROR;
			std::string errors;

			do
			{
				error = glGetError();

				if ( error != GL_NO_ERROR )
				{
					hasError = true;
					errors += " " + Errors[error - GL_INVALID_ENUM];
				}
			}
			while ( error != 0u );

			if ( hasError )
			{
				std::cerr << "OpenGL error(s) at file: " << file << ":" << line << ", function: "
					<< text << " -" << errors << std::endl;
			}
		}

		return !hasError;
	}

	void Debug::filterMessage( uint32_t message )
	{
		m_filteredOut.insert( message );
	}

	bool Debug::filtered( uint32_t message )const
	{
		return m_filteredOut.find( message ) != m_filteredOut.end();
	}

	void Debug::debugLog( uint32_t source
		, uint32_t type
		, uint32_t id
		, uint32_t severity
		, int length
		, const char * message
		, void * userParam )
	{
		reinterpret_cast< Debug * >( userParam )->doDebugLog( source
			, type
			, id
			, severity
			, length
			, message );
	}

	void Debug::debugLogAMD( uint32_t id
		, uint32_t category
		, uint32_t severity
		, int length
		, const char * message
		, void * userParam )
	{
		reinterpret_cast< Debug * >( userParam )->doDebugLogAMD( id
			, category
			, severity
			, length
			, message );
	}

	void Debug::doDebugLog( uint32_t source
		, uint32_t type
		, uint32_t id
		, uint32_t severity
		, int length
		, const char * message )const
	{
#ifndef GL_DEBUG_SOURCE_API
		static constexpr uint32_t GL_DEBUG_SOURCE_API = 0x8246;
		static constexpr uint32_t GL_DEBUG_SOURCE_WINDOW_SYSTEM = 0x8247;
		static constexpr uint32_t GL_DEBUG_SOURCE_SHADER_COMPILER = 0x8248;
		static constexpr uint32_t GL_DEBUG_SOURCE_THIRD_PARTY = 0x8249;
		static constexpr uint32_t GL_DEBUG_SOURCE_APPLICATION = 0x824A;
		static constexpr uint32_t GL_DEBUG_SOURCE_OTHER = 0x824B;
#endif

		static std::map< uint32_t, std::string > SourceName
		{
			{ GL_DEBUG_SOURCE_API, "OpenGl" },
			{ GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System" },
			{ GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader compiler" },
			{ GL_DEBUG_SOURCE_THIRD_PARTY, "Third party" },
			{ GL_DEBUG_SOURCE_APPLICATION, "Application" },
			{ GL_DEBUG_SOURCE_OTHER, "Other" },
		};

#ifndef GL_DEBUG_TYPE_ERROR
		static constexpr uint32_t GL_DEBUG_TYPE_ERROR = 0x824C;
		static constexpr uint32_t GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR = 0x824D;
		static constexpr uint32_t GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR = 0x824E;
		static constexpr uint32_t GL_DEBUG_TYPE_PORTABILITY = 0x824F;
		static constexpr uint32_t GL_DEBUG_TYPE_PERFORMANCE = 0x8250;
		static constexpr uint32_t GL_DEBUG_TYPE_OTHER = 0x8251;
		static constexpr uint32_t GL_DEBUG_TYPE_MARKER = 0x8268;
		static constexpr uint32_t GL_DEBUG_TYPE_PUSH_GROUP = 0x8269;
		static constexpr uint32_t GL_DEBUG_TYPE_POP_GROUP = 0x826A;
#endif

		static std::map< uint32_t, std::string > TypeName
		{
			{ GL_DEBUG_TYPE_ERROR, "Error" },
			{ GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated behavior" },
			{ GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined behavior" },
			{ GL_DEBUG_TYPE_PORTABILITY, "Portability" },
			{ GL_DEBUG_TYPE_PERFORMANCE, "Performance" },
			{ GL_DEBUG_TYPE_OTHER, "Other" },
			{ GL_DEBUG_TYPE_MARKER, "Marker" },
			{ GL_DEBUG_TYPE_PUSH_GROUP, "Push Group" },
			{ GL_DEBUG_TYPE_POP_GROUP, "Pop Group" },
		};

#ifndef GL_DEBUG_SEVERITY_HIGH
		static constexpr uint32_t GL_DEBUG_SEVERITY_HIGH = 0x9146;
		static constexpr uint32_t GL_DEBUG_SEVERITY_MEDIUM = 0x9147;
		static constexpr uint32_t GL_DEBUG_SEVERITY_LOW = 0x9148;
		static constexpr uint32_t GL_DEBUG_SEVERITY_NOTIFICATION = 0x826B;
#endif

		static std::map< uint32_t, std::string > SeverityName
		{
			{ GL_DEBUG_SEVERITY_HIGH, "High" },
			{ GL_DEBUG_SEVERITY_MEDIUM, "Medium" },
			{ GL_DEBUG_SEVERITY_LOW, "Low" },
			{ GL_DEBUG_SEVERITY_NOTIFICATION, "Notification" },
		};

		if ( !filtered( id ) )
		{
			std::stringstream toLog;
			toLog << "OpenGl Debug\n ";
			toLog << "  Source: " << SourceName[source] << "\n";
			toLog << "  Type: " << TypeName[type] << "\n";
			toLog << "  Severity: " << SeverityName[severity] << "\n";
			toLog << "  ID: 0x" << std::hex << std::setfill( '0' )
				<< std::setw( 8 ) << id << "\n";
			toLog << "  Message: " << message;

			switch ( severity )
			{
			case GL_DEBUG_SEVERITY_HIGH:
				std::cerr << toLog.str() << std::endl;
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
				std::cerr << toLog.str() << std::endl;
				break;

			case GL_DEBUG_SEVERITY_LOW:
				std::cout << toLog.str() << std::endl;
				break;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				std::clog << toLog.str() << std::endl;
				break;

			default:
				std::cerr << toLog.str() << std::endl;
				break;
			}
		}
	}

	void Debug::doDebugLogAMD( uint32_t id
		, uint32_t category
		, uint32_t severity
		, int length
		, const char * message )const
	{
#ifndef GL_DEBUG_CATEGORY_API_ERROR_AMD
		static constexpr uint32_t GL_DEBUG_CATEGORY_API_ERROR_AMD = 0x9149;
		static constexpr uint32_t GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD = 0x914A;
		static constexpr uint32_t GL_DEBUG_CATEGORY_DEPRECATION_AMD = 0x914B;
		static constexpr uint32_t GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD = 0x914C;
		static constexpr uint32_t GL_DEBUG_CATEGORY_PERFORMANCE_AMD = 0x914D;
		static constexpr uint32_t GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD = 0x914E;
		static constexpr uint32_t GL_DEBUG_CATEGORY_APPLICATION_AMD = 0x914F;
		static constexpr uint32_t GL_DEBUG_CATEGORY_OTHER_AMD = 0x9150;
#endif

		static std::map< uint32_t, std::string > CategoryName
		{
			{ GL_DEBUG_CATEGORY_API_ERROR_AMD, "OpenGl" },
			{ GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD, "Window System" },
			{ GL_DEBUG_CATEGORY_DEPRECATION_AMD, "Deprecated Behavior" },
			{ GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD, "Undefined Behavior" },
			{ GL_DEBUG_CATEGORY_PERFORMANCE_AMD, "Performance" },
			{ GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD, "Shader Compiler" },
			{ GL_DEBUG_CATEGORY_APPLICATION_AMD, "Application" },
			{ GL_DEBUG_CATEGORY_OTHER_AMD, "Other" }
		};

#ifndef GL_DEBUG_SEVERITY_HIGH_AMD
		static constexpr uint32_t GL_DEBUG_SEVERITY_HIGH_AMD = 0x9146;
		static constexpr uint32_t GL_DEBUG_SEVERITY_MEDIUM_AMD = 0x9147;
		static constexpr uint32_t GL_DEBUG_SEVERITY_LOW_AMD = 0x9148;
#endif
		static constexpr uint32_t GL_DEBUG_SEVERITY_NOTIFICATION_AMD = 0x826B;

		static std::map< uint32_t, std::string > SeverityName
		{
			{ GL_DEBUG_SEVERITY_HIGH_AMD, "High" },
			{ GL_DEBUG_SEVERITY_MEDIUM_AMD, "Medium" },
			{ GL_DEBUG_SEVERITY_LOW_AMD, "Low" },
			{ GL_DEBUG_SEVERITY_NOTIFICATION_AMD, "Notification" }
		};

		std::stringstream toLog;
		toLog << "OpenGl Debug\n ";
		toLog << "  Category: " << CategoryName[category] << "\n";
		toLog << "  Severity: " << SeverityName[severity] << "\n";
		toLog << "  ID: 0x" << std::hex << std::setfill( '0' )
			<< std::setw( 8 ) << id << "\n";
		toLog << "  Message: " << message;

		switch ( severity )
		{
		case GL_DEBUG_SEVERITY_HIGH_AMD:
			std::cerr << toLog.str() << std::endl;
			break;

		case GL_DEBUG_SEVERITY_MEDIUM_AMD:
			std::cerr << toLog.str() << std::endl;
			break;

		case GL_DEBUG_SEVERITY_LOW_AMD:
			std::cout << toLog.str() << std::endl;
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION_AMD:
			std::clog << toLog.str() << std::endl;
			break;

		default:
			std::cerr << toLog.str() << std::endl;
			break;
		}
	}
}
