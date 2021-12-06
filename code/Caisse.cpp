

#include "Caisse.hpp"

// slots:

void Caisse::changerValeur(int valeur) {
	if (valeur != valeur_) {
		valeur_ = valeur;
		emit valeurChangee(valeur_);
	}
}
void Caisse::effacer() {
	operation_ = egal; changerValeur(0);
}
void Caisse::ajouterChiffre(int chiffre) {
	if (estResultat_) {
		valeur_ = 0;
		estResultat_ = false;
	}
	changerValeur(valeur_ * 10 + chiffre);
}

void Caisse::changerOperation(const function<int(int, int)>& operation) {
	if (!estResultat_)
		effectuerOperation();
	operation_ = operation;
}
	
void Caisse::operationPlus()  { changerOperation(plus); }
void Caisse::operationMoins() { changerOperation(moins); }
void Caisse::operationEgal()  { changerOperation(egal); }
void Caisse::operationReset()	{ changerOperation(reset); }


void Caisse::effectuerOperation() {
	resultatPrecedent_ = operation_(resultatPrecedent_, valeur_);
	estResultat_ = true;
	changerValeur(resultatPrecedent_);
}
void Caisse::ajouterArticle(Article& article) {
	article_.push_back(article);
	emit ArticleAjoutee(article);
}


// Fonctions pour l'opération:
// (on aurait pu à la place mettre des lambdas dans operationPlus() ...)

int Caisse::plus (int x, int y) { return x + y; }
int Caisse::moins(int x, int y) { return x - y; }
int Caisse::egal (int  , int y) { return y; }
int Caisse::reset(int,int) { return 0; }
