#include "Game/Game.h"

int main()
{
    Game::get().start();

	// pentru meniul de pawn promotion: cand convertim din external move in internal, daca external are un al 6-lea caracter, atunci stim ca-i promotion (al 6-lea caracter va fi setat de meniul suplimentar)


	// de adaugat meniu pentru pawn promotion (momentan putem promova doar in regina)

	// de adaugat un check sound in loc de move sound-ul obisnuit (schimbam in metoda applyMoveExternal) + (un capture sound?)






	// buton de undo in singleplayer

	// de reinitializat stivele cand reintram intr-un meci!!! (in board visualizer, in initialize) (cred ca e rezolvat, de testat)

	// pentur butonul de back trb marcat pe false agentul de minmax, ca sa fie board visualizer ca nu mai ruleaza (chiar daca el ruleaza detasat in background) (aici am putea putin mai bine cu niste boolene pentru thread-uri, sa stie cand sa se opreasca)


	// inca nu se genereaza toate mutarile corect...





	// de verificat corectitudinea la min-max + alpha-beta si la multithreading-ul de la getBestMove


	// de inteles alpha-beta + de inteles futures/promises + multithreading

	// de documentat tot ce am facut (ca pare ca merge, de tinut minte pe viitor)

	// apply internal move mai poate fi optimizat (ar fi bine, ca va fi apelat in minmax)
	// validWhitePawns si validBlackPawns sunt redundante, nu mai trb & negat, ca nu se va ajunge intr-o situatie de genul niciodata


	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



