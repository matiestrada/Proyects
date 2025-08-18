#include "Inventario.h"
#include <iostream>

Inventario::Inventario() {}

Inventario::~Inventario() {
    for (auto p : productos) delete p;
    productos.clear();
}

void Inventario::agregarProducto(Producto* p) {
    if (p) productos.push_back(p);
}

bool Inventario::eliminarProducto(int codigo, GestorProveedores& gestorProveedores) {
    for (auto it = productos.begin(); it != productos.end(); ++it) {
        if ((*it)->getCodigo() == codigo) {
            // 1. Desasociar primero el producto del proveedor
            gestorProveedores.desasociarProductoDeProveedor(codigo);

            // 2. Liberar memoria del producto
            delete *it;

            // 3. Eliminar del vector
            productos.erase(it);

            return true;
        }
    }
    return false; // No se encontró el producto
}

Producto* Inventario::buscarProducto(int codigo) const {
    for (auto p : productos) {
        if (p->getCodigo() == codigo) return p;
    }
    return nullptr;
}

void Inventario::aumentarStock(int codigo, int cantidad) {
    Producto* p = buscarProducto(codigo);
    if (p && cantidad > 0) p->setStock(p->getStock() + cantidad);
}

void Inventario::reducirStock(int codigo, int cantidad) {
    Producto* p = buscarProducto(codigo);
    if (p && cantidad > 0) {
        int nuevoStock = p->getStock() - cantidad;
        p->setStock(nuevoStock >= 0 ? nuevoStock : 0);
    }
}

const vector<Producto*>& Inventario::getProductos() const {
    return productos;
}

vector<Producto*> Inventario::getProductosStockBajo() const {
    vector<Producto*> bajos;
    for (auto p : productos) {
        if (p->getStock() <= p->getUmbralStockBajo())
            bajos.push_back(p);
    }
    return bajos;
}

void Inventario::vaciarInventario() {
    for (Producto* p : productos)
        delete p;
    productos.clear();
}


void Inventario::mostrarProductos() const {
    for (auto p : productos) p->mostrarInfo();
}
