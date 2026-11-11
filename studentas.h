#ifndef STUDENT_PROCESSOR_STUDENTAS_H
#define STUDENT_PROCESSOR_STUDENTAS_H

#include <algorithm>
#include <list>
#include <numeric>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include "nustatymai.h"
#include "utils.h"

constexpr double ISLAIKYMO_RIBA = 5.0;


class Studentas {
private:
    std::string vardas_, pavarde_;
    std::vector<int> paz_;
    int exam_{0};
    double galutinis_{0};
    bool valid_{false};
public:
    Studentas() = default;
    Studentas(std::istream &, int ndKiekis);
    Studentas(bool zinomas_sk, Rezimas rezimas,std::mt19937& gen, std::uniform_int_distribution<int>& pazymiu_dist);
    const std::string& vardas() const noexcept  { return vardas_; }
    const  std::string& pavarde() const noexcept { return pavarde_; }
    double galutinis() const noexcept { return galutinis_; }
    bool valid() const noexcept { return valid_; }
    void skaiciuotiGalutini();
    std::istream& skaitytiFaila(std::istream&, int ndKiekis);
    std::ostream& rasytiStudenta(std::ostream&, Statistika) const;
    void ivestiRanka(bool zinomas_sk, Rezimas rezimas,std::mt19937& gen, std::uniform_int_distribution<int>& pazymiu_dist);

    ~Studentas() {
        vardas_.clear();
        pavarde_.clear();
        paz_.clear();
        exam_ = 0; galutinis_ = 0;
    };

    Studentas(const Studentas&) = default;
    Studentas& operator=(const Studentas&) = default;
    Studentas(Studentas&&) noexcept = default;
    Studentas& operator=(Studentas&&) noexcept = default;
};
void nuskaitytiStudentus(std::vector<Studentas>& grupe, const std::string& filename);

bool comparePagalVarda(const Studentas&, const Studentas&);
bool comparePagalPavarde(const Studentas&, const Studentas&);
bool comparePagalGalutini(const Studentas&, const Studentas&);
void rusiuotiStudentus(std::vector<Studentas>& grupe, Rusiuoti r, Tvarka t);

void skirstytiStudentus(std::vector<Studentas>& grupe, std::vector<Studentas>& nuskriaustukai);
#endif  // STUDENT_PROCESSOR_STUDENTAS_H
