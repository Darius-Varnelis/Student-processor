#ifndef STUDENT_PROCESSOR_FAILAI_H
#define STUDENT_PROCESSOR_FAILAI_H

#include <string>

std::string failoPavadinimas(const std::string& pradzia, const int& n);

void generuotiFaila(const int& n);
void trintiFaila(const std::string& fileName);

bool arFailaiEgzistuoja(int min, int max);

#endif  // STUDENT_PROCESSOR_FAILAI_H
