#pragma once

#include <RenderLib/CameraState.h>
#include <RenderLib/Debug.h>
#include <RenderLib/FontTexture.h>
#include <RenderLib/RenderWindow.h>
#include <RenderLib/Scene.h>

#include <AndroidUtils/AndroidWindow.h>

class Window
	: public utils::AndroidWindow
{
public:
	Window( utils::AndroidApp const & parent
		, ANativeWindow * window
		, render::ByteArray const & state );
	~Window();

private:
	/**
	*\brief
	*	Création de la fenêtre.
	*/
	void onCreate()override;
	/**
	*\brief
	*	Destruction de la fenêtre.
	*/
	void onDestroy()override;
	/**
	*\brief
	*	Création de la fenêtre.
	*/
	void onDraw()override;
	/**
	*\return
	*	Sauvegarde les données de l'application.
	*/
	void onSave( render::ByteArray & state )override;
	/**
	*\return
	*	Restaure les données de l'application.
	*/
	void onRestore( render::ByteArray const & state )override;
	/**
	*\brief
	*	Gestion d'un évènement de type tap.
	*/
	void onSingleTap( gl::IVec2 const & position )override;
	/**
	*\brief
	*	Gestion d'un évènement de type double tap.
	*/
	void onDoubleTap( gl::IVec2 const & position )override;
	/**
	*\brief
	*	Gestion d'un évènement de type déplacement avec un pointeur.
	*/
	void onSingleMove( gl::IVec2 const & position )override;
	/**
	*\brief
	*	Gestion d'un évènement de type déplacement avec deux pointeur.
	*/
	void onDoubleMove( gl::IVec2 const & posDiff, int distDiff )override;
	/**
	*\brief
	*	Gestion du signal de sélection d'objet.
	*/
	void onObjectPicked( render::Object & object );
	/**
	*\brief
	*	Gestion du signal de sélection d'objet.
	*/
	void onBillboardPicked( render::Billboard & billboard, uint32_t index );
	/**
	*\brief
	*	Gestion du signal de désélection.
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
	*	Crée un matériau depuis une couleur et une opacité.
	*\param[in] name
	*	Le nom du matériau.
	*\param[in] colour
	*	La couleur du matériau.
	*\param[in] opacity
	*	L'opacité du matériau.
	*/
	render::MaterialPtr doCreateOverlayMaterial( std::string const & name
		, gl::RgbColour const & colour
		, float opacity );

private:
	render::RenderWindowPtr m_renderWindow;

	render::Connection< render::OnObjectPicked > m_onObjectPicked;
	render::Connection< render::OnBillboardPicked > m_onBillboardPicked;
	render::Connection< render::OnUnpick > m_onUnpick;

	render::Object * m_pickedObject{ nullptr };
	render::Billboard * m_pickedBillboard{ nullptr };
	render::BillboardPtr m_picked;
	render::FontTexturePtr m_fontTexture;
};

class Engine
{
public:
	/**
	*\brief
	*	Traitement de l'évènement d'entrée suivant.
	*/
	static int32_t handleInput( android_app * app, AInputEvent * event );
	/**
	*\brief
	*	Traitement de la commande principale suivante.
	*/
	static void handleCommand( android_app * app, int32_t cmd );
};
