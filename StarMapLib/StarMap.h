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
		void restore( render::CameraState const & state );
		/**
		*\brief
		*	Sauvegarde l'état de la carte du ciel.
		*\param[out] state
		*	Reçoit l'état actuel.
		*/
		void save( render::CameraState & state );
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
		void initialise( gl::Size2D const & size
			, render::ByteArray const & opacityMap
			, render::FontLoader & loader );
		/**
		*\brief
		*	Nettoie les données GPU.
		*/
		void cleanup();
		/**
		*\brief
		*	Redimensionne la fenêtre.
		*\param[in] size
		*	Les nouvelles dimensions.
		*/
		void resize( gl::Size2D const & size );
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
		/**
		*\brief
		*	Crée et ajoute une constellation à la carte du ciel.
		*\param[in] name
		*	Le nom de la constellation.
		*/
		Constellation & createConstellation( std::string const & name );
		/**
		*\brief
		*	Applique un filtre.
		*\param[in] type
		*	Le type d'élément sur lequel est appliqué le filtre.
		*\param[in] show
		*	\p true pour afficher les éléments du type donné,
		*	\p false pour les cacher
		*/
		void filter( ElementType type, bool show );

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
		StarHolder & doFindHolder( gl::RgbColour const & colour );
		/**
		*\brief
		*	Initialise les billboards pour les étoiles du conteneur.
		*\param[in] holder
		*	Le conteneur du tampon et de la couleur.
		*/
		void doInitialiseHolder( StarHolder & holder );
		/**
		*\brief
		*	Ajoute une étoile au conteneur de billboards approprié.
		*\param[in] star
		*	L'étoile à ajouter.
		*\param[in] range
		*	L'intervalle des magnitudes.
		*/
		void doAddStar( Star const & star
			, render::Range< float > const & range );
		/**
		*\brief
		*	Ajoute une constellation à la polyligne.
		*\param[in] constellation
		*	La constellation à ajouter.
		*/
		void doAddConstellation( Constellation & constellation );
		/**
		*\brief
		*	Charge les textures de police.
		*\param[in] loader
		*	Le loader de police.
		*/
		void doLoadFontTextures( render::FontLoader & loader );
		/**
		*\brief
		*	Charge la texture d'opacité des étoiles.
		*\param[in] opacityMap
		*	Le contenu de la texture.
		*/
		void doLoadOpacityMap( render::ByteArray const & opacityMap );
		/**
		*\brief
		*	Initialise les objets affichés par dessus les éléments
		*	sélectionnés.
		*/
		void doInitialisePickObjects();
		/**
		*\brief
		*	Initialise la polyligne contenant les constellations.
		*/
		void doInitialiseLines();
		/**
		*\brief
		*	Initialise les incrustations contenant les noms des étoiles.
		*/
		void doInitialiseStarNames();
		/**
		*\brief
		*	Initialise les incrustations contenant les noms des constellations.
		*/
		void doInitialiseConstellationNames();
		/**
		*\brief
		*	Met à jour la position de l'incrustation de sélection.
		*/
		void doUpdatePickDescription();
		/**
		*\brief
		*	Met à jour la position des noms des étoiles.
		*/
		void doUpdateStarNames();
		/**
		*\brief
		*	Met à jour la position des noms des constellations.
		*/
		void doUpdateConstellationNames();
		/**
		*\brief
		*	Met à jour la position d'une incrustation texte par rapport à la
		*	position donnée.
		*\param[in] position
		*\	La position monde 3D de l'objet.
		*\param[in] offset
		*\	L'offset 2D donné à la position écran de l'incrustation.
		*/
		void doUpdateOverlay( render::TextOverlay & overlay
			, gl::Vector3D const & position
			, gl::Offset2D const & offset );
		/**
		*\brief
		*	Affiche ou cache les constellations.
		*/
		void doFilterConstellations( bool show );
		/**
		*\brief
		*	Affiche ou cache les étoiles.
		*/
		void doFilterStars( bool show );

	private:
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

	private:
		//! Le tableau de StarHolders.
		StarHolderArray m_holders;
		//! La polyligne contenant les constellations.
		render::PolyLinePtr m_lines;
		//! La fenêtre où s'effectue le rendu.
		render::RenderWindowPtr m_window;
		//! Les informations de débogage.
		render::Debug m_debug;
		//! Les étoiles.
		StarSet m_stars;
		//! Les constellations.
		ConstellationMap m_constellations;
		//! L'objet sélectionné.
		render::Object * m_pickedObject{ nullptr };
		//! Le billboard sélectionné.
		render::Billboard * m_pickedBillboard{ nullptr };
		//! L'étoile sélectionnée.
		Star const * m_pickedStar{ nullptr };
		//! Le billboard apparaissant sur la sélection (billboard ou objet).
		render::BillboardPtr m_pickBillboard;
		//! L'incrustation décrivant la sélection (billboard ou objet).
		render::TextOverlayPtr m_pickDescription;
		//! La texture d'opacité.
		render::TexturePtr m_opacity;
		//! La texture de police des noms.
		render::FontTexturePtr m_fontTextureNames;
	};
}

#endif
