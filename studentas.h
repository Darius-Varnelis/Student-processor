#ifndef STUDENT_PROCESSOR_STUDENTAS_H
#define STUDENT_PROCESSOR_STUDENTAS_H

#include <iosfwd>
#include <random>
#include <string>
#include <vector>

#include "nustatymai.h"

constexpr double ISLAIKYMO_RIBA = 5.0;


class Studentas {
private:
    std::string vardas_, pavarde_;
    std::vector<int> paz_;
    int exam_{0};
    double galutinisVid_{0.0};
    double galutinisMed_{0.0};
    bool valid_{false};
public:
    Studentas() = default;
    Studentas(std::istream &, int ndKiekis);
    Studentas(bool zinomas_sk, Rezimas rezimas, std::mt19937& gen, std::uniform_int_distribution<int>& pazymiu_dist, Statistika statistika = Statistika::Vidurkis);
    const std::string& vardas() const noexcept  { return vardas_; }
    const std::string& pavarde() const noexcept { return pavarde_; }
    double galutinis(Statistika s = Statistika::Vidurkis) const noexcept {
        return (s == Statistika::Mediana) ? galutinisMed_ : galutinisVid_;
    }
    double galutinisVid() const noexcept { return galutinisVid_; }
    double galutinisMed() const noexcept { return galutinisMed_; }
    bool valid() const noexcept { return valid_; }
    void skaiciuotiGalutini(Statistika s = Statistika::Vidurkis);
    std::istream& skaitytiFaila(std::istream&, int ndKiekis);
    std::ostream& rasytiStudenta(std::ostream&, Statistika) const;
    void ivestiRanka(bool zinomas_sk, Rezimas rezimas, std::mt19937& gen, std::uniform_int_distribution<int>& pazymiu_dist, Statistika statistika = Statistika::Vidurkis);

    ~Studentas() {
        vardas_.clear();
        pavarde_.clear();
        paz_.clear();
        exam_ = 0;
        galutinisVid_ = 0.0;
        galutinisMed_ = 0.0;
    };

    Studentas(const Studentas&) = default;
    Studentas& operator=(const Studentas&) = default;
    Studentas(Studentas&&) noexcept = default;
    Studentas& operator=(Studentas&&) noexcept = default;
};
void nuskaitytiStudentus(std::vector<Studentas>& grupe, const std::string& filename);

bool comparePagalVarda(const Studentas&, const Studentas&);
bool comparePagalPavarde(const Studentas&, const Studentas&);
bool comparePagalGalutini(const Studentas&, const Studentas&, Statistika s = Statistika::Vidurkis);
void rusiuotiStudentus(std::vector<Studentas>& grupe, Rusiuoti r, Tvarka t, Statistika s = Statistika::Vidurkis);

void skirstytiStudentus(std::vector<Studentas>& grupe, std::vector<Studentas>& nuskriaustukai, Statistika s = Statistika::Vidurkis);
#endif  // STUDENT_PROCESSOR_STUDENTAS_H
