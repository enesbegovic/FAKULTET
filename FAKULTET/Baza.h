#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "StmtPtr.h"
using namespace std;
struct Student {
	int ID;
	string ImePrezime;
	string DatumRodjenja;
	string BrojIndexa;
	int Godina;
};
struct Predmet {
	int ID;
	string Naziv;
	string Profesor;
	int ECTS;
	int Godina;
};
struct Prijava {
	int ID;
	string ImePrezimeStudenta;
	string BrojIndexa;
	int Godina;
	string NazivPredmeta;
	string ImePrezimeProfesora;
	int ECTS;
	string DatumPrijave;
	string DatumPolaganja;
};
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
	vector<Student>UzmiSveStudenteSaOdredjeneGodine(int godina);
	void KreirajPredmet(string Naziv, string PredmetniProfesor, int ECTS, int GKPP);
	int BrojPredmeta();
	void UkloniPredmet(string Naziv);
	void PrikaziPredmeteSaOdredjeneGodine(int GKPP);
	vector<Predmet>UzmiSvePredmeteSaOdredjeneGodine(int godina);
	int DodajPrijavu(string Student, string Predmet, string DatumPrijave, string DatumPolaganja);
	int BrojPrijava();
	void UkloniPrijavu(string Student, string Predmet);
	void PrikaziPrijaveZaOdredjenuGodinu(int godina);
	vector<Prijava>UzmiSvePrijaveZaOdredjenuGodinu(int godina);
	void UnesiOcjenu(int PrijavaID, int Ocjena);
	void UnesiRezultateIspita(string Predmet,string DatumPolaganja);
	void ObrisiIspit(string Predmet,string DatumPolaganja);
	void PrikaziRezultateZaTrazenoPolaganje(string Predmet,string DatumPolaganja);
	int BrojIspita();
};