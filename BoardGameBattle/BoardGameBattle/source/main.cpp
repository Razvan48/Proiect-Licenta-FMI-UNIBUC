#include "Game/Game.h"

int main()
{
    Game::get().start();

	// TODO: generare de mutari / mutari legale / mutari ilegale
	// TODO: implementare BOT pentru Singleplayer

	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



