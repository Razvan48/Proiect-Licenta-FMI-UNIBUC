#include "Game/Game.h"

int main()
{
    Game::get().start();
	

	// de verificat corectitudinea la min-max + alpha-beta si la multithreading-ul de la getBestMove

	// inca nu se genereaza toate mutarile corect...

	// de inteles alpha-beta + de inteles futures/promises + multithreading

	// de adaugat un check sound in loc de move sound-ul obisnuit (schimbam in metoda applyMoveExternal) + (un capture sound?)

	// de documentat tot ce am facut (ca pare ca merge, de tinut minte pe viitor)

	// apply internal move mai poate fi optimizat (ar fi bine, ca va fi apelat in minmax)
	// validWhitePawns si validBlackPawns sunt redundante, nu mai trb & negat, ca nu se va ajunge intr-o situatie de genul niciodata

	// buton de undo in singleplayer

	// cand are loc o mutare de vazut acest lucru pe tabla de joc




	// pentur butonul de back trb marcat pe false agentul de minmax, ca sa fie board visualizer ca nu mai ruleaza (chiar daca el ruleaza detasat in background)







	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



