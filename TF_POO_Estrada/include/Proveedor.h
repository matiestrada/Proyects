#ifndef PROVEEDOR_H
#define PROVEEDOR_H

#include <iostream>
#include <string>
#include <vector>
#include "Producto.h"

using namespace std;

// =====================
// Declaracion Proveedor
// =====================
class Proveedor{
private:
    int id;
    string nombre;
    string contacto;
    vector<Producto*> productosAsociados;

public:
    //Constructor y destructor
    Proveedor(int id, const string& nombre, const string& contacto);
    virtual ~Proveedor();

    // Getters
    int getId() const;
    string getNombre() const;
    string getContacto() const;
    const vector<Producto*>& getProductosAsociados() const;

    // Setters
    void setId(int nuevoId);
    void setNombre(const string& nuevoNombre);
    void setContacto(const string& nuevoContacto);

    // Asociacion de productos
    void asociarProducto(Producto* producto);
    void desasociarProducto(int codigoProducto);

    void mostrarInfo() const;
};


#endif // PROVEEDOR_H
