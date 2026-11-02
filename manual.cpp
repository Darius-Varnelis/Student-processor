//
// Created by Darc on 10/13/2026.
//

#include "manual.h"
#include "studentas.h"
#include "utils.h"
#include <iostream>
#include <sstream>
using std::string;

void rasytiAntraste(std::ostream& out, Statistika statistika) {
    out << std::left << std::setw(PLOTIS) << "Vardas" << "|";
    out << std::setw(utf8Plotis("Pavardė")) << "Pavardė" << "|";
    if (statistika == Statistika::Vidurkis) {
        out << std::setw(PLOTIS) << "Galutinis (vidurkis)" << "|\n";
    } else if (statistika == Statistika::Mediana) {
        out << std::setw(PLOTIS) << "Galutinis (mediana)" << "|\n";
    } else if (statistika == Statistika::Abu) {
        out << std::setw(PLOTIS) << "Galutinis (vidurkis)" << "|" << std::setw(PLOTIS) << "Galutinis (mediana)" << "|\n";
    }
}


bool iSkaiciu(const string& tekstas, int& rezultatas) {
    try {
        size_t pabaiga;
        int sk = std::stoi(tekstas, &pabaiga);
        if (tekstas.find_first_not_of(" \t\r", pabaiga) != string::npos) return false;
        rezultatas = sk;
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

int ivestiSk(const string& klausimas, int nuo, int iki) {
    string eilute;
    std::cout << klausimas;
    while (std::getline(std::cin, eilute)) {
        int sk;
        if (!iSkaiciu(eilute, sk)) {
            std::cout << "Įvesta netinkama reikšmė (ne skaičius arba per didelis skaičius), bandykite vėl.\n";
        } else if (sk < nuo || sk > iki) {
            if (iki == BE_RIBOS) {
                std::cout << "Skaičius turi būti ne mažesnis nei " << nuo << ".\n";
            } else {
                std::cout << "Skaičius turi būti nuo " << nuo << " iki " << iki << ".\n";
            }
        } else {
            return sk;
        }
        std::cout << klausimas;
    }
    std::cout << "Įvedamas skaičius " << nuo << std::endl;
    return nuo;
}

bool taipArNe(const string& klausimas) {
    string eilute;
    std::cout << klausimas << " [y/n]: ";
    while (std::getline(std::cin, eilute)) {
        if (eilute == "y") {
            return true;
        }
        if (eilute == "n") {
            return false;
        }
        std::cout << "Įveskite \"n\" arba \"y\"\n";
        std::cout << klausimas << " [y/n]: ";
    }
    std::cout << "Nepavyko nuskaityti atsakymo, priskiaramas [n] atsakynas" << std::endl;
    return false;
}

int nustatymai(const string& klausimas, const string& pasirinkimai) {
    std::cout << klausimas << '\n';
    std::istringstream ss(pasirinkimai);
    string pasirinkimas;
    int i = 0;
    while (std::getline(ss, pasirinkimas, ';')) {
        std::cout << "[" << i << "] - " << pasirinkimas << '\n';
        i++;
    }
    return ivestiSk("Įveskite savo pasirinkimą (nuo 0 iki " + std::to_string(i - 1) + "): ", 0, i - 1);
}

void rasytiGrupe(std::ostream& out, const std::vector<Studentas>& grupe, Statistika statistika) {
    rasytiAntraste(out, statistika);
    for (const auto& st : grupe) {
        st.rasytiStudenta(out, statistika);
    }
}

void rankinisIvedimas(bool zinomas_sk, Statistika statistika, Rezimas rezimas, std::ostream& output, Rusiuoti rusiuoti, Tvarka tvarka) {
    const auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<std::mt19937::result_type>(seed));
    std::uniform_int_distribution<int> pazymiu_dist(MIN_PAZ, MAX_PAZ);
    std::vector<Studentas> grupe;
    const int studkiekis = ivestiSk("Kiek yra studentų? ", 0, BE_RIBOS);
    for (int i = 0; i < studkiekis; i++) {
        Studentas st(zinomas_sk, rezimas, gen, pazymiu_dist);
        st.skaiciuotiGalutini();
        grupe.push_back(st);
    }
    rusiuotiStudentus(grupe,rusiuoti,tvarka);
    rasytiGrupe(output, grupe, statistika);
}

void nuskaitytiIsFailo(const std::string& filename, Statistika statistika, std::ostream& output, Rusiuoti rusiuoti, Tvarka tvarka) {
    std::vector<Studentas> grupe;
    nuskaitytiStudentus(grupe, filename);
    for (auto& st : grupe) {
        st.skaiciuotiGalutini();
    }
    rusiuotiStudentus(grupe,rusiuoti,tvarka);
    rasytiGrupe(output,grupe,statistika);
}