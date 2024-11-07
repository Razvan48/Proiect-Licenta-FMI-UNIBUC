#include "Game/Game.h"

int main()
{
    Game::get().start();

    // TODO: conexiunea cu serverul?
    // TODO: de mutat logica de server/singleton in singleplayervisualinterface si multiplayervisualinterface
    // TODO: logica pentru conectare client la server + transmisie mesaje sa fie mutata din game in interfete

    // TODO: problema: cand schimbam interfata vizuala intr-una de game aceasta trebuie sa incarce si board visualizer-ul,
    // dar are la dispozitie un singur apel de update() (care a fost executat de interfata precedenta), urmat imediat de un draw(), neavand
    // board visualizer-ul pregatit...

	return 0;
}



