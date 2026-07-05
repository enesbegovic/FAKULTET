# FAKULTET

Konzolna C++ aplikacija za upravljanje studentima, predmetima, prijavama i ispitima, sa SQLite bazom podataka.

## Tabele
- Studenti (PrezimeIme, DatumRodjenja, Index, GodinaStudija)
- Predmeti (Naziv, PredmetniProfesor, ECTSBodovi, GodinaKojojPredmetPripada)
- Prijave (StudentID, PredmetID, DatumPrijave)
- Ispiti (StudentID, PredmetID, DatumPrijaveID, DatumPolaganja, Polozio, Ocjena)

## Funkcionalnosti
- Dodavanje/brisanje studenata i predmeta
- Prijava i odjava ispita
- Unos rezultata ispita (položio/nije, ocjena)
- Prikaz studenata, predmeta, prijava i ispita po godini studija

## Tehnologije
- C++ 
- SQLite3
