

#include "CaisseWindow.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QString>
#include <QVariant>
#include <QLineEdit>
#pragma pop()
#include <iostream>
#include <type_traits>
#include <cppitertools/range.hpp>

using iter::range;

template <typename T>
QPushButton* CaisseWindow::nouveauBouton(const QString& text, const T& slot)
{
	auto bouton = new QPushButton(this);
	bouton->setText(text);
	bouton->setFixedSize(100, 50);
	if constexpr (!is_same_v<T, decltype(nullptr)>)
		QObject::connect(bouton, &QPushButton::clicked, &Caisse_, slot);
	return bouton;	
}

CaisseWindow::CaisseWindow(QWidget* parent) :
	QMainWindow(parent)
{
	// Si on objet n'a pas encore de parent on lui met "this" comme parent en attendant, si possible, pour s'assurer que tous les pointeurs sont gérés par un delete automatique en tout temps sans utiliser de unique_ptr.
	auto widgetPrincipal = new QWidget(this);
	auto layoutPrincipal = new QVBoxLayout(widgetPrincipal);  // Donner un parent à un layout est comme un setLayout.

	// Pour faire la série de 10 boutons qui doivent tous appeler la même fonction du modèle mais avec des valeurs différentes d'argument, voici trois manières de le faire:
	
	
	// Version avec QButtonGroup:
	{
		auto layout = new QHBoxLayout();
		layoutPrincipal->addLayout(layout);

		layout->setSpacing(1);
		//auto groupeBoutons = new QButtonGroup(this);
		//for (int i : range(10)) {
		//	auto bouton = nouveauBouton(QString::number(i));
		//	groupeBoutons->addButton(bouton, i); // L'ID du bouton est i (doit être un entier).
		//	layout->addWidget(bouton);
		//}

		layout->addWidget(nouveauBouton("Ajouter article", &Caisse::operationPlus));
		layout->addWidget(nouveauBouton("Retirer article"));
		layout->addWidget(nouveauBouton("Reinitialiser list"));
		
		#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)  // Le nom du signal idClicked existe depuis Qt 5.15
		//QObject::connect(groupeBoutons, &QButtonGroup::idClicked, &Caisse_, &Caisse::ajouterChiffre); // ajouterChiffre prend un int, donc le ID du bouton est bon directement.
		#else
		QObject::connect(groupeBoutons, SIGNAL(buttonClicked(int)), &Caisse_, SLOT(ajouterChiffre(int)));
		#endif

		/*layout->addSpacing(10);
		layout->addWidget(nouveauBouton("+", &Caisse::operationPlus));
		layout->addWidget(nouveauBouton("-", &Caisse::operationMoins));
		layout->addWidget(nouveauBouton("=", &Caisse::operationEgal));*/

		layout->addSpacing(10);
		auto label = new QLabel(this);
		affichage_ = label;
		label->setMinimumWidth(100);
		QObject::connect(&Caisse_, &Caisse::valeurChangee, this, &CaisseWindow::changerValeurAffichee);
		layout->addWidget(label);

	}
	
	 /*------------------------------------------------------------
	 Version avec setProperty:*/
	{
		auto layout = new QHBoxLayout();
		layoutPrincipal->addLayout(layout);
	
		


		auto nomArticle = new QLineEdit(this);
		nomArticle->setFixedSize(300, 50);
		nomArticle->setPlaceholderText("Description de l'article");
		layout->addWidget(nomArticle);

		layout->addSpacing(10);
		auto prixArticle = new QLineEdit(this);
		prixArticle->setPlaceholderText("Prix de l'article");
		prixArticle->setFixedSize(100, 50);
		layout->addWidget(prixArticle);


		//layout->setSpacing(0);
		//for (int i : range(10)) {
		//	auto bouton = nouveauBouton(QString::number(i));
		//	// On donne un nom à la propriété, et on lui donne une valeur QVariant (comme dans les notes de cours) d'un type quelconque (doit enregistrer le type avec Q_DECLARE_METATYPE(LeType) si ce n'est pas un type déjà connu de Qt).
		//	bouton->setProperty("chiffre", QVariant::fromValue<int>(i));
		//	QObject::connect(bouton, &QPushButton::clicked, this, &CaisseWindow::chiffreAppuye);
		//	layout->addWidget(bouton);
		//}

		//layout->addSpacing(10);
		//layout->addWidget(nouveauBouton("+", &Caisse::operationPlus));
		//layout->addWidget(nouveauBouton("-", &Caisse::operationMoins));
		//layout->addWidget(nouveauBouton("=", &Caisse::operationEgal));

		//// On ne met pas un autre affichage, on en a déjà deux versions différentes.
		//layout->addSpacing(110);
	}

	setCentralWidget(widgetPrincipal);
	setWindowTitle("Caisse enregistreuse");
}


// Pour la version QButtonGroup.
// Pourrait aussi être sans paramètre et faire Caisse_.obtenirValeur()
void CaisseWindow::changerValeurAffichee(int valeur)
{
	affichage_->setText(QString::number(valeur));
}


// Pour la version setProperty.
void CaisseWindow::chiffreAppuye()
{
	// QObject::sender() est l'objet d'où vient le signal connecté à ce slot; attention qu'il sera nullptr si le slot est appelé directement au lieu de passer par un signal.
	Caisse_.ajouterChiffre(QObject::sender()->property("chiffre").value<int>());
}
