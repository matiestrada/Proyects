#include "Fecha.h"

// ====================
// Implementacion Fecha
// ====================
Fecha::Fecha(int d, int m, int y) : day(d), month(m), year(y) {}

Fecha::~Fecha() {}

// Getters
int Fecha::getDay() const { return day; }
int Fecha::getMonth() const { return month; }
int Fecha::getYear() const { return year; }

// Setters
void Fecha::setDay(int d) { day = d; }
void Fecha::setMonth(int m) { month = m; }
void Fecha::setYear(int y) { year = y; }

string Fecha::toString() const {
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%04d", day, month, year);
    return string(buffer);
}
