#ifndef REMITO_H
#define REMITO_H

#include <iostream>
#include <string>
#include <vector>
#include "Fecha.h"

using namespace std;

// ========================
// Estructura DetalleRemito
// ========================
struct DetalleRemito {
    int codigoProducto;  // Codigo del producto involucrado, no guardo punteros a los productos ya que si se elimina un producto del inventario se perderia la informacion del remito
    int cantidad;        // Cantidad de la operación

    DetalleRemito(int codigo, int c) : codigoProducto(codigo), cantidad(c) {}
};

// ==================
// Declaracion Remito
// ==================
class Remito{
protected:
    int id;
    Fecha fecha;
    vector<DetalleRemito> detalles; // Lista de codigos de productos + cantidades

public:
    // Constructor y destructor
    Remito(int id, const Fecha& fecha);
    virtual ~Remito();

    // Getters
    int getId() const;
    Fecha getFecha() const;
    const vector<DetalleRemito>& getDetalles() const;

    // Setters
    void setId(int nuevoId);
    void setFecha(const Fecha& nuevaFecha);

    // Operaciones sobre los detalles
    void agregarProducto(int codigoProducto, int cantidad);

    // Métodos polimórficos
    virtual string tipoRemito() const = 0;
    virtual void mostrarInfo() const = 0;
};



// =========================
// Declaracion RemitoEntrada
// =========================
class RemitoEntrada : public Remito{
public:
    RemitoEntrada(int id, const Fecha& fecha);
    virtual ~RemitoEntrada();

    virtual string tipoRemito() const override;
    virtual void mostrarInfo() const override;
};



// =========================
// Declaracion RemitoSalida
// =========================
class RemitoSalida : public Remito{
public:
    RemitoSalida(int id, const Fecha& fecha);
    virtual ~RemitoSalida();

    virtual string tipoRemito() const override;
    virtual void mostrarInfo() const override;
};

#endif // REMITO_H
