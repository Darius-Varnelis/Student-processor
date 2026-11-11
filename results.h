
#ifndef STUDENT_PROCESSOR_RESULTS_H
#define STUDENT_PROCESSOR_RESULTS_H
#include <string>
#include <vector>

struct Rezultatai {
    int dydis;
    std::vector<double> nuskaitymai, galutiniai, rusiavimai, dalijimai, isvedimaiNuskriaustuku, isvedimaiKietuku,
        bendri;
};
void spausdintiLentele(const std::string& filename, const std::vector<Rezultatai>& visi);

#endif  // STUDENT_PROCESSOR_RESULTS_H
