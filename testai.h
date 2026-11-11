
#ifndef STUDENT_PROCESSOR_TESTAI_H
#define STUDENT_PROCESSOR_TESTAI_H
#include <iostream>
#include <string>

#include "failai.h"
#include "nustatymai.h"
#include "results.h"
#include "studentas.h"
#include "timer.h"
#include "manual.h"

template <typename C>
void isvalyti(C& c) { C().swap(c); }

void testas(int i, int j, const std::string& pavadinimas, Rezultatai& rezultatai, Rusiuoti rusiuoti, Tvarka tvarka);

Rezultatai testuPaleistuvas(int i, const Rusiuoti& rusiuoti, const Tvarka& tvarka);

int vienasTestas(const std::string& strategija, const std::string& failas);

void paleistiAtmintiesTestus(const std::string& exe);
#endif //STUDENT_PROCESSOR_TESTAI_H
