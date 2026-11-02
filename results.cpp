#include <string>
#include <vector>
#include <fstream>
#include "results.h"
#include "utils.h"
#include <iomanip>

namespace {
void header(std::ostream& out) {
    out << "|" << "Student count" << "|"
        << "Average file read time" << "|"
        << "Average final grade calculation time" << "|"
        << "Average sorting time" << "|"
        << "Average grouping time" << "|"
        << "Average failed student output time" << "|"
        << "Average passing student output time" << "|"
        << "Average processing time" << "|\n";
    out << "|---|---|---|---|---|---|---|---|\n";
}

void eilute(std::ostream& out, const Rezultatai& rezultatai) {
    out << "|" << rezultatai.dydis << "|"
        << vidurkis(rezultatai.nuskaitymai) << "s|"
        << vidurkis(rezultatai.galutiniai) << "s|"
        << vidurkis(rezultatai.rusiavimai) << "s|"
        << vidurkis(rezultatai.dalijimai) << "s|"
        << vidurkis(rezultatai.isvedimaiNuskriaustuku) << "s|"
        << vidurkis(rezultatai.isvedimaiKietuku) << "s|"
        << vidurkis(rezultatai.bendri) << "s|\n";
}
}

void spausdintiLentele(const std::string& filename, const std::vector<Rezultatai>& visi) {
    std::ofstream out(filename);
    out << std::fixed << std::setprecision(3);
    header(out);
    for (const auto& rezultatai : visi) {
        eilute(out, rezultatai);
    }
}
