# FAKULTET

![Build and Test](https://github.com/enesbegovic/FAKULTET/actions/workflows/build.yml/badge.svg)

Konzolna C++ aplikacija za upravljanje studentima, predmetima, prijavama i ispitima, sa SQLite bazom podataka. Sav rad sa bazom ide preko prepared statements.

## Tehnologije

- C++ (Visual Studio, konzolna aplikacija)
- SQLite3 (biblioteka `sqlite3.h` / `sqlite3.c`)
- Git / GitHub za verzionisanje

## Struktura baze

### Studenti
- `ID` (PRIMARY KEY AUTOINCREMENT)
- `PrezimeIme`
- `DatumRodjenja`
- `Index` (UNIQUE)
- `GodinaStudija` (CHECK 1–5)

### Predmeti
- `ID` (PRIMARY KEY AUTOINCREMENT)
- `Naziv` (UNIQUE)
- `PredmetniProfesor`
- `ECTSBodovi`
- `GodinaKojojPredmetPripada` (CHECK 1–5)

### Prijave
- `ID` (PRIMARY KEY AUTOINCREMENT)
- `StudentID` (FOREIGN KEY → Studenti, ON DELETE RESTRICT)
- `PredmetID` (FOREIGN KEY → Predmeti, ON DELETE RESTRICT)
- `DatumPrijave`
- `DatumOdrzavanjaIspita`

### Ispiti
- `ID` (PRIMARY KEY AUTOINCREMENT)
- `PrijavaID` (FOREIGN KEY → Prijave, UNIQUE, ON DELETE RESTRICT)
- `Ocjena` (CHECK 5–10; 5 = pao, 6–10 = prosao)

`Polozio` se ne cuva kao posebna kolona vec se izvodi iz `Ocjena` (Ocjena >= 6 = polozio).

## Funkcionalnosti

**Studenti**
- Dodaj studenta
- Ukloni studenta
- Prikazi sve studente prema godini studija

**Predmeti**
- Dodaj predmet
- Ukloni predmet
- Prikazi sve predmete prema godini

**Prijave**
- Dodaj prijavu (student + predmet + datum prijave + datum odrzavanja ispita)
- Ukloni prijavu
- Prikazi sve prijave prema godini

**Ispiti**
- Unesi rezultate ispita — automatski pronalazi sve prijavljene studente za dati predmet i datum odrzavanja, unosi ocjenu za svakog (ili automatski ocjenu 5 ako student nije izasao)
- Ukloni ispit (sve rezultate za dati predmet i datum)
- Prikazi rezultate za trazeno polaganje (predmet + datum)

## Kljucni koncepti koriscени u projektu

- Prepared statements za sve upite (bind/step/reset umjesto sqlite3_exec)
- FOREIGN KEY constraint-i sa ON DELETE RESTRICT
- CHECK constraint-i za validaciju (ocjena, godina studija/predmeta)
- UNIQUE constraint-i (Index, Naziv predmeta, PrijavaID)
- Transakcije (BEGIN/COMMIT) za grupne operacije (npr. unos rezultata za vise studenata odjednom)
- JOIN upiti za povezivanje podataka kroz vise tabela (Ispiti → Prijave → Studenti/Predmeti)
- Ponovno koriscenje pripremljenog statementa u petlji (prepare jednom, bind+step+reset za svaki red)

## Pokretanje

1. Otvoriti `FAKULTET.slnx` u Visual Studio-u
2. Build (Debug/x64)
3. Pokrenuti — baza (`Fakultet.db`) se automatski kreira pri prvom pokretanju ako ne postoji

## Testiranje

Projekat sadrži 15 jediničnih testova (Google Test) koji pokrivaju rad sa Studentima, Predmetima, Prijavama i Ispitima, uključujući unos i provjeru ocjena.

Testovi se nalaze u `FAKULTET_Testovi` projektu i mogu se pokrenuti kroz Visual Studio Test Explorer (Test → Run All Tests).

## CI/CD

Svaki push na `master` granu automatski pokreće build i sve testove preko GitHub Actions (vidi `.github/workflows/build.yml`).
