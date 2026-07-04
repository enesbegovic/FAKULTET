#include <iostream>
#include <string>
#include "Baza.h"
using namespace std;
Baza::Baza() {
	sqlite3_open("Fakultet.db", &db);
}
Baza::~Baza() {
	sqlite3_close(db);
}
void Baza::KreirajTabelu() {
	char* errMsg = nullptr;
	string KreirajStudente =
		"CREATE TABLE IF NOT EXISTS Studenti("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"PrezimeIme TEXT NOT NULL,"
		"DatumRodjenja TEXT NOT NULL,"
		"Index TEXT NOT NULL,"
		"GodinaStudija INTEGER NOT NULL);";
	sqlite3_exec(db, KreirajStudente.c_str(), nullptr, nullptr, &errMsg);

	string KreirajPredmete =
		"CREATE TABLE IF NOT EXISTS Predmeti("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"Naziv TEXT NOT NULL,"
		"PredmetniProfesor TEXT NOT NULL,"
		"ECTSBodovi INTEGER NOT NULL,"
		"GodinaKojojPredmetPripada INTEGER  NOT NULL);";
	sqlite3_exec(db, KreirajPredmete.c_str(), nullptr, nullptr, &errMsg);
	
	string KreirajPrijave =
		"CREATE TABLE IF NOT EXISTS Prijave("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"StudentID INTEGER NOT NULL,"
		"PredmetID,INTEGER NOT NULL,"
		"DatumPrijave TEXT NOT NULL);";
	sqlite3_exec(db, KreirajPrijave.c_str(), nullptr, nullptr, &errMsg);

	string KreirajIspite =
		"CREATE TABLE IF NOT EXISTS Ispiti("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"StudentID INTEGER NOT NULL,"
		"PredmetID INTEGER NOT NULL,"
		"DatumPrijaveID INTEGER NOT NULL,"
		"DatumPolaganja TEXT NOT NULL,"
		"Polozio INTEGER DEFAULT 0,"
		"Ocjena INTEGER);";
	sqlite3_exec(db, KreirajIspite.c_str(), nullptr, nullptr, &errMsg);
}