#include "GlShaderProgram.h"

#include "OpenGL.h"

#include <regex>

namespace gl
{
	namespace
	{
		inline uint32_t doCreateShader( uint32_t type
			, std::string const & source )noexcept
		{
			auto ret = glCheckError( glCreateShader, type );
			int size = int( source.size() );
			char const * shader = source.c_str();
			glCheckError( glShaderSource, ret, 1, &shader, &size );
			glCheckError( glCompileShader, ret );

			GLint status = 0;
			glCheckError( glGetShaderiv
				, ret
				, GL_COMPILE_STATUS
				, &status );

			if ( status == 0 )
			{
				GLint length = 0;
				glCheckError( glGetShaderiv
					, ret
					, GL_INFO_LOG_LENGTH
					, &length );

				if ( length > 0 )
				{
					std::vector< char > log( length + 1, '\0' );
					glCheckError( glGetShaderInfoLog
						, ret
						, length + 1
						, nullptr
						, log.data() );
					std::string err = log.data();
					std::cerr << err << std::endl;
					std::cerr << "In shader:" << std::endl;
					std::cerr << source << std::endl;
				}

				glCheckError( glDeleteShader, ret );
				ret = GL_INVALID_INDEX;
			}

			return ret;
		}

		void doLinkProgram( uint32_t & program, uint32_t vtx, uint32_t pxl )
		{
			glCheckError( glAttachShader, program, vtx );
			glCheckError( glAttachShader, program, pxl );
			glCheckError( glLinkProgram, program );

			GLint status = 0;
			glCheckError( glGetProgramiv
				, program
				, GL_LINK_STATUS
				, &status );

			if ( status == 0 )
			{
				GLint length = 0;
				glCheckError( glGetProgramiv
					, program
					, GL_INFO_LOG_LENGTH
					, &length );

				if ( length > 0 )
				{
					std::vector< char > log( length + 1, '\0' );
					glCheckError( glGetProgramInfoLog
						, program
						, length + 1
						, &length
						, log.data() );
					std::string err = log.data();
					std::cerr << err << std::endl;
				}

				glCheckError( glDeleteProgram, program );
				program = GL_INVALID_INDEX;
			}

			glCheckError( glDeleteShader, vtx );
			glCheckError( glDeleteShader, pxl );
		}

		template< typename KeyT, typename ValueT >
		ValueT doGetValue( std::map< KeyT, ValueT > const & map
			, std::string const & key )
		{
			ValueT result{};
			auto it = map.find( key );

			if ( it != map.end() )
			{
				result = it->second;
			}

			return result;
		}

		AttributeFullType doGetType( std::string const & text )
		{
			static std::map< std::string, AttributeFullType > const types
			{
				{ "int", AttributeFullType::eInt },
				{ "float", AttributeFullType::eFloat },
				{ "vec2", AttributeFullType::eVec2 },
				{ "vec3", AttributeFullType::eVec3 },
				{ "vec4", AttributeFullType::eVec4 },
				{ "ivec2", AttributeFullType::eIVec2 },
				{ "ivec3", AttributeFullType::eIVec3 },
				{ "ivec4", AttributeFullType::eIVec4 },
			};
			return doGetValue( types, text );
		}

		gl::AttributeInfoArray doRetrieveAttributes( std::string const & src )
		{
			gl::AttributeInfoArray result;
			std::regex regex( "(in|attribute) ([^ ]*) ([^;]*);" );
			auto begin = std::begin( src );
			auto end = std::end( src );
			const std::sregex_iterator endit;

			for ( std::sregex_iterator it( begin, end, regex ); it != endit; ++it )
			{
				std::smatch match = *it;
				auto type = match[2].str();
				auto name = match[3].str();
				result.push_back( { name, doGetType( type ) } );
			}

			return result;
		}
	}

	ShaderProgram::ShaderProgram( std::string const & vtx
		, std::string const & pxl )noexcept
		: m_vertex{ vtx }
		, m_pixel{ pxl }
	{
		auto vertex = doCreateShader( GL_VERTEX_SHADER, vtx );
		auto pixel = doCreateShader( GL_FRAGMENT_SHADER, pxl );
		assert( vertex != GL_INVALID_INDEX );
		assert( pixel != GL_INVALID_INDEX );
		m_name = glCheckError( glCreateProgram );
		doLinkProgram( m_name, vertex, pixel );

		if ( m_name != GL_INVALID_INDEX )
		{
			auto attributes = doRetrieveAttributes( vtx );

			for ( auto & attribute : attributes )
			{
				doAddAttributeInfos( attribute );
			}
		}
	}

	ShaderProgram::~ShaderProgram()noexcept
	{
		glCheckError( glDeleteProgram, m_name );
	}

	void ShaderProgram::bind()const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glUseProgram, m_name );
	}

	void ShaderProgram::unbind()const noexcept
	{
		glCheckError( glUseProgram, 0u );
	}

	uint32_t ShaderProgram::uniformLocation
		( std::string const & name )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		return uint32_t( glGetUniformLocation( m_name, name.c_str() ) );
	}

	uint32_t ShaderProgram::uniformBlockIndex
		( std::string const & name )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		return uint32_t( glGetUniformBlockIndex( m_name, name.c_str() ) );
	}

	UInt32Array ShaderProgram::uniformIndices
		( StringArray const & names )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		std::vector< const char * > chars;

		for ( auto name : names )
		{
			char * szChar = new char[name.size() + 1];
			szChar[name.size()] = 0;
			strncpy( szChar, name.c_str(), name.size() );
			chars.push_back( szChar );
		}

		UInt32Array ret( chars.size() );
		glCheckError( glGetUniformIndices
			, m_name
			, int( chars.size() )
			, chars.data()
			, ret.data() );

		for ( auto name : chars )
		{
			delete[] name;
		}

		return ret;
	}

	int ShaderProgram::activeUniforms( uint32_t index
		, uint32_t name )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		int value{ 0 };
		glCheckError( glGetActiveUniformsiv, m_name, 1, &index, name, &value );
		return value;
	}

	int ShaderProgram::activeUniformBlock( uint32_t index
		, uint32_t name )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		int value{ 0 };
		glCheckError( glGetActiveUniformBlockiv, m_name, index, name, &value );
		return value;
	}

	void ShaderProgram::uniformBlockBinding( uint32_t index
		, uint32_t point )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glUniformBlockBinding, m_name, index, point );
	}

	uint32_t ShaderProgram::doGetAttributeLocation
		( std::string const & name )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		return uint32_t( glGetAttribLocation( m_name, name.c_str() ) );
	}

	bool ShaderProgram::doFindAttributeInfos( std::string const & name
		, AttributeFullType type
		, AttributeInfo &  infos )const
	{
		auto it = std::find_if( m_attributes.begin()
			, m_attributes.end()
			, [&name]( auto & attribute )
		{
			return attribute.m_name == name;
		} );

		bool found{ false };

		if ( it != m_attributes.end() )
		{
			if ( it->m_type == type )
			{
				infos = *it;
				found = true;
			}
			else
			{
				std::cerr << "Attribute " << name << " was found with another type." << std::endl;
			}
		}
		else
		{
			std::cerr << "Attribute " << name << " was not found." << std::endl;
		}

		return found;
	}
}
