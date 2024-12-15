#include "Game/Game.h"

int main()
{
    Game::get().start();
	

	// de verificat corectitudinea la min-max + alpha-beta si la multithreading-ul de la getBestMove

	// inca nu se genereaza toate mutarile corect...

	// de inteles alpha-beta + de inteles futures/promises + multithreading

	// metoda de getBestMove pt GameAgent (clasa abstracta) ar fi mai bine sa fie o functie void si numita probabil altfel, care doar porneste pe alt thread cautarea la best move
	// apoi avem un boolean + mutex care ne spune cand e gata si apoi clasa tine intern raspunsul si il colectam cand avem nevoie

	// generateMovesForPiecePosition (din board visualizer) va genera mutari pt jucator doar daca e randul lui si jucatorul a apasat pe o celula unde e o piesa de culoarea lui (nu gol sau culoare opusa)
	// nu exista mutari care au tile de inceput si tile de sfarsit acelasi tile (si board visualizer tine cont de acest lucru, deci e ok)

	// de adaugat un check sound in loc de move sound-ul obisnuit (schimbam in metoda applyMoveExternal)

	// de populat position scores tables la agent

	// de documentat tot ce am facut (ca pare ca merge, de tinut minte pe viitor)

	// apply internal move mai poate fi optimizat (ar fi bine, ca va fi apelat in minmax)
	// validWhitePawns si validBlackPawns sunt redundante, nu mai trb & negat, ca nu se va ajunge intr-o situatie de genul niciodata

	// buton de undo in singleplayer

	// cand are loc o mutare de vazut acest lucru pe tabla de joc

	// de schimbat sunetul de button hovered







	// TODO: implementare BOT pentru Singleplayer





	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



