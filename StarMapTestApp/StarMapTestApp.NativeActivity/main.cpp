#include "Engine.h"

/**
* Il s'agit du point d'entrée principal d'une application native qui utilise
* android_native_app_glue. Elle s'exécute dans son propre thread, avec sa propre boucle d'évènements
* pour recevoir des évènements d'entrée et effectuer d'autres actions.
*/
void android_main( android_app * state )
{
	Engine engine{ state };

	state->userData = &engine;
	state->onAppCmd = &Engine::handleCommand;
	state->onInputEvent = &Engine::handleInput;

	// Boucle utilisée en attente de tâches à effectuer.

	while ( 1 )
	{
		// Lecture de tous les évènements en attente.
		int ident;
		int events;
		struct android_poll_source* source;

		// Si aucune animation n'a lieu, l'attente d'évènements est bloquée indéfiniment.
		// En cas d'animation, la boucle est répétée jusqu'à ce que tous les évènements soient lus, puis
		// la prochaine image d'animation est dessinée.
		while ( ( ident = ALooper_pollAll( engine.isAnimating() ? 0 : -1
			, nullptr
			, &events
			, reinterpret_cast< void ** >( &source ) ) ) >= 0 )
		{

			// Traitement de cet évènement.
			if ( source )
			{
				source->process( state, source );
			}

			// Traitement d'un capteur s'il possède des données.
			if ( ident == LOOPER_ID_USER )
			{
				engine.handleUserEvents();
			}

			// Vérification de la procédure de sortie.
			if ( state->destroyRequested != 0 )
			{
				engine.cleanup();
				return;
			}
		}

		engine.update();
	}
}
