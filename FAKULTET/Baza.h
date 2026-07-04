#pragma once
#include <iostream>
#include <string>
#include "sqlite3.h"
using namespace std;
class Baza {
	sqlite3* db;
public:
	Baza();
	~Baza();
	void KreirajTabelu();
	void KreirajStudenta(string PrezimeIme, string DatumRodjenja, string Index, int GKP);
	void UkloniStudenta(string Index);
	void PrikaziStudenteSaOdredjeneGodine(int GKP);
	void KreirajPredmet(string Naziv, string PredmetniProfesor, int ECTS, int GKPP);
	void UkloniPredmet(string Naziv);
	void PrikaziPredmeteSaOdredjeneGodine(int GKPP);
};