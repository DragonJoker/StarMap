/**
*\mainpage Documentation
*\tableofcontents
*\section Présentation
*Bibliothèque de gestion d'une carte des étoiles, fonctionnant sous OpenGL ES 2 et 3.
*
*\section subprojects Sous-projets
*\li \c GlLib : Un simple wrapper sur les objets OpenGL, voir \ref gllib.
*\li \c RenderLib : Bibliothèque de gestion des objets et du rendu, voir \ref renderlib.
*\li \c FreeType : Intégration de freetype, pour la générer sur Android.
*\li \c StarMapLib : Bibliothèque de gestion de la carte des étoiles, voir \ref starmaplib.
*\li \c AndroidUtils : Bibliothèque intermédiaire contenant des fonctions et classes communes aux projets Android.
*\li \c DesktopUtils : Bibliothèque intermédiaire contenant des fonctions et classes communes aux projets Windows.
*\li \c RenderLibTestApp : Applications de test (Windows et Android), pour valider RenderLib et GlLib.
*\li \c StarMapLibTestApp : Applications de test (Windows et Android), pour valider StarMapLib.
*
*\subsection feat_renderlib Fonctionnalités de RenderLib
*\li Objets : Il peuvent être définis manuellement via render::Mesh, ou importés aud format OBJ via utils::loadObjFile.
*\li Billboards : Implémentés sans instanciation hardware, afin de les garder compatibles avec OpenGL ES 2.
*\li PolyLines : Implémentées en utilisant des quads, pour un meilleur rendu et des dimensions indépendantes de la plateforme.
*\li Incrustations texte : Utilisent une texture de police commune pouvant être chargée en utilisant FreeType.
*\li Gestion de scène : Ajoutez les objets (billboards, objets ou lignes) à la render::Scene, et ils seront dessinés en respectant leur render::Material.
*
*\subsection feat_starmaplib Fonctionnalités de StarMapLib
*\li Etoiles : Sont affichées via des Billboards.
*\li Constellations : Sont affichées en tant que PolyLines, et ont leur nom affiché.
*\li StarMap : Affiche les étoiles avec un seuil d'affichage en fonction de la magnitude.\n
*Permet de sélectionner des étoiles, dont les informations seront affichées.
*
*\section build Générer StarMapLib
*\subsection build_android Sous Android
*Le dépôt contient la solution Visual Studio 2015 Update 3 permettant de générer les projets.\n
*
*\subsection build_windows Sous Windows
*Une solution CMake est prête ne nécessitant pas de configuration spécifique pour fonctionner.\n
*/
