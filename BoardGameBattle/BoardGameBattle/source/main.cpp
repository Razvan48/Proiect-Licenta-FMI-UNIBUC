#include "Game/Game.h"

int main()
{
    Game::get().start();

	// TODO: de scris extract file bitmask, diagonal bitmask 



	// TODO: de verificat ca attack zone-urile pt sliding pieces (linie, coloana, ambele diagonale), cat si log-ul sunt facute corect

	// TODO: de schimbat attack zone-urile
	// dp[i][j][k] = i pozitie, j e masca pt piesele jucatorului curent pe linie/coloana/diagonala respectiva,
	// j e acelasi lucru dar pt jucatorul inamic
	// dp[i][j][k] = pair(masca, masca) <- masca raw attack zones(nu tin cont daca piesa e pinned) + pinned pieces mask (numai pt piesele adversarului) (pinned fata de orice alta piesa a adversarului)
	// pentru piesele pinned ele pot merge numai pe pozitiile de pe aceeasi linie/coloana/diagonala

	// cum hash-uim in O(1) o linie/coloana/diagonala (nu cred ca se poate, facem in O(8), aproape constant, aia e)

	// la pin nu cred ca trb si stanga/dreapta/sus/jos, e suficient doar linie/coloana/diagonala0/diagonala1

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



