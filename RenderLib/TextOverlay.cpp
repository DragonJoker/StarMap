#include "TextOverlay.h"

#include "OverlayRenderer.h"
#include "StringUtils.h"

#include <algorithm>

#include <GlLib/glm/gtc/matrix_transform.hpp>

namespace render
{
	TextOverlay::TextOverlay()
	{
	}

	TextOverlay::~TextOverlay()
	{
	}

	void TextOverlay::update()
	{
		assert( m_fontTexture );

		if ( !m_currentCaption.empty() )
		{
			if ( m_positionChanged )
			{
				doUpdatePosition();
			}

			if ( m_textChanged )
			{
				doUpdateBuffer();
			}
		}
	}

	TextOverlay::DisplayableLineArray TextOverlay::doPrepareText()
	{
		std::vector< std::string > lines = split( m_previousCaption
			, "\n"
			, uint32_t( std::count( m_previousCaption.begin()
				, m_previousCaption.end()
				, '\n' ) + 1 )
			, true );
		DisplayableLineArray ret;
		DisplayableLine line;
		m_computedSize.x = 0;
		m_computedSize.y = 0;

		for ( auto const & lineText : lines )
		{
			int32_t left = 0;
			int32_t wordWidth = 0;
			std::string word;

			for ( auto c : lineText )
			{
				Glyph const & glyph{ m_fontTexture->font()[c] };
				DisplayableChar character{ gl::Position2D{}
					, gl::Size2D{ glyph.advance(), glyph.size().y }
					, glyph };

				if ( glyph.character() == ' '
						|| glyph.character() == '\t' )
				{
					// Write the word and leave space before next word.
					doPrepareWord( word
						, wordWidth
						, left
						, line
						, ret );
					word.clear();
					wordWidth = 0;
					left += character.m_size.x;
				}
				else
				{
					word += glyph.character();
					wordWidth += character.m_size.x;
				}
			}

			if ( !word.empty() )
			{
				doPrepareWord( word
					, wordWidth
					, left
					, line
					, ret );
			}

			doFinishLine( line, left );
			ret.push_back( line );
			m_computedSize.x = std::max( m_computedSize.x, line.m_width );
		}

		for ( auto & line : ret )
		{
			doAlign( line, ret );
			m_computedSize.y = std::max( m_computedSize.y, line.m_height );
		}

		return ret;
	}

	void TextOverlay::doPrepareWord( std::string const & word
		, int32_t wordWidth
		, int32_t & left
		, DisplayableLine & line
		, DisplayableLineArray & lines )
	{
		for ( auto character : word )
		{
			Glyph const & glyph = m_fontTexture->font()[character];
			gl::Size2D charSize( glyph.advance(), glyph.size().y );

			left += glyph.bearing().x;

			if ( charSize.x > 0 )
			{
				line.m_characters.push_back( { gl::Position2D{ left, 0 }, charSize, glyph } );
			}

			left += charSize.x;
		}
	}

	void TextOverlay::doFinishLine
		( DisplayableLine & line
		, int32_t & left )
	{
		line.m_height = 0;
		int32_t maxBottomBearing{ 0 };

		for ( auto const & c : line.m_characters )
		{
			auto bottomBearing = std::max( 0
				, c.m_glyph.size().y - c.m_glyph.bearing().y );
			line.m_height = std::max( line.m_height
				, c.m_size.y + bottomBearing );
			maxBottomBearing = std::max( maxBottomBearing, bottomBearing );
		}

		for ( auto & c : line.m_characters )
		{
			c.m_position.y = std::max( 0
				, maxBottomBearing + c.m_glyph.bearing().y );
		}

		line.m_width = left;
		left = 0;
	}

	void TextOverlay::doAlign( DisplayableLine line
		, DisplayableLineArray & lines )
	{
		// Move letters according to halign
		if ( m_hAlign != HAlign::eLeft )
		{
			auto offset = m_computedSize.x - line.m_width;

			if ( m_hAlign == HAlign::eCenter )
			{
				offset /= 2;
			}

			for ( auto & c : line.m_characters )
			{
				c.m_position.x += offset;
			}
		}
	}

	void TextOverlay::doUpdatePosition()
	{
		if ( m_positionChanged )
		{
			m_transform = gl::Matrix4x4{ 1 };
			m_transform = glm::translate( m_transform
				, gl::Vector3D{ m_position.x, m_position.y, 0 } );
			m_positionChanged = false;
		}
	}

	void TextOverlay::doUpdateBuffer()
	{
		if ( m_textChanged )
		{
			m_previousCaption = m_currentCaption;
			m_vertex.clear();
			m_vertex.reserve( m_previousCaption.size() * 6 );

			DisplayableLineArray lines = doPrepareText();
			gl::Vector2D texDim = m_fontTexture->texture().dimensions();

			for ( auto const & line : lines )
			{
				auto const height = line.m_height;

				for ( auto const & c : line.m_characters )
				{
					auto topCrop = std::max( 0
						, -line.m_position.y - ( height - c.m_glyph.bearing().y ) );
					auto bottomCrop = std::max( 0
						, line.m_position.y + ( height - c.m_position.y ) + c.m_size.y - m_computedSize.y );

					//
					// Compute Letter's Position.
					//
					auto const topUncropped = line.m_position.y + height - c.m_position.y;
					auto const left = c.m_position.x + line.m_position.x;
					auto const top = std::max( 0, int32_t( topUncropped + topCrop ) );
					auto const right = left + c.m_size.x;
					auto const bottom = int32_t( std::min( topUncropped + c.m_size.y - bottomCrop, m_computedSize.y ) );

					//
					// Compute Letter's Font UV.
					//
					auto const fontUvTopCrop = topCrop / texDim.y;
					auto const fontUvBottomCrop = bottomCrop / texDim.y;
					gl::Position2D const fontUvPosition = m_fontTexture->glyphPosition( c.m_glyph.character() );
					auto const fontUvTopUncropped = fontUvPosition.y / texDim.y;
					auto const fontUvLeft = fontUvPosition.x / texDim.x;
					auto const fontUvRight = fontUvLeft + ( c.m_size.x / texDim.x );
					// The UV is vertically inverted since the image is top-bottom, and the overlay is drawn bottom-up.
					auto const fontUvBottom = fontUvTopUncropped + fontUvBottomCrop;
					auto const fontUvTop = fontUvTopUncropped + ( c.m_size.y / texDim.y ) - fontUvTopCrop;

					//
					// Fill buffer
					//
					TextOverlay::Vertex const l_vertexTR = { { right, top },{ fontUvRight, fontUvTop } };
					TextOverlay::Vertex const l_vertexTL = { { left,  top },{ fontUvLeft,  fontUvTop } };
					TextOverlay::Vertex const l_vertexBL = { { left,  bottom },{ fontUvLeft,  fontUvBottom } };
					TextOverlay::Vertex const l_vertexBR = { { right, bottom },{ fontUvRight, fontUvBottom } };

					m_vertex.push_back( l_vertexBL );
					m_vertex.push_back( l_vertexBR );
					m_vertex.push_back( l_vertexTL );

					m_vertex.push_back( l_vertexTR );
					m_vertex.push_back( l_vertexTL );
					m_vertex.push_back( l_vertexBR );
				}
			}

			m_textChanged = false;
		}
	}
}
