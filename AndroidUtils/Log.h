/**
*\file
*	Log.h
*\author
*	Sylvain Doremus
*/
#ifndef ___AndroidUtils_Log_HPP___
#define ___AndroidUtils_Log_HPP___
#pragma once

#include <string>
#include <ostream>

#include <RenderLib/LogUtils.h>

namespace utils
{
	/**
	*\brief
	*	Classe de traits, pour afficher une ligne dans le logcat.
	*/
	template< int LogType >
	struct TLogStreambufTraits
	{
		using CharType = char;
		using string_type = std::basic_string< CharType >;

		/**
		*\brief
		*	Affiche la ligne donnée dans le logcat.
		*\param[in] appName
		*	Le nom de l'application.
		*\param[in] text
		*	La ligne à logger.
		*/
		static void Log( string_type const & appName
			, string_type const & text )
		{
			__android_log_print( LogType
				, appName.c_str()
				, "%s"
				, text.c_str() );
		}
	};
	//! Spécialisation de TLogStreambufTraits pour les logs de débogage.
	using DebugLogStreambufTraits = TLogStreambufTraits< ANDROID_LOG_DEBUG >;
	//! Spécialisation de TLogStreambufTraits pour les logs informatifs.
	using InfoLogStreambufTraits = TLogStreambufTraits< ANDROID_LOG_INFO >;
	//! Spécialisation de TLogStreambufTraits pour les logs de warning.
	using WarningLogStreambufTraits = TLogStreambufTraits< ANDROID_LOG_WARN >;
	//! Spécialisation de TLogStreambufTraits pour les logs d'erreur.
	using ErrorLogStreambufTraits = TLogStreambufTraits< ANDROID_LOG_ERROR >;
}

#endif
