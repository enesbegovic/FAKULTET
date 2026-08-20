#include <httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "Baza.h"
using json = nlohmann::json;
int main() {
	Baza baza("C:\\Users\\enesb\\source\\repos\\FAKULTET\\FAKULTET\\FAKULTET.db");
	baza.KreirajTabelu();
	httplib::Server server;
	server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content("Server radi!!!", "text/plain");
		});
	server.Get("/StudentiPrveGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Student>studenti = baza.UzmiSveStudenteSaOdredjeneGodine(1);
		json rezultat = json::array();
		for (const auto& s : studenti) {
			json js;
			js["ID"] = s.ID;
			js["Ime i Prezime"] = s.ImePrezime;
			js["Datum Rodjenja"] = s.DatumRodjenja;
			js["Broj Indexa"] = s.BrojIndexa;
			js["Godina Studija"] = s.Godina;
			rezultat.push_back(js);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/StudentiDrugeGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Student>studenti = baza.UzmiSveStudenteSaOdredjeneGodine(2);
		json rezultat = json::array();
		for (const auto& s : studenti) {
			json js;
			js["ID"] = s.ID;
			js["Ime i Prezime"] = s.ImePrezime;
			js["Datum Rodjenja"] = s.DatumRodjenja;
			js["Broj Indexa"] = s.BrojIndexa;
			js["Godina Studija"] = s.Godina;
			rezultat.push_back(js);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/StudentiTreceGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Student>studenti = baza.UzmiSveStudenteSaOdredjeneGodine(3);
		json rezultat = json::array();
		for (const auto& s : studenti) {
			json js;
			js["ID"] = s.ID;
			js["Ime i Prezime"] = s.ImePrezime;
			js["Datum Rodjenja"] = s.DatumRodjenja;
			js["Broj Indexa"] = s.BrojIndexa;
			js["Godina Studija"] = s.Godina;
			rezultat.push_back(js);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/StudentiPrveGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string ImePrezime = tijelo.at("imeprezime");
			string DatumRodjenja = tijelo.at("datumrodjenja");
			string index = tijelo.at("index");
			int godina = tijelo.at("godina");
			baza.KreirajStudenta(ImePrezime, DatumRodjenja, index, godina);
			json Poruka;
			Poruka["Poruka"] = "Student je uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/StudentiPrveGodine/:index", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string Index = req.path_params.at("index");
			baza.UkloniStudenta(Index);
			json Poruka;
			Poruka["Poruka"] = "Student uspijesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Post("/StudentiDrugeGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string ImePrezime = tijelo.at("imeprezime");
			string DatumRodjenja = tijelo.at("datumrodjenja");
			string index = tijelo.at("index");
			int godina = tijelo.at("godina");
			baza.KreirajStudenta(ImePrezime, DatumRodjenja, index, godina);
			json Poruka;
			Poruka["Poruka"] = "Student je uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/StudentiDrugeGodine/:index", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string Index = req.path_params.at("index");
			baza.UkloniStudenta(Index);
			json Poruka;
			Poruka["Poruka"] = "Student uspijesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Post("/StudentiTreceGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string ImePrezime = tijelo.at("imeprezime");
			string DatumRodjenja = tijelo.at("datumrodjenja");
			string index = tijelo.at("index");
			int godina = tijelo.at("godina");
			baza.KreirajStudenta(ImePrezime, DatumRodjenja, index, godina);
			json Poruka;
			Poruka["Poruka"] = "Student je uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/StudentiTreceGodine/:index", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string Index = req.path_params.at("index");
			baza.UkloniStudenta(Index);
			json Poruka;
			Poruka["Poruka"] = "Student uspijesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/PredmetiPrveGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Predmet>predmeti = baza.UzmiSvePredmeteSaOdredjeneGodine(1);
		json rezultat = json::array();
		for (const auto& p : predmeti) {
			json jp;
			jp["ID"] = p.ID;
			jp["Naziv"] = p.Naziv;
			jp["Predmetni Profesor"] = p.Profesor;
			jp["ECTS Bodovi"] = p.ECTS;
			jp["Godina Na Kojoj Se Predmet Nalazi:"] = p.Godina;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/PredmetiDrugeGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Predmet>predmeti = baza.UzmiSvePredmeteSaOdredjeneGodine(2);
		json rezultat = json::array();
		for (const auto& p : predmeti) {
			json jp;
			jp["ID"] = p.ID;
			jp["Naziv"] = p.Naziv;
			jp["Predmetni Profesor"] = p.Profesor;
			jp["ECTS Bodovi"] = p.ECTS;
			jp["Godina Na Kojoj Se Predmet Nalazi:"] = p.Godina;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/PredmetiTreceGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Predmet>predmeti = baza.UzmiSvePredmeteSaOdredjeneGodine(3);
		json rezultat = json::array();
		for (const auto& p : predmeti) {
			json jp;
			jp["ID"] = p.ID;
			jp["Naziv"] = p.Naziv;
			jp["Predmetni Profesor"] = p.Profesor;
			jp["ECTS Bodovi"] = p.ECTS;
			jp["Godina Na Kojoj Se Predmet Nalazi:"] = p.Godina;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/PredmetiPrveGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string Naziv = tijelo.at("naziv");
			string Profesor = tijelo.at("profesor");
			int ECTS = tijelo.at("ECTS");
			int Godina = tijelo.at("godina");
			baza.KreirajPredmet(Naziv, Profesor, ECTS, Godina);
			json Poruka;
			Poruka["Poruka"] = "Predmet je uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/PredmetiPrveGodine/:naziv", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string naziv = req.path_params.at("naziv");
			baza.UkloniPredmet(naziv);
			json Poruka;
			Poruka["Poruka"] = "Predmet uspijesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Post("/PredmetiDrugeGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string Naziv = tijelo.at("naziv");
			string Profesor = tijelo.at("profesor");
			int ECTS = tijelo.at("ECTS");
			int Godina = tijelo.at("godina");
			baza.KreirajPredmet(Naziv, Profesor, ECTS, Godina);
			json Poruka;
			Poruka["Poruka"] = "Predmet je uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/PredmetiDrugeGodine/:naziv", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string naziv = req.path_params.at("naziv");
			baza.UkloniPredmet(naziv);
			json Poruka;
			Poruka["Poruka"] = "Predmet uspijesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Post("/PredmetiTreceGodine", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string Naziv = tijelo.at("naziv");
			string Profesor = tijelo.at("profesor");
			int ECTS = tijelo.at("ECTS");
			int Godina = tijelo.at("godina");
			baza.KreirajPredmet(Naziv, Profesor, ECTS, Godina);
			json Poruka;
			Poruka["Poruka"] = "Predmet je uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/PredmetiTreceGodine/:naziv", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string naziv = req.path_params.at("naziv");
			baza.UkloniPredmet(naziv);
			json Poruka;
			Poruka["Poruka"] = "Predmet uspijesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/PrijaveZaPrvuGodinu", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Prijava>prijave = baza.UzmiSvePrijaveZaOdredjenuGodinu(1);
		json rezultat = json::array();
		for (const auto& p : prijave) {
			json jp;
			jp["ID"] = p.ID;
			jp["Ime i Prezime Studenta"] = p.ImePrezimeStudenta;
			jp["Broj Indexa"] = p.BrojIndexa;
			jp["Godina Studija"] = p.Godina;
			jp["Naziv Predmeta"] = p.NazivPredmeta;
			jp["Ime i Prezime Predmetnog Profesora"] = p.ImePrezimeProfesora;
			jp["ECTS Bodovi"] = p.ECTS;
			jp["Datum Prijave Ispita"] = p.DatumPrijave;
			jp["Datum Polaganja Ispita"] = p.DatumPolaganja;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/PrijaveZaDruguGodinu", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Prijava>prijave = baza.UzmiSvePrijaveZaOdredjenuGodinu(2);
		json rezultat = json::array();
		for (const auto& p : prijave) {
			json jp;
			jp["ID"] = p.ID;
			jp["Ime i Prezime Studenta"] = p.ImePrezimeStudenta;
			jp["Broj Indexa"] = p.BrojIndexa;
			jp["Godina Studija"] = p.Godina;
			jp["Naziv Predmeta"] = p.NazivPredmeta;
			jp["Ime i Prezime Predmetnog Profesora"] = p.ImePrezimeProfesora;
			jp["ECTS Bodovi"] = p.ECTS;
			jp["Datum Prijave Ispita"] = p.DatumPrijave;
			jp["Datum Polaganja Ispita"] = p.DatumPolaganja;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/PrijaveZaTrecuGodinu", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Prijava>prijave = baza.UzmiSvePrijaveZaOdredjenuGodinu(3);
		json rezultat = json::array();
		for (const auto& p : prijave) {
			json jp;
			jp["ID"] = p.ID;
			jp["Ime i Prezime Studenta"] = p.ImePrezimeStudenta;
			jp["Broj Indexa"] = p.BrojIndexa;
			jp["Godina Studija"] = p.Godina;
			jp["Naziv Predmeta"] = p.NazivPredmeta;
			jp["Ime i Prezime Predmetnog Profesora"] = p.ImePrezimeProfesora;
			jp["ECTS Bodovi"] = p.ECTS;
			jp["Datum Prijave Ispita"] = p.DatumPrijave;
			jp["Datum Polaganja Ispita"] = p.DatumPolaganja;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/PrijaveZaPrvuGodinu", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string Index = tijelo.at("index");
			string Predmet = tijelo.at("predmet");
			string DatumPrijave = tijelo.at("prijava");
			string DatumIspita = tijelo.at("ispit");
			baza.DodajPrijavu(Index, Predmet, DatumPrijave, DatumIspita);
			json Poruka;
			Poruka["Poruka"] = "Prijava je uspijesno dodana";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/PrijaveZaPrvuGodinu/:index/:predmet", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string Index = req.path_params.at("index");
			string Predmet = req.path_params.at("predmet");
			baza.UkloniPrijavu(Index, Predmet);
			json Poruka;
			Poruka["Poruka"] = "Prijava je uspijesno uklonjena";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Post("/PrijaveZaDruguGodinu", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string Index = tijelo.at("index");
			string Predmet = tijelo.at("predmet");
			string DatumPrijave = tijelo.at("prijava");
			string DatumIspita = tijelo.at("ispit");
			baza.DodajPrijavu(Index, Predmet, DatumPrijave, DatumIspita);
			json Poruka;
			Poruka["Poruka"] = "Prijava je uspijesno dodana";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/PrijaveZaDruguGodinu/:index/:predmet", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string Index = req.path_params.at("index");
			string Predmet = req.path_params.at("predmet");
			baza.UkloniPrijavu(Index, Predmet);
			json Poruka;
			Poruka["Poruka"] = "Prijava je uspijesno uklonjena";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Post("/PrijaveZaTrecuGodinu", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string Index = tijelo.at("index");
			string Predmet = tijelo.at("predmet");
			string DatumPrijave = tijelo.at("prijava");
			string DatumIspita = tijelo.at("ispit");
			baza.DodajPrijavu(Index, Predmet, DatumPrijave, DatumIspita);
			json Poruka;
			Poruka["Poruka"] = "Prijava je uspijesno dodana";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/PrijaveZaTrecuGodinu/:index/:predmet", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string Index = req.path_params.at("index");
			string Predmet = req.path_params.at("predmet");
			baza.UkloniPrijavu(Index, Predmet);
			json Poruka;
			Poruka["Poruka"] = "Prijava je uspijesno uklonjena";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	std::cout << "Server pokrenut na http://localhost:8080" << std::endl;
	server.listen("localhost", 8080);
	return 0;
}