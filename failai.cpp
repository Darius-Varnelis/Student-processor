#include "failai.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <filesystem>

#include "utils.h"

using std::string;
using std::vector;

string failoPavadinimas(const string& pradzia, const int& n) {
    return pradzia + std::to_string(n) + ".txt";
}

void generuotiFaila(const int& n) {
    const auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<std::mt19937::result_type>(seed));
    std::uniform_int_distribution<int> pazymiuDist(MIN_PAZ, MAX_PAZ);
    // std::uniform_int_distribution<int> ndDist(1, 20);
    const int ndKiekis = 20;  // set to constant for testing and benchmarking

    const string pavadinimas = failoPavadinimas("kursiokai", n);
    std::cout << "Generuojamas failas " << pavadinimas << "\n";
    std::ostringstream ss;
    std::ofstream output(pavadinimas);
    ss << std::left << std::setw(PLOTIS) << "Vardas";
    ss << std::setw(utf8Plotis("Pavardė")) << "Pavardė";
    for (int i = 0; i < ndKiekis; i++) {
        ss << std::setw(PLOTIS) << "ND" + std::to_string(i + 1);
    }
    ss << std::setw(PLOTIS) << "Egzaminas" << "\n";
    int bufferSize = 0;
    for (int i = 0; i < n; i++) {
        ss << std::left << std::setw(PLOTIS) << "Vardenis" + std::to_string(i + 1);
        ss << std::setw(PLOTIS) << "Pavardenis" + std::to_string(i + 1);
        ss << std::right;
        for (int j = 0; j < ndKiekis; j++) {
            ss << std::setw(PLOTIS) << pazymiuDist(gen);
        }
        ss << std::setw(PLOTIS) << pazymiuDist(gen) << "\n";
        bufferSize++;
        if (bufferSize > 10000) {
            output << ss.str();
            ss.str("");
            bufferSize = 0;
        }
    }
    output << ss.str();
}

bool arFailaiEgzistuoja(int min, int max) {
    for (int i = min; i <= max; i *= 10) {
        std::ifstream file(failoPavadinimas("kursiokai", i));
        if (!file.is_open()) {
            return false;
        }
    }
    return true;
}

void trintiFaila(const string& fileName) {
    std::error_code ec;
    if (!std::filesystem::remove(fileName, ec) && ec)
        std::cerr << "Ištrinti nepavyko: " << ec.message() << '\n';
}
