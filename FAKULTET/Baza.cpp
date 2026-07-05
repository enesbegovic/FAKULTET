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
void Baza::IzvrsiUpit(string sql, string PorukaGreske) {
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << PorukaGreske << ": " << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::KreirajTabelu() {
	char* errMsg = nullptr;
	string KreirajStudente =
		"CREATE TABLE IF NOT EXISTS Studenti("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"PrezimeIme TEXT NOT NULL,"
		"DatumRodjenja TEXT NOT NULL,"
		"[Index] TEXT NOT NULL,"
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
		"PredmetID INTEGER NOT NULL,"
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
void Baza::KreirajStudenta(string PrezimeIme, string DatumRodjenja, string Index, int GKP) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Studenti WHERE [Index]='" + Index + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count > 0) {
		cout << "STUDENT VEC POSTOJI!!!" << endl;
		return;
	}
	string sql =
		"INSERT INTO Studenti(PrezimeIme,DatumRodjenja,[Index],GodinaStudija) VALUES('"
		+ PrezimeIme + "', '" + DatumRodjenja + "', '" + Index + "', '" + to_string(GKP) + "');";
	IzvrsiUpit(sql, "GRESKA PRI UNOSU STUDENATA");
}
void Baza::UkloniStudenta(string Index) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Studenti WHERE [Index]='" + Index + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count == 0) {
		cout << "NE POSTOJI TRAZENI STUDENT!!!" << endl;
		return;
	}
	string sql =
		"DELETE FROM Studenti WHERE [Index]='" + Index + "';";
	IzvrsiUpit(sql, "GRESKA PRI BRISANJU STUDENTA");
}
void Baza::PrikaziStudenteSaOdredjeneGodine(int GKP) {
	string sql =
		"SELECT * FROM Studenti Where GodinaStudija='" + to_string(GKP) + "';";
	sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << podaci[i] << endl;
			cout << "---" << endl;
			return 0;

		}, nullptr, nullptr);
}
void Baza::KreirajPredmet(string Naziv, string PredmetniProfesor, int ECTS, int GKPP) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Predmeti WHERE Naziv='" + Naziv + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count > 0) {
		cout << "PREDMET VEC POSTOJI!!!" << endl;
		return;
	}
	string sql =
		"INSERT INTO Predmeti(Naziv,PredmetniProfesor,ECTSBodovi,GodinaKojojPredmetPripada) VALUES('"
		+ Naziv + "', '" + PredmetniProfesor + "', '" + to_string(ECTS) + "', '" + to_string(GKPP) + "');";
	IzvrsiUpit(sql,"GRESKA UNOSU PREDMETA");
}
void Baza::UkloniPredmet(string Naziv) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Predmeti WHERE Naziv='" + Naziv + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count == 0) {
		cout << "NE POSTOJI TRAZENI PREDMET!!!" << endl;
		return;
	}
	string sql =
		"DELETE FROM Predmeti WHERE Naziv='" + Naziv + "';";
	IzvrsiUpit(sql,"GRESKA PRI BRISANJU PREDMETA");
}
void Baza::PrikaziPredmeteSaOdredjeneGodine(int GKPP) {
	string sql =
		"SELECT * FROM Predmeti Where GodinaKojojPredmetPripada='" + to_string(GKPP) + "';";
	sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << podaci[i] << endl;
			cout << "---" << endl;
			return 0;

		}, nullptr, nullptr);
}
void Baza::DodajPrijavu(string Student, string Predmet, string DatumPrijave) {
	int StudentID = 0;
	string ProvjeraStudenta =
		"SELECT ID FROM Studenti WHERE [Index]='" + Student + "';";
	sqlite3_exec(db, ProvjeraStudenta.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &StudentID, nullptr);
	int PredmetID = 0;
	string ProvjeraPredmeta =
		"SELECT ID FROM Predmeti WHERE Naziv='" + Predmet + "';";
	sqlite3_exec(db, ProvjeraPredmeta.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &PredmetID, nullptr);
	if (StudentID == 0 || PredmetID == 0) {
		cout << "GRESKA PRI PRIJAVLJIVANJU ISPITA!!!" << endl;
		return;
	}
	string sql =
		"INSERT INTO Prijave(StudentID,PredmetID,DatumPrijave) VALUES('"
		+ to_string(StudentID) + "', '" + to_string(PredmetID) + "', '" + DatumPrijave + "');";
	IzvrsiUpit(sql,"GRESKA PRI UNOSU PRIJAVE");
}
void Baza::UkloniPrijavu(string Student, string Predmet) {
	int StudentId=0;
	string ProvjeraStudenta =
		"SELECT ID FROM Studenti WHERE [Index]='" + Student + "';";
	sqlite3_exec(db, ProvjeraStudenta.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &StudentId, nullptr);
	int PredmetId = 0;
	string ProvjeraPredmeta =
		"SELECT ID FROM Predmeti WHERE Naziv='" + Predmet + "';";
	sqlite3_exec(db, ProvjeraPredmeta.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &PredmetId, nullptr);
	if (StudentId == 0 || PredmetId == 0) {
		cout << "GRESKA PRI BRISANJU PRIJAVE!!!" << endl;
		return;
	}
	int imaIspit = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Ispiti WHERE DatumPrijaveID IN"
		"(SELECT ID FROM Prijave WHERE StudentID=" + to_string(StudentId) + " AND PredmetID=" + to_string(PredmetId) + ");";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &imaIspit, nullptr);
	if (imaIspit > 0) {
		cout << "GRESKA PRI BRISANJU PRIJAVE!!!" << endl;
		return;
	}
	string sql =
		"DELETE FROM Prijave WHERE StudentID=" + to_string(StudentId) + " AND PredmetID=" + to_string(PredmetId) + ";";
	IzvrsiUpit(sql,"GRESKA PRI BRISANJU PRIJAVE");
}
void Baza::PrikaziPrijaveZaOdredjenuGodinu(int godina) {
	string sql =
		"SELECT Studenti.PrezimeIme, Studenti.[Index], Studenti.GodinaStudija, Predmeti.Naziv, Predmeti.PredmetniProfesor, Predmeti.ECTSBodovi, Prijave.DatumPrijave "
		"FROM Prijave "
		"JOIN Studenti ON Prijave.StudentID=Studenti.ID "
		"JOIN Predmeti ON Prijave.PredmetID=Predmeti.ID "
		"WHERE Studenti.GodinaStudija=" + to_string(godina) + ";";
	sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << podaci[i] << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, nullptr);
}
void Baza::KreirajIspit(string Student, string Predmet, string DatumPrijave, string DatumPolaganja, bool Polozio, int Ocjena) {
	int StudentId = 0;
	string ProvjeraStudenta =
		"SELECT ID FROM Studenti WHERE [Index]='" + Student + "';";
	sqlite3_exec(db, ProvjeraStudenta.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &StudentId, nullptr);
	int PredmetId = 0;
	string ProvjeraPredmeta =
		"SELECT ID FROM Predmeti WHERE Naziv='" + Predmet + "';";
	sqlite3_exec(db, ProvjeraPredmeta.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &PredmetId, nullptr);
	int PrijaveId = 0;
	string ProvjeraPrijave =
		"SELECT ID FROM Prijave WHERE StudentID=" + to_string(StudentId) + " AND PredmetID=" + to_string(PredmetId) + " AND DatumPrijave='" + DatumPrijave +"';";
	sqlite3_exec(db, ProvjeraPrijave.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &PrijaveId, nullptr);
	if (StudentId == 0 || PredmetId == 0 || PrijaveId == 0) {
		cout << "GRESKA PRI DODAVANJU ISPITA!!!" << endl;
		return;
	}
	string sql =
		"INSERT INTO Ispiti(StudentID,PredmetID,DatumPrijaveID,DatumPolaganja,Polozio,Ocjena) VALUES('"
		+ to_string(StudentId) + "', '" + to_string(PredmetId) + "', '" + to_string(PrijaveId) + "', '" + DatumPolaganja + "', '" + to_string(Polozio)+ "', '" + to_string(Ocjena) + "');";
	IzvrsiUpit(sql,"GRESKA PRI UNOSU ISPITA");
}
void Baza::ObrisiIspit(string Student,string Predmet,string DatumPrijave){
	int StudentId = 0;
	string ProvjeraStudenta =
		"SELECT ID FROM Studenti WHERE [Index]='" + Student + "';";
	sqlite3_exec(db, ProvjeraStudenta.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &StudentId, nullptr);
	int PredmetId = 0;
	string ProvjeraPredmeta =
		"SELECT ID FROM Predmeti WHERE Naziv='" + Predmet + "';";
	sqlite3_exec(db, ProvjeraPredmeta.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &PredmetId, nullptr);
	int PrijavaId = 0;
	string ProvjeraPrijave =
		"SELECT ID FROM Prijave WHERE StudentID=" + to_string(StudentId) + " AND PredmetID=" + to_string(PredmetId) + " AND DatumPrijave='" + DatumPrijave + "';";
	sqlite3_exec(db, ProvjeraPrijave.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &PrijavaId, nullptr);
	if (StudentId == 0 || PredmetId == 0 || PrijavaId == 0) {
		cout << "GRESKA PRI BRISANJU ISPITA!!!" << endl;
		return;
	}
	string sql =
		"DELETE FROM Ispiti WHERE StudentID=" + to_string(StudentId) + " AND PredmetID=" + to_string(PredmetId) + " AND DatumPrijaveID=" + to_string(PrijavaId) + ";";
	IzvrsiUpit(sql,"GRESKA PRI BRISANJU ISPITA");
}
void Baza::PrikaziIspiteZaOdredjenuGodinu(int godina) {
	string sql =
		"SELECT Studenti.PrezimeIme,Studenti.[Index],Predmeti.Naziv,Predmeti.PredmetniProfesor,Predmeti.ECTSBodovi,Prijave.DatumPrijave,Ispiti.DatumPolaganja, "
		"CASE WHEN Ispiti.Polozio=1 THEN 'DA' ELSE 'NE' END AS Polozio, "
		"CASE WHEN Ispiti.Polozio=1 THEN CAST(Ispiti.Ocjena AS TEXT) ELSE '---' END AS Ocjena "
		"FROM Ispiti "
		"JOIN Studenti ON Ispiti.StudentID=Studenti.ID "
		"JOIN Predmeti ON Ispiti.PredmetID=Predmeti.ID "
		"JOIN Prijave ON Ispiti.DatumPrijaveID=Prijave.ID "
		"WHERE Studenti.GodinaStudija=" + to_string(godina) + ";";
	sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << (podaci[i] ? podaci[i] : "NULL") << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, nullptr);
}