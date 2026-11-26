#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "demonstracija.h"
#include "failai.h"
#include "nustatymai.h"
#include "studentas.h"
#include "timer.h"
#include "manual.h"
#include "results.h"
#include "testai.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif


    std::cout << std::fixed << std::setprecision(3);
    auto rezimas = static_cast<Rezimas>(nustatymai(
        "Koks programos rėžimas?",
        "Studentų įvedimas ranka;Studentų nuskaitymas iš failo;Automatinis generavimas;Programos greitumo testavimas;Trijų metodų taisyklės, klasių paveldėjimo demonstracija"));

    if (rezimas == Rezimas::Demonstracija) {
        demonstracija();
        return 0;
    }

    auto rusiuoti =
        static_cast<Rusiuoti>(nustatymai("Pagal ką rūšiuoti išvesties failus?", "Vardą;Pavardę;Galutinį balą"));
    auto tvarka =
        static_cast<Tvarka>(nustatymai("Kokia tvarka rūšiuoti išvesties failus?", "Didėjimo;Mažėjimo"));

    if (rezimas == Rezimas::Benchmark) {
        constexpr int MAXFAILAS = 1000000;
        constexpr int MINFAILAS = 100000;
        if (!arFailaiEgzistuoja(MINFAILAS, MAXFAILAS) or taipArNe("Ar generuoti naujus failus?")) {
            Timer visasGeneravimas;
            for (int i = MINFAILAS; i <= MAXFAILAS; i *= 10) {
                Timer t;
                generuotiFaila(i);
                std::cout << "  Generavimas užtruko: " << t.elapsed() << " s\n";
            }
            std::cout << "Visų failų generavimas užtruko: " << visasGeneravimas.elapsed() << " s\n\n";
        }
        std::vector<Rezultatai> visiVector;
        for (int i = MINFAILAS; i <= MAXFAILAS; i *= 10) {
            std::cout << "Atliekamas testas su Vector.\n";
            Rezultatai rezultatai = testuPaleistuvas(i, rusiuoti, tvarka);
            visiVector.push_back(rezultatai);
        }
        std::string lentele = "lenteleVector.txt";
        spausdintiLentele(lentele, visiVector);
        if (taipArNe("Ar trinti nuskriaustukų ir kietukų failus? ")) {
            for (int i = MINFAILAS; i <= MAXFAILAS; i *= 10) {
                trintiFaila(failoPavadinimas("nuskriaustukai",i));
                trintiFaila(failoPavadinimas("kietukai",i));
            }
        }
        if (taipArNe("Ar trinti kursiokų failus? ")) {
            if (taipArNe("Ar tikrai?!!!")) {
                for (int i = MINFAILAS; i <= MAXFAILAS; i *= 10) {
                    trintiFaila(failoPavadinimas("kursiokai",i));
                }
            }
        }

        return 0;
    }

    auto isvedimas = static_cast<Isvedimas>(nustatymai("Kur išvesti studentų duomenis?", "Į konsolę;Į failą"));
    auto statistika =
    static_cast<Statistika>(nustatymai("Kokios statistikos pageidaujate?", "Vidurkio;Medianos;Abiejų"));

    Statistika rusiavimoStatistika = statistika;
    if (statistika == Statistika::Abu) {
        if (rusiuoti == Rusiuoti::Pazymi) {
            std::cout << "\nPasirinkote rodyti abu balus (vidurkį ir medianą).\n";
            rusiavimoStatistika = static_cast<Statistika>(nustatymai(
                "Pagal kurį balą rūšiuoti studentus?",
                "Pagal vidurkį;Pagal medianą"
            ));
        } else {
            rusiavimoStatistika = Statistika::Vidurkis;
        }
    }

    bool arZinomasSk;
    if (rezimas == Rezimas::Ranka) {
        arZinomasSk = taipArNe("Ar žinomas namų darbų skaičius?");
    } else {
        arZinomasSk = true;
    }
    if (rezimas == Rezimas::Ranka or rezimas == Rezimas::Generavimas) {
        if (isvedimas == Isvedimas::Konsole) {
            rankinisIvedimas(arZinomasSk, statistika, rezimas, std::cout, rusiuoti, tvarka, rusiavimoStatistika);
        }
        else {
            std::string failoPavadinimas;
            std::cout << "Įveskite failo pavadinimą: ";
            std::cin >> failoPavadinimas;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::ofstream failas(failoPavadinimas + ".txt");
            if (!failas) {
                std::cerr << "Nepavyko atidaryti failo!\n";
                return 1;
            }
            rankinisIvedimas(arZinomasSk, statistika, rezimas, failas, rusiuoti, tvarka, rusiavimoStatistika);
        }
    }

    if (rezimas == Rezimas::Failas) {
        std::string failoPavadinimas;
        std::cout << "Įveskite failo pavadinimą nuskaitymui: ";
        std::cin >> failoPavadinimas;
        try {
            if (isvedimas == Isvedimas::Konsole) {
                nuskaitytiIsFailo(failoPavadinimas + ".txt", statistika, std::cout, rusiuoti, tvarka, rusiavimoStatistika);
            }
            else {
                std::string outputPavadinimas;
                std::cout << "Įveskite išvesties failo pavadinimą: ";
                std::cin >> outputPavadinimas;

                if (outputPavadinimas == failoPavadinimas) {
                    std::cerr << "Išvesties failas negali sutapti su įvesties failu!\n";
                    return 1;
                }
                apdorotiIsFailoIFaila(failoPavadinimas + ".txt", outputPavadinimas + ".txt", statistika, rusiuoti, tvarka, rusiavimoStatistika);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Failo nepavyko apdoroti: " << e.what() << '\n';
            return 1;
        }
    }
    return 0;
}




