#include "pch.h"
#include "Baza.h"
TEST(BazaTest,KreirajStudentaDodajJednogStudenta) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojStudenata(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
    EXPECT_EQ(testBaza.BrojStudenata(),1);
}
TEST(BazaTest, ProvjeraDodavanjaDuplikata) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojStudenata(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	EXPECT_EQ(testBaza.BrojStudenata(), 1);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	EXPECT_EQ(testBaza.BrojStudenata(), 1);
}
TEST(BazaTest, UkloniStudentaProvjeraBrojaStudenata) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojStudenata(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	EXPECT_EQ(testBaza.BrojStudenata(), 1);
	testBaza.UkloniStudenta("123456");
	EXPECT_EQ(testBaza.BrojStudenata(), 0);
}
TEST(BazaTest, ProvjeraUklanjanjaNepostojecegStudenta) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojStudenata(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	EXPECT_EQ(testBaza.BrojStudenata(), 1);
	testBaza.UkloniStudenta("999999");
	EXPECT_EQ(testBaza.BrojStudenata(), 1);
}
TEST(BazaTest, ProvjeraBrojaPredmetaDodajPredmet) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojPredmeta(), 0);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	EXPECT_EQ(testBaza.BrojPredmeta(), 1);
}
TEST(BazaTest, ProvjeraDuplikataProvjeraBrojaPredmeta) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojPredmeta(), 0);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	EXPECT_EQ(testBaza.BrojPredmeta(), 1);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	EXPECT_EQ(testBaza.BrojPredmeta(), 1);
}
TEST(BazaTest, UkloniPredmetProvjeraBrojaPredmeta) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojPredmeta(), 0);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	EXPECT_EQ(testBaza.BrojPredmeta(), 1);
	testBaza.UkloniPredmet("Predmet1");
	EXPECT_EQ(testBaza.BrojPredmeta(), 0);
}
TEST(BazaTest, UkloniPredmetProvjeraUklanjanjaNePostojecegPredmeta) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojPredmeta(), 0);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	EXPECT_EQ(testBaza.BrojPredmeta(), 1);
	testBaza.UkloniPredmet("Abcdef");
	EXPECT_EQ(testBaza.BrojPredmeta(), 1);
}