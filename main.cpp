#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

struct Part {
    int id;
    int nr;
    string dir;     
    double lungime; 
    double angle;
    double vmedie;  
};

struct Circuit {
    int id;
    string nume;
    int nrParts;
    Part parts[100];
};

struct Piesa {
    int id;
    string nume;
    double pctDF;
};

struct Masina {
    int id;
    string nume;
    int nrPiese;
    double totPctDf;
    Piesa piesa[100];
};

struct Result {
    Masina car;
    double lapTime;
};

// ------------------ Citire ------------------
void citPartiMasina(Masina &m, const string& numeFisier) {
    ifstream fin(numeFisier);
    if (!fin) {
        cout << "Eroare fisier piese\n";
        m.nrPiese = 0;
        m.totPctDf = 0;
        return;
    }

    fin >> m.nrPiese;
    if (m.nrPiese > 100) m.nrPiese = 100;

    m.totPctDf = 0;
    for (int i = 0; i < m.nrPiese; i++) {
        fin >> m.piesa[i].id >> m.piesa[i].nume >> m.piesa[i].pctDF;
        m.totPctDf += m.piesa[i].pctDF;
    }
    fin.close();
}

void citPartsDinFisier(Circuit &c, const string& numeFisier) {
    ifstream fin(numeFisier);
    if (!fin) {
        cout << "Eroare fisier circuit\n";
        c.nrParts = 0;
        return;
    }

    fin >> c.nrParts;
    if (c.nrParts > 100) c.nrParts = 100;

    for (int i = 0; i < c.nrParts; i++) {
        fin >> c.parts[i].id
            >> c.parts[i].nr
            >> c.parts[i].dir
            >> c.parts[i].lungime
            >> c.parts[i].angle
            >> c.parts[i].vmedie;
    }
    fin.close();
}

// ------------------ Calcul Lap Time ------------------
double calcLapTime(const Masina &m, const Circuit &c) {
    double totalTime = 0.0;
    double dfFactor = m.totPctDf / 1000.0;

    for (int i = 0; i < c.nrParts; i++) {
        double speed = c.parts[i].vmedie;
        if (c.parts[i].dir == "S") speed *= (1.0 - 0.08 * dfFactor);
        else speed *= (1.0 + 0.12 * dfFactor);

        if (speed < 60) speed = 60;
        if (speed > 360) speed = 360;

        double speedMS = speed * 1000.0 / 3600.0;
        totalTime += c.parts[i].lungime / speedMS;
    }
    return totalTime;
}

// ------------------ Bubble Sort ------------------
void bubbleSort(Result arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j].lapTime > arr[j+1].lapTime) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// ------------------ Afisare ------------------
void afisMasina(const Masina &m) {
    cout << "\nMasina: " << m.nume << " (ID " << m.id << "), Total pctDF: " << m.totPctDf << "\n";
    for (int i = 0; i < m.nrPiese; i++) {
        cout << "  Piesa " << m.piesa[i].id
             << ": " << m.piesa[i].nume
             << ", pctDF=" << m.piesa[i].pctDF << "\n";
    }
}

void afisCircuit(const Circuit &c) {
    cout << "\nCircuit: " << c.nume << " (ID " << c.id << ")\n";
    cout << "Numar parti: " << c.nrParts << "\n";
    for (int j = 0; j < c.nrParts; j++) {
        cout << "  Part " << c.parts[j].nr
             << " | Dir=" << c.parts[j].dir
             << " | Lungime=" << c.parts[j].lungime << "m"
             << " | Unghi=" << c.parts[j].angle
             << " | Vmedie=" << c.parts[j].vmedie << " km/h\n";
    }
}

// ------------------ Menu Functions ------------------
void afiseazaMasini(Masina masini[], int nrMasini) {
    cout << "\n=== Masinile Introduse ===\n";
    for (int i = 0; i < nrMasini; i++) afisMasina(masini[i]);
}

void afiseazaClasament(Masina masini[], int nrMasini, Circuit circuite[], int nrCircuite) {
    cout << "\n=== Clasament Timpi ===\n";
    for (int j = 0; j < nrCircuite; j++) {
        cout << "\nCircuit: " << circuite[j].nume << "\n";

        Result results[10];
        for (int i = 0; i < nrMasini; i++) {
            results[i].car = masini[i];
            results[i].lapTime = calcLapTime(masini[i], circuite[j]);
        }

        bubbleSort(results, nrMasini);

        for (int i = 0; i < nrMasini; i++) {
            int min = int(results[i].lapTime) / 60;
            double sec = results[i].lapTime - min * 60;
            cout << i+1 << ". " << results[i].car.nume << " - "
                 << min << "m " << sec << "s\n";
        }
    }
}

// ------------------ Main ------------------
int main() {
    Masina masini[10];
    int nrMasini = 0;

    cout << "Cate masini? ";
    int nMasini; cin >> nMasini;

    for (int i = 0; i < nMasini && nrMasini < 10; i++) {
        Masina m;
        cout << "\nID masina: "; cin >> m.id;
        cout << "Nume masina: "; cin >> m.nume;
        cout << "Fisier piese: "; string fisier; cin >> fisier;
        citPartiMasina(m, fisier);
        masini[nrMasini++] = m;
    }

    Circuit circuite[10];
    int nrCircuite = 0;

    cout << "\nCate circuite? ";
    int nCircuite; cin >> nCircuite;

    for (int i = 0; i < nCircuite && nrCircuite < 10; i++) {
        Circuit c;
        cout << "\nID circuit: "; cin >> c.id;
        cout << "Nume circuit: "; cin >> c.nume;
        cout << "Fisier circuit: "; string fisier; cin >> fisier;
        citPartsDinFisier(c, fisier);
        circuite[nrCircuite++] = c;
    }

    // ------------------ Menu ------------------
    int option = 0;
    do {
        cout << "\n=== Meniu ===\n";
        cout << "1. Afiseaza masinile introduse\n";
        cout << "2. Afiseaza clasament timpi\n";
        cout << "0. Iesire\n";
        cout << "Optiune: "; cin >> option;

        switch(option) {
            case 1: afiseazaMasini(masini, nrMasini); break;
            case 2: afiseazaClasament(masini, nrMasini, circuite, nrCircuite); break;
            case 0: cout << "Program incheiat.\n"; break;
            default: cout << "Optiune invalida.\n";
        }
    } while(option != 0);

    return 0;
}
