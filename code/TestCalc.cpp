//
//#include "Caisse.hpp"
//
//#if __has_include("gtest/gtest.h")
//#include "gtest/gtest.h"
//#endif
//#ifdef TEST
//
//TEST(Caisse, simple) {
//	Caisse Caisse;
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(4);
//	Caisse.ajouterChiffre(3);
//	EXPECT_EQ(Caisse.obtenirValeur(), 143);
//	Caisse.effacer();
//	Caisse.ajouterChiffre(2);
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(8);
//	EXPECT_EQ(Caisse.obtenirValeur(), 218);
//}
//
//TEST(Caisse, operations) {
//	Caisse Caisse;
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(4);
//	Caisse.ajouterChiffre(3);
//	Caisse.operationPlus();
//	Caisse.ajouterChiffre(2);
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(8);
//	Caisse.operationEgal();
//	EXPECT_EQ(Caisse.obtenirValeur(), 143+218);
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(4);
//	Caisse.ajouterChiffre(3);
//	Caisse.operationMoins();
//	Caisse.ajouterChiffre(2);
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(8);
//	Caisse.operationEgal();
//	EXPECT_EQ(Caisse.obtenirValeur(), 143-218);
//}
//
//TEST(Caisse, changement_operation) {
//	Caisse Caisse;
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(4);
//	Caisse.ajouterChiffre(3);
//	Caisse.operationPlus();
//	Caisse.operationMoins();
//	Caisse.ajouterChiffre(2);
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(8);
//	Caisse.operationEgal();
//	EXPECT_EQ(Caisse.obtenirValeur(), 143-218);
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(4);
//	Caisse.ajouterChiffre(3);
//	Caisse.operationMoins();
//	Caisse.operationPlus();
//	Caisse.ajouterChiffre(2);
//	Caisse.ajouterChiffre(1);
//	Caisse.ajouterChiffre(8);
//	Caisse.operationEgal();
//	EXPECT_EQ(Caisse.obtenirValeur(), 143+218);
//}
//
//#endif
