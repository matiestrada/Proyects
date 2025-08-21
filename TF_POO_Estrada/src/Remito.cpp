#include "Remito.h"

// =====================
// Implementacion Remito
// =====================
Remito::Remito(int id, const Fecha& fecha) : id(id), fecha(fecha) {}

// se limpia el vector de detalles
Remito::~Remito() {
    detalles.clear();
}

// Getters
int Remito::getId() const { return id; }
Fecha Remito::getFecha() const { return fecha; }
const vector<DetalleRemito>& Remito::getDetalles() const { return detalles; }

// Setters
void Remito::setId(int nuevoId) { id = nuevoId; }
void Remito::setFecha(const Fecha& nuevaFecha) { fecha = nuevaFecha; }

// Agregar producto mediante código y cantidad
void Remito::agregarProducto(int codigoProducto, int cantidad) {
    if (codigoProducto > 0 && cantidad > 0) {
        detalles.emplace_back(codigoProducto, cantidad);
    }
}



// ============================
// Implementacion RemitoEntrada
// ============================
RemitoEntrada::RemitoEntrada(int id, const Fecha& fecha) : Remito(id, fecha) {}
RemitoEntrada::~RemitoEntrada() {}

string RemitoEntrada::tipoRemito() const { return "Entrada"; }

void RemitoEntrada::mostrarInfo() const {
    cout << "=== Remito de ENTRADA ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Detalles:" << endl;

    for (const auto& d : detalles) {
        cout << "Codigo Producto: " << d.codigoProducto
             << " | Cantidad: " << d.cantidad << endl;
    }

    cout << "==========================" << endl;
}

// ============================
// Implementacion RemitoSalida
// ============================
RemitoSalida::RemitoSalida(int id, const Fecha& fecha) : Remito(id, fecha) {}
RemitoSalida::~RemitoSalida() {}

string RemitoSalida::tipoRemito() const { return "Salida"; }

void RemitoSalida::mostrarInfo() const {
    cout << "=== Remito de SALIDA ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Detalles:" << endl;

    for (const auto& d : detalles) {
        cout << "Codigo Producto: " << d.codigoProducto
             << " | Cantidad: " << d.cantidad << endl;
    }

    cout << "==========================" << endl;
}

