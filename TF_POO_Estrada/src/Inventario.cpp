#include "Inventario.h"
#include <iostream>

// el constructor recibe como parametro la estrategia creada en el main
Inventario::Inventario(IProductosStore* s) : store(s) {}

// se eliminan los productos y se limpia el vector
Inventario::~Inventario() {
    for (auto p : productos) delete p;
    productos.clear();
}

void Inventario::agregarProducto(Producto* p) {
    if (p) productos.push_back(p);
}

// recibe referencia al gestorProveedores para poder desasociar el producto del proveedor al que pertenece antes de eliminarlo.
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

// metodo para aumentar el stock cuando se registra una transaccion de Entrada
void Inventario::aumentarStock(int codigo, int cantidad) {
    Producto* p = buscarProducto(codigo);
    if (p && cantidad > 0) p->setStock(p->getStock() + cantidad);
}

// metodo para reducir el stock cuando se registra una transaccion de Salida
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

// recorre el vector de productos y verifica si el atributo stock es menor o igual que el umbral de alerta.
// devuelve un vector con punteros a los productos que cumplan esa condicion.
vector<Producto*> Inventario::getProductosStockBajo() const {
    vector<Producto*> bajos;
    for (auto p : productos) {
        if (p->getStock() <= p->getUmbralStockBajo())
            bajos.push_back(p);
    }
    return bajos;
}

// metodo para setear una nueva estrategia en caso de querer modificarla durante la ejecucion.
void Inventario::setEstrategia(IProductosStore* str) { store = str; }

// metodos para realizar la persistencia de los productos con el patron strategy
void Inventario::guardar() {
    if(store) store->guardar(productos);
}

void Inventario::cargar() {
    if(store) productos = store->cargar();
}

void Inventario::mostrarProductos() const {
    for (auto p : productos) p->mostrarInfo();
}
