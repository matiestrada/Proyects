#ifndef PROVEEDORESSTORE_H
#define PROVEEDORESSTORE_H

#include <iostream>
#include <vector>
#include "Proveedor.h"

using namespace std;

// ========================
// Declaracion ProveedorDTO (Data Transfer Object para cargar los proveedores y sus codigos de productos)
// ========================
struct ProveedorDTO {
    int id;
    string nombre;
    string contacto;
    vector<int> codigosProductos;

    ProveedorDTO() {}
};


// ====================================================
// Declaracion IProveedoresStore (Estrategia Abstracta)
// ====================================================
class IProveedoresStore {
public:
    virtual ~IProveedoresStore() = default;

    // Guarda los proveedores en un archivo
    virtual void guardar(const vector<Proveedor*>& proveedores) = 0;

    // Carga proveedores como DTOs
    virtual vector<ProveedorDTO> cargar() = 0;
};


// =======================================================
// Declaracion ProveedoresTextoStore (Estrategia Concreta)
// =======================================================
class ProveedoresTextoStore : public IProveedoresStore {
private:
    string nombreArchivo;
public:
    ProveedoresTextoStore(const string& archivo) : nombreArchivo(archivo) {}
    void guardar(const vector<Proveedor*>& proveedores) override;
    vector<ProveedorDTO> cargar() override; // devuelve un DTO, no el vector de proveedores
};


// =======================================================
// Declaracion ProveedoresBinarioStore (Estrategia Concreta)
// =======================================================
class ProveedoresBinarioStore : public IProveedoresStore {
private:
    string nombreArchivo;
public:
    ProveedoresBinarioStore(const string& archivo) : nombreArchivo(archivo) {}
    void guardar(const vector<Proveedor*>& proveedores) override;
    vector<ProveedorDTO> cargar() override; // devuelve un DTO, no el vector de proveedores.
};


#endif // PROVEEDORESSTORE_H
