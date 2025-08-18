#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <vector>
#include "Producto.h"
#include "GestorProveedores.h"

using namespace std;

// ===================
// Declaracion Inventario
// ===================
class Inventario {
private:
    vector<Producto*> productos; // lista de productos disponibles

public:
    Inventario();
    ~Inventario();

    // Gesti�n de productos
    void agregarProducto(Producto* p);
    bool eliminarProducto(int codigo, GestorProveedores& gestorProv); // elimina producto por codigo y utiliza el gestorProveedores para desasociar el producto antes de borrarlo
    Producto* buscarProducto(int codigo) const;

    // Operaciones sobre stock
    void aumentarStock(int codigo, int cantidad);
    void reducirStock(int codigo, int cantidad);

    // Obtenci�n de productos
    const vector<Producto*>& getProductos() const;
    vector<Producto*> getProductosStockBajo() const;
    void vaciarInventario(); // borra todos los productos

    // Mostrar informaci�n
    void mostrarProductos() const;
};

#endif // INVENTARIO_H
