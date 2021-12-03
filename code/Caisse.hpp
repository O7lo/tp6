#pragma once
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QObject>
#pragma pop()
#include <functional>

using namespace std; // Dans ce cours on accepte le using namespace std dans le .hpp .

class Caisse : public QObject {
	Q_OBJECT
public:
	Caisse() = default;

	int obtenirValeur() { return valeur_; }

public slots:
	//void changerValeur(int valeur);
	//void effacer();
	//void ajouterChiffre(int chiffre);
	void changerOperation(const function<int(int,int)>& operation);
	void operationAjouter();
	void operationRetirer();
	void operationReset();
	void effectuerOperation();

signals:
	void totalChange(int valeur);

private:
	static int Ajouter (int x, int y);
	static int Retirer(int x, int y);
	static int reset(int,int);

	bool estResultat_ = true;
	int valeur_ = 0;
	int resultatPrecedent_ = 0;
	function<int(int,int)> operation_;
};
