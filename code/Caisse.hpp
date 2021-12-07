#pragma once
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QObject>
#pragma pop()
#include <functional>
#include <iostream>
#include <vector>
#include <QString>

using namespace std;

struct Article {
	const string nom;
	double prix;
	bool taxable;

	QString toQString() {
		string tax = "";
		string article;
		int prix100 = round(prix * 100);
		string str = to_string(prix100);
		string prixAffichable = str.substr(0, str.length() - 2) + "." + str.substr(str.length() - 2, str.length());
		if (taxable) {tax = "taxable";}
		article = nom + "\t" + prixAffichable + "\t" + tax;
		return QString(article.c_str());
	}
};

using namespace std; // Dans ce cours on accepte le using namespace std dans le .hpp .

class Caisse : public QObject {
	Q_OBJECT
public:
	Caisse() = default;

	vector<Article> getArticles() { return articles_; }

public slots:
	//void changerValeur(int valeur);
	//void effacer();
	//void ajouterChiffre(int chiffre);
	//void changerOperation(const function<int(int,int)>& operation);
	//void operationAjouter();
	//void operationRetirer();
	//void operationReset();
	//void effectuerOperation();
	void ajouter(QString description,double prix, bool taxable);
	void retirer();
	void reset();
	//void ajouterArticle(Article& article);

signals:
	void valeurChangee(int valeur);
	void vecteurModifie();

private:
	vector<Article> articles_;

	/*bool estResultat_ = true;
	int valeur_ = 0;
	int resultatPrecedent_ = 0;
	function<int(int,int)> operation_;*/
};
