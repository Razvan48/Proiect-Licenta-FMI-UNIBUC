#include "Game/Game.h"

int main()
{
    Game::get().start();

	// TODO: de vazut daca game.setStatus este ok





	// INFO: La BoardManager.cpp de evitat this->configurationMetadata, trebuie doar configurationMetadata, ca sa afecteze doar instanta locala.

	// de facut un agent singlethreaded, ca sa nu mai blocheze dispozitivul cand ruleaza



	// de adaugat un check sound in loc de move sound-ul obisnuit (schimbam in metoda applyMoveExternal) + (un capture sound?)







	// de reinitializat stivele cand reintram intr-un meci!!! (in board visualizer, in initialize) (cred ca e rezolvat, de testat)


	// de documentat tot ce am facut (ca pare ca merge, de tinut minte pe viitor)

	// apply internal move mai poate fi optimizat (ar fi bine, ca va fi apelat in minmax)
	// validWhitePawns si validBlackPawns sunt redundante, nu mai trb & negat, ca nu se va ajunge intr-o situatie de genul niciodata


	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



