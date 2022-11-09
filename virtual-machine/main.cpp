#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

//---------ZAPISYWANIE PROGRAMU------------------------------------

const int B1 = 2;  // ilosc bajtow liczby 1 (instrukcja)
const int B2 = 4;	// ilosc bajtow liczby 2 (skok lub stala)

class program {
public:
	program(ofstream* plik) : plik(plik) {}
	void p1();
	void p2();
	void p3();
private:
	ofstream* plik;

	void dodaj(char r2, char r1);
	void odejmij(char r2, char r1);
	void mnoz(char r2, char r1);
	void dziel_reszta(char r2, char r1);
	void porownaj(char r2, char r1);
	void kopiuj(char r2, char r1);
	void skocz(char r1, int skok);
	void czytaj_stala(char r1, int stala);
	void czytaj_z_klaw(char r1);
	void wypisz(char r1);
	void zakoncz();

	short operacja(char r2, char r1, char op);
	void zapisz(const char* p, int ile);    // zapisuje do pliku binarnego; ile- ile bajtow zapisac
};

void ZapiszProgram();
void opis_programow();
void wybierz_program(program pr, int n);

//----------URUCHAMIANIE PROGRAMU--------------------------------------

const int MAX_R = 64;   // ilosc rejestrow

class instrukcja {
public:
	short ins = 0;
	int liczba = 0;
	instrukcja* nast = 0;

	char czytR2();
	char czytR1();
	char czytOp();
	int  czytLiczbe();
};

class VM {
public:
	void uruchom(const instrukcja* p);
	void wykonaj_program();
	void wczytaj_program();
private:
	int R[MAX_R];
	char F;     // rejestr flag
	int I;      // rejestr instrukcji
	void rFlag(short x);   // ustawia rejestr flag
	instrukcja* T; // wskaznik na tablice z instrukcjami dla vm
	unsigned int ilosc_instrukcji = 0;
	bool czy_skok(short x);
	bool koniec;
	void Op0();
	void Op1();
	void Op2();
	void Op3();
	void Op4();
	void Op5();
	void Op6();
	void Op7();
	void Op8();
	void Op9();
	void Op10();

	void zeruj_rejestry();
	void wykonaj_instrukcje(instrukcja x);
};

void UruchomProgram();
void usun_tablice(instrukcja* t);


//===========================================================================================================================
//===========================================================================================================================

int main() {
	char x;

	while (true) {
		cout << "Co chcesz zrobic? 'z'- zapisz program do pliku," << endl
			<< "'u'- uruchom program z pliku, inny klawisz- zakoncz program: ";
		cin >> x;
		cout << endl;

		if (x == 'z') ZapiszProgram();
		else if (x == 'u') UruchomProgram();
		else break;
		cout << endl << endl;
	}

	return 0;
}
//===========================================================================================================================
//===========================================================================================================================


void ZapiszProgram() {
	const string ROZSZERZENIE = ".vm";
	ofstream plik_WY;
	string nazwa;

	cout << "Podaj nazwe pliku (bez rozszerzenia): ";
	cin >> nazwa;

	nazwa = nazwa + ROZSZERZENIE;

	while (true) {
		plik_WY.open(nazwa, ios::binary | ios::out | ios::ate);
		if (!plik_WY) {
			cout << "Nie mozna utworzyc pliku! Podaj inna nazwe: ";
			cin >> nazwa;
		}
		else
			break;
	}

	program prog(&plik_WY);

	int nr_prog;
	opis_programow();

	cout << "Wybierz numer programu, ktory chcesz zapisac do pliku: ";
	cin >> nr_prog;
	while (true) {
		if (nr_prog < 1 || nr_prog>3) {
			cout << "Blad! Podaj inny numer: ";
			cin >> nr_prog;
		}
		else break;
	}

	cout << endl;

	wybierz_program(prog, nr_prog);

	plik_WY.close();
}




short program::operacja(char r2, char r1, char op) {
	return (r2 << 10) + (r1 << 4) + op;
}

void program::dodaj(char r2, char r1) {
	short a = operacja(r2, r1, 0);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::odejmij(char r2, char r1) {
	short a = operacja(r2, r1, 1);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::mnoz(char r2, char r1) {
	short a = operacja(r2, r1, 2);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::dziel_reszta(char r2, char r1) {
	short a = operacja(r2, r1, 3);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::porownaj(char r2, char r1) {
	short a = operacja(r2, r1, 4);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::kopiuj(char r2, char r1) {
	short a = operacja(r2, r1, 5);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::skocz(char r1, int skok) {
	short a = operacja(0, r1, 6);
	zapisz(reinterpret_cast <const char*>(&a), B1);
	zapisz(reinterpret_cast <const char*>(&skok), B2);
}

void program::czytaj_stala(char r1, int stala) {
	short a = operacja(0, r1, 7);
	zapisz(reinterpret_cast <const char*>(&a), B1);
	zapisz(reinterpret_cast <const char*>(&stala), B2);
}

void program::czytaj_z_klaw(char r1) {
	short a = operacja(0, r1, 8);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::wypisz(char r1) {
	short a = operacja(0, r1, 9);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::zakoncz() {
	short a = operacja(0, 0, 10);
	zapisz(reinterpret_cast <const char*>(&a), B1);
}

void program::zapisz(const char* p, int ile) {
	plik->write(p, ile);
}

void wybierz_program(program pr, int n) {
	switch (n) {
	case 1: pr.p1(); break;
	case 2: pr.p2(); break;
	case 3: pr.p3(); break;
	}
}

void opis_programow() {
	cout << "Wybierz jeden z programow:\n"
		<< endl
		<< "PROGRAM 1- POTEGOWANIE:\n"
		<< "\t Program zczytuje najpierw podstawe, a nastepnie wykladnik potegi\n"
		<< "\t i zwraca wynik potegowania.\n"
		<< "\t UWAGA! Program dziala tylko na liczbach calkowitych i nie wykonuje\n"
		<< "\t potegowania do liczby ujemnej.\n"
		<< endl
		<< "PROGRAM 2- TROJKAT:\n"
		<< "\t Program zczytuje dlugosci trzech odcinkow i sprawdza, czy da sie z nich\n"
		<< "\t zbudowac trojkat. Jesli tak- zwraca '1', jesli nie- zwraca '0'.\n"
		<< endl
		<< "PROGRAM 3- CIAG ARYTMETYCZNY:\n"
		<< "\t Program oblicza sume n poczatkowych wyrazow ciagu arytmetycznego.\n"
		<< "\t Program z czytuje po kolei: pierwszy wyraz ciagu, roznice ciagu\n"
		<< "\t i ilosc pierwszych wyrazow do zsumowania (n)."
		<< endl << endl;
}

void program::p1() {
	czytaj_stala(0, 1);
	czytaj_z_klaw(1);  // podstawa potegi- r1 = a
	czytaj_z_klaw(2);  // wykladnik potegi- r2 = n
	kopiuj(1, 5);
	kopiuj(0, 3);  // r3- licznik
	porownaj(4, 2);   // sprawdza znak wykladnika
	skocz(4, 9); // jesli n<0, skacze do komendy 'zakoncz'
	skocz(1, 7); // jesli n=0, skacze do komendy 'wypisz r0'
	mnoz(1, 5);
	dodaj(0, 3); // zwieksza licznik
	porownaj(3, 2);
	skocz(3, -3);
	wypisz(5);
	skocz(0, 2);
	wypisz(0);
	zakoncz();
}

void program::p2() {
	czytaj_stala(0, 0);
	czytaj_z_klaw(1);  // dlugosc pierwszego odcinka- r1 = a
	porownaj(0, 1);
	skocz(6, 20);
	czytaj_z_klaw(2); // dlugosc drugiego odcinka- r2 = b
	porownaj(0, 2);
	skocz(6, 17);
	czytaj_z_klaw(3);   // dlugosc trzeciego odcinka- r3 = c
	porownaj(0, 3);
	skocz(6, 14);
	kopiuj(1, 4);
	kopiuj(1, 5);
	kopiuj(2, 6);
	dodaj(2, 4);         // r4 = a + b
	dodaj(3, 5);         // r5 = a + c
	dodaj(3, 6);         // r6 = b + c
	porownaj(3, 4);
	skocz(6, 6);
	porownaj(2, 5);
	skocz(6, 4);
	porownaj(1, 6);
	skocz(6, 2);
	czytaj_stala(0, 1);
	wypisz(0);
	zakoncz();
}

void program::p3() {
	czytaj_stala(1, 1);
	czytaj_stala(2, 2);
	czytaj_z_klaw(3);  // pierwszy wyraz ciagu- r3 = a1
	czytaj_z_klaw(4);   // roznica ciagu- r4 = r
	czytaj_z_klaw(5);   // ile pierwszych wyrazow zsumowac?- r5 = n
	kopiuj(5, 6);
	odejmij(1, 6);       // r6 = n - 1
	mnoz(6, 4);      // r4 = r * (n - 1)
	mnoz(2, 3);      // r3 = 2 * a1
	dodaj(4, 3);     // r3 = 2*a1 + r*(n-1)
	mnoz(5, 3);      // r3 = ( 2*a1 + r*(n-1) ) * n
	dziel_reszta(2, 3);  // r3 = 0,5*n*( 2*a1 + r*(n-1) )
	wypisz(3);
	zakoncz();
}

//**********************************************************************************************************************

void UruchomProgram() {
	VM maszyna;
	maszyna.wczytaj_program();
	maszyna.wykonaj_program();
}

char instrukcja::czytR2() {
	char w = (ins >> 10) & 0x3f;
	return w;
}

char instrukcja::czytR1() {
	char w = (ins >> 4) & 0x3f;
	return w;
}

char instrukcja::czytOp() {
	char w = ins & 0xf;
	return w;
}

int instrukcja::czytLiczbe() {
	int w = liczba;
	return w;
}

void VM::zeruj_rejestry() {
	for (int i = 0; i < MAX_R; i++) {
		R[i] = 0;
	}
	I = 0;
	F = 'Z';
	koniec = false;
}

void VM::rFlag(short x) {
	if (x == 0) F = 'Z';
	else if (x > 0) F = 'D';
	else F = 'U';
}

void VM::Op0() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	char r2 = a.czytR2();
	R[r1] += R[r2];
	rFlag(R[r1]);
	I++;
}

void VM::Op1() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	char r2 = a.czytR2();
	R[r1] -= R[r2];
	rFlag(R[r1]);
	I++;
}

void VM::Op2() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	char r2 = a.czytR2();
	R[r1] *= R[r2];
	rFlag(R[r1]);
	I++;
}

void VM::Op3() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	char r2 = a.czytR2();
	char r = R[r1] % R[r2];
	R[r1] /= R[r2];
	R[r2] = r;
	rFlag(R[r1]);
	I++;
}

void VM::Op4() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	char r2 = a.czytR2();
	int x = R[r1] - R[r2];
	rFlag(x);
	I++;
}

void VM::Op5() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	char r2 = a.czytR2();
	R[r1] = R[r2];
	I++;
}

void VM::Op6() {
	instrukcja a = T[I];
	int liczba = a.czytLiczbe();
	bool skok = czy_skok(a.czytR1());
	if (skok && liczba != 0) I += liczba;
	else I++;
}

void VM::Op7() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	R[r1] = a.czytLiczbe();
	I++;
}

void VM::Op8() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	int x;
	cout << "Podaj liczbe: ";
	cin >> x;
	R[r1] = x;
	I++;
}

void VM::Op9() {
	instrukcja a = T[I];
	char r1 = a.czytR1();
	cout << "R" << (int)r1 << ": " << R[r1] << endl;
	I++;
}

void VM::Op10() {
	koniec = true;
	I++;
}

bool VM::czy_skok(short x) {
	bool s = false;
	if ((x == 0) ||
		(x == 1 && F == 'Z') ||
		(x == 2 && F != 'Z') ||
		(x == 3 && F == 'D') ||
		(x == 4 && F == 'U') ||
		(x == 5 && F != 'U') ||
		(x == 6 && F != 'D')) s = true;
	return s;
}

void VM::wykonaj_instrukcje(instrukcja x) {
	short r1 = x.czytR1();
	short r2 = x.czytR2();
	cout << "\t\t\t\t\tI: " << I << " Instrukcja: " << x.ins << " " << x.liczba << endl;
	switch (x.czytOp()) {
	case 0: Op0(); break;
	case 1: Op1(); break;
	case 2: Op2(); break;
	case 3: Op3(); break;
	case 4: Op4(); break;
	case 5: Op5(); break;
	case 6: Op6(); break;
	case 7: Op7(); break;
	case 8: Op8(); break;
	case 9: Op9(); break;
	case 10: Op10(); break;
	default: cout << "Blad!" << endl; break;
	}
	cout << "\t\t\t\t\tR" << r1 << "=" << R[x.czytR1()] << "; "
		<< "R" << r2 << "=" << R[x.czytR2()] << "; "
		<< "flaga: " << F << endl << endl;
}

void VM::wczytaj_program() {
	string sciezka;

	instrukcja* pierwsza_inst = new instrukcja();
	instrukcja* iterator = pierwsza_inst;
	ilosc_instrukcji = 0;

	ifstream plik;

	cout << "Podaj sciezke do pliku: ";
	cin >> sciezka;
	cout << endl;

	while (true) {
		plik.open(sciezka, ios::binary | ios::in);
		if (!plik) {
			cout << "Nie mozna utworzyc pliku! Podaj inna sciezke: ";
			cin >> sciezka;
			cout << endl;
		}
		else
			break;
	}

	while (true) {
		plik.read(reinterpret_cast<char*>(&(iterator->ins)), 2);
		if (iterator->czytOp() == 6 || iterator->czytOp() == 7)
			plik.read(reinterpret_cast <char*>(&(iterator->liczba)), 4);
		ilosc_instrukcji++;
		if (!plik.eof()) {
			iterator->nast = new instrukcja();
			iterator = iterator->nast;
		}
		else break;
	}

	T = new instrukcja[ilosc_instrukcji];
	iterator = pierwsza_inst;
	for (unsigned int i = 0; i < ilosc_instrukcji; i++) {
		T[i] = *iterator;
		if (iterator->nast == 0) break;
		iterator = iterator->nast;
	}
	usun_tablice(pierwsza_inst);
}

void usun_tablice(instrukcja* t) {
	do {
		instrukcja* p;
		p = t;
		t = t->nast;
		delete p;
	} while (t->nast != 0);
}

void VM::wykonaj_program() {
	zeruj_rejestry();
	while (!koniec && I < ilosc_instrukcji) {
		wykonaj_instrukcje(T[I]);
	}
	cout << "Koniec programu" << endl;
}