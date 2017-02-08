/**
*\file
*	StarMap.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_StarMap_HPP___
#define ___StarMapLib_StarMap_HPP___
#pragma once

#include "StarMapState.h"

#include <RenderLib/Debug.h>
#include <RenderLib/RenderWindow.h>
#include <RenderLib/Signal.h>

namespace starmap
{
	/**
	*\brief
	*	La carte du ciel.
	*/
	class StarMap
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		StarMap( ScreenEvents & events );
		/**
		*\brief
		*	Restaure l'état de la carte du ciel.
		*\param[in] state
		*	L'état à récupérer.
		*/
		void restore( StarMapState const & state );
		/**
		*\brief
		*	Sauvegarde l'état de la carte du ciel.
		*\param[out] state
		*	Reçoit l'état actuel.
		*/
		void save( StarMapState & state );
		/**
		*\brief
		*	Initialise les données GPU.
		*\param[in] size
		*	Les dimensions de la fenêtre.
		*\param[in] opacityMap
		*	Le contenu de la texture d'opacité des étoiles.
		*\param[in] loader
		*	Le loader de police.
		*/
		void initialise( glm::ivec2 const & size
			, render::ByteArray const & opacityMap
			, render::FontLoader const & loader );
		/**
		*\brief
		*	Nettoie les données GPU.
		*/
		void cleanup();
		/**
		*\brief
		*	Démarre le dessin d'une image.
		*/
		void beginFrame();
		/**
		*\brief
		*	Dessine la carte.
		*/
		void drawFrame();
		/**
		*\brief
		*	Termine le dessin d'une image.
		*/
		void endFrame();
		/**
		*\brief
		*	Ajoute une étoile à la carte du ciel.
		*\param[in] star
		*	L'étoile.
		*/
		void add( Star const & star );
		/**
		*\brief
		*	Ajoute une liste d'étoiles à la carte du ciel.
		*\param[in] stars
		*	La liste d'étoiles.
		*/
		void add( StarArray const & stars );

	private:
		/**
		*\brief
		*	Traitement de l'évènement de sélection d'objet.
		*\param[in] object
		*	L'object sélectionné.
		*/
		void onObjectPicked( render::Object & object );
		/**
		*\brief
		*	Traitement de l'évènement de sélection de billboard.
		*\param[in] billboard
		*	Le billboard sélectionné.
		*\param[in] index
		*	L'indice du quad.
		*/
		void onBillboardPicked( render::Billboard & billboard, uint32_t index );
		/**
		*\brief
		*	Traitement de l'évènement de désélection.
		*/
		void onUnpick();

	private:
		/**
		*\brief
		*	Met à jour la position et la taille de l'objet sélectionné.
		*\param[in] movable
		*	Les informations de position.
		*/
		void doUpdatePicked( render::Movable const & movable );
		/**
		*\brief
		*	Met à jour la position et la taille de l'objet sélectionné.
		*\param[in] object
		*	Les informations de position et taille.
		*/
		void doUpdatePicked( render::Object const & object );
		/**
		*\brief
		*	Met à jour la position et la taille de l'objet sélectionné.
		*\param[in] billboard
		*	Les informations de position et taille.
		*/
		void doUpdatePicked( render::Billboard const & billboard
			, uint32_t index );
		/**
		*\brief
		*	Récupère un conteneur de billboard pour les étoiles de la couleur donnée.
		*\param[in] colour
		*	La couleur.
		*\return
		*	Le conteneur
		*/
		StarHolder & doFindHolder( glm::vec3 const & colour );
		/**
		*\brief
		*	Initialise les billboards pour les étoiles du conteneur.
		*\param[in] holder
		*	Le conteneur du tampon et de la couleur.
		*/
		void doInitialiseHolder( StarHolder & holder );

	private:
		//! L'état de la carte.
		StarMapState m_state;
		//! La fenêtre où s'effectue le rendu.
		render::RenderWindowPtr m_window;
		//! Les informations de débogage.
		render::Debug m_debug;
		//! La connexion à la notification d'objet sélectionné.
		render::Connection< render::OnObjectPicked > m_onObjectPicked;
		//! La connexion à la notification de billboard sélectionné.
		render::Connection< render::OnBillboardPicked > m_onBillboardPicked;
		//! La connexion à la notification d'objet/billboard désélectionné.
		render::Connection< render::OnUnpick > m_onUnpick;
		//! La connexion à la notification de sélection.
		render::Connection< OnPick > m_onPick;
		//! La connexion à la notification de réinitialisation de la caméra.
		render::Connection< OnReset > m_onReset;
		//! La connexion à la notification de définition de la vitesse de rotation de la caméra.
		render::Connection< OnSetVelocity > m_onSetVelocity;
		//! La connexion à la notification de définition de la vitesse de zoom de la caméra.
		render::Connection< OnSetZoomVelocity > m_onSetZoomVelocity;
		//! L'objet sélectionné.
		render::Object * m_pickedObject{ nullptr };
		//! Le billboard sélectionné.
		render::Billboard * m_pickedBillboard{ nullptr };
		//! Le billboard apparaissant sur la sélection (billboard ou objet).
		render::BillboardPtr m_picked;
		//! La texture d'opacité.
		render::TexturePtr m_opacity;
	};
}

#endif
