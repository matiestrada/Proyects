#include "Banco.h"

Banco::Banco(){}

Banco::~Banco(){}

void Banco::agregarcuenta(CuentaBancaria* cuenta)
{
    cuentas.push_back(cuenta);
}

float Banco::calcularactivo()
{
    float activo=0.0;
    for (const auto& cuenta: cuentas)
    {
        activo+=cuenta->Getsaldo();
    }
    return activo;
}
