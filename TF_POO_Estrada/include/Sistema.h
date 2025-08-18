#ifndef SISTEMA_H
#define SISTEMA_H

#include <memory>
#include <stdexcept>
#include <string>
#include "Inventario.h"
#include "GestorProveedores.h"
#include "GestorTransacciones.h"
#include "Repositorio.h"  // contiene Persistencia*, Repositorio, Persistencias concretas

class Sistema {
private:
    // Núcleo del dominio
    Inventario inventario;
    GestorProveedores gestorProveedores;
    GestorTransacciones gestorTransacciones;
    Repositorio repo;

    // ======================
    // Menús
    // ======================
    void menuPrincipal();
    void alertaStockBajo() const;

    // Productos
    void menuProductos();
    void opAgregarProducto();
    void opModificarProducto();
    void opEliminarProducto();
    void opMostrarProductos() const;

    // Proveedores
    void menuProveedores();
    void opAgregarProveedor();
    void opModificarProveedor();
    void opEliminarProveedor();
    void opMostrarProveedores() const;

    // Transacciones
    void menuTransacciones();
    void opRegistrarRemitoEntrada();
    void opRegistrarRemitoSalida();
    void opMostrarRemitos() const;

    // ======================
    // Persistencia
    // ======================
    void cargarDatos();   // productos -> proveedores -> remitos
    void guardarDatos();  // guarda todo

    // Auxiliares de construcción/edición
    int elegirProveedorId() const;          // lista y retorna id o -1 si no existe
    int elegirTipoProducto() const;         // 1: Alimenticio, 2: Electrónico, 3: Limpieza
    void editarAtributosProducto(Producto* p);
    void editarAtributosProveedor(Proveedor* prov);

    // ======================
    // Utilidades de UI/Input
    // ======================
    void limpiarPantalla() const;
    void pausar() const;
    int leerEntero(const std::string& prompt, bool permitirCeroNeg = true) const;
    double leerDouble(const std::string& prompt) const;
    std::string leerLinea(const std::string& prompt) const;
    bool leerBoolSN(const std::string& prompt) const; // S/N o 1/0

public:
    Sistema();
    void ejecutar(); // entrypoint
};

#endif // SISTEMA_H
