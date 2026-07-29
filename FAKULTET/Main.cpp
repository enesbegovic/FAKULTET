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
		cout << "7.Dodaj rezultate ispit" << endl;
		cout << "8.Ukloni rezultate ispit" << endl;
		cout << "9.Prikazi sve studente sa 1. godine" << endl;
		cout << "10.Prikazi sve studente sa 2. godine" << endl;
		cout << "11.Prikazi sve studente sa 3. godine" << endl;
		cout << "12.Prikazi sve predmete sa 1. godine" << endl;
		cout << "13.Prikazi sve predmete sa 2. godine" << endl;
		cout << "14.Prikazi sve predmete sa 3. godine" << endl;
		cout << "15.Prikazi sve prijave sa 1. godine" << endl;
		cout << "16.Prikazi sve prijave sa 2. godine" << endl;
		cout << "17.Prikazi sve prijave sa 3. godine" << endl;
		cout << "18.Prikazi sve rezultate za trazeno polaganje" << endl;
		cout << "19.Izlaz" << endl;
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
			string DatumISO = to_string(godina) + "-" + (mjesec < 10 ? "0" : "") + to_string(mjesec) + "-" + (dan < 10 ? "0" : "") + to_string(dan);
			string Index;
			cout << "Unesite broj indexa: ";
			getline(cin, Index);
			int GKP;
			cout << "Unesite godinu studija koju pohadja student: ";
			cin >> GKP;
			cin.ignore();
			baza.KreirajStudenta(PrezimeIme, DatumISO, Index, GKP);
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
			cout << "Unesite broj indexa studenta koji prijavljuje ispit: ";
			getline(cin, Index);
			string Predmet;
			cout << "Unesite naziv predmeta koji student prijavljuje: ";
			getline(cin, Predmet);
			int dan, mjesec, godina;
			cout << "Unesite dan prijave: ";
			cin >> dan;
			cout << "Unesite mjesec prijave: ";
			cin >> mjesec;
			cout << "Unesite godinu prijave: ";
			cin >> godina;
			cin.ignore();
			string DatumPrijaveISO = to_string(godina) + "-" + (mjesec < 10 ? "0" : "") + to_string(mjesec) + "-" + (dan < 10 ? "0" : "") + to_string(dan);
			int dp, mp, gp;
			cout << "Unesite dan polaganja: ";
			cin >> dp;
			cout << "Unesite mjesec polaganja: ";
			cin >> mp;
			cout << "Unesite godinu polaganja: ";
			cin >> gp;
			cin.ignore();
			string DatumPolaganjaISO = to_string(gp) + "-" + (mp < 10 ? "0" : "") + to_string(mp) + "-" + (dp < 10 ? "0" : "") + to_string(dp);
			baza.DodajPrijavu(Index, Predmet, DatumPrijaveISO,DatumPolaganjaISO);
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
			string Naziv;
			cout << "Unesite naziv predmete: ";
			getline(cin, Naziv);
			int dp, mp, gp;
			cout << "Unesite dan polaganja: ";
			cin >> dp;
			cout << "Unesite mjesec polaganja: ";
			cin >> mp;
			cout << "Unesite godinu polaganja: ";
			cin >> gp;
			cin.ignore();
			string DatumPolaganjaISO = to_string(gp) + "-" + (mp < 10 ? "0" : "") + to_string(mp) + "-" + (dp < 10 ? "0" : "") + to_string(dp);
			baza.UnesiRezultateIspita(Naziv, DatumPolaganjaISO);
			break;
		}
		case 8:
		{
			string Naziv;
			cout << "Unesite naziv predmeta: ";
			getline(cin, Naziv);
			int dp, mp, gp;
			cout << "Unesite dan polaganja: ";
			cin >> dp;
			cout << "Unesite mjesec polaganja: ";
			cin >> mp;
			cout << "Unesite godinu polaganja: ";
			cin >> gp;
			cin.ignore();
			string DatumPolaganjaISO = to_string(gp) + "-" + (mp < 10 ? "0" : "") + to_string(mp) + "-" + (dp < 10 ? "0" : "") + to_string(dp);
			baza.ObrisiIspit(Naziv, DatumPolaganjaISO);
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
			string Naziv;
			cout << "Unesite naziv predmeta: ";
			getline(cin, Naziv);
			int dp, mp, gp;
			cout << "Unesite dan polaganja: ";
			cin >> dp;
			cout << "Unesite mjesec polaganja: ";
			cin >> mp;
			cout << "Unesite godinu polaganja: ";
			cin >> gp;
			cin.ignore();
			string DatumPolaganjaISO = to_string(gp) + "-" + (mp < 10 ? "0" : "") + to_string(mp) + "-" + (dp < 10 ? "0" : "") + to_string(dp);
			baza.PrikaziRezultateZaTrazenoPolaganje(Naziv,DatumPolaganjaISO);
			break;
		}
		case 19:
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
	}while (izbor != 19);
}