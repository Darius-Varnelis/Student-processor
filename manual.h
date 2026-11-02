//
// Created by Darc on 10/13/2026.
//

#ifndef OOP_PROJECT_MANUAL_H
#define OOP_PROJECT_MANUAL_H
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <random>

#include "nustatymai.h"
#include "studentas.h"
#include "utils.h"
#include "failai.h"


void rasytiAntraste(std::ostream& out, Statistika statistika);


void rasytiGrupe(std::ostream& out, const std::vector<Studentas>& grupe, Statistika statistika);

// Skaičiaus įvedimas per konsolę
int ivestiSk(const std::string& klausimas, int nuo, int iki);

// Funkcija, gražinanti loginę reikšmę taip ar ne klausimui
bool taipArNe(const std::string& klausimas);
bool iSkaiciu(const std::string& tekstas, int& rezultatas);

void rankinisIvedimas(bool zinomas_sk, Statistika statistika, Rezimas rezimas, std::ostream& output, Rusiuoti rusiuoti, Tvarka tvarka);
void nuskaitytiIsFailo(const std::string& filename, Statistika statistika, std::ostream& output, Rusiuoti rusiuoti, Tvarka tvarka);

int nustatymai(const std::string& klausimas, const std::string& pasirinkimai);

#endif  // OOP_PROJECT_MANUAL_H
