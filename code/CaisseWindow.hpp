#pragma once
// La Vue-Controlleur pour Caisseulatrice simple.
// Par Francois-R.Boyer@PolyMtl.ca

#include "Caisse.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QListWidget>
#include <QHBoxLayout>
#include <stdexcept>
#pragma pop()

class CaisseWindow : public QMainWindow {
	Q_OBJECT

public:
	CaisseWindow(QWidget* parent = nullptr);
	~CaisseWindow() override = default;

public slots:
	//void changerValeurAffichee(int valeur);  // Pour la version QButtonGroup.
	//void chiffreAppuye(); //QAbstractButton* bouton);  // Pour la version setProperty.
	void envoyerNouvelArticle();
	void envoyerRetirerArticle();
	void envoyerReinitialiser();
	void rafraichirArticles();
	void rafraichirTotaux(QString sousTotal, QString taxes, QString total);
	

signals:
	void reinitialiser();

private:
	template <typename T = decltype(nullptr)>
	QPushButton* nouveauBouton(const QString& text,const CaisseWindow* receiver , const T& slot);

	QHBoxLayout* nouveauLabelTotal(const QString& texte, QLabel*& qlabel);
	QLabel* erreurLabel(const QString& text);

	void effacerErreurs();

	Caisse Caisse_;  // Le Modèle (pourrait être un pointeur mais pas nécessaire dans ce cas).
	QListWidget* listeArticles_;
	QLineEdit* nomArticle_;
	QLineEdit* prixArticle_;
	QCheckBox* taxable_;
	QLabel* totalAvantTaxes_;
	QLabel* totalTaxes_;
	QLabel* total_;
	QPushButton* boutonRetirer_;
	QLabel* erreurRetirer_;
	QLabel* erreurNom_;
	QLabel* erreurPrix_;
};
