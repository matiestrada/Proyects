#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <vector>
#include "Producto.h"
#include "GestorProveedores.h"
#include "ProductosStore.h"

using namespace std;

class GestorProveedores;

// ===================
// Declaracion Inventario
// ===================
class Inventario {
private:
    vector<Producto*> productos; // lista de productos disponibles
    IProductosStore* store; // estrategia de persistencia

public:
    //Constructor y destructor
    Inventario(IProductosStore* s);
    ~Inventario();

    // Gestión de productos
    void agregarProducto(Producto* p);
    bool eliminarProducto(int codigo, GestorProveedores& gestorProv); // elimina producto por codigo y utiliza el gestorProveedores para desasociar el producto antes de borrarlo
    Producto* buscarProducto(int codigo) const;

    // Operaciones sobre stock
    void aumentarStock(int codigo, int cantidad);
    void reducirStock(int codigo, int cantidad);

    // Obtención de productos
    const vector<Producto*>& getProductos() const;
    vector<Producto*> getProductosStockBajo() const;

    // Operaciones de persistencia
    void setEstrategia(IProductosStore* str);
    void guardar();
    void cargar();

    // Mostrar información
    void mostrarProductos() const;
};

#endif // INVENTARIO_H
