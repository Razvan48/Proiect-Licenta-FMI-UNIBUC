#include "Game/Game.h"

int main()
{
    Game::get().start();

	// regii nu mai pot ataca piesa care ii ataca acuma, de rezolvat (din cauza la boolene-le alea 4 adaugate)
	// de asemenea, exista cazuri cand o piesa e aparata de alta piesa si teoretic regii nu o pot captura dar aplicatia nu observa asta

	// en passant-ul de asemenea nu tine cont de king defense zone

	// posibile solutii:
	// de inteles faptul ca attack zone-urile sunt utile doar regelui, atat, deci putem altera putin ce e in ele fara sa patim nimic
	// trebuie ca piesele cand genereaza attack zone-urile sa extinda putin aceste attack zone-uri, de exemplu pionii au attack zone si stanga si dreapta chiar daca nu e o piesa inamica acolo
	// sliding piece-urile sa includa si ultimul bit chiar daca e piesa de aceeasi culoare, ca asa simbolizam faptul ca daca piesa aia ar fi capturata piesa ce a capturat ar putea fi atacata

	// e o problema cand regele e in sah de un sliding piece, inca poate da inapoi, chiar daca e o mutare ilegala
	// se poate rezolva precalculand nearest bits si pt coloana si diagonale si apoi pt regele pin-ul la rege va avea alta conotatie, dar va merge

	// cred ca numPiecesAttackingWhiteKing si numPiecesAttackingBlackKing sunt redundante
	// apply internal move mai poate fi optimizat (ar fi bine, ca va fi apelat in minmax)
	// validWhitePawns si validBlackPawns sunt redundante, nu mai trb & negat, ca nu se va ajunge intr-o situatie de genul niciodata

	// buton de undo in singleplayer

	// cand are loc o mutare de vazut acest lucru pe tabla de joc

	// de schimbat sunetul de button hovered




	// singurul loc unde nu dam referinta e cand avansam in minimax, acolo facem o copie a structurii de config



	// TODO: implementare BOT pentru Singleplayer





	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



