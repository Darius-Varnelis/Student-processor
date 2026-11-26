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
        if (statistika == Statistika::Vidurkis) {
            out << st;
        } else {
            st.rasytiStudenta(out, statistika);
        }
    }
}

void rankinisIvedimas(bool zinomas_sk, Statistika statistika, Rezimas rezimas, std::ostream& output, Rusiuoti rusiuoti, Tvarka tvarka, Statistika rusiavimoStatistika) {
    const auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<std::mt19937::result_type>(seed));
    std::uniform_int_distribution<int> pazymiu_dist(MIN_PAZ, MAX_PAZ);
    std::vector<Studentas> grupe;
    const int studkiekis = ivestiSk("Kiek yra studentų? ", 0, BE_RIBOS);
    for (int i = 0; i < studkiekis; i++) {
        Studentas st;
        if (rezimas == Rezimas::Ranka && !zinomas_sk) {
            std::cin >> st;
        } else {
            st.ivestiRanka(zinomas_sk, rezimas, gen, pazymiu_dist, statistika);
        }
        if (!st.valid()) {
            std::cerr << "Įvestis nutraukta arba klaidinga. Sustabdoma.\n";
            break;
        }
        grupe.push_back(std::move(st));
    }
    rusiuotiStudentus(grupe, rusiuoti, tvarka, rusiavimoStatistika);
    rasytiGrupe(output, grupe, statistika);
}

std::vector<Studentas> apdorotiStudentus(const std::string& filename, Statistika statistika, Rusiuoti rusiuoti, Tvarka tvarka, Statistika rusiavimoStatistika) {
    std::vector<Studentas> grupe;
    nuskaitytiStudentus(grupe, filename);
    for (auto& st : grupe) {
        st.skaiciuotiGalutini(statistika);
    }
    rusiuotiStudentus(grupe, rusiuoti, tvarka, rusiavimoStatistika);
    return grupe;
}

void nuskaitytiIsFailo(const std::string& filename, Statistika statistika, std::ostream& output, Rusiuoti rusiuoti, Tvarka tvarka, Statistika rusiavimoStatistika) {
    auto grupe = apdorotiStudentus(filename, statistika, rusiuoti, tvarka, rusiavimoStatistika);
    rasytiGrupe(output, grupe, statistika);
}

void apdorotiIsFailoIFaila(const std::string& inputFilename, const std::string& outputFilename, Statistika statistika, Rusiuoti rusiuoti, Tvarka tvarka, Statistika rusiavimoStatistika) {
    auto grupe = apdorotiStudentus(inputFilename, statistika, rusiuoti, tvarka, rusiavimoStatistika);
    std::ofstream failas(outputFilename);
    if (!failas) {
        throw std::runtime_error("Nepavyko sukurti išvesties failo: " + outputFilename);
    }
    rasytiGrupe(failas, grupe, statistika);
}

void demonstracija() {
    Studentas a;
    if (taipArNe("Ar norite įvesti studentą ranka per std::cin >> st? (n = naudoti pavyzdinį srautą)")) {
        if (!(std::cin >> a) || !a.valid()) {
            std::cerr << "Nepavyko nuskaityti studento iš std::cin.\n";
            return;
        }
    } else {
        std::istringstream pavyzdys("Jonas Jonaitis 8 9 10 9");
        pavyzdys >> a;
        std::cout << "Nuskaityta iš srauto per operator>>: \"Jonas Jonaitis 8 9 10 9\"\n";
    }

    std::cout << "\n1. Originalus objektas 'a' (išvesta per std::cout << a):\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << a;

    std::cout << "\n2. Kopijavimo konstruktorius (Studentas b(a)):\n";
    Studentas b(a);
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << b;

    std::istringstream kitas("Ona Onaite 5 6 7 6");
    kitas >> b;
    std::cout << "Pakeitus 'b' per operator>> (\"Ona Onaite 5 6 7 6\"), 'a' lieka nepakitęs:\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << a << b;

    std::cout << "\n3. Kopijavimo priskyrimo operatorius (Studentas c; c = a;):\n";
    Studentas c;
    c = a;
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << c;

    Studentas& ref = c;
    c = ref;
    std::cout << "Po savęs priskyrimo (c = ref, kur this == &other):\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << c;

    std::cout << "\n4. Destruktorius (~Studentas()):\n";
    {
        Studentas laikinas(a);
        std::cout << "Sukurta laikina kopija vidiniame bloke:\n";
        rasytiAntraste(std::cout, Statistika::Vidurkis);
        std::cout << laikinas;
    }
    std::cout << "Vidinis blokas baigėsi (iškviestas ~Studentas()), 'a' vis dar galioja:\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << a;

    std::cout << "\n5. Išvestis ir įvestis iš failo per operator<< ir operator>>:\n";
    const std::string demoIsvestis = "demo_isvestis.txt";
    {
        std::ofstream isvestiesFailas(demoIsvestis);
        if (isvestiesFailas) {
            rasytiAntraste(isvestiesFailas, Statistika::Vidurkis);
            isvestiesFailas << a << b;
            std::cout << "Įrašyta į failą '" << demoIsvestis << "' per operator<<.\n";
        }
    }

    std::ifstream ivestiesFailas("kursiokai.txt");
    if (ivestiesFailas) {
        std::string antraste;
        std::getline(ivestiesFailas, antraste);
        std::cout << "Nuskaityta iš 'kursiokai.txt' per while (failas >> st):\n";
        rasytiAntraste(std::cout, Statistika::Vidurkis);
        Studentas st;
        while (ivestiesFailas >> st) {
            std::cout << st;
        }
    }
}