#pragma once
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QObject>
#pragma pop()
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

struct Article {
	const string nom;
	double prix;
	bool taxable;
};

using namespace std; // Dans ce cours on accepte le using namespace std dans le .hpp .

class Caisse : public QObject {
	Q_OBJECT
public:
	Caisse() = default;

	int obtenirValeur() { return valeur_; }

public slots:
	void changerValeur(int valeur);
	void effacer();
	void ajouterChiffre(int chiffre);
	void changerOperation(const function<int(int,int)>& operation);
	void operationPlus();
	void operationMoins();
	void operationEgal();
	void operationReset();
	void effectuerOperation();
	void ajouterArticle(Article& article);
signals:
	void valeurChangee(int valeur);
	void articleAjoutee(const Article& article);

private:
	vector<Article> article_;
	
	static int plus (int x, int y);
	static int moins(int x, int y);
	static int egal (int  , int y);
	static int reset(int,int);

	bool estResultat_ = true;
	int valeur_ = 0;
	int resultatPrecedent_ = 0;
	function<int(int,int)> operation_ = egal;
};
