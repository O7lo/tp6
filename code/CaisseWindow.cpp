

#include "CaisseWindow.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QString>
#include <QVariant>
#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
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
	auto layoutPrincipal = new QHBoxLayout(widgetPrincipal);  // Donner un parent à un layout est comme un setLayout.


	auto layoutGauche = new QVBoxLayout();
	layoutPrincipal->addLayout(layoutGauche);
	auto layoutBoutons = new QHBoxLayout();
	layoutGauche->addLayout(layoutBoutons);

	layoutBoutons->setSpacing(1);

	layoutBoutons->addWidget(nouveauBouton("Ajouter article", &Caisse::operationPlus));
	layoutBoutons->addWidget(nouveauBouton("Retirer article"));
	layoutBoutons->addWidget(nouveauBouton("Reinitialiser list"));

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)  // Le nom du signal idClicked existe depuis Qt 5.15
	//QObject::connect(groupeBoutons, &QButtonGroup::idClicked, &Caisse_, &Caisse::ajouterChiffre); // ajouterChiffre prend un int, donc le ID du bouton est bon directement.
#else
	QObject::connect(groupeBoutons, SIGNAL(buttonClicked(int)), &Caisse_, SLOT(ajouterChiffre(int)));
#endif

	/*layoutGauche->addSpacing(10);
	auto label = new QLabel(this);
	affichage_ = label;
	label->setMinimumWidth(100);
	QObject::connect(&Caisse_, &Caisse::valeurChangee, this, &CaisseWindow::changerValeurAffichee);
	layoutGauche->addWidget(label);*/

	auto layoutLineEdits = new QHBoxLayout();
	layoutGauche->addLayout(layoutLineEdits);

	auto nomArticle = new QLineEdit(this);
	nomArticle->setFixedSize(300, 50);
	nomArticle->setPlaceholderText("Description de l'article");
	layoutLineEdits->addWidget(nomArticle);

	layoutLineEdits->addSpacing(10);
	auto prixArticle = new QLineEdit(this);
	prixArticle->setPlaceholderText("Prix de l'article");
	prixArticle->setFixedSize(100, 50);
	layoutLineEdits->addWidget(prixArticle);

	auto listeArticles = new QListWidget(this);
	new QListWidgetItem(tr("Hazel"), listeArticles);
	layoutPrincipal->addWidget(listeArticles);
	//listeArticles->set


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


	QCheckBox* taxable = new QCheckBox("&Taxable", this);
	layoutGauche->addWidget(taxable);

	QLabel* totalAvantTaxe = new QLabel("Total avant taxes: ", this);
	layoutGauche->addWidget(totalAvantTaxe);

	QLabel* totalTaxes = new QLabel("Total des taxes: ", this);
	layoutGauche->addWidget(totalTaxes);

	QLabel* total = new QLabel("Total a payer: ", this);
	layoutGauche->addWidget(total);


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
