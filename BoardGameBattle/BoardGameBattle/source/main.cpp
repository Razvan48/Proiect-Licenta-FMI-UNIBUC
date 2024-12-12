#include "Game/Game.h"

int main()
{
    Game::get().start();

	// de vazut daca conversiile internal - external move sunt ok facute

	// de schimbat sunetul de button hovered

	// cand are loc o mutare de vazut acest lucru pe tabla de joc



	// promovarile se pot face si prin atacuri de pioni

	// singurul loc unde nu dam referinta e cand avansam in minimax, acolo facem o copie a structurii de config



	// TODO: implementare BOT pentru Singleplayer





	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



