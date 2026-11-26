#ifndef STUDENT_PROCESSOR_ZMOGUS_H
#define STUDENT_PROCESSOR_ZMOGUS_H

#include <iosfwd>
#include <string>
#include <utility>

class Zmogus {
protected:
    std::string vardas_;
    std::string pavarde_;

    Zmogus() = default;
    Zmogus(std::string vardas, std::string pavarde)
        : vardas_(std::move(vardas)), pavarde_(std::move(pavarde)) {}
    Zmogus(const Zmogus&) = default;
    Zmogus& operator=(const Zmogus&) = default;
    Zmogus(Zmogus&&) noexcept = default;
    Zmogus& operator=(Zmogus&&) noexcept = default;

public:
    virtual ~Zmogus() = default;

    const std::string& vardas() const noexcept { return vardas_; }
    const std::string& pavarde() const noexcept { return pavarde_; }

    virtual void spausdinti(std::ostream& out) const = 0;
};

#endif  // STUDENT_PROCESSOR_ZMOGUS_H
