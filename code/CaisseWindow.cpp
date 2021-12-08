

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
	boutonRetirer_ = nouveauBouton("Retirer article", this, &CaisseWindow::envoyerRetirerArticle);
	boutonRetirer_->setEnabled(false);
	layoutEntree->addWidget(boutonRetirer_);
	layoutEntree->addWidget(nouveauBouton("Tout réinitialiser",this,&CaisseWindow::envoyerReinitialiser));
	auto layoutLineEdits = new QHBoxLayout();
	layoutEntree->addLayout(layoutLineEdits);
	QCheckBox* taxable = new QCheckBox("&Taxable", this);
	taxable_ = taxable;
	layoutEntree->addWidget(taxable);


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
	layoutLineEdits->addWidget(nomArticle);
	layoutLineEdits->addSpacing(10);
	auto prixArticle = new QLineEdit(this);
	prixArticle_ = prixArticle;
	prixArticle->setPlaceholderText("Prix de l'article");
	prixArticle->setFixedHeight(30);
	prixArticle->setMaximumWidth(75);
	prixArticle->setValidator(new QDoubleValidator(this));
	layoutLineEdits->addWidget(prixArticle);

	QObject::connect(this, &CaisseWindow::nouvelArticle, &Caisse_, &Caisse::ajouter);
	QObject::connect(this, &CaisseWindow::retirerArticle, &Caisse_, &Caisse::retirer);
	QObject::connect(this, &CaisseWindow::reinitialiser, &Caisse_, &Caisse::reinitialiser);
	QObject::connect(&Caisse_, &Caisse::vecteurModifie, this, &CaisseWindow::rafraichirArticles );
	QObject::connect(&Caisse_, &Caisse::vecteurModifie, &Caisse_, &Caisse::calculerTotaux);
	QObject::connect(&Caisse_, &Caisse::totauxModifies, this, &CaisseWindow::rafraichirTotaux);


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
}

void CaisseWindow::envoyerNouvelArticle() {
	/*problème de compatibilité entre QDoubleValidator et QString::toDouble().
	merci à Cory Quammen pour la solution trouvée en ligne : https://gitlab.kitware.com/paraview/paraview/-/issues/15786
	---------------------------------------------------*/
	QLocale oL;
	double dValue = oL.toDouble(CaisseWindow::prixArticle_->text());
	//--------------------------------------------------
	emit nouvelArticle	(CaisseWindow::nomArticle_->text(),
						dValue,
						CaisseWindow::taxable_->checkState());
}

void CaisseWindow::envoyerRetirerArticle() {
	//ajouter une gestion d'erreur si pas d'article selectionné
	emit retirerArticle(listeArticles_->selectedItems());
}

void CaisseWindow::envoyerReinitialiser() {
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
