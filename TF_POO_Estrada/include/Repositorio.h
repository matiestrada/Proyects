#ifndef REPOSITORIO_H
#define REPOSITORIO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "Producto.h"
#include "Inventario.h"
#include "GestorProveedores.h"
#include "GestorTransacciones.h"

using namespace std;
// =====================================
// Declaracion Persistencia (Estrategia)
// =====================================
class Persistencia{
public:
    //Constructor y destructor
    Persistencia();
    virtual ~Persistencia();

    //Metodos virtuales puros.
    virtual void guardar() const = 0;
    virtual void cargar() const = 0;
};



// ==================================
// Declaracion Repositorio (Contexto)
// ==================================
class Repositorio{
private:
    unique_ptr<Persistencia> estrategia; // estrategia concreta

public:
    //Constructor y destructor
    Repositorio() : estrategia(nullptr) {}
    Repositorio(Persistencia* estr);
    virtual ~Repositorio() = default;

    //Setter
    void setEstrategia(Persistencia* estr);

    //Metodos que utilizan la estrategia concreta.
    void guardar();
    void cargar();
};


// =======================================================
// Declaracion PersistenciaProductos (Estrategia Concreta)
// =======================================================
class PersistenciaProductos : public Persistencia{
private:
    Inventario& inventario;
    string nombreArchivo;
public:
    //Constructor y destructor
    PersistenciaProductos(Inventario& inv, const string& archivo = "productos.txt");
    virtual ~PersistenciaProductos();

    virtual void guardar() const override;
    virtual void cargar() const override;
};



// ==========================================================
// Declaracion PersistenciaProveedores (Estrategia Concreta)
// ==========================================================
class PersistenciaProveedores : public Persistencia{
private:
    GestorProveedores& gestor;
    Inventario& inventario; // Necesitamos acceso a los productos para asociarlos
    string nombreArchivo;

public:
    //Constructor y destructor
    PersistenciaProveedores(GestorProveedores& gp, Inventario& inv, const string& archivo = "proveedores.dat");
    virtual ~PersistenciaProveedores();

    virtual void guardar() const override;
    virtual void cargar() const override;
};



// ==========================================================
// Declaracion PersistenciaRemitos (Estrategia Concreta)
// ==========================================================
class PersistenciaRemitos : public Persistencia{
private:
    GestorTransacciones& gestor;
    string nombreArchivo;

public:
    PersistenciaRemitos(GestorTransacciones& g, const string& archivo = "remitos.txt");
    virtual ~PersistenciaRemitos();

    void guardar() const override;
    void cargar() const override;
};


#endif // REPOSITORIO_H
