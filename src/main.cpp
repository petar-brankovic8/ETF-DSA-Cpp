#include <fstream>
#include <iostream>
#include <iomanip>
#include "Graph.h"

void clear_input() {	//Za reset-ovanje ulaza nakon neuspelog unosa
	cin.clear();
	char c;
	while (cin.get(c) && c != '\n');
}

int main()
{
	cout << fixed << setprecision(3);

	Graph SocialNetwork;

	cout << "Izaberite operaciju:\n" << "1 - Ucitaj socijalnu mrezu iz fajla\n" << "2 - Dodaj korisnika\n";
	cout << "3 - Ukloni korisnika\n" << "4 - Dodaj pracenje\n" << "5 - Ukloni pracenje\n";
	cout << "6 - Prikazi socijalnu mrezu\n" << "7 - Izbrisi socijalnu mrezu\n" << "8 - Lajkuj objavu\n";
	cout << "9 - Nadji najvecu strogo povezanu komponentu socijalne mreze\n" << "10 - Ispisi najverovatniji put od korisnika do korisnika\n";
	cout << "11 - Nadji k-tu najuticajniju osobu\n" << "12 - Ispisi meni sa operacijama\n" << " 0 - Prekid rada\n\n";

	int choice = 1;
	while (choice) {
		cout << "Unesi broj operacije: ";

		if (!(cin >> choice)) {
			cout << "Error: Nevalidan broj za izbor operacije.\n";
			clear_input();
			choice = 12;
		}
		switch (choice) {
		case 0:
			break;
		case 1: {
			//Ucitaj socijalnu mrezu iz fajla
			cout << "Unesi ime fajla: ";
			string fileName;
			if (!(cin >> fileName)) {
				cout << "Error: Nevalidno ime datoteke.\n";
				clear_input();
				break;
			}
			ifstream dat;
			dat.open(fileName);
			if (!dat.is_open()) {
				cout << "Error: Datoteka ne postoji.\n";
				break;
			}
			int errorStatus = SocialNetwork.Load_Graph(dat);
			if (!errorStatus) cout << "Socijalna mreza je uspesno ucitana.\n";
			else if (errorStatus == 1) cout << "Error: Neuspesno citanje broja cvorova 'n'.\n";
			else if (errorStatus == 2) cout << "Error: Prekoracen maksimalni broj korisnika.\n";
			else if (errorStatus == 3) cout << "Error: Neuspesno citanje broja grana 'e'.\n";
			else if (errorStatus == 4) cout << "Error: Negativan broj cvorova 'n'.\n";
			else if (errorStatus == 5) cout << "Error: Negativan broj grana 'e'.\n";
			else if (errorStatus == 6) cout << "Error: Neuspesno citanje imena korisnika.\n";
			else if (errorStatus == 7) cout << "Error: Neuspesno citanje grana.\n";
			else if (errorStatus == 8) cout << "Error: Korisnik sa datim imenom u grani ne postoji.\n";
			else if (errorStatus == 9) cout << "Error: Dupla grana izmedju dva korisnika.\n";
			dat.close();
			break;
		}
		case 2: {
			//Dodaj korisnika
			cout << "Unesi ime korisnika: ";
			string name;
			if (!(cin >> name)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}
			if (!SocialNetwork.Add_Vertice(name)) cout << "Korisnik je uspesno dodat.\n";
			else cout << "Error: Prekoracen maksimalni broj korisnika.\n";
			break;
		}
		case 3: {
			//Ukloni korisnika
			cout << "Unesi ime korisnika: ";
			string name;
			if (!(cin >> name)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}
			if (!SocialNetwork.Remove_Vertice(name)) cout << "Korisnik je uspesno uklonjen.\n";
			else cout << "Error: Korisnik \"" << name << "\" ne postoji.\n";
			break;
		}
		case 4: {
			//Dodaj pracenje
			string name1, name2;
			cout << "Unesi ime korisnika koji zapracuje: ";		
			if (!(cin >> name1)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}
			cout << "Unesi ime korisnika koji je zapracen: ";
			if (!(cin >> name2)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}
			cout << "Unesi intenzitet odnosa [0, 1]: ";
			double weight;
			if (!(cin >> weight)) {
				cout << "Error: Nevalidan broj za intenzitet odnosa.\n";
				clear_input();
				break;
			}
			if (weight < 0 || weight > 1) {
				cout << "Error: Intezitet odnosa nije u opsegu [0, 1].\n";
				break;
			}
			int errorStatus = SocialNetwork.Add_Edge(name1, name2, weight);
			if (!errorStatus) cout << "Pracenje je uspesno dodato.\n";
			else if (errorStatus == 1) cout << "Error: Korisnik \"" << name1 << "\" ne postoji.\n";
			else if (errorStatus == 2) cout << "Error: Korisnik \"" << name2 << "\" ne postoji.\n";
			break;
		}
		case 5: {
			//Ukloni pracenje
			cout << "Unesi ime korisnika koji otpracuje: ";
			string name1, name2;
			if (!(cin >> name1)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}
			cout << "Unesi ime korisnika koje je otpracen: ";
			if (!(cin >> name2)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}

			int errorStatus = SocialNetwork.Remove_Edge(name1, name2);
			if (!errorStatus) cout << "Pracenje je uspesno uklonjeno.\n";
			else if (errorStatus == 1) cout << "Error: Korisnik \"" << name1 << "\" ne postoji.\n";
			else if (errorStatus == 2) cout << "Error: Korisnik \"" << name2 << "\" ne postoji.\n";
			else if (errorStatus == 3) cout << "Error: Pracenje ne postoji.\n";
			break;
		}
		case 6: {
			//Prikazi socijalnu mrezu
			SocialNetwork.Display_Graph();
			break;
		}
		case 7: {
			//Izbrisi socijalnu mrezu
			SocialNetwork.Delete_Graph();
			cout << "Socijalna mreza je uspesno izbrisana.\n";
			break;
		}
		case 8: {
			//Lajkuj objavu
			cout << "Unesi ime korisnika koji lajkuje: ";
			string name1, name2;
			if (!(cin >> name1)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}
			cout << "Unesi ime korisnika cija je objava lajkovana: ";
			if (!(cin >> name2)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}

			int errorStatus = SocialNetwork.Like_Post(name1, name2);
			if (!errorStatus) cout << "Objava je uspesno lajkovana.\n";
			else if (errorStatus == 1) cout << "Error: Korisnik \"" << name1 << "\" ne postoji.\n";
			else if (errorStatus == 2) cout << "Error: Korisnik \"" << name2 << "\" ne postoji.\n";
			else if (errorStatus == 3) cout << "Error: Pracenje ne postoji.\n";
			break;
		}
		case 9: {
			//Nadji najvecu strogo povezanu komponentu socijalne mreze
			SocialNetwork.Largest_Component();
			break;
		}
		case 10: {
			//Ispisi najverovatniji put od korisnika do korisnika
			string name1, name2;
			cout << "Unesi ime prvog korisnika: ";
			if (!(cin >> name1)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}
			cout << "Unesi ime drugog korisnika: ";
			if (!(cin >> name2)) {
				cout << "Error: Nevalidno ime korisnika.\n";
				clear_input();
				break;
			}

			int errorStatus = SocialNetwork.Shortest_Path(name1, name2);
			if (errorStatus == 1) cout << "Error: Ne postoji put izmedju korisnika.\n";
			else if (errorStatus == 2) cout << "Error: Korisnik \"" << name1 << "\" ne postoji.\n";
			else if (errorStatus == 3) cout << "Error: Korisnik \"" << name2 << "\" ne postoji.\n";
			break;
		}
		case 11: {
			//Nadji k-tu najuticajniju osobu
			cout << "Unesi broj k: ";
			int k;
			if (!(cin >> k)) {
				cout << "Error: Nevalidno k.\n";
				clear_input();
				break;
			}
			if (SocialNetwork.K_Popular(k)) cout << "Error: Broj k mora biti u opsegu [1, n].\n";
			break;
		}
		case 12: {
			cout << "Izaberite operaciju:\n" << "1 - Ucitaj socijalnu mrezu iz fajla\n" << "2 - Dodaj korisnika\n";
			cout << "3 - Ukloni korisnika\n" << "4 - Dodaj pracenje\n" << "5 - Ukloni pracenje\n";
			cout << "6 - Prikazi socijalnu mrezu\n" << "7 - Izbrisi socijalnu mrezu\n" << "8 - Lajkuj objavu\n";
			cout << "9 - Nadji najvecu komponentu socijalne mreze\n" << "10 - Ispisi najverovatniji put od korisnika do korisnika\n";
			cout << "11 - Nadji k-tu najuticajniju osobu\n" << "12 - Ispisi meni sa operacijama\n" << "0 - Prekid rada\n\n";
			break;
		}
		default: {
			cout << "Error: Opcija ne postoji.\n";
		}
		}
		cout << '\n';
	}
	return 0;
}