#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <string>

using namespace std;

// =================
// Declaracion Fecha
// =================
class Fecha{
private:
    int day, month, year;

public:
    //Constructor y destructor
    Fecha(int d = 1, int m = 1, int y = 2000);
    virtual ~Fecha();

    //Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    //Setters
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);

    // sobrecarga del operador << para poder imprimir en formato fecha tradicional: dd/nn/yyyy
    friend ostream& operator<<(ostream& os, const Fecha& f);
};

#endif // FECHA_H
