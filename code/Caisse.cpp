

#include "Caisse.hpp"

//slots:

//void Caisse::changerValeur(int valeur) {
//	if (valeur != valeur_) {
//		valeur_ = valeur;
//		emit valeurChangee(valeur_);
//	}
//}
//void Caisse::effacer() {
//	operation_ = egal; changerValeur(0);
//}
//void Caisse::ajouterChiffre(int chiffre) {
//	if (estResultat_) {
//		valeur_ = 0;
//		estResultat_ = false;
//	}
//	changerValeur(valeur_ * 10 + chiffre);
//}

//void Caisse::changerOperation(const function<int(int, int)>& operation) {
//	if (!estResultat_)
//		effectuerOperation();
//	operation_ = operation;
//}

//void Caisse::operationAjouter()  { changerOperation(Ajouter); }
//void Caisse::operationRetirer() { changerOperation(Retirer); }
//void Caisse::operationReset()	{ changerOperation(reset); }

//void Caisse::effectuerOperation() {
	//resultatPrecedent_ = operation_(resultatPrecedent_, valeur_);
	//estResultat_ = true;
	//changerValeur(resultatPrecedent_);
//}

// Fonctions pour l'opération:
// (on aurait pu à la place mettre des lambdas dans operationPlus() ...)
void Caisse::ajouter (QString description,float prix, bool taxable) {
	std::cout << description.toStdString()<<"\t"<<prix;
	if (taxable) { cout << "\ttaxable"; }
}
void Caisse::description(QString description) {
	description_ = description;
}
void Caisse::prix(QString prix) {
	prix_ = prix.toFloat();
}
void Caisse::retirer() {
	//element de la liste
}
void Caisse::reset() {
	//vider la liste et les zones de texte
}
