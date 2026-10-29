#include <mutex>
#include <sodium.h>
#include <chrono>
#include <map>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "Baza.h"
using json = nlohmann::json;
int main() {
	if (sodium_init() < 0) {
		std::cout << "Libsodium nije inicijaliziran" << std::endl;
		return 1;
	}
	Baza baza("Fakultet.db");
	struct Sesija {
		string UserName;
		string Uloga;
		std::chrono::steady_clock::time_point Vrijeme;
	};
	std::map<string, Sesija>sesije;
	std::mutex sesijeMutex;
	baza.KreirajTabelu();
	httplib::Server server;
	server.set_pre_routing_handler([&sesije, &sesijeMutex](const httplib::Request& req, httplib::Response& res) {
		if (req.path == "/" || req.path == "/register" || req.path == "/login") {
			return httplib::Server::HandlerResponse::Unhandled;
		}
		string auth = req.get_header_value("Authorization");
		string Prefiks = "Bearer ";
		if (auth.rfind(Prefiks, 0) == 0) {
			string token = auth.substr(Prefiks.size());
			std::lock_guard<mutex>lock(sesijeMutex);
			auto it = sesije.find(token);
			if (it != sesije.end() && it->second.Vrijeme > std::chrono::steady_clock::now()) {
				bool Mijenja = (req.method == "POST" || req.method == "PUT" || req.method == "DELETE")
					&&req.path != "/logout";
				if (Mijenja && it->second.Uloga != "admin") {
					res.status = 403;
					res.set_content("{\"Greska\":\"Nemate dozvolu za ovu akciju\"}", "application/json");
					return httplib::Server::HandlerResponse::Handled;
				}
				return httplib::Server::HandlerResponse::Unhandled;
			}
		}
		res.status = 401;
		res.set_content("{\"Greska\":\"Nevazeci ili istekao token\"}", "application/json");
		return httplib::Server::HandlerResponse::Handled;
		});
	server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content("Server radi!!!", "text/plain");
		});
	server.Post("/register", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string UserName = tijelo.at("username");
			string PassWord = tijelo.at("password");
			char hash[crypto_pwhash_STRBYTES];
			if (crypto_pwhash_str(hash, PassWord.c_str(), PassWord.size(), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
				res.status = 500;
				res.set_content("{\"Greska\":\"Hashiranje nije uspijelo\"}", "application/json");
				return;
			}
			baza.DodajKorisnika(UserName, hash);
			json Poruka;
			Poruka["Poruka"] = "Korisnik uspijesno registrovan";
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
	server.Post("/login", [&baza, &sesije, &sesijeMutex](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string UserName = tijelo.at("username");
			string PassWord = tijelo.at("password");
			string hash = baza.UzmiHash(UserName);
			if (hash.empty() || crypto_pwhash_str_verify(hash.c_str(), PassWord.c_str(), PassWord.size()) != 0) {
				res.status = 401;
				res.set_content("{\"Greska\":\"Pogresno korisnicko ime ili lozinka\"}", "application/json");
				return;
			}
			unsigned char bajtovi[32];
			randombytes_buf(bajtovi, sizeof bajtovi);
			char token[sodium_base64_ENCODED_LEN(sizeof bajtovi, sodium_base64_VARIANT_URLSAFE_NO_PADDING)];
			sodium_bin2base64(token, sizeof token, bajtovi, sizeof bajtovi, sodium_base64_VARIANT_URLSAFE_NO_PADDING);
			string Uloga = baza.UzmiUlogu(UserName);
			{
				std::lock_guard<mutex>lock(sesijeMutex);
				for (auto it = sesije.begin(); it != sesije.end();) {
					if (it->second.Vrijeme < std::chrono::steady_clock::now()) {
						it = sesije.erase(it);
					}
					else {
						++it;
					}
				}
				sesije[token] = { UserName,Uloga,std::chrono::steady_clock::now()+std::chrono::hours(1)};
			}
			json Poruka;
			Poruka["Token"] = token;
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
	server.Post("/logout", [&sesije, &sesijeMutex](const httplib::Request& req, httplib::Response& res) {
		string auth = req.get_header_value("Authorization");
		string Prefiks = "Bearer ";
		string token = auth.substr(Prefiks.size());
		{
			std::lock_guard<mutex>lock(sesijeMutex);
			sesije.erase(token);
		}
		res.status = 200;
		res.set_content("{\"Poruka\":\"Odjava uspijesna\"}", "application/json");
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
	server.Get("/Ispiti", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Ispit>ispiti = baza.UzmiSveIspite();
		json rezultat = json::array();
		for (const auto& i : ispiti) {
			json ji;
			ji["ID"] = i.ID;
			ji["Ime i Prezime Studenta"] = i.ImePrezime;
			ji["Broj Indexa"] = i.BrIndexa;
			ji["Naziv Predmeta"] = i.Predmet;
			ji["Datum Polaganja"] = i.DatumPolaganja;
			ji["Ocjena"] = i.Ocjena;
			rezultat.push_back(ji);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/Ispiti", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			vector<pair<int, int>>rezultati;
			for (const auto& x : tijelo.at("rezultati")) {
				int PrijavaID = x.at("prijava");
				int Ocjena = x.at("ocjena");
				rezultati.push_back({ PrijavaID,Ocjena });
			}
			baza.UnesiRezultateIspita1(rezultati);
			json Poruka;
			Poruka["Poruka"] = "Rezultati su uspijesno uneseni";
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
	server.Delete("/Ispiti/:predmet/:datumpolaganja", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string Predmet = req.path_params.at("predmet");
			string DatumPolaganja = req.path_params.at("datumpolaganja");
			baza.ObrisiIspit(Predmet, DatumPolaganja);
			json Poruka;
			Poruka["Poruka"] = "Uspijeno uklonjeni rezultati ispita";
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
	server.listen("0.0.0.0", 8080);
	return 0;
}