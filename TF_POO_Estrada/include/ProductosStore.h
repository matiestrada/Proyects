#ifndef PRODUCTOSSTORE_H
#define PRODUCTOSSTORE_H

#include <iostream>
#include <vector>
#include "Producto.h"

using namespace std;

// ==================================================
// Declaracion IProductosStore (Estrategia abstracta)
// ==================================================
class IProductosStore {
public:
    virtual ~IProductosStore() = default;

    // Guarda los productos en un archivo (texto o binario según la implementación)
    virtual void guardar(const vector<Producto*>& productos) = 0;

    // Carga los productos desde un archivo
    virtual vector<Producto*> cargar() = 0;
};


// =====================================================
// Declaracion ProductosTextoStore (Estrategia concreta)
// =====================================================
class ProductosTextoStore : public IProductosStore {
private:
    string nombreArchivo;
public:
    ProductosTextoStore(const string& archivo) : nombreArchivo(archivo) {}
    virtual ~ProductosTextoStore() {}

    virtual void guardar(const vector<Producto*>& productos) override;
    virtual vector<Producto*> cargar() override;
};


// =====================================================
// Declaracion ProductosBinarioStore (Estrategia concreta)
// =====================================================
class ProductosBinarioStore : public IProductosStore {
private:
    string nombreArchivo;
public:
    ProductosBinarioStore(const string& archivo) : nombreArchivo(archivo) {}
    virtual ~ProductosBinarioStore() {}

    virtual void guardar(const vector<Producto*>& productos) override;
    virtual vector<Producto*> cargar() override;
};

#endif // PRODUCTOSSTORE_H
