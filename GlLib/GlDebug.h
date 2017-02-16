/**
*\file
*	GlDebug.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Debug_HPP___
#define ___GlLib_Debug_HPP___
#pragma once

#include <set>

#if !defined( CALLBACK )
#	if defined( _WIN32 )
#		define CALLBACK __stdcall
#	else
#		define CALLBACK
#	endif
#endif

namespace gl
{
	/**
	*\brief
	*	Classe de gestion des extensions de débogage OpenGL.
	*/
	class Debug
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		Debug();
		/**
		*\brief
		*	Constructeur.
		*/
		~Debug();
		/**
		*\brief
		*	Cherche les extensions de débogage disponibles, et charge celle
		*	qui va bien.
		*\remarks
		*	Si aucune extension n'est trouvée, glGetError sera appelé sur les
		*	appels OpenGL.
		*/
		void initialise();
		/**
		*\brief
		*	Vérifie une erreur, si aucune extension de débogage n'est
		*	disponible.
		*\param[in] file
		*	Le nom du fichier où l'appel se fait.
		*\param[in] line
		*	La ligne dans le fichier.
		*\param[in] message
		*	Le message, affiché en cas d'erreur détectée.
		*\return
		*	\p true si aucune erreur n'a été détectée.
		*/
		static bool checkError( std::string const & file
			, uint32_t line
			, std::string const & message );
		/**
		*\brief
		*	Filtre un message, qui ne sera donc plus affiché.
		*\remarks
		*	Ne fonctionne que si une extension de débogage est chargée.
		*\param[in] id
		*	L'id du message.
		*/
		void filterMessage( uint32_t id );
		/**
		*\brief
		*	Dit si un message est filtré.
		*\param[in] id
		*	L'id du message.
		*\return
		*	\p true si le message est filtré.
		*/
		bool filtered( uint32_t id )const;

	private:
		/**
		*\brief
		*	Log de débogage KHR ou ARB.
		*\param[in] source
		*	La source du message.
		*\param[in] type
		*	Le type du message.
		*\param[in] id
		*	L'id du message.
		*\param[in] severity
		*	L'importance du message.
		*\param[in] length
		*	La longueur du message.
		*\param[in] message
		*	Le message.
		*\param[in] userParam
		*	Données fournies par l'utilisateur.
		*/
		static void CALLBACK debugLog( uint32_t source
			, uint32_t type
			, uint32_t id
			, uint32_t severity
			, int length
			, const char * message
			, void * userParam );
		/**
		*\brief
		*	Log de débogage AMD.
		*\param[in] id
		*	L'id du message.
		*\param[in] category
		*	La catégorie du message.
		*\param[in] severity
		*	L'importance du message.
		*\param[in] length
		*	La longueur du message.
		*\param[in] message
		*	Le message.
		*\param[in] userParam
		*	Données fournies par l'utilisateur.
		*/
		static void CALLBACK debugLogAMD( uint32_t id
			, uint32_t category
			, uint32_t severity
			, int length
			, const char * message
			, void * userParam );
		/**
		*\brief
		*	Log de débogage KHR ou ARB.
		*\param[in] source
		*	La source du message.
		*\param[in] type
		*	Le type du message.
		*\param[in] id
		*	L'id du message.
		*\param[in] severity
		*	L'importance du message.
		*\param[in] length
		*	La longueur du message.
		*\param[in] message
		*	Le message.
		*/
		void doDebugLog( uint32_t source
			, uint32_t type
			, uint32_t id
			, uint32_t severity
			, int length
			, const char * message )const;
		/**
		*\brief
		*	Log de débogage AMD.
		*\param[in] id
		*	L'id du message.
		*\param[in] category
		*	La catégorie du message.
		*\param[in] severity
		*	L'importance du message.
		*\param[in] length
		*	La longueur du message.
		*\param[in] message
		*	Le message.
		*/
		void doDebugLogAMD( uint32_t id
			, uint32_t category
			, uint32_t severity
			, int length
			, const char * message )const;

	private:
		//! Liste de messages filtrés.
		std::set< uint32_t > m_filteredOut;
		//! Dit si une extension de débogage est disponible.
		static bool m_hasExtension;
	};
}

#endif
