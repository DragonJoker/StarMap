#include "GlPipeline.h"

namespace gl
{
	Pipeline::Pipeline( bool culling
		, bool depthTest
		, bool depthWrite
		, bool blending )noexcept
		: m_culling{ culling }
		, m_depthTest{ depthTest }
		, m_depthWrite{ depthWrite ? GL_TRUE : GL_FALSE }
		, m_blending{ blending }
	{
	}

	void Pipeline::apply()const noexcept
	{
		if ( m_culling )
		{
			glCheckError( glEnable, GL_CULL_FACE );
		}
		else
		{
			glCheckError( glDisable, GL_CULL_FACE );
		}
		
		if ( m_depthTest )
		{
			glCheckError( glEnable, GL_DEPTH_TEST );
		}
		else
		{
			glCheckError( glDisable, GL_DEPTH_TEST );
		}

		glCheckError( glDepthMask, m_depthWrite );

		if ( m_blending )
		{
			glCheckError( glEnable, GL_BLEND );
			glCheckError( glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glCheckError( glBlendEquation, GL_FUNC_ADD );
		}
		else
		{
			glCheckError( glDisable, GL_BLEND );
		}
	}
}
