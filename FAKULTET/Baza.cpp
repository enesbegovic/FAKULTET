#include <iostream>
#include <string>
#include "Baza.h"
using namespace std;
Baza::Baza() {
	sqlite3_open("Fakultet.db", &db);
	sqlite3_exec(db, "PRAGMA foreign_keys=ON;", nullptr, nullptr, nullptr);
}
Baza::~Baza() {
	sqlite3_close(db);
}
void Baza::IzvrsiUpit(string sql, string PorukaGreske) {
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << PorukaGreske << ": " << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::KreirajTabelu() {
	string KreirajStudente =
		"CREATE TABLE IF NOT EXISTS Studenti("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"PrezimeIme TEXT NOT NULL,"
		"DatumRodjenja TEXT NOT NULL,"
		"[Index] TEXT NOT NULL UNIQUE,"
		"GodinaStudija INTEGER NOT NULL CHECK(GodinaStudija BETWEEN 1 AND 5));";
	IzvrsiUpit(KreirajStudente,"GRESKA PRI KREIRANJU TABELE STUDENTI");

	string KreirajPredmete =
		"CREATE TABLE IF NOT EXISTS Predmeti("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"Naziv TEXT NOT NULL UNIQUE,"
		"PredmetniProfesor TEXT NOT NULL,"
		"ECTSBodovi INTEGER NOT NULL,"
		"GodinaKojojPredmetPripada INTEGER  NOT NULL CHECK(GodinaKojojPredmetPripada BETWEEN 1 AND 5));";
	IzvrsiUpit(KreirajPredmete,"GRESKA PRI KREIRANJU TABELE PREDMETI");
	
	string KreirajPrijave =
		"CREATE TABLE IF NOT EXISTS Prijave("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"StudentID INTEGER NOT NULL,"
		"PredmetID INTEGER NOT NULL,"
		"DatumPrijave TEXT NOT NULL,"
		"DatumPolaganja TEXT NOT NULL,"
		"FOREIGN KEY (StudentID) REFERENCES Studenti(ID) ON DELETE RESTRICT,"
		"FOREIGN KEY (PredmetID) REFERENCES Predmeti(ID) ON DELETE RESTRICT);";
	IzvrsiUpit(KreirajPrijave,"GRESKA PRI KREIRANJU TABELE PRIJAVE");

	string KreirajIspite =
		"CREATE TABLE IF NOT EXISTS Ispiti("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"PrijavaID INTEGER NOT NULL UNIQUE,"
		"Ocjena INTEGER NOT NULL CHECK(Ocjena BETWEEN 5 AND 10),"
		"FOREIGN KEY (PrijavaID) REFERENCES Prijave(ID) ON DELETE RESTRICT);";
	IzvrsiUpit(KreirajIspite,"GRESKA PRI KREIRANJU TABELE ISPITI");
}
void Baza::KreirajStudenta(string PrezimeIme, string DatumRodjenja, string Index, int GKP) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Studenti WHERE [Index]=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE STUDENTA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, Index.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count > 0) {
		cout << "STUDENT VEC POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Studenti(PrezimeIme, DatumRodjenja, [Index], GodinaStudija) VALUES(?, ?, ?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA STUDENTA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, PrezimeIme.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, DatumRodjenja.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, Index.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, GKP);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU STUDENTA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "STUDENT USPJESNO DODAN" << endl;
}
void Baza::UkloniStudenta(string Index) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Studenti WHERE [Index]=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, Index.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count == 0) {
		cout << "NE POSTOJI TRAZENI STUDENT!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"DELETE FROM Studenti WHERE [Index]=?;";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UKLANJANJA STUDENTA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Index.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI UKLANJANJU STUDENTA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "STUDENT USPJESNO UKLONJEN" << endl;
}
void Baza::PrikaziStudenteSaOdredjeneGodine(int GKP) {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT * FROM Studenti WHERE GodinaStudija=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA STUDENATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, GKP);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string Ime = (const char*)sqlite3_column_text(stmt, 1);
		string Datum = (const char*)sqlite3_column_text(stmt, 2);
		string Index = (const char*)sqlite3_column_text(stmt, 3);
		int Godina = sqlite3_column_int(stmt, 4);

		cout << "ID:" << ID << endl;
		cout << "Prezime I Ime Studenta:" << Ime << endl;
		cout << "Datum Rodjenja:" << Datum << endl;
		cout << "Broj Indexa:" << Index << endl;
		cout << "Godina Studija:" << Godina << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::KreirajPredmet(string Naziv, string PredmetniProfesor, int ECTS, int GKPP) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Predmeti WHERE Naziv=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, Naziv.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count > 0) {
		cout << "PREDMET VEC POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Predmeti(Naziv, PredmetniProfesor, ECTSBodovi, GodinaKojojPredmetPripada) VALUES(?, ?, ?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA PREDMETA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Naziv.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, PredmetniProfesor.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 3, ECTS);
	sqlite3_bind_int(stmt, 4, GKPP);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU PREDMETA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "PREDMET USPJESNO DODAN" << endl;
}
void Baza::UkloniPredmet(string Naziv) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Predmeti WHERE Naziv=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, Naziv.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count == 0) {
		cout << "NE POSTOJI TRAZENI PREDMET!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"DELETE FROM Predmeti WHERE Naziv=?;";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UKLANJANJA PREDMETA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Naziv.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI UKLANJANU PREDMETA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "PREDMET USPJESNO UKLONJEN" << endl;
}
void Baza::PrikaziPredmeteSaOdredjeneGodine(int GKPP) {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT * FROM Predmeti WHERE GodinaKojojPredmetPripada=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, GKPP);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string Naziv = (const char*)sqlite3_column_text(stmt, 1);
		string Profesor = (const char*)sqlite3_column_text(stmt, 2);
		int ECTS = sqlite3_column_int(stmt, 3);
		int GKPP = sqlite3_column_int(stmt, 4);

		cout << "ID:" << ID << endl;
		cout << "Naziv Predmeta:" << Naziv << endl;
		cout << "Predmeti Profesor:" << Profesor << endl;
		cout << "ECTS Bodovi:" << ECTS << endl;
		cout << "Godina Kojoj Predmet Pripada:" << GKPP << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::DodajPrijavu(string Student, string Predmet, string DatumPrijave, string DatumPolaganja) {
	sqlite3_stmt* stmtProvjerStudenta;
	int StudentID = 0;
	string ProvjeraStudenta =
		"SELECT ID FROM Studenti WHERE [Index]=?;";
	int rc = sqlite3_prepare_v2(db, ProvjeraStudenta.c_str(), -1, &stmtProvjerStudenta, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE STUDENTA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjerStudenta, 1, Student.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjerStudenta) == SQLITE_ROW) {
		StudentID = sqlite3_column_int(stmtProvjerStudenta, 0);
	}
	sqlite3_finalize(stmtProvjerStudenta);

	sqlite3_stmt* stmtProvjeraPredmeta;
	int PredmetID = 0;
	string ProvjeraPredmeta =
		"SELECT ID FROM Predmeti WHERE Naziv=?;";
	int rc1 = sqlite3_prepare_v2(db, ProvjeraPredmeta.c_str(), -1, &stmtProvjeraPredmeta, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE PREDMETA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjeraPredmeta, 1, Predmet.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjeraPredmeta) == SQLITE_ROW) {
		PredmetID = sqlite3_column_int(stmtProvjeraPredmeta, 0);
	}
	sqlite3_finalize(stmtProvjeraPredmeta);
	if (StudentID == 0 || PredmetID == 0) {
		cout << "GRESKA PRI PRIJAVLJIVANJU ISPITA-NEPOSTOJECI STUDENT-NEPOSTOJECI PREDMET!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Prijave(StudentID, PredmetID, DatumPrijave, DatumPolaganja) VALUES(?, ?, ?, ?);";
	int rc2 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc2 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA PRIJAVE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, StudentID);
	sqlite3_bind_int(stmt, 2, PredmetID);
	sqlite3_bind_text(stmt, 3, DatumPrijave.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 4, DatumPolaganja.c_str(), -1, SQLITE_TRANSIENT);
	rc2 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc2 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU PRIJAVE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "PRIJAVA USPIJESNO DODANA" << endl;
}
void Baza::UkloniPrijavu(string Student, string Predmet) {
	sqlite3_stmt* stmtProvjerStudenta;
	int StudentID = 0;
	string ProvjeraStudenta =
		"SELECT ID FROM Studenti WHERE [Index]=?;";
	int rc = sqlite3_prepare_v2(db, ProvjeraStudenta.c_str(), -1, &stmtProvjerStudenta, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE STUDENTA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjerStudenta, 1, Student.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjerStudenta) == SQLITE_ROW) {
		StudentID = sqlite3_column_int(stmtProvjerStudenta, 0);
	}
	sqlite3_finalize(stmtProvjerStudenta);

	sqlite3_stmt* stmtProvjeraPredmeta;
	int PredmetID = 0;
	string ProvjeraPredmeta =
		"SELECT ID FROM Predmeti WHERE Naziv=?;";
	int rc1 = sqlite3_prepare_v2(db, ProvjeraPredmeta.c_str(), -1, &stmtProvjeraPredmeta, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE PREDMETA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjeraPredmeta, 1, Predmet.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjeraPredmeta) == SQLITE_ROW) {
		PredmetID = sqlite3_column_int(stmtProvjeraPredmeta, 0);
	}
	sqlite3_finalize(stmtProvjeraPredmeta);
	if (StudentID == 0 || PredmetID == 0) {
		cout << "GRESKA PRI BRISANJU PRIJAVE-NEPOSTOJECI STUDENT-NEPOSTOJECI PREDMET!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"DELETE FROM Prijave WHERE StudentID=? AND PredmetID=?;";
	int rc2 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc2 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BRISANJA PRIJAVE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, StudentID);
	sqlite3_bind_int(stmt, 2, PredmetID);
	rc2 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc2 != SQLITE_DONE) {
		cout << "GRESKA PRI BRISANJU PRIJAVE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "PRIJAVA USPIJENO UKLONJENA" << endl;
}
void Baza::PrikaziPrijaveZaOdredjenuGodinu(int godina) {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Prijave.ID, Studenti.PrezimeIme, Studenti.[Index], Studenti.GodinaStudija, Predmeti.Naziv, Predmeti.PredmetniProfesor, Predmeti.ECTSBodovi, Prijave.DatumPrijave, Prijave.DatumPolaganja "
		"FROM Prijave "
		"JOIN Studenti ON Prijave.StudentID=Studenti.ID "
		"JOIN Predmeti ON Prijave.PredmetID=Predmeti.ID "
		"WHERE Studenti.GodinaStudija=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA PRIJAVA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, godina);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string Ime = (const char*)sqlite3_column_text(stmt, 1);
		string Index = (const char*)sqlite3_column_text(stmt, 2);
		int Godina = sqlite3_column_int(stmt, 3);
		string Naziv = (const char*)sqlite3_column_text(stmt, 4);
		string Profesor = (const char*)sqlite3_column_text(stmt, 5);
		int ECTS = sqlite3_column_int(stmt, 6);
		string DatumPrijave = (const char*)sqlite3_column_text(stmt, 7);
		string DatumPolaganja = (const char*)sqlite3_column_text(stmt, 8);

		cout << "Broj Prijave:" << ID << endl;
		cout << "Student:" << Ime << endl;
		cout << "Broj Indexa:" << Index << endl;
		cout << "Godina Studija:" << Godina << endl;
		cout << "Predmet:" << Naziv << endl;
		cout << "Predmetni Profesor:" << Profesor << endl;
		cout << "ECTS Bodovi:" << ECTS << endl;
		cout << "Datum Prijave:" << DatumPrijave << endl;
		cout << "Datum Ispita:" << DatumPolaganja << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::UnesiRezultateIspita(string Predmet, string DatumPolaganja) {
	sqlite3_stmt* stmtPrijava;
	string sqlPrijava =
		"SELECT Prijave.ID, Studenti.PrezimeIme "
		"FROM Prijave "
		"JOIN Studenti ON Prijave.StudentID=Studenti.ID "
		"JOIN Predmeti ON Prijave.PredmetID=Predmeti.ID "
		"WHERE Predmeti.Naziv=? AND Prijave.DatumPolaganja=? "
		"AND Prijave.ID NOT IN (SELECT PrijavaID FROM Ispiti);";
	int rc = sqlite3_prepare_v2(db, sqlPrijava.c_str(), -1, &stmtPrijava, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtPrijava, 1, Predmet.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_text(stmtPrijava, 2, DatumPolaganja.c_str(),-1,SQLITE_TRANSIENT);

	sqlite3_stmt* stmtInsert;
	string sqlInsert =
		"INSERT INTO Ispiti(PrijavaID, Ocjena) VALUES(?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sqlInsert.c_str(), -1, &stmtInsert, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI KREIRANJA ISPITA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	IzvrsiUpit("BEGIN TRANSACTION;", "GRESKA PRI POKRETANJU TRANSAKCIJE");
	int brojStudenata = 0;
	while (sqlite3_step(stmtPrijava) == SQLITE_ROW) {
		brojStudenata++;

		int PrijavaID = sqlite3_column_int(stmtPrijava, 0);
		string Ime = (const char*)sqlite3_column_text(stmtPrijava, 1);

		cout << "Student:" << Ime << endl;
		string Odgovor;
		cout << "Izasao Na Ispit(DA/NE):" << endl;
		cin >> Odgovor;
		int Ocjena = 5;
		if (Odgovor == "DA" || Odgovor == "Da") {
			cout << "Unesite Ocjenu(5-10)" << endl;
			cin >> Ocjena;
		}
		sqlite3_bind_int(stmtInsert, 1, PrijavaID);
		sqlite3_bind_int(stmtInsert, 2, Ocjena);

		sqlite3_step(stmtInsert);
		sqlite3_reset(stmtInsert);
	}
	sqlite3_finalize(stmtPrijava);
	sqlite3_finalize(stmtInsert);
	IzvrsiUpit("COMMIT;", "GRESKA PRI COMMIT-u");
	if (brojStudenata == 0) {
		cout << "PREDMET NE POSTOJI ILI NEMA PRIJAVLJENIH STUDENATA" << endl;
	}
	else {
		cout << "USPJESNO DODANI REZULTATI ISPITA" << endl;
	}
}
void Baza::ObrisiIspit(string Predmet,string DatumPolaganja){
	sqlite3_stmt* stmt;
	string sql =
		"DELETE  FROM Ispiti "
		"WHERE PrijavaID IN("
		" SELECT Prijave.ID FROM Prijave "
		" JOIN Predmeti ON Prijave.PredmetID=Predmeti.ID "
		" WHERE Predmeti.Naziv=? AND Prijave.DatumPolaganja=?"
		");";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BRISANJA REZULTATA ISPITA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Predmet.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, DatumPolaganja.c_str(), -1, SQLITE_TRANSIENT);
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		cout << "GRESKA PRI BRISANJU REZULTATA ISPITA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "REZULTATI ISPITA USPIJENO OBRISANI" << endl;
}
void Baza::PrikaziRezultateZaTrazenoPolaganje(string Predmet,string DatumPolaganja) {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Studenti.PrezimeIme, Studenti.[Index], Ispiti.Ocjena "
		"FROM Ispiti "
		"JOIN Prijave ON Ispiti.PrijavaID=Prijave.ID "
		"JOIN Studenti ON Prijave.StudentID=Studenti.ID "
		"JOIN Predmeti ON Prijave.PredmetID=Predmeti.ID "
		"WHERE Predmeti.Naziv=? AND Prijave.DatumPolaganja=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA REZULTATA ISPITA ZA ODREDJENU GODINU:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Predmet.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, DatumPolaganja.c_str(),-1, SQLITE_TRANSIENT);
	int brojRezultata = 0;
	cout << "REZULTATI TRAZENOG ISPITA:" << endl;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		brojRezultata++;
		string Ime = (const char*)sqlite3_column_text(stmt, 0);
		string Index = (const char*)sqlite3_column_text(stmt, 1);
		int Ocjena = sqlite3_column_int(stmt, 2);

		cout << "Student:" << Ime << "(" << Index << ") - " << Ocjena << endl;

	}
	sqlite3_finalize(stmt);
	if (brojRezultata == 0) {
		cout << "ZA TRAZENI ISPIT NEMA REZULTATA" << endl;
	}
}