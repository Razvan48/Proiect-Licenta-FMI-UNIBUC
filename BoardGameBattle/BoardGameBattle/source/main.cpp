#include "Game/Game.h"

int main()
{
    Game::get().start();

	// de schimbat sunetul de button hovered

	// cand are loc o mutare de vazut acest lucru pe tabla de joc

	// includem si bitul piesei ce ataca regele cand vrem sa aparam regele si regele e atacat de o singura piesa

	// daca regele e atacat de o singura piesa asta include si cal si pion si celalalt rege, nu doar sliding pieces (ba chiar trebuie marcat cu 1 bitul lor)


	// de schimbat configuratia intreaga, nu mai tinem minte daca am facut rocade, tinem minte daca regii + turele s-au mutat de la inceputul jocului!!

	// TODO: de facut toate todo-urile din BoardManager.cpp + nu exista castling si en passant!!!!
	// de pus si turn-ul in configuration metadata si sa am o variabila de configuration metadata in clasa, elimin bool-ul de whites- turn

	// promovarile se pot face si prin atacuri de pioni

	// singurul loc unde nu dam referinta e cand avansam in minimax, acolo facem o copie a structurii de config


	// TODO: de verificat ca attack zone-urile pt sliding pieces (linie, coloana, ambele diagonale), cat si log-ul sunt facute corect

	// TODO: implementare BOT pentru Singleplayer

	// TODO: constructor copiere si initializare cu string pt configurationMetadata si initializare simpla (va trebui asa ca sa putem face multithreading)
	// de adaugat ulterior promovare pion, en passant, castling





	// TODO: este o problema unde daca utilizatorul face hold si nu da drop la fereastra,
	// atunci aplicatia nu mai ruleaza si serverul crede ca a picat client-ul si implicit il deconecteaza.
	// Momentan am presupus ca acel client care creeaza serverul nu ii poate pica conexiunea niciodata,
	// dar asta nu rezolva daca cel ce da join la joc face acelasi lucru cu drag and drop la fereastra.

	return 0;
}



