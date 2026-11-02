//
// Created by darc on 02/11/2026.
//
#include "studentas.h"
#include <random>
#include <string>
#include <iostream>
#include <chrono>
#include "manual.h"
#include "nustatymai.h"

Studentas::Studentas(std::istream& is, int ndKiekis) {
    skaitytiFaila(is, ndKiekis);
}
std::istream& Studentas::skaitytiFaila(std::istream& is, int ndKiekis) {
    if (!(is >> vardas_ >> pavarde_)) return is;

    paz_.resize(ndKiekis);
    for (int i = 0; i < ndKiekis; ++i) {
        is >> paz_[i];
    }
    is >> exam_;

    return is;
}
Studentas::Studentas(bool zinomas_sk, Rezimas rezimas,std::mt19937 gen, std::uniform_int_distribution<int> pazymiu_dist) {
    ivestiRanka(zinomas_sk, rezimas, gen, pazymiu_dist);
}


void Studentas::ivestiRanka(bool zinomas_sk, Rezimas rezimas,std::mt19937 gen, std::uniform_int_distribution<int> pazymiu_dist) {
    std::cout << "Įveskite vardą: ";
    std::cin >> vardas_;
    std::cout << "Įveskite pavardę: ";
    std::cin >> pavarde_;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (zinomas_sk) {
        const int kiekis = ivestiSk("Kiek studentas turi pažymių? ", 1, BE_RIBOS);
        if (rezimas == Rezimas::Ranka) {
            for (int j = 1; j <= kiekis; j++) {
                paz_.push_back(ivestiSk("Įveskite " + std::to_string(j) + "-ąjį pažymį: ", MIN_PAZ, MAX_PAZ));
                std::cout << '\n';
            }
        } else {
            std::cout << "Suvesti atsitiktiniai pažymiai: ";
            for (int j = 0; j < kiekis; j++) {
                const int sk = pazymiu_dist(gen);
                paz_.push_back(sk);
                std::cout << sk << " ";
            }
            std::cout << '\n';
        }
    } else {
        std::string eilute;
        while (true) {
            std::cout << "Įveskite " << paz_.size() + 1 << "-ąjį pažymį (Enter, kad užbaigti): ";
            if (!std::getline(std::cin, eilute) || eilute.empty()) break;
            int pazymys;
            if (!iSkaiciu(eilute, pazymys)) {
                std::cout << "Įvesta netinkama reikšmė (ne skaičius arba per didelis skaičius)!\n";
            } else if (pazymys < MIN_PAZ || pazymys > MAX_PAZ) {
                std::cout << "Pažymys turi būti nuo " << MIN_PAZ << " iki " << MAX_PAZ << "!\n";
            } else {
                paz_.push_back(pazymys);
            }
        }
    }
    if (rezimas == Rezimas::Generavimas) {
        exam_ = pazymiu_dist(gen);
        std::cout << "Atsitiktinis egzamino rezultatas: " << exam_ << '\n';
    } else {
        exam_ = ivestiSk("Įveskite egzamino rezultatą: ", MIN_PAZ, MAX_PAZ);
    }

}
void Studentas::skaiciuotiGalutini() {
    if (paz_.empty()) {
        galutinis_ = 0.6 * exam_;
    }
    else {
        galutinis_ = 0.4 * vidurkis(paz_) + 0.6 * exam_;
    }
}

std::ostream& Studentas::rasytiStudenta(std::ostream& out, const Statistika statistika) const {
    out << std::left << std::setw(utf8Plotis(vardas_)) << vardas_ << "|";
    out << std::setw(utf8Plotis(pavarde_)) << pavarde_ << "|";
    if (statistika == Statistika::Vidurkis) {
        out << std::setw(PLOTIS) << std::fixed << std::setprecision(2) << galutinis_ << "|\n";
    } else if (statistika == Statistika::Mediana) {
        out << std::setw(PLOTIS) << std::fixed << std::setprecision(2) << 0.4 * mediana(paz_) + 0.6 * exam_
            << "|\n";
    } else if (statistika == Statistika::Abu) {
        out << std::setw(PLOTIS) << std::fixed << std::setprecision(2) << galutinis_ << "|" << std::setw(PLOTIS)
            << 0.4 * mediana(paz_) + 0.6 * exam_ << "|\n";
    }
    return out;
}

bool comparePagalVarda(const Studentas& a, const Studentas& b) {
    return a.vardas() < b.vardas();
};
bool comparePagalPavarde(const Studentas& a, const Studentas& b) {
    return a.pavarde() < b.pavarde();
};
bool comparePagalGalutini(const Studentas& a, const Studentas& b) {
    return a.galutinis() < b.galutinis();
};

void rusiuotiStudentus(std::vector<Studentas>& grupe, Rusiuoti r, Tvarka t) {
    auto maziau = [r](const Studentas& a, const Studentas& b) {
        switch (r) {
            case Rusiuoti::Varda:   return comparePagalVarda(a, b);
            case Rusiuoti::Pavarde: return comparePagalPavarde(a, b);
            case Rusiuoti::Pazymi:  return comparePagalGalutini(a,b);
        }
        return false;
    };
    auto cmp = [maziau, t](const Studentas& a, const Studentas& b) {
        return t == Tvarka::Didejimo ? maziau(a, b) : maziau(b, a);
    };

    std::sort(grupe.begin(), grupe.end(), cmp);

}

void nuskaitytiStudentus(std::vector<Studentas>& grupe, const std::string& filename) {
    std::ifstream ins(filename);
    if (!ins) throw std::runtime_error("Nepavyko atidaryti failo " + filename);

    std::string eilute;
    std::getline(ins, eilute);  // antraste
    std::istringstream antraste(eilute);
    std::string zodis;
    int stulpeliu = 0;
    while (antraste >> zodis)
        stulpeliu++;
    const int pazKiekis = std::max(stulpeliu - 3, 0);  // minus Vardas, Pavardė, Egzaminas


    while (true) {
        Studentas st(ins, pazKiekis);

        if (!ins) {
            break;
        }
        grupe.push_back(std::move(st));
    }
}

void skirstytiStudentus(std::vector<Studentas>& grupe, std::vector<Studentas>& nuskriaustukai) {
    auto riba = std::partition(grupe.begin(), grupe.end(), [](const auto&st) { return st.galutinis() >= ISLAIKYMO_RIBA; });
    nuskriaustukai.insert(nuskriaustukai.end(), std::make_move_iterator(riba), std::make_move_iterator(grupe.end()));
    grupe.erase(riba, grupe.end());
}
