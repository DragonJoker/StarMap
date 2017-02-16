/**
*\file
*	TextOverlay.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_TextOverlay_HPP___
#define ___RenderLib_TextOverlay_HPP___
#pragma once

#include "FontTexture.h"

namespace render
{
	/**
	*\brief
	*	Modes d'espacement des lignes du texte, gère la manière dont les lignes
	*	sont espacées entre elles, dans un TextOverlay.
	*/
	enum class TextLineSpacingMode
	{
		//! La hauteur d'une ligne est sa propre hauteur.
		eOwnHeight,
		//! La hauteur des lignes est la plus grande hauteur parmi elles.
		eMaxLineHeight,
		//! La hauteur des lignes est la plus grande hauteur des caractères de la police.
		eMaxFontHeight,
		GlLib_EnumBounds( eOwnHeight )
	};
	/**
	*\brief
	*	Alignemens horizontaux pour les incrustations texte.
	*/
	enum class HAlign
	{
		//! Aligné à gauche.
		eLeft,
		//! Centré, horizontalement.
		eCenter,
		//! Aligné à droite.
		eRight,
		GlLib_EnumBounds( eLeft )
	};
	/**
	*\brief
	*	Une incrustation contenant du texte.
	*/
	class TextOverlay
	{
	public:
		/**
		*\brief
		*	Contient les données spécifiques de sommet pour un TextOverlay.
		*/
		struct Vertex
		{
			//! Sa position.
			gl::Vector2D coords;
			//! Ses coordonnées de texture dans la texture de police.
			gl::Vector2D text;
		};
		//! Liste de sommets composant l'incrustation.
		using VertexArray = std::vector< Vertex >;

	public:
		/**
		*\brief
		*	Constructeur.
		*/
		TextOverlay();
		/**
		*\brief
		*	Destructeur.
		*/
		~TextOverlay();
		/**
		*\brief
		*	Met à jour le tampon de sommets.
		*/
		void update();
		/**
		*\brief
		*	Définit la texture de police utilisée pour le dessin de
		*	cette incrustation.
		*\param[in] fontTexture
		*	La texture de police.
		*/
		void fontTexture( FontTexture const & fontTexture )noexcept
		{
			m_textChanged |= ( &fontTexture != m_fontTexture );
			m_fontTexture = &fontTexture;
		}
		/**
		*\return
		*	La texture de police utilisée pour le dessin de cette incrustation.
		*/
		FontTexture const & fontTexture()const noexcept
		{
			assert( m_fontTexture );
			return *m_fontTexture;
		}
		/**
		*\brief
		*	Définit la position de l'incrustation.
		*\param[in] position
		*	La nouvelle valeur.
		*/
		inline void position( gl::Position2D const & position )noexcept
		{
			m_position = position;
			m_positionChanged = true;
		}
		/**
		*\return
		*	La position de l'incrustation.
		*/
		inline gl::Position2D const & position()const noexcept
		{
			return m_position;
		}
		/**
		*\return
		*	La taille de l'incrustation.
		*/
		inline gl::Size2D const & size()const noexcept
		{
			return m_computedSize;
		}
		/**
		*\return
		*	Le tampon de sommets du panneau.
		*/
		inline VertexArray const & textVertex()const noexcept
		{
			return m_vertex;
		}
		/**
		*\return
		*	Le texte de l'incrustation.
		*/
		inline std::string const & caption()const noexcept
		{
			return m_currentCaption;
		}
		/**
		*\brief
		*	Définit le texte de l'incrustation.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void caption( std::string const & value )
		{
			m_currentCaption = value;
			m_textChanged = true;
		}
		/**
		*\return
		*	L'alignement horizontal.
		*/
		inline HAlign align()const noexcept
		{
			return m_hAlign;
		}
		/**
		*\brief
		*	Définit l'alignement horizontal.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void align( HAlign value )noexcept
		{
			m_textChanged |= m_hAlign != value;
			m_hAlign = value;
		}
		/**
		*\return
		*	Le mode d'espacement des lignes.
		*/
		inline TextLineSpacingMode lineSpacingMode()const noexcept
		{
			return m_lineSpacingMode;
		}
		/**
		*\brief
		*	Définit le mode d'espacement des lignes.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void lineSpacingMode( TextLineSpacingMode value )noexcept
		{
			m_textChanged |= m_lineSpacingMode != value;
			m_lineSpacingMode = value;
		}
		/**
		*\return
		*	La couleur de l'incrustation.
		*/
		inline gl::RgbaColour const & colour()const noexcept
		{
			return m_colour;
		}
		/**
		*\brief
		*	Définit la couleur de l'incrustation.
		*\param[in] colour
		*	La nouvelle couleur.
		*/
		inline void colour( gl::RgbaColour const & colour )noexcept
		{
			m_colour = colour;
		}
		/**
		*\return
		*	La matrice de transformation de l'incrustation.
		*/
		inline gl::Matrix4x4 const & transform()const noexcept
		{
			return m_transform;
		}
		/**
		*\return
		*	Le statut de visibilité de l'objet.
		*/
		inline bool visible()const noexcept
		{
			return m_visible;
		}
		/**
		*\brief
		*	Définit le statut de visibilité de l'objet.
		*\param[in] show
		*	Le nouveau statut.
		*/
		inline void show( bool show = true )noexcept
		{
			m_visible = show;
		}

	private:
		/**
		*\brief
		*	Un caractère, avec ses dimensions et sa position relative.
		*/
		struct DisplayableChar
		{
			//! La position du caractère, relative à sa ligne.
			gl::Position2D m_position;
			//! Les dimensions du caractère.
			gl::Size2D m_size;
			//! Le caractère à afficher.
			Glyph const & m_glyph;
			/**
			*\brief
			*	Constructeur.
			*\param[in] position
			*	La position du caractère dans sa ligne.
			*\param[in] size
			*	Les dimensions du caractère.
			*\param[in] glyph
			*	La glyphe du caractère.
			*/
			DisplayableChar( gl::Position2D const & position
				, gl::Size2D const & size
				, Glyph const & glyph )
				: m_position{ position }
				, m_size{ size }
				, m_glyph{ glyph }
			{
			}
			/**
			*\brief
			*	Constructeur par copie.
			*\param[in,out] rhs
			*	L'objet à copier.
			*/
			DisplayableChar( DisplayableChar const & rhs )
				: m_position{ rhs.m_position }
				, m_size{ rhs.m_size }
				, m_glyph{ rhs.m_glyph }
			{
			}
			/**
			*\brief
			*	Constructeur par déplacement.
			*\param[in,out] rhs
			*	L'objet à déplacer.
			*/
			DisplayableChar( DisplayableChar && rhs )
				: m_position{ std::move( rhs.m_position ) }
				, m_size{ std::move( rhs.m_size ) }
				, m_glyph{ rhs.m_glyph }
			{
			}
			/**
			*\brief
			*	Opérateur d'affectation par copie.
			*\param[in,out] rhs
			*	L'objet à copier.
			*/
			DisplayableChar & operator=( DisplayableChar const & rhs )
			{
				m_position = rhs.m_position;
				m_size = rhs.m_size;
				return *this;
			}
			/**
			*\brief
			*	Opérateur d'affectation par déplacement.
			*\param[in,out] rhs
			*	L'objet à déplacer.
			*/
			DisplayableChar & operator=( DisplayableChar && rhs )
			{
				if ( &rhs != this )
				{
					m_position = std::move( rhs.m_position );
					m_size = std::move( rhs.m_size );
				}

				return *this;
			}
		};
		/**
		\brief
		*	Une ligne de texte, avec ses dimensions et sa position.
		*/
		struct DisplayableLine
		{
			//! La position de la ligne.
			gl::Position2D m_position;
			//! La largeur de la ligne.
			int32_t m_width;
			//! La hauteur de la ligne.
			int32_t m_height;
			//! Les caractères affichables.
			std::vector< DisplayableChar > m_characters;
		};
		//! Un tableau de lignes affichables.
		using DisplayableLineArray = std::vector< DisplayableLine >;
		/**
		*\brief
		*	Calcule les lignes à afficher.
		*\return
		*	Les lignes.
		*/
		DisplayableLineArray doPrepareText();
		/**
		*\brief
		*	Ajoute un mot au tampon de sommets.
		*\param[in] word
		*	Le mot à ajouter.
		*\param[in] wordWidth
		*	La largeur du mot.
		*\param[out] left
		*	La position à gauche.
		*\param[out] line
		*	La ligne.
		*\param[out] lines
		*	Les lignes.
		*/
		void doPrepareWord( std::string const & word
			, int32_t wordWidth
			, int32_t & left
			, DisplayableLine & line
			, DisplayableLineArray & lines );
		/**
		*\brief
		*	Finit la ligne et passe à la ligne suivante.
		*\param[in,out] line
		*	La ligne, dont la position et la largeur sont complétées.
		*\param[out] left
		*	La position à gauche.
		*/
		void doFinishLine( DisplayableLine & line
			, int32_t & left );
		/**
		*\brief
		*	Aligne horizontalement une ligne.
		*\param[in] width
		*	La largeur de l'incrustation.
		*\param[in] line
		*	La ligne.
		*\param[out] lines
		*	Les lignes.
		*/
		void doAlign( DisplayableLine line
			, DisplayableLineArray & lines );
		/**
		*\brief
		*	Met à jour la matrice de transformation de l'incrustation.
		*/
		void doUpdatePosition();
		/**
		*\brief
		*	Met à jour le tampon de sommets.
		*/
		void doUpdateBuffer();

	private:
		//! La taille relative à l'écran.
		gl::Size2D m_computedSize;
		//! La position relative à l'écran.
		gl::Position2D m_position;
		//! Dit si la position de cette incrustation a changé.
		bool m_positionChanged{ true };
		//! Le texte courant de l'incrustation.
		std::string m_currentCaption;
		//! Le texte précédent de l'incrustation.
		std::string m_previousCaption;
		//! Dit si le texte (contenu, mode de découpe, alignements) a changé.
		bool m_textChanged{ true };
		//! Les données du tampon de sommets.
		VertexArray m_vertex;
		//! Le mode d'espacement des lignes.
		TextLineSpacingMode m_lineSpacingMode{ TextLineSpacingMode::eOwnHeight };
		//! L'alignement horizontal du texte.
		HAlign m_hAlign{ HAlign::eLeft };
		//! La couleur de l'incrustation.
		gl::RgbaColour m_colour;
		//! La matrice de transformation de l'incrustation.
		gl::Matrix4x4 m_transform;
		//! La texture de police utilisée pour dessiner cette incrustation.
		FontTexture const * m_fontTexture{ nullptr };
		//! Le statut de visibilité.
		bool m_visible{ true };
	};
}

#endif
