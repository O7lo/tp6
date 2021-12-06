#pragma once
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QObject>
#pragma pop()
#include <functional>
#include <iostream>

using namespace std; // Dans ce cours on accepte le using namespace std dans le .hpp .

class Caisse : public QObject {
	Q_OBJECT
public:
	Caisse() = default;

	//int obtenirValeur() { return valeur_; }

public slots:
	//void changerValeur(int valeur);
	//void effacer();
	//void ajouterChiffre(int chiffre);
	//void changerOperation(const function<int(int,int)>& operation);
	//void operationAjouter();
	//void operationRetirer();
	//void operationReset();
	//void effectuerOperation();
	void ajouter(QString description,float prix, bool taxable);
	void description(QString description);
	void prix(QString prix);
	void retirer();
	void reset();

signals:
	void totalChange(int valeur);

private:
	//static int ajouter (int x, int y);
	//static int retirer(int x, int y);
	//static int reset(int,int);

	/*bool estResultat_ = true;
	int valeur_ = 0;
	int resultatPrecedent_ = 0;
	function<int(int,int)> operation_;*/
	QString description_;
	float prix_;
};
