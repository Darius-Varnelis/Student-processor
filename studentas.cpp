//
// Created by darc on 02/11/2026.
//
#include "studentas.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>

#include "manual.h"
#include "nustatymai.h"
#include "utils.h"

Studentas::Studentas(std::istream& is, int ndKiekis) {
    skaitytiFaila(is, ndKiekis);
}
std::istream& Studentas::skaitytiFaila(std::istream& is, int ndKiekis) {
    valid_ = false;
    if (!(is >> vardas_ >> pavarde_)) return is;
    paz_.assign(ndKiekis, 0);
    for (int& p : paz_) {
        if (!(is >> p) || p < MIN_PAZ || p > MAX_PAZ) return is;
    }
    if (!(is >> exam_) || exam_ < MIN_PAZ || exam_ > MAX_PAZ) return is;
    std::string extra;
    if (is >> extra) {
        return is;
    }
    valid_ = true;
    return is;
}
Studentas::Studentas(bool zinomas_sk, Rezimas rezimas, std::mt19937& gen, std::uniform_int_distribution<int>& pazymiu_dist, Statistika statistika) {
    ivestiRanka(zinomas_sk, rezimas, gen, pazymiu_dist, statistika);
}


void Studentas::ivestiRanka(bool zinomas_sk, Rezimas rezimas, std::mt19937& gen, std::uniform_int_distribution<int>& pazymiu_dist, Statistika statistika) {
    valid_ = false;
    paz_.clear();
    std::cout << "Įveskite vardą: ";
    if (!(std::cin >> vardas_)) return;
    std::cout << "Įveskite pavardę: ";
    if (!(std::cin >> pavarde_)) return;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (zinomas_sk) {
        const int kiekis = ivestiSk("Kiek studentas turi pažymių? ", 1, BE_RIBOS);
        if (std::cin.fail()) return;
        if (rezimas == Rezimas::Ranka) {
            for (int j = 1; j <= kiekis; j++) {
                paz_.push_back(ivestiSk("Įveskite " + std::to_string(j) + "-ąjį pažymį: ", MIN_PAZ, MAX_PAZ));
                if (std::cin.fail()) return;
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
        if (std::cin.fail()) return;
    }
    skaiciuotiGalutini(statistika);
    valid_ = true;
}

void Studentas::skaiciuotiGalutini(Statistika s) {
    if (paz_.empty()) {
        galutinisVid_ = 0.6 * exam_;
        galutinisMed_ = 0.6 * exam_;
        return;
    }

    if (s == Statistika::Vidurkis) {
        galutinisVid_ = 0.4 * vidurkis(paz_) + 0.6 * exam_;
    } else if (s == Statistika::Mediana) {
        galutinisMed_ = 0.4 * mediana(paz_) + 0.6 * exam_;
    } else { // Statistika::Abu
        galutinisVid_ = 0.4 * vidurkis(paz_) + 0.6 * exam_;
        galutinisMed_ = 0.4 * mediana(paz_) + 0.6 * exam_;
    }
}

std::ostream& Studentas::rasytiStudenta(std::ostream& out, const Statistika statistika) const {
    out << std::left << std::setw(utf8Plotis(vardas_)) << vardas_ << "|";
    out << std::setw(utf8Plotis(pavarde_)) << pavarde_ << "|";
    if (statistika == Statistika::Vidurkis) {
        out << std::setw(PLOTIS) << std::fixed << std::setprecision(2) << galutinisVid_ << "|\n";
    } else if (statistika == Statistika::Mediana) {
        out << std::setw(PLOTIS) << std::fixed << std::setprecision(2) << galutinisMed_ << "|\n";
    } else if (statistika == Statistika::Abu) {
        out << std::setw(PLOTIS) << std::fixed << std::setprecision(2) << galutinisVid_ << "|" << std::setw(PLOTIS)
            << galutinisMed_ << "|\n";
    }
    return out;
}

std::istream& operator>>(std::istream& is, Studentas& st) {
    if (&is == &std::cin) {
        static std::mt19937 gen(static_cast<std::mt19937::result_type>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()));
        static std::uniform_int_distribution<int> pazymiu_dist(MIN_PAZ, MAX_PAZ);
        st.ivestiRanka(false, Rezimas::Ranka, gen, pazymiu_dist, Statistika::Abu);
        if (!st.valid_) {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    st.valid_ = false;
    st.paz_.clear();
    st.exam_ = 0;
    st.galutinisVid_ = 0.0;
    st.galutinisMed_ = 0.0;

    std::string eilute;
    while (std::getline(is, eilute)) {
        if (eilute.find_first_not_of(" \t\r") != std::string::npos) {
            break;
        }
    }
    if (!is) return is;

    std::istringstream eilutesSrautas(eilute);
    if (!(eilutesSrautas >> st.vardas_ >> st.pavarde_)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::vector<int> ivestiPazymiai;
    int pazymys;
    while (eilutesSrautas >> pazymys) {
        if (pazymys < MIN_PAZ || pazymys > MAX_PAZ) {
            is.setstate(std::ios::failbit);
            return is;
        }
        ivestiPazymiai.push_back(pazymys);
    }

    if (!eilutesSrautas.eof() || ivestiPazymiai.empty()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    st.exam_ = ivestiPazymiai.back();
    ivestiPazymiai.pop_back();
    st.paz_ = std::move(ivestiPazymiai);
    st.skaiciuotiGalutini(Statistika::Abu);
    st.valid_ = true;
    return is;
}

std::ostream& operator<<(std::ostream& out, const Studentas& st) {
    return st.rasytiStudenta(out, Statistika::Abu);
}

bool comparePagalVarda(const Studentas& a, const Studentas& b) {
    return a.vardas() < b.vardas();
}
bool comparePagalPavarde(const Studentas& a, const Studentas& b) {
    return a.pavarde() < b.pavarde();
}
bool comparePagalGalutini(const Studentas& a, const Studentas& b, Statistika s) {
    Statistika tikra = (s == Statistika::Mediana) ? Statistika::Mediana : Statistika::Vidurkis;
    return a.galutinis(tikra) < b.galutinis(tikra);
}

void rusiuotiStudentus(std::vector<Studentas>& grupe, Rusiuoti r, Tvarka t, Statistika s) {
    auto maziau = [r, s](const Studentas& a, const Studentas& b) {
        switch (r) {
            case Rusiuoti::Varda:   return comparePagalVarda(a, b);
            case Rusiuoti::Pavarde: return comparePagalPavarde(a, b);
            case Rusiuoti::Pazymi:  return comparePagalGalutini(a, b, s);
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
    if (!std::getline(ins, eilute)) {
        throw std::runtime_error("Failas yra tuščias: " + filename);
    }
    std::istringstream antraste(eilute);
    std::vector<std::string> stulpeliai;
    std::string zodis;
    while (antraste >> zodis) stulpeliai.push_back(zodis);

    if (stulpeliai.size() < 3) {
        throw std::runtime_error("Netinkama failo antraštė (mažiau nei 3 stulpeliai) faile " + filename);
    }
    const int pazKiekis = static_cast<int>(stulpeliai.size()) - 3;

    int eilNr = 1;
    while (std::getline(ins, eilute)) {
        ++eilNr;
        if (eilute.find_first_not_of(" \t\r") == std::string::npos) continue;
        std::istringstream is(eilute);
        Studentas st(is, pazKiekis);
        if (!st.valid()) {
            std::cerr << "Praleista netinkama eilutė " << eilNr << '\n';
            continue;
        }
        grupe.push_back(std::move(st));
    }
}

void skirstytiStudentus(std::vector<Studentas>& grupe, std::vector<Studentas>& nuskriaustukai, Statistika s) {
    Statistika tikra = (s == Statistika::Mediana) ? Statistika::Mediana : Statistika::Vidurkis;
    auto riba = std::partition(grupe.begin(), grupe.end(), [tikra](const auto& st) { return st.galutinis(tikra) >= ISLAIKYMO_RIBA; });
    nuskriaustukai.insert(nuskriaustukai.end(), std::make_move_iterator(riba), std::make_move_iterator(grupe.end()));
    grupe.erase(riba, grupe.end());
}
