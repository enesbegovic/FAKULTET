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
TEST(BazaTest, DodajPrijavuProvjeraBrojaPrijava) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojPrijava(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	testBaza.DodajPrijavu("123456", "Predmet1", "2026-01-22", "2026-01-30");
	EXPECT_EQ(testBaza.BrojPrijava(), 1);
}
TEST(BazaTest, DodajPrijavuNepostojeciStudent) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojPrijava(), 0);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	testBaza.DodajPrijavu("222222", "Predmet1", "2026-01-22", "2026-01-30");
	EXPECT_EQ(testBaza.BrojPrijava(), 0);
}
TEST(BazaTest, UkloniPrijavuProvjeraBrojaPrijava) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojPrijava(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	testBaza.DodajPrijavu("123456", "Predmet1", "2026-01-22", "2026-01-30");
	EXPECT_EQ(testBaza.BrojPrijava(), 1);
	testBaza.UkloniPrijavu("123456", "Predmet1");
	EXPECT_EQ(testBaza.BrojPrijava(), 0);
}
TEST(BazaTest, UkloniPrijavuNepostojeciPredmet) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojPrijava(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	testBaza.DodajPrijavu("123456", "Predmet1", "2026-01-22", "2026-01-30");
	EXPECT_EQ(testBaza.BrojPrijava(), 1);
	testBaza.UkloniPrijavu("123456", "Abcdef");
	EXPECT_EQ(testBaza.BrojPrijava(), 1);
}
TEST(BazaTest, UnesiOcjenuProvjeraBrojaIspita) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojIspita(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	int PrijavaID=testBaza.DodajPrijavu("123456", "Predmet1", "2026-01-22", "2026-01-30");
	testBaza.UnesiOcjenu(PrijavaID, 8);
	EXPECT_EQ(testBaza.BrojIspita(), 1);
}
TEST(BazaTest, ObrisiIspitProvjeraBrojaIspita) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojIspita(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	int PrijavaID = testBaza.DodajPrijavu("123456", "Predmet1", "2026-01-22", "2026-01-30");
	testBaza.UnesiOcjenu(PrijavaID, 8);
	EXPECT_EQ(testBaza.BrojIspita(), 1);
	testBaza.ObrisiIspit("Predmet1", "2026-01-30");
	EXPECT_EQ(testBaza.BrojIspita(), 0);
}
TEST(BazaTest, ObrisiNepostojeciIspitProvjeraBrojaIspita) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojIspita(), 0);
	testBaza.KreirajStudenta("Test Testic", "2000-01-01", "123456", 1);
	testBaza.KreirajPredmet("Predmet1", "Profesor1", 6, 1);
	int PrijavaID = testBaza.DodajPrijavu("123456", "Predmet1", "2026-01-22", "2026-01-30");
	testBaza.UnesiOcjenu(PrijavaID, 8);
	EXPECT_EQ(testBaza.BrojIspita(), 1);
	testBaza.ObrisiIspit("Abcdef", "2026-01-30");
	EXPECT_EQ(testBaza.BrojIspita(), 1);
}