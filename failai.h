#ifndef OOP_PROJECT_FAILAI_H
#define OOP_PROJECT_FAILAI_H

#include <iomanip>
#include <ios>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include "studentas.h"
#include "utils.h"

std::string failoPavadinimas(const std::string& pradzia, const int& n);

void generuotiFaila(const int& n);
void rasytiAntraste(std::ostream& out);
void trintiFaila(const std::string& fileName) ;

bool arFailaiEgzistuoja(int min, int max);

#endif  // OOP_PROJECT_FAILAI_H
