# Projekat: Modelovanje društvene mreže pomoću grafova

## Ukratko

Ovaj projekat predstavlja model društvene mreže korišćenjem **usmerenih težinskih grafova** u programskom jeziku **C++**, u okviru predmeta *Algoritmi i strukture podataka*. Korisnici mreže predstavljeni su kao čvorovi grafa, a interakcije između njih kao grane sa težinama koje opisuju jačinu odnosa. Program omogućava učitavanje, izmenu i prikaz grafa, kao i simulaciju ponašanja korisnika u mreži kroz akcije poput „lajkovanja“, analizu povezanosti i identifikaciju uticajnih korisnika.

## Zahtev zadatka

Zadatak se sastoji u modelovanju društvene mreže pomoću usmerenog težinskog grafa, uz implementaciju sledećih funkcionalnosti:
- Učitavanje grafa iz fajla
- Dodavanje/brisanje čvorova i grana
- Prikaz grafa
- Simulacija lajkovanja
- Pronalazak najverovatnijeg puta između korisnika
- Identifikacija najuticajnijeg korisnika u mreži

[Detaljan opis zadatka možete pogledati ovde.](Zadatak.pdf)

## Opis rešenja

[Klikni ovde da pročitaš detaljan izveštaj.](Izvestaj.txt)

## Pokretanje

Za kompajliranje i pokretanje ovog projekta koristi se **CMake**.

```cmd
git clone https://github.com/petar-brankovic8/ETF-DSA-Cpp
cd ETF-DSA-Cpp

md build
cd build
cmake ..
cmake --build .

ETF-DSA-Cpp.exe
```

## Primer korišćenja

```text
Izaberite operaciju:
1 - Ucitaj socijalnu mrezu iz fajla
2 - Dodaj korisnika
3 - Ukloni korisnika
4 - Dodaj pracenje
5 - Ukloni pracenje
6 - Prikazi socijalnu mrezu
7 - Izbrisi socijalnu mrezu
8 - Lajkuj objavu
9 - Nadji najvecu strogo povezanu komponentu socijalne mreze
10 - Ispisi najverovatniji put od korisnika do korisnika
11 - Nadji k-tu najuticajniju osobu
12 - Ispisi meni sa operacijama
 0 - Prekid rada

Unesi broj operacije: 2
Unesi ime korisnika: Atanasije
Korisnik je uspešno dodat.

Unesi broj operacije: 4
Unesi ime korisnika koji zapraćuje: Atanasije
Unesi ime korisnika koji je zapraćen: Jovan
Unesi intenzitet odnosa [0, 1]: 0.4
Praćenje je uspešno dodato.

Unesi broj operacije: 6
Lista korisnika i njihovih konekcija:
    - Atanasije: Jovan(0.400)
    - Jovan:
```
