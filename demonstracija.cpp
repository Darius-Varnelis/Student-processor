#include "demonstracija.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include "manual.h"
#include "nustatymai.h"
#include "studentas.h"
#include "zmogus.h"

void demonstracija() {
    Studentas a;
    if (taipArNe("Ar norite įvesti studentą ranka per std::cin >> st? (n = naudoti pavyzdinį srautą)")) {
        if (!(std::cin >> a) || !a.valid()) {
            std::cerr << "Nepavyko nuskaityti studento iš std::cin.\n";
            return;
        }
    } else {
        std::istringstream pavyzdys("Jonas Jonaitis 8 9 10 9");
        pavyzdys >> a;
        std::cout << "Nuskaityta iš srauto per operator>>: \"Jonas Jonaitis 8 9 10 9\"\n";
    }

    std::cout << "\n1. Originalus objektas 'a' (išvesta per std::cout << a):\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << a;

    std::cout << "\n2. Kopijavimo konstruktorius (Studentas b(a)):\n";
    Studentas b(a);
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << b;

    std::istringstream kitas("Ona Onaite 5 6 7 6");
    kitas >> b;
    std::cout << "Pakeitus 'b' per operator>> (\"Ona Onaite 5 6 7 6\"), 'a' lieka nepakitęs:\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << a << b;

    std::cout << "\n3. Kopijavimo priskyrimo operatorius (Studentas c; c = a;):\n";
    Studentas c;
    c = a;
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << c;

    Studentas& ref = c;
    c = ref;
    std::cout << "Po savęs priskyrimo (c = ref, kur this == &other):\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << c;

    std::cout << "\n4. Destruktorius (~Studentas()):\n";
    {
        Studentas laikinas(a);
        std::cout << "Sukurta laikina kopija vidiniame bloke:\n";
        rasytiAntraste(std::cout, Statistika::Vidurkis);
        std::cout << laikinas;
    }
    std::cout << "Vidinis blokas baigėsi (iškviestas ~Studentas()), 'a' vis dar galioja:\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    std::cout << a;

    std::cout << "\n5. Išvestis ir įvestis iš failo per operator<< ir operator>>:\n";
    const std::string demoIsvestis = "demo_isvestis.txt";
    {
        std::ofstream isvestiesFailas(demoIsvestis);
        if (isvestiesFailas) {
            rasytiAntraste(isvestiesFailas, Statistika::Vidurkis);
            isvestiesFailas << a << b;
            std::cout << "Įrašyta į failą '" << demoIsvestis << "' per operator<<.\n";
        }
    }

    std::ifstream ivestiesFailas("kursiokai.txt");
    if (ivestiesFailas) {
        std::string antraste;
        std::getline(ivestiesFailas, antraste);
        std::cout << "Nuskaityta iš 'kursiokai.txt' per while (failas >> st):\n";
        rasytiAntraste(std::cout, Statistika::Vidurkis);
        Studentas st;
        while (ivestiesFailas >> st) {
            std::cout << st;
        }
    }

    std::cout << "\n6. Abstrakti bazinė klasė Zmogus ir paveldėjimas (Studentas : public Zmogus):\n";
    static_assert(std::is_abstract_v<Zmogus>, "Zmogus turi būti abstrakti klasė");
    static_assert(std::is_base_of_v<Zmogus, Studentas>, "Studentas turi paveldėti iš Zmogus");
    // Zmogus z; // Negalima: kompiliatoriaus klaida "cannot declare variable 'z' to be of abstract type 'Zmogus'"

    std::cout << "  std::is_abstract_v<Zmogus>: " << std::boolalpha << std::is_abstract_v<Zmogus> << '\n';
    std::cout << "  std::is_abstract_v<Studentas>: " << std::is_abstract_v<Studentas> << '\n';
    std::cout << "  std::is_base_of_v<Zmogus, Studentas>: " << std::is_base_of_v<Zmogus, Studentas> << '\n';

    std::unique_ptr<Zmogus> zmogusPtr = std::make_unique<Studentas>(a);
    std::cout << "Polimorfinis spausdinimas per Zmogus* (zmogusPtr->spausdinti(std::cout)):\n";
    rasytiAntraste(std::cout, Statistika::Vidurkis);
    zmogusPtr->spausdinti(std::cout);
}
