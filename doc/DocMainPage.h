/**
*\mainpage Documentation
*\tableofcontents
*\section features Fonctionnalités
*\li Objets : Il peuvent être définis manuellement via render::Mesh, ou importés aud format OBJ via utils::loadObjFile.
*\li Billboards : Implémentés sans instanciation hardware, afin de les garder compatibles avec OpenGL ES 2.
*\li PolyLines : Implémentées en utilisant des quads, pour un meilleur rendu et des dimensions indépendantes de la plateforme.
*\li Incrustations texte : Utilisent une texture de police commune pouvant être chargée en utilisant FreeType.
*\li Gestion de scène : Ajoutez les objets (billboards, objets ou lignes) à la render::Scene, et ils seront dessinés en respectant leur render::Material.
*
*\section build Générer RenderLib
*Le dépôt contient la solution Visual Studio 2015 Update 3 permettant de générer les projets.\n
*Cette solution contient les projets suivants :
*\li \c GlLib : Un simple wrapper sur les objets OpenGL, voir \ref gllib.
*\li \c RenderLib : Bibliothèque de gestion des objets et du rendu.
*\li \c FreeType : Intégration de freetype, pour la générer sur Android.
*\li \c AndroidUtils : Bibliothèque intermédiaire contenant des fonctions et classes communes aux projets Android.
*\li \c RenderLibTestApp : Application de test, pour valider RenderLib et GlLib.
*
*/
