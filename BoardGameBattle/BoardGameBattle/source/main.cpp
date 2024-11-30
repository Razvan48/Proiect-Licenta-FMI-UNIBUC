#include "Game/Game.h"

int main()
{
    Game::get().start();

	// TODO: de verificat ca attack zone-urile pt sliding pieces (linie, coloana, ambele diagonale), cat si log-ul sunt facute corect

	// TODO: generare de mutari / mutari legale / mutari ilegale
	// TODO: implementare BOT pentru Singleplayer

	// TODO: constructor copiere si initializare cu string pt configurationMetadata si initializare simpla (va trebui asa ca sa putem face multithreading)
	// de adaugat ulterior promovare pion, en passant, castling

	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



