#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Part {
    int id;
    int nr;
    string dir;      
    float lungime;
    float angle;
    float vmedie; // average speed in km/h
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
    float pctDF; // downforce points
};

struct Masina {
    int id;
    string nume;
    int nrPiese;
    float totPctDf;
    Piesa piesa[100];
};

// ------------------ Citire ------------------
void citPartiMasina(Masina &m, const string& numeFisier) {
    ifstream fin(numeFisier);
    if (!fin) {
        cout << "Eroare la deschiderea fisierului: " << numeFisier << endl;
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
        cout << "Eroare la deschiderea fisierului: " << numeFisier << endl;
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

// ------------------ Calcul lap time ------------------
float calcLapTime(const Masina &m, const Circuit &c) {
    float totalLapTime = 0;

    for (int i = 0; i < c.nrParts; i++) {
        float speed = c.parts[i].vmedie; // baseline speed from circuit

        if (c.parts[i].dir == "S") {
            // More downforce slows the car on straights
            speed -= m.totPctDf * 0.5; 
        } else {
            // More downforce improves cornering speed
            speed += m.totPctDf * 0.7; 
        }

        if (speed < 1) speed = 1; // avoid zero or negative speed

        float speedMS = speed * 1000 / 3600.0; // km/h -> m/s
        totalLapTime += c.parts[i].lungime / speedMS;
    }

    return totalLapTime; // seconds
}

// ------------------ Main ------------------
int main() {
    // ------------------ Masini ------------------
    Masina masini[10];
    int nrMasini = 0;

    cout << "Cate masini vrei sa introduci? ";
    int nMasini;
    cin >> nMasini;

    for (int i = 0; i < nMasini && nrMasini < 10; i++) {
        Masina m;
        cout << "\nIntrodu ID masina: ";
        cin >> m.id;

        cout << "Introdu nume masina: ";
        cin >> m.nume;

        cout << "Introdu nume fisier piese: ";
        string fisierPiese;
        cin >> fisierPiese;

        citPartiMasina(m, fisierPiese);

        masini[nrMasini++] = m;
    }

    // Afisare masini
    for (int i = 0; i < nrMasini; i++) {
        afisMasina(masini[i]);
    }

    // ------------------ Circuite ------------------
    Circuit circuite[10];
    int nrCircuite = 0;

    cout << "\nCate circuite vrei sa introduci? ";
    int nCircuite;
    cin >> nCircuite;

    for (int i = 0; i < nCircuite && nrCircuite < 10; i++) {
        Circuit c;
        cout << "\nIntrodu ID circuit: ";
        cin >> c.id;

        cout << "Introdu nume circuit: ";
        cin >> c.nume;

        cout << "Introdu nume fisier cu partile circuitului: ";
        string fisierCircuit;
        cin >> fisierCircuit;

        citPartsDinFisier(c, fisierCircuit);

        if (c.nrParts > 0) {
            circuite[nrCircuite++] = c;
        }
    }

    // Afisare circuite
    for (int i = 0; i < nrCircuite; i++) {
        afisCircuit(circuite[i]);
    }

    // ------------------ Simulare lap time ------------------
    cout << "\n=== Simulare Lap Time ===\n";
    for (int i = 0; i < nrMasini; i++) {
        cout << "\nMasina: " << masini[i].nume << "\n";
        for (int j = 0; j < nrCircuite; j++) {
            float lapTime = calcLapTime(masini[i], circuite[j]);
            int minutes = int(lapTime) / 60;
            float seconds = lapTime - minutes * 60;
            cout << "  Circuit " << circuite[j].nume << ": "
                 << minutes << "m " << seconds << "s\n";
        }
    }

    return 0;
}
