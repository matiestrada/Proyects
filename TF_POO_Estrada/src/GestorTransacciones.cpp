#include "GestorTransacciones.h"
#include <iostream>

// el constructor recibe como parametro la estrategia creada en el main
GestorTransacciones::GestorTransacciones(IRemitosStore* s) : store(s) {}

// se eliminan los remitos y se limpia el vector
GestorTransacciones::~GestorTransacciones() {
    for (auto r : remitos) delete r;
    remitos.clear();
}

void GestorTransacciones::agregarRemito(Remito* r) {
    remitos.push_back(r);
}

// recibe referencia al inventario como parametro
// para poder actualizar automaticamente el stock de los productos involucrados en el detalle del remito creado.
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

    // Agregar a historial una vez actualizado el stock de los detalles
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

// metodo para setear una nueva estrategia en caso de querer modificarla durante la ejecucion.
void GestorTransacciones::setEstrategia(IRemitosStore* str) { store = str; }

// metodos para realizar la persistencia de los remitos con el patron strategy
void GestorTransacciones::guardar() {
    if(store) store->guardar(remitos);
}

void GestorTransacciones::cargar() {
    if(store) remitos = store->cargar();
}

void GestorTransacciones::mostrarRemitos() const {
    for (auto r : remitos) {
        r->mostrarInfo();
        cout << endl;
    }
}
