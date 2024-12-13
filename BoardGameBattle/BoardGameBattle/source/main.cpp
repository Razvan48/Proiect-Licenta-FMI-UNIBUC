#include "Game/Game.h"

int main()
{
    Game::get().start();

	// la sliding pieces putem calcula tot attack zone-ul de o data si apoi filtram cu mastile de biti in caz ca piesa e pinuita si/sau nu e in king defense zone (pare cel mai simplu si eficient asa)

	// cred ca numPiecesAttackingWhiteKing si numPiecesAttackingBlackKing sunt redundante
	// apply internal move mai poate fi optimizat (ar fi bine, ca va fi apelat in minmax)
	// validWhitePawns si validBlackPawns sunt redundante, nu mai trb & negat, ca nu se va ajunge intr-o situatie de genul niciodata

	// buton de undo in singleplayer

	// cand are loc o mutare de vazut acest lucru pe tabla de joc

	// de schimbat sunetul de button hovered



	// promovarile se pot face si prin atacuri de pioni

	// singurul loc unde nu dam referinta e cand avansam in minimax, acolo facem o copie a structurii de config



	// TODO: implementare BOT pentru Singleplayer





	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



