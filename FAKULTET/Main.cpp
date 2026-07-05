#include <iostream>
#include <string>
#include "Baza.h"
using namespace std;
int main() {
	Baza baza;
	baza.KreirajTabelu();
	int izbor;
	do {
		cout << "MENI" << endl;
		cout << "1.Dodaj studenta" << endl;
		cout << "2.Ukloni studenta" << endl;
		cout << "3.Dodaj predmet" << endl;
		cout << "4.Ukloni predmet" << endl;
		cout << "5.Dodaj prijavu" << endl;
		cout << "6.Ukloni prijavu" << endl;
		cout << "7.Dodaj ispit" << endl;
		cout << "8.Ukloni ispit" << endl;
		cout << "9.Prikazi sve studente sa 1. godine" << endl;
		cout << "10.Prikazi sve studente sa 2. godine" << endl;
		cout << "11.Prikazi sve studente sa 3. godine" << endl;
		cout << "12.Prikazi sve predmete sa 1. godine" << endl;
		cout << "13.Prikazi sve predmete sa 2. godine" << endl;
		cout << "14.Prikazi sve predmete sa 3. godine" << endl;
		cout << "15.Prikazi sve prijave sa 1. godine" << endl;
		cout << "16.Prikazi sve prijave sa 2. godine" << endl;
		cout << "17.Prikazi sve prijave sa 3. godine" << endl;
		cout << "18.Prikazi sve ispite za 1.godinu" << endl;
		cout << "19.Prikazi sve ispite za 2.godinu" << endl;
		cout << "20.Prikazi sve ispite za 3.godinu" << endl;
		cout << "21.Izlaz" << endl;
		cin >> izbor;
		cin.ignore();
		switch (izbor) {
		case 1:
		{
			string PrezimeIme;
			cout << "Unesite prezime i ime studenta: ";
			getline(cin, PrezimeIme);
			int dan, mjesec, godina;
			cout << "Unesite dan rodjenja studenta: ";
			cin >> dan;
			cout << "Unesite mjesec rodjenja studenta: ";
			cin >> mjesec;
			cout << "Unesite godinu rodjenja studenta: ";
			cin >> godina;
			cin.ignore();
			string DatumStr = to_string(dan) + "." + to_string(mjesec) + "." + to_string(godina);
			string Index;
			cout << "Unesite broj indexa: ";
			getline(cin, Index);
			int GKP;
			cout << "Unesite godinu studija koju pohadja student: ";
			cin >> GKP;
			cin.ignore();
			baza.KreirajStudenta(PrezimeIme, DatumStr, Index, GKP);
			break;
		}
		case 2:
		{
			string Index;
			cout << "Unesite broj indexa studenta kojeg zelite ukloniti: ";
			getline(cin, Index);
			baza.UkloniStudenta(Index);
			break;
		}
		case 3:
		{
			string Naziv;
			cout << "Unesite naziv predmeta: ";
			getline(cin, Naziv);
			string PredmetniProfesor;
			cout << "Unesite prezime i ime predmetnog profesora: ";
			getline(cin, PredmetniProfesor);
			int ECTS;
			cout << "Unesite ECTS bodove predmeta: ";
			cin >> ECTS;
			cin.ignore();
			int GKPP;
			cout << "Unesite godinu na kojoj se predmet nalazi: ";
			cin >> GKPP;
			cin.ignore();
			baza.KreirajPredmet(Naziv, PredmetniProfesor, ECTS, GKPP);
			break;
		}
		case 4:
		{
			string Naziv;
			cout << "Unesite naziv predmeta kojeg zelite ukloniti: ";
			getline(cin, Naziv);
			baza.UkloniPredmet(Naziv);
			break;
		}
		case 5:
		{
			string Index;
			cout << "Broj indexa studenta koji prijavljuje ispit: ";
			getline(cin, Index);
			string Predmet;
			cout << "Naziv predmeta koji student prijavljuje: ";
			getline(cin, Predmet);
			int dan, mjesec, godina;
			cout << "Unesite dan prijave: ";
			cin >> dan;
			cout << "Unesite mjesec prijave: ";
			cin >> mjesec;
			cout << "Unesite godinu prijave: ";
			cin >> godina;
			cin.ignore();
			string DatumPrijaveStr = to_string(dan) + "." + to_string(mjesec) + "." + to_string(godina);
			baza.DodajPrijavu(Index, Predmet, DatumPrijaveStr);
			break;
		}
		case 6:
		{
			string Index;
			cout << "Unesite broj indexa studenta ciju prijavu zelite obrisati: ";
			getline(cin, Index);
			string Predmet;
			cout << "Unesite naziv predmeta: ";
			getline(cin, Predmet);
			baza.UkloniPrijavu(Index, Predmet);
			break;
		}
		case 7:
		{
			string Index;
			cout << "Unesite broj indexa studenta koji je polagao ispit: ";
			getline(cin, Index);
			string Predmet;
			cout << "Unesite naziv predmeta koji je student polagao: ";
			getline(cin, Predmet);
			int d, m, g;
			cout << "Unesite dan prijave: ";
			cin >> d;
			cout << "Unesite mjesec prijave: ";
			cin >> m;
			cout << "Unesite godinu prijave: ";
			cin >> g;
			cin.ignore();
			string DatumPrijaveStr = to_string(d) + "." + to_string(m) + "." + to_string(g);
			int dan, mjesec, godina;
			cout << "Unesite dan polaganja ispita: ";
			cin >> dan;
			cout << "Unesite mjesec polaganja ispita: ";
			cin >> mjesec;
			cout << "Unesite godinu polaganja ispita: ";
			cin >> godina;
			cin.ignore();
			string DatumPolaganjaStr = to_string(dan) + "." + to_string(mjesec) + "." + to_string(godina);
			string odgovor;
			cout << "Da li je student polozio:(Da/Ne): ";
			cin >> odgovor;
			cin.ignore();
			bool Polozio=(odgovor=="Da"||odgovor=="da");
			int Ocjena=0;
			if (Polozio) {
				cout << "Student je polozio" << endl;
				cout << "Ocjena koju je student ostvario: ";
				cin >> Ocjena;
				cin.ignore();
			}
			else
				cout << "Student nije polozio" << endl;
			baza.KreirajIspit(Index, Predmet, DatumPrijaveStr, DatumPolaganjaStr, Polozio, Ocjena);
			break;
		}
		case 8:
		{
			string Index;
			cout << "Unesite broj indexa studenta koji je polagao ispit: ";
			getline(cin, Index);
			string Predmet;
			cout << "Unesite naziv predmeta koji je student polagao: ";
			getline(cin, Predmet);
			int d, m, g;
			cout << "Unesite dan prijave: ";
			cin >> d;
			cout << "Unesite mjesec prijave: ";
			cin >> m;
			cout << "Unesite godinu prijave: ";
			cin >> g;
			cin.ignore();
			string DatumPrijaveStr = to_string(d) + "." + to_string(m) + "." + to_string(g);
			baza.ObrisiIspit(Index, Predmet, DatumPrijaveStr);
			break;
		}
		case 9:
		{
			baza.PrikaziStudenteSaOdredjeneGodine(1);
			break;
		}
		case 10:
		{
			baza.PrikaziStudenteSaOdredjeneGodine(2);
			break;
		}
		case 11:
		{
			baza.PrikaziStudenteSaOdredjeneGodine(3);
			break;
		}
		case 12:
		{
			baza.PrikaziPredmeteSaOdredjeneGodine(1);
			break;
		}
		case 13:
		{
			baza.PrikaziPredmeteSaOdredjeneGodine(2);
			break;
		}
		case 14:
		{
			baza.PrikaziPredmeteSaOdredjeneGodine(3);
			break;
		}
		case 15:
		{
			baza.PrikaziPrijaveZaOdredjenuGodinu(1);
			break;

		}
		case 16:
		{
			baza.PrikaziPrijaveZaOdredjenuGodinu(2);
			break;
		}
		case 17:
		{
			baza.PrikaziPrijaveZaOdredjenuGodinu(3);
			break;
		}
		case 18:
		{
			baza.PrikaziIspiteZaOdredjenuGodinu(1);
			break;
		}
		case 19:
		{
			baza.PrikaziIspiteZaOdredjenuGodinu(2);
			break;
		}
		case 20:
		{
			baza.PrikaziIspiteZaOdredjenuGodinu(3);
			break;
		}
		case 21:
		{
			cout << "DOVIDJENJA" << endl;
			break;
		}
		default:
		{
			cout << "POGRESAN IZBOR!!!" << endl;
			break;
		}
		}
	}while (izbor != 21);
}