

#include "CaisseWindow.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QString>
#include <QVariant>
#include <QCheckBox>
#include <QSplitter>
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
	bouton->setFixedHeight(40);
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

	//layout principal : splitter permet à l'utilisateur de modifier la taille des widgets de droite vs ceux de gauche
	auto splitter = new	QSplitter(this);
	splitter->setHandleWidth(1);
	widgetPrincipal->setStyleSheet("QSplitter::handle { background-color: black }");
	layoutPrincipal->addWidget(splitter);

	auto widgetGauche = new QWidget(splitter);
	auto widgetDroite = new QWidget(splitter);
	auto layoutGauche = new QVBoxLayout(widgetGauche);
	auto layoutDroite = new QVBoxLayout(widgetDroite);

	//layout de droite : ajout d'un widget bidon pour permettre aux elements de rester dans le haut de la fenêtre
	auto widgetEntree = new QWidget(this);
	auto widgetBidon = new QWidget(this);
	layoutDroite->addWidget(widgetEntree);
	layoutDroite->addWidget(widgetBidon);
	widgetEntree->setMaximumHeight(300);
	auto layoutEntree = new QVBoxLayout(widgetEntree);

	//layout pour les entrées utilisateur
	layoutEntree->addWidget(nouveauBouton("Ajouter article"/*, &Caisse::operationAjouter*/));
	layoutEntree->addWidget(nouveauBouton("Retirer article"/*,&Caisse::operationRetirer*/));
	layoutEntree->addWidget(nouveauBouton("Tout réinitialiser",&Caisse::operationReset));
	auto layoutLineEdits = new QHBoxLayout();
	layoutEntree->addLayout(layoutLineEdits);
	QCheckBox* taxable = new QCheckBox("&Taxable", this);
	layoutEntree->addWidget(taxable);

//#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)  // Le nom du signal idClicked existe depuis Qt 5.15
//	QObject::connect(groupeBoutons, &QButtonGroup::idClicked, &Caisse_, &Caisse::ajouterChiffre); // ajouterChiffre prend un int, donc le ID du bouton est bon directement.
//#else
//	QObject::connect(groupeBoutons, SIGNAL(buttonClicked(int)), &Caisse_, SLOT(ajouterChiffre(int)));
//#endif

	/*layoutDroite->addSpacing(10);
	auto label = new QLabel(this);
	affichage_ = label;
	label->setMinimumWidth(100);
	QObject::connect(&Caisse_, &Caisse::valeurChangee, this, &CaisseWindow::changerValeurAffichee);
	layoutDroite->addWidget(label);*/

	
	//layout pour la description et le prix de l'article
	auto nomArticle = new QLineEdit(this);
	nomArticle->setFixedHeight(30);
	nomArticle->setPlaceholderText("Description de l'article");
	layoutLineEdits->addWidget(nomArticle);
	layoutLineEdits->addSpacing(10);
	auto prixArticle = new QLineEdit(this);
	prixArticle->setPlaceholderText("Prix de l'article");
	prixArticle->setFixedHeight(30);
	prixArticle->setMaximumWidth(75);
	layoutLineEdits->addWidget(prixArticle);




	
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

	//layout de gauche
	auto listeArticles = new QListWidget(this);
	layoutGauche->addWidget(listeArticles);
	new QListWidgetItem(tr("Hazel"), listeArticles);

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
//void CaisseWindow::changerValeurAffichee(int valeur)
//{
//	affichage_->setText(QString::number(valeur));
//}


// Pour la version setProperty.
//void CaisseWindow::chiffreAppuye()
//{
//	// QObject::sender() est l'objet d'où vient le signal connecté à ce slot; attention qu'il sera nullptr si le slot est appelé directement au lieu de passer par un signal.
//	Caisse_.ajouterChiffre(QObject::sender()->property("chiffre").value<int>());
//}
