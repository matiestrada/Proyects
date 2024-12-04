#include <iostream>
#include <string>
#include "Banco.h"
#include "CuentaCorriente.h"
#include "CajaAhorro.h"
#include "CuentaUniversitaria.h"

using namespace std;

int main()
{
    Banco MiBanco;
    CuentaBancaria* Cuenta1 = new CuentaCorriente("Juan Perez", 70000, 5000);
    CuentaBancaria* Cuenta2 = new CuentaUniversitaria("Raul Gonzalez", 3000);
    CuentaBancaria* Cuenta3 = new CajaAhorro("Roberto Garcia", 40000, 1);

    MiBanco.agregarcuenta(Cuenta1);
    MiBanco.agregarcuenta(Cuenta2);
    MiBanco.agregarcuenta(Cuenta3);
    cout << "Activo inicial: $ " << MiBanco.calcularactivo() << endl;

    Cuenta1->extraer();
    Cuenta3->extraer();
    Cuenta3->extraer();
    Cuenta2->extraer();
    Cuenta2->extraer();

    cout << "Activo actual: $" << MiBanco.calcularactivo() << endl;
    return 0;
}
