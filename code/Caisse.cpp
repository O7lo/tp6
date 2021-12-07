

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
//	resultatPrecedent_ = operation_(resultatPrecedent_, valeur_);
//	estResultat_ = true;
//	changerValeur(resultatPrecedent_);
//}
//void Caisse::ajouterArticle(Article& article) {
//	
//	
//}


// Fonctions pour l'opération:
// (on aurait pu à la place mettre des lambdas dans operationPlus() ...)
void Caisse::ajouter (QString description,double prix, bool taxable) {

	articles_.push_back(Article(description.toStdString(),prix,taxable));
	emit vecteurModifie();
}

void Caisse::retirer() {
	//element de la liste
}
void Caisse::reset() {
	//vider la liste et les zones de texte
}

void Caisse::calculerTotaux() {
	sousTotal_ = 0;
	for (Article article : articles_) {
		sousTotal_ += article.prix;
	}
	double taxes = 0;
	std::for_each	(articles_.begin(),
					articles_.end(), 
					[&](const Article& article) { taxes+=article.taxable*0.14975*article.prix; });
	double total = sousTotal_ + taxes;
	emit totauxModifies(QString(doubleVersString(sousTotal_).c_str()),
						QString(doubleVersString(taxes).c_str()),
						QString(doubleVersString(total).c_str()));
}
