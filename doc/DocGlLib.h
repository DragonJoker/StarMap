/**
*\page gllib GlLib
*\tableofcontents
*\section gllib_intro Introduction
*Il s'agit d'un simple wrapper sur les objets OpenGL de base.\n
*Ses fonctionnalités sont contenues dans l'espace de nommage gl
*\section gllib_content Contenu
*\li \c gl::Buffer : Implémentation d'un tampon VRAM (glGenBuffers, glBindBuffer, ...).
*\li \c gl::ShaderProgram : Implémentation d'un programme shader.
*\li \c gl::Attribute : Implémentation d'un attribut de shader.
*\li \c gl::Uniform : Implémentation d'une variable uniforme de shader.
*\li \c gl::FrameBuffer : Implémentation d'un tampon de frame OpenGL.
*\li \c gl::Texture : Implémentation d'une texture OpenGL.
*
*\section gllib_desc Description
*\subsection gllib_buffer gl::Buffer
*Implémente les fonctions de base d'un tampon OpenGL :
*\li bind / unbind : Pour activer / désactiver le tampon.
*\li resize : Pour redimensionner le tampon.
*\li upload : Pour transférer des données depuis la RAM vers la VRAM.
*\li download : Pour transférer des données depuis la VRAM vers la RAM( seulement en OpenGL ES 3 ).
*\li lock / unlock : Pour mapper le contenu du tampon en RAM( seulement en OpenGL ES 3 ).
*
*gl::Buffer est une classe template, permettant de simplifier son utilisation dans votre code.\n
*Elle utilise en interne la classe gl::BufferBase qui elle contient les appels OpenGL.
*Il existe une fonction d'aide à la création d'attribut : gl::makeBuffer.\n
*\see gl::Buffer
*\see gl::BufferBase
*\subsection gllib_shader gl::ShaderProgram
*Implémente les fonctionnalités de base d'un programme shader :
*\li bind / unbind : Pour activer / désactiver le shader.
*
*\see gl::ShaderProgram
*\subsection gllib_attribute gl::Attribute
*Cette classe est templatée au type de données que contient l'attribut.\n
*Cela permet notamment de détecter au plus tôt les erreurs de constitution d'un attribut.\n
*Les types supportés sont donc :
*\li \c int8_t
*\li \c int16_t
*\li \c int32_t
*\li \c uint8_t
*\li \c uint16_t
*\li \c uint32_t
*\li \c float
*\li \c glm::vec2
*\li \c glm::vec3
*\li \c glm::vec4
*
*Les attributs peuvent donc être activées et désactivés.\n
*Il est possible d'activer un attribut en relation avec un tampon GPU, ou de l'activer en lui donnant des données.
*\see gl::Attribute
*\see gl::AttributeBase
*\subsection gllib_uniform gl::Uniform
*Il y a deux classes pour décrire les variables uniformes : gl::Uniform et gl::ArrayUniform.\n
*Chacune est une classe templatée au type de données qu'elle peut contenir.\n
*Elles implémentent les fonctionnalités de base d'une variable uniforme :
*\li setValue : Pour définir sa valeur.
*\li bind : Pour transmettre sa valeur au shader actif.
*
*Il existe une fonction d'aide à la création de variable uniforme : gl::makeUniform.\n
*\see gl::Uniform
*\see gl::ArrayUniform
*\see gl::UniformBase
*\subsection gllib_framebuffer gl::FrameBuffer
*Permet d'utiliser les framebuffers OpenGL.\n
*On peut lui attacher un gl::RenderBuffer ou une gl::Texture.\n
*Fournit les fonctionnalités de base :
*\li bind / unbind : Active ou désactive le tampon.
*\li attach / detach : Attache ou détache un gl::RenderBuffer ou une gl::Texture au point d'attache voulu.
*
*\see gl::FrameBuffer
*\subsection gllib_texture gl::Texture
*Permet de définir une texture, ayant le format de pixels et les dimensions souhaités.\n
*Afin de pouvoir l'utiliser correctement, il faut la coupler avec un gl::Sampler ou un gl::SamplerObject.\n
*Fournit les fonctionnalités suivantes :
*\li bind / unbind : Pour activer / désactiver la texture, sur l'unité de texture donnée.
*\li upload : Pour transférer des données depuis la RAM vers la VRAM.
*\li generateMipmaps : Génère les mipmaps.
*
*\see gl::PixelFormat
*\see gl::Sampler
*\see gl::SamplerObject
*/
