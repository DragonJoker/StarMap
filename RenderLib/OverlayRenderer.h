/**
*\file
*	OverlayRenderer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_OverlayRenderer_H___
#define ___RenderLib_OverlayRenderer_H___
#pragma once

#include "FontTexture.h"
#include "OverlayCategory.h"
#include "Viewport.h"

#include <GlLib/GlPipeline.h>
#include <GlLib/GlUniformBuffer.h>

namespace render
{
	/**
	*\brief
	*	Le renderer d'incrustations.
	*/
	class OverlayRenderer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] maxCharsPerBuffer
		*	Le nombre maximal de caractères par tampon de sommets de texte.
		*/
		explicit OverlayRenderer( uint32_t maxCharsPerBuffer = 600 );
		/**
		*brief
		*	Destructeur.
		*/
		~OverlayRenderer();
		/**
		*brief
		*	Fonction de dessin des TextOverlays.
		*\param[in] overlays
		*	Les incrustations texte à dessiner.
		*/
		void draw( OverlayList const & overlays );
		/**
		*brief
		*	Dessine un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawPanel( PanelOverlay const & overlay );
		/**
		*brief
		*	Dessine un BorderPanelOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawBorderPanel( BorderPanelOverlay const & overlay );
		/**
		*brief
		*	Dessine un TextOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawText( TextOverlay const & overlay );
		/**
		*brief
		*	Commence le rendu des incrustations.
		*param[in] size
		*	Les dimensions de la fenêtre de rendu.
		*/
		void beginRender( gl::Size2D const & size );
		/**
		*brief
		*	Termine le rendu des incrustations.
		*/
		void endRender();

	private:
		struct OverlayProgram
		{
			/**
			*\brief
			*	Constructeur.
			*\param[in] text
			*	Dit si c'est un programme pour les incrustations texte.
			*/
			OverlayProgram( bool text );

			//! Le programme sshader utilisé pour dessiner les incrustations.
			gl::ShaderProgramPtr m_program;
			//! La variable uniforme contenant la matrice modèle - projection.
			gl::Mat4UniformPtr m_mpUniform{ nullptr };
			//! La variable uniforme contenant l'échantillonneur de la texture.
			gl::IntUniformPtr m_map;
			//! La variable uniforme contenant la couleur de l'incrustation.
			gl::Vec4UniformPtr m_colour;
		};
		/**
		*\brief
		*	Décrit un tampon de sommets pour les incrustations.
		*/
		struct VertexBuffer
		{
			//! Le tampon de sommets.
			gl::BufferPtr< Overlay::Quad > m_vbo;
			//! L'attribut de position.
			gl::Vec2AttributePtr m_position;
			//! L'attribut de coordonnées de texture.
			gl::Vec2AttributePtr m_texture;
		};
		//! Un pointeur sur un VertexBuffer.
		using VertexBufferPtr = std::unique_ptr< VertexBuffer >;
		/**
		*\brief
		*	Crée un tampon de sommets et ses attrobite pour les incrustations
		*	texte, et l'ajoute à la liste.
		*/
		VertexBuffer const & doCreateTextBuffer();
		/**
		*brief
		*	Fonction de dessin d'une incrustation.
		*param[in] buffer
		*	Le tampon de la géométrie de l'incrustation.
		*param[in] count
		*	Le nombre de sommets.
		*param[in] transform
		*	La matrice de transformation de l'incrustation.
		*param[in] colour
		*	La couleur de l'incrustation.
		*/
		void doDrawBuffer( VertexBuffer const & buffer
			, uint32_t count
			, gl::Matrix4x4 const & transform
			, gl::RgbaColour const & colour );
		/**
		*brief
		*	Fonction de dessin d'une incrustation texte.
		*param[in] buffer
		*	Le tampon de la géométrie de l'incrustation.
		*param[in] count
		*	Le nombre de sommets.
		*param[in] transform
		*	La matrice de transformation de l'incrustation.
		*param[in] colour
		*	La couleur de l'incrustation.
		*param[in] texture
		*	La texture contenant les caractères.
		*/
		void doDrawBuffer( VertexBuffer const & buffer
			, uint32_t count
			, gl::Matrix4x4 const & transform
			, gl::RgbaColour const & colour
			, Texture const & texture );
		/**
		*brief
		*	Remplit un GeometryBuffers d'une partie d'un tableau de sommets
		*	pour texte.
		*param[in] count
		*	Le nombre de sommets à copier dans le GeometryBuffers.
		*param[in] it
		*	Le début du tableau de sommets.
		*param[in] index
		*	L'indice courant dans le tableau de GeometryBuffers de texte.
		*return
		*	Le GeometryBuffers utilisé.
		*/
		VertexBuffer const & doFillTextPart( uint32_t count
			, uint32_t & offset
			, Overlay::QuadArray::const_iterator & it
			, uint32_t & index );

	private:
		//! Le programme sshader utilisé pour dessiner les incrustations panneau.
		OverlayProgram m_panelProgram;
		//! Le programme sshader utilisé pour dessiner les incrustations texte.
		OverlayProgram m_textProgram;
		//! Le pipeline utilisé pour le dessin des incrustations texte.
		gl::Pipeline m_pipeline;
		//! Les tampons de sommets utilisés pour rendre les panneaux.
		VertexBuffer m_panelBuffer;
		//! Les tampons de sommets utilisés pour rendre les bordures.
		VertexBuffer m_borderBuffer;
		//! Les tampons de sommets utilisés pour rendre les textes.
		std::vector< VertexBufferPtr > m_textBuffers;
		//! Le nombre maximal de caractères par tampon de sommets texte.
		uint32_t m_maxCharsPerBuffer;
		//! Dit si les dimension du rendu ont changé.
		bool m_sizeChanged{ true };
		//! La matrice de projection.
		gl::Matrix4x4 m_transform;
		//! Le viewport de rendu des incrustations.
		Viewport m_viewport;
	};
}

#endif
