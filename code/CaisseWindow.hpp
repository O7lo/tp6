#pragma once
// La Vue-Controlleur pour Caisseulatrice simple.
// Par Francois-R.Boyer@PolyMtl.ca

#include "Caisse.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QLabel>
#pragma pop()

class CaisseWindow : public QMainWindow {
	Q_OBJECT

public:
	CaisseWindow(QWidget* parent = nullptr);
	~CaisseWindow() override = default;

public slots:
	//void changerValeurAffichee(int valeur);  // Pour la version QButtonGroup.
	//void chiffreAppuye(); //QAbstractButton* bouton);  // Pour la version setProperty.

private:
	template <typename T = decltype(nullptr)>
	QPushButton* nouveauBouton(const QString& text, const T& slot = nullptr);

	Caisse Caisse_;  // Le Modèle (pourrait être un pointeur mais pas nécessaire dans ce cas).

	QLabel* affichage_;  // Pour la version QButtonGroup.
};
