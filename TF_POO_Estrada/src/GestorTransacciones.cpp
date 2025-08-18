#include "GestorTransacciones.h"
#include <iostream>

GestorTransacciones::GestorTransacciones() {}
GestorTransacciones::~GestorTransacciones() {
    for (auto r : remitos) delete r;
    remitos.clear();
}

void GestorTransacciones::agregarRemito(Remito* r) {
    remitos.push_back(r);
}

void GestorTransacciones::registrarRemito(Remito* r, Inventario& inventario) {
    if (!r) return;

    // Actualizar stock automáticamente
    if (r->tipoRemito() == "Entrada") {
        for (auto& d : r->getDetalles())
            inventario.aumentarStock(d.codigoProducto, d.cantidad);
    } else if (r->tipoRemito() == "Salida") {
        for (auto& d : r->getDetalles())
            inventario.reducirStock(d.codigoProducto, d.cantidad);
    }

    // Agregar a historial
    this->agregarRemito(r);
}

Remito* GestorTransacciones::buscarRemito(int id) const {
    for (auto r : remitos) {
        if (r->getId() == id) return r;
    }
    return nullptr;
}

const vector<Remito*>& GestorTransacciones::getRemitos() const {
    return remitos;
}

void GestorTransacciones::mostrarRemitos() const {
    for (auto r : remitos) {
        r->mostrarInfo();
        cout << endl;
    }
}
