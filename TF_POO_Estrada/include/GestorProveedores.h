#ifndef GESTORPROVEEDORES_H
#define GESTORPROVEEDORES_H

#include <vector>
#include "Proveedor.h"
#include "Producto.h"
#include "ProveedoresStore.h"
#include "Inventario.h"

using namespace std;

class Inventario; //  Forward declaration

// =======================
// Declaracion GestorProveedores
// =======================
class GestorProveedores {
private:
    vector<Proveedor*> proveedores;
    IProveedoresStore* store; // estrategia de persistencia

public:
    GestorProveedores(IProveedoresStore* s);
    virtual ~GestorProveedores();

    // Gestión de proveedores
    void agregarProveedor(Proveedor* p);
    void eliminarProveedor(int id);
    Proveedor* buscarProveedor(int id) const;
    const vector<Proveedor*>& getProveedores() const;

    // Gestión de productos de proveedores
    bool asociarProductoAProveedor(int idProveedor, Producto* p);
    void desasociarProductoDeProveedor(int codigoProducto);

    // Operaciones de persistencia
    void setEstrategia(IProveedoresStore* str);
    void guardar();
    void cargar(Inventario& inventario); // recibe inventario para resolver productos asociados

    void mostrarProveedores() const;
};

#endif // GESTORPROVEEDORES_H
