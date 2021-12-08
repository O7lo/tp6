#pragma once
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QObject>
#pragma pop()
#include <functional>
#include <iostream>
#include <vector>
#include <QString>

using namespace std;

inline std::string doubleVersString(double valeur) {
	double valeur100 = round(valeur * 100)/100;
	std::string str = to_string(valeur100);
	std::string valeurString = "0";
		valeurString = str.substr(0, str.find(".")+3);
	return valeurString;
}

struct Article {
	const string nom;
	double prix;
	bool taxable;

	QString toQString() {
		string tax = "";
		string article;
		string prixAffichable = doubleVersString(prix);
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
	
	void ajouter(QString description,double prix, bool taxable);
	void retirer();
	void reset();
	void calculerTotaux();

signals:
	void valeurChangee(int valeur);
	void vecteurModifie();
	void totauxModifies(QString sousTotal, QString taxes, QString total);

private:
	vector<Article> articles_;
	double sousTotal_ = 0;
};
