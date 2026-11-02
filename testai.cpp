#include "testai.h"
#include <cstdlib>
#include "utils.h"
#include "studentas.h"
constexpr int KARTAI = 5;
Rezultatai testuPaleistuvas(int i, const Rusiuoti& rusiuoti, const Tvarka& tvarka) {
    Rezultatai rezultatai;
    rezultatai.dydis = i;
    std::string fileName = failoPavadinimas("kursiokai", i);
    std::cout << "Apdorojamas failas " << fileName << "\n";
    for (int j = 0; j < KARTAI; j++) {
        std::cout << j + 1 << "-oji iteracija\n";
        testas(i, j, fileName, rezultatai, rusiuoti, tvarka);
    }
    return rezultatai;
}


void testas(int i, int j, const std::string& pavadinimas, Rezultatai& rezultatai, Rusiuoti rusiuoti, Tvarka tvarka) {
    std::vector<Studentas> grupe;
    Timer apdorojimas;
    Timer t;
    nuskaitytiStudentus(grupe, pavadinimas);
    rezultatai.nuskaitymai.push_back(t.elapsed());
    std::cout << "  Nuskaitymas: " << rezultatai.nuskaitymai[j] << " s\n";

    t.reset();
    for (auto& st : grupe) {
        st.skaiciuotiGalutini();
    }
    rezultatai.galutiniai.push_back(t.elapsed());
    std::cout << "  Galutinių skaičiavimas: " << rezultatai.galutiniai[j] << " s\n";

    std::vector<Studentas> nuskriaustukai;
    t.reset();
    skirstytiStudentus(grupe, nuskriaustukai);
    rezultatai.dalijimai.push_back(t.elapsed());
    grupe.clear();
    std::cout << "  Dalijimas į dvi grupes: " << rezultatai.dalijimai[j] << " s\n";


    rusiuotiStudentus(grupe, rusiuoti, tvarka);
    rusiuotiStudentus(nuskriaustukai, rusiuoti, tvarka);
    rezultatai.rusiavimai.push_back(t.elapsed());
    std::cout << "  Rūšiavimas: " << rezultatai.rusiavimai[j] << " s\n";

    t.reset();
    std::ofstream outn(failoPavadinimas("nuskriaustukai", i));
    rasytiGrupe( outn, nuskriaustukai, Statistika::Vidurkis);
    rezultatai.isvedimaiNuskriaustuku.push_back(t.elapsed());
    std::cout << "  Nuskriaustukų išvedimas: " << rezultatai.isvedimaiNuskriaustuku[j] << " s\n";

    t.reset();
    std::ofstream outk(failoPavadinimas("kietukai", i));
    rasytiGrupe( outk, nuskriaustukai, Statistika::Vidurkis);
    rezultatai.isvedimaiKietuku.push_back(t.elapsed());
    std::cout << "  Kietukų išvedimas: " << rezultatai.isvedimaiKietuku[j] << " s\n";
    rezultatai.bendri.push_back(apdorojimas.elapsed());
    std::cout << "  Viso apdorojimas: " << rezultatai.bendri[j] << " s\n\n";
}