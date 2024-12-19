#include "Game/Game.h"

int main()
{
    Game::get().start();

	// hyperparameter tuning pentru attack zones (pare ca ii pasa prea mult de ele si pierde piese inutil) (deci sa le scadem putin valoarea)
	// max depth dinamic (daca branching factor-ul e mic, putem sa mergem mai adanc) (sigur va ajuta in endgame, unde branching factor-ul e mic)



	// pentru meniul de pawn promotion: cand convertim din external move in internal, daca external are un al 6-lea caracter, atunci stim ca-i promotion (al 6-lea caracter va fi setat de meniul suplimentar)


	// de adaugat meniu pentru pawn promotion (momentan putem promova doar in regina)

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



