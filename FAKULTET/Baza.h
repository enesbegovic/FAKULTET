#pragma once
#include <iostream>
#include <string>
#include "sqlite3.h"
using namespace std;
class Baza {
	sqlite3* db;
public:
	Baza(string PutanjaBaze="Fakultet.db");
	~Baza();
	void IzvrsiUpit(string sql, string PorukaGreske);
	void KreirajTabelu();
	void KreirajStudenta(string PrezimeIme, string DatumRodjenja, string Index, int GKP);
	int BrojStudenata();
	void UkloniStudenta(string Index);
	void PrikaziStudenteSaOdredjeneGodine(int GKP);
	void KreirajPredmet(string Naziv, string PredmetniProfesor, int ECTS, int GKPP);
	int BrojPredmeta();
	void UkloniPredmet(string Naziv);
	void PrikaziPredmeteSaOdredjeneGodine(int GKPP);
	int DodajPrijavu(string Student, string Predmet, string DatumPrijave, string DatumPolaganja);
	int BrojPrijava();
	void UkloniPrijavu(string Student, string Predmet);
	void PrikaziPrijaveZaOdredjenuGodinu(int godina);
	void UnesiOcjenu(int PrijavaID, int Ocjena);
	void UnesiRezultateIspita(string Predmet,string DatumPolaganja);
	void ObrisiIspit(string Predmet,string DatumPolaganja);
	void PrikaziRezultateZaTrazenoPolaganje(string Predmet,string DatumPolaganja);
	int BrojIspita();
};