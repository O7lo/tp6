

#include "CaisseWindow.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
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
QPushButton* CaisseWindow::nouveauBouton(const QString& text,const CaisseWindow* receiver, const T& slot)
{
	auto bouton = new QPushButton(this);
	bouton->setText(text);
	bouton->setFixedHeight(40);
	QObject::connect(bouton, &QPushButton::clicked, receiver, slot);
	return bouton;	
}

QHBoxLayout* CaisseWindow::nouveauLabelTotal(const QString& texte, QLabel*& qlabel) {
	QLabel* total = new QLabel(texte);
	QLabel* valeur = new QLabel("0");
	valeur->setAlignment(Qt::AlignRight);
	qlabel = valeur;
	auto layout = new QHBoxLayout();
	layout->addWidget(total);
	layout->addWidget(valeur);
	return layout;
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
	
	layoutEntree->addWidget(nouveauBouton("Ajouter article", this, &CaisseWindow::envoyerNouvelArticle));
	auto layoutRetirer = new QVBoxLayout();
	layoutRetirer->setSpacing(0);
	boutonRetirer_ = nouveauBouton("Retirer article", this, &CaisseWindow::envoyerRetirerArticle);
	boutonRetirer_->setEnabled(false);
	layoutRetirer->addWidget(boutonRetirer_);
	erreurRetirer_ = new QLabel();
	erreurRetirer_->setFixedHeight(0);
	erreurRetirer_->setText("*Aucun article sélectionné");
	erreurRetirer_ ->setStyleSheet("QLabel { color : red; }");
	layoutRetirer->addWidget(erreurRetirer_);
	layoutEntree->addLayout(layoutRetirer);
	layoutEntree->addWidget(nouveauBouton("Tout réinitialiser",this,&CaisseWindow::envoyerReinitialiser));
	auto layoutLineEdits = new QHBoxLayout();
	layoutEntree->addLayout(layoutLineEdits);
	QCheckBox* taxable = new QCheckBox("&Taxable", this);
	taxable_ = taxable;
	layoutEntree->addWidget(taxable);


	//layout pour la description et le prix de l'article
	auto layoutNom = new QVBoxLayout();
	auto nomArticle = new QLineEdit(this);
	nomArticle_ = nomArticle;
	nomArticle->setFixedHeight(30);
	nomArticle->setPlaceholderText("Description de l'article");
	layoutNom->addWidget(nomArticle);
	layoutNom->setSpacing(0);
	erreurNom_ = new QLabel();
	erreurNom_->setFixedHeight(0);
	erreurNom_->setText("*description manquante");
	erreurNom_->setStyleSheet("QLabel { color : red; }");
	layoutNom->addWidget(erreurNom_);
	layoutLineEdits->addLayout(layoutNom);
	auto layoutPrix = new QVBoxLayout();
	auto prixArticle = new QLineEdit(this);
	prixArticle_ = prixArticle;
	prixArticle->setPlaceholderText("Prix de l'article");
	prixArticle->setFixedHeight(30);
	prixArticle->setMaximumWidth(75);
	prixArticle->setValidator(new QDoubleValidator(this));
	layoutPrix->addWidget(prixArticle);
	layoutPrix->setSpacing(0); layoutRetirer->setSpacing(0);
	erreurPrix_ = new QLabel();
	erreurPrix_->setFixedHeight(0);
	erreurPrix_->setText("*prix manquant");
	erreurPrix_->setStyleSheet("QLabel { color : red; }");
	layoutPrix->addWidget(erreurPrix_);
	layoutLineEdits->addLayout(layoutPrix);
	

	//layout de gauche
	auto listeArticles = new QListWidget(this);
	listeArticles_ = listeArticles;
	layoutGauche->addWidget(listeArticles);
	listeArticles_->setSelectionMode(QAbstractItemView::ExtendedSelection);

	layoutGauche->addLayout(nouveauLabelTotal("Sous-Total: ", totalAvantTaxes_));
	layoutGauche->addLayout(nouveauLabelTotal("Taxes: ", totalTaxes_));
	layoutGauche->addLayout(nouveauLabelTotal("Total: ", total_));

	setCentralWidget(widgetPrincipal);
	setWindowTitle("Caisse enregistreuse");


	//connections
	QObject::connect(this, &CaisseWindow::reinitialiser, &Caisse_, &Caisse::reinitialiser);
	QObject::connect(&Caisse_, &Caisse::vecteurModifie, this, &CaisseWindow::rafraichirArticles );
	QObject::connect(&Caisse_, &Caisse::vecteurModifie, &Caisse_, &Caisse::calculerTotaux);
	QObject::connect(&Caisse_, &Caisse::totauxModifies, this, &CaisseWindow::rafraichirTotaux);

}

void CaisseWindow::envoyerNouvelArticle() {
	effacerErreurs();
	try {
		Caisse_.ajouter(CaisseWindow::nomArticle_->text(),
						CaisseWindow::prixArticle_->text(),
						CaisseWindow::taxable_->checkState());
	}
	catch (const std::invalid_argument& erreur) {
		std::cout << erreur.what();
		if (erreur.what() == QString("description et prix")) {
			erreurNom_->setFixedHeight(15);
			erreurPrix_->setFixedHeight(15);
		}
		else if (erreur.what() == QString("description")) {
			erreurNom_->setFixedHeight(15);
		}
		else if(erreur.what() == QString("prix")){
			erreurPrix_->setFixedHeight(15);
		}
	}
}

void CaisseWindow::envoyerRetirerArticle() {
	effacerErreurs();
	try {
		Caisse_.retirer(listeArticles_->selectedItems());
	}
	catch (const std::invalid_argument& erreur) {
		std::cout<<erreur.what();
		erreurRetirer_->setFixedHeight(15);
	}
}

void CaisseWindow::envoyerReinitialiser() {
	effacerErreurs();
	nomArticle_->clear();
	prixArticle_->clear();
	taxable_->setChecked(false);
	emit reinitialiser();
}

void CaisseWindow::rafraichirArticles() {
	vector<Article> articles = Caisse_.getArticles();
	if (articles.size() == 0) {boutonRetirer_->setEnabled(false);}
	else boutonRetirer_->setEnabled(true);
	CaisseWindow::listeArticles_->clear();
	for (Article article : articles) {
		CaisseWindow::listeArticles_->addItem(article.toQString());
	}
}

void CaisseWindow::rafraichirTotaux(QString sousTotal, QString taxes, QString total) {
	CaisseWindow::totalAvantTaxes_->setText(sousTotal);
	CaisseWindow::totalTaxes_->setText(taxes);
	CaisseWindow::total_->setText(total);
}

void CaisseWindow::effacerErreurs() {
	erreurNom_->setFixedHeight(0);
	erreurPrix_->setFixedHeight(0);
	erreurRetirer_->setFixedHeight(0);
}
