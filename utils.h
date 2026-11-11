#ifndef STUDENT_PROCESSOR_UTILS_H
#define STUDENT_PROCESSOR_UTILS_H

#include <algorithm>
#include <limits>
#include <list>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>


constexpr int MIN_PAZ = 0;
constexpr int MAX_PAZ = 10;
constexpr int BE_RIBOS = std::numeric_limits<int>::max();
constexpr int PLOTIS = 20;

// Simbolių (ne baitų) skaičius UTF-8 eilutėje
size_t utf8Ilgis(const std::string& tekstas);

// setw plotis, kompensuojantis daugiabaičius UTF-8 simbolius
int utf8Plotis(const std::string& tekstas);

template <typename T>
double vidurkis(const T& v) {
    if (v.empty()) return 0.0;
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}
template <typename T>
double mediana(T paz) {
    if (paz.empty()) return 0.0;

    if constexpr (std::is_same_v<T, std::list<typename T::value_type>>)
        paz.sort();
    else
        std::sort(paz.begin(), paz.end());

    std::size_t n = paz.size();
    auto mid = std::next(paz.begin(), n / 2);
    if (n % 2 == 1) return *mid;
    return (*std::prev(mid) + *mid) / 2.0;
}
size_t pikineAtmintis();
size_t dabartineAtmintis();

#endif  // STUDENT_PROCESSOR_UTILS_H
