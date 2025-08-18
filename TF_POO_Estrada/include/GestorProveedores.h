#ifndef GESTORPROVEEDORES_H
#define GESTORPROVEEDORES_H

#include <vector>
#include "Proveedor.h"
#include "Producto.h"

using namespace std;

// =======================
// Declaracion GestorProveedores
// =======================
class GestorProveedores {
private:
    vector<Proveedor*> proveedores;

public:
    GestorProveedores();
    virtual ~GestorProveedores();

    // Gestión de proveedores
    void agregarProveedor(Proveedor* p);
    void eliminarProveedor(int id);
    Proveedor* buscarProveedor(int id) const;
    const vector<Proveedor*>& getProveedores() const;

    // Gestión de productos de proveedores
    bool asociarProductoAProveedor(int idProveedor, Producto* p);
    void desasociarProductoDeProveedor(int codigoProducto);


    void mostrarProveedores() const;
};

#endif // GESTORPROVEEDORES_H
