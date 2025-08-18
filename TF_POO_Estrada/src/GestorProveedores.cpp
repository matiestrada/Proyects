#include "GestorProveedores.h"
#include <iostream>

GestorProveedores::GestorProveedores() {}
GestorProveedores::~GestorProveedores() {
    for (auto p : proveedores) delete p;
    proveedores.clear();
}

void GestorProveedores::agregarProveedor(Proveedor* p) {
    if (p) proveedores.push_back(p);
}

void GestorProveedores::eliminarProveedor(int id) {
    for (auto it = proveedores.begin(); it != proveedores.end(); ++it) {
        if ((*it)->getId() == id) {
            delete *it;
            proveedores.erase(it);
            return;
        }
    }
}

Proveedor* GestorProveedores::buscarProveedor(int id) const {
    for (auto p : proveedores) {
        if (p->getId() == id) return p;
    }
    return nullptr;
}

const vector<Proveedor*>& GestorProveedores::getProveedores() const {
    return proveedores;
}

bool GestorProveedores::asociarProductoAProveedor(int idProveedor, Producto* producto) {
    // Verificar si ya está en otro proveedor
    for (auto& prov : proveedores) {
        for (auto* p : prov->getProductosAsociados()) {
            if (p == producto) {
                cout << "El producto ya pertenece al proveedor: " << prov->getNombre() << endl;
                return false;
            }
        }
    }
    // Si no está, lo asociamos
    Proveedor* proveedor = buscarProveedor(idProveedor);
    if (proveedor) {
        proveedor->asociarProducto(producto);
        return true;
    }
    return false;
}


void GestorProveedores::desasociarProductoDeProveedor(int codigoProducto) {
    for (auto& proveedor : proveedores) {
        proveedor->desasociarProducto(codigoProducto);
    }
}


void GestorProveedores::mostrarProveedores() const {
    for (auto p : proveedores) p->mostrarInfo();
}
