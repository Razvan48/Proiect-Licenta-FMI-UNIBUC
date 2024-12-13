#include "Game/Game.h"

int main()
{
    Game::get().start();

	// de documentat tot ce am facut (ca pare ca merge, de tinut minte pe viitor)

	// de trecut inca o data prin move creation, (attack zone generation-ul e ok)
	// en passant-ul de asemenea nu tine cont de king defense zone

	// cred ca numPiecesAttackingWhiteKing si numPiecesAttackingBlackKing sunt redundante
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



