

#include "CaisseWindow.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QString>
#include <QVariant>
#include <QSplitter>
#pragma pop()
#include <iostream>
#include <type_traits>
#include <vector>
#include <cppitertools/range.hpp>

using iter::range;



template <typename T>
QPushButton* CaisseWindow::nouveauBouton(const QString& text, const T& slot)
{
	auto bouton = new QPushButton(this);
	bouton->setText(text);
	bouton->setFixedHeight(40);
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
	
	auto boutonAjouter = new QPushButton(this);
	layoutEntree->addWidget(boutonAjouter);
	boutonAjouter->setText("Ajouter article");
	boutonAjouter->setFixedHeight(40);
	

	layoutEntree->addWidget(nouveauBouton("Retirer article",&Caisse::retirer));
	layoutEntree->addWidget(nouveauBouton("Tout réinitialiser",&Caisse::reset));
	auto layoutLineEdits = new QHBoxLayout();
	layoutEntree->addLayout(layoutLineEdits);
	QCheckBox* taxable = new QCheckBox("&Taxable", this);
	taxable_ = taxable;
	layoutEntree->addWidget(taxable);
	//QObject::connect(taxable, &QCheckBox::isChecked, &Caisse_, slot);

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
	nomArticle_ = nomArticle;
	nomArticle->setFixedHeight(30);
	nomArticle->setPlaceholderText("Description de l'article");
	//nomArticle->setDisabled(true);
	layoutLineEdits->addWidget(nomArticle);
	layoutLineEdits->addSpacing(10);
	auto prixArticle = new QLineEdit(this);
	prixArticle_ = prixArticle;
	prixArticle->setPlaceholderText("Prix de l'article");
	prixArticle->setFixedHeight(30);
	prixArticle->setMaximumWidth(75);
	prixArticle->setValidator(new QDoubleValidator(this));
	layoutLineEdits->addWidget(prixArticle);

	QObject::connect(boutonAjouter, &QPushButton::clicked, this, &CaisseWindow::envoyerNouvelArticle);
	QObject::connect(this, &CaisseWindow::nouvelArticle, &Caisse_, &Caisse::ajouter);
	QObject::connect(&Caisse_, &Caisse::vecteurModifie, this, &CaisseWindow::rafraichirArticles );

	
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
	vector<const char*> Article;
	Article.push_back("banana");
	Article.push_back("orange");
	Article.push_back("honey");
	auto listeArticles = new QListWidget(this);
	listeArticles_ = listeArticles;
	layoutGauche->addWidget(listeArticles);
	
	
	

	QLabel* totalAvantTaxe = new QLabel("Total avant taxes: ", this);
	layoutGauche->addWidget(totalAvantTaxe);

	QLabel* totalTaxes = new QLabel("Total des taxes: ", this);
	layoutGauche->addWidget(totalTaxes);

	QLabel* total = new QLabel("Total a payer: ", this);
	layoutGauche->addWidget(total);


	setCentralWidget(widgetPrincipal);
	setWindowTitle("Caisse enregistreuse");
}

void CaisseWindow::envoyerNouvelArticle() {
	/*probleme de compatibilité entre QDoubleValidator et QString::toDouble().
	merci à Cory Quammen pour la solution trouvée en ligne : https://gitlab.kitware.com/paraview/paraview/-/issues/15786
	---------------------------------------------------*/
	QLocale oL;
	double dValue = oL.toDouble(CaisseWindow::prixArticle_->text());
	//--------------------------------------------------
	emit nouvelArticle	(CaisseWindow::nomArticle_->text(),
						dValue,
						CaisseWindow::taxable_->checkState());
}

void CaisseWindow::rafraichirArticles() {
	vector<Article> articles = Caisse_.getArticles();
	CaisseWindow::listeArticles_->clear();
	for (Article article : articles) {
		CaisseWindow::listeArticles_->addItem(article.toQString());
	}
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
