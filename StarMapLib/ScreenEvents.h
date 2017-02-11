/**
*\file
*	ScreenEvents.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_ScreenEvents_HPP___
#define ___StarMapLib_ScreenEvents_HPP___
#pragma once

#include "StarMapLibPrerequisites.h"

#include <RenderLib/Signal.h>

namespace starmap
{
	/**
	*\brief
	*	Classe de gestion des évènements écran.
	*\brief
	*	Fait la traduction des évènemens écran en évènements utilisés
	*	par StarMap.
	*/
	class ScreenEvents
	{
	private:
		ScreenEvents( ScreenEvents const & ) = delete;
		ScreenEvents & operator=( ScreenEvents const & ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in,out] onScreenTap
		*	Evènement d'écran "cliqué", pour s'y abonner.
		*\param[in,out] onScreenDoubleTap
		*	Evènement d'écran "double cliqué", pour s'y abonner.
		*\param[in,out] onScreenSingleMove
		*	Evènement de déplacement avec un doigt, pour s'y abonner.
		*\param[in,out] onScreenDoubleMove
		*	Evènement de déplacement avec deux doigts, pour s'y abonner.
		*/
		ScreenEvents( OnScreenTap & onScreenTap
			, OnScreenDoubleTap & onScreenDoubleTap
			, OnScreenSingleMove & onScreenSingleMove
			, OnScreenDoubleMove & onScreenDoubleMove );
		/**
		*\brief
		*	D�finit la carte des étoiles.
		*/
		void setStarMap( StarMap * starmap )
		{
			m_starmap = starmap;
		}

	public:
		//! Le signal émis lorsque l'on effectue une sélection.
		OnPick onPick;
		//! Le signal émis lorsque l'on réinitialise la caméra.
		OnReset onReset;
		//! Le signal émis lorsque l'on définit la vitesse de rotation de la caméra.
		OnSetVelocity onSetVelocity;
		//! Le signal émis lorsque l'on définit la vitesse de zoom de la caméra.
		OnSetZoomVelocity onSetZoomVelocity;

	private:
		/**
		*\brief
		*	Traitement de l'évènement de "clic" sur l'écran.
		*\param[in] coord
		*	La coordonnée.
		*/
		void onScreenTap( glm::vec2 const & coord );
		/**
		*\brief
		*	Traitement de l'évènement de "double clic" sur l'écran.
		*\param[in] coord
		*	La coordonnée.
		*/
		void onScreenDoubleTap( glm::vec2 const & coord );
		/**
		*\brief
		*	Traitement de l'évènement de déplacement avec un doigt.
		*\param[in] diff
		*	La valeur du déplacement.
		*/
		void onScreenSingleMove( glm::vec2 const & diff );
		/**
		*\brief
		*	Traitement de l'évènement de déplacement avec deux doigts.
		*\param[in] diff
		*	La différence de position des deux doigts.
		*\param[in] distanceOffset
		*	La différence de distance entre les deux doigts, entre les
		*	déplacements.
		*/
		void onScreenDoubleMove( glm::vec2 const & diff
			, float distanceOffset );

	private:
		//! La connexion au signal émis lorsque l'on "clique" sur l'écran.
		render::Connection< OnScreenTap > m_onScreenTap;
		//! La connexion au signal émis lorsque l'on "double clique" sur l'écran.
		render::Connection< OnScreenDoubleTap > m_onScreenDoubleTap;
		//! La connexion au signal émis lorsque l'on effectue un déplacement avec un doigt sur l'écran.
		render::Connection< OnScreenSingleMove > m_onScreenSingleMove;
		//! La connexion au signal émis lorsque l'on effectue un déplacement avec deux doigts sur l'écran.
		render::Connection< OnScreenDoubleMove > m_onScreenDoubleMove;
		//! La carte.
		StarMap * m_starmap;
	};
}

#endif
