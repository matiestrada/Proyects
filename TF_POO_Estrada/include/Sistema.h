#ifndef SISTEMA_H
#define SISTEMA_H

#include <iostream>
#include <string>
#include "Inventario.h"
#include "GestorProveedores.h"
#include "GestorTransacciones.h"
#include "InputUtils.h" // Header que contiene el namespace de utilidades de entrada auxiliares

class Sistema{
private:
    Inventario& inventario;
    GestorProveedores& gestorProveedores;
    GestorTransacciones& gestorTransacciones;

    // Autoincrementales
    int ultimoCodigoProducto = 0; // variables para que se asignen codigos/ids automaticamente
    int ultimoIdProveedor = 0;
    int ultimoIdRemito = 0;

public:
    Sistema(Inventario& inv, GestorProveedores& gp, GestorTransacciones& gt);
    virtual ~Sistema() = default;

    // ===========================
    // Operaciones de Productos
    // ===========================
    void operacionAgregarProducto();
    void operacionModificarProducto();
    void operacionEliminarProducto();
    void operacionMostrarProductos() const;
    void alertaProductosStockBajo() const;

    // ===========================
    // Operaciones de Proveedores
    // ===========================
    void operacionAgregarProveedor();
    void operacionModificarProveedor();
    void operacionEliminarProveedor();
    void operacionMostrarProveedores() const;

    // ===========================
    // Operaciones de Transacciones
    // ===========================
    void operacionRegistrarRemitoEntrada();
    void operacionRegistrarRemitoSalida();
    void operacionMostrarRemitos() const;

    // =================================================
    // Operaciones de guardar y cargar todos los objetos
    // =================================================
    void guardarTodos();
    void cargarTodos();

    // Recalcular contadores tras cargar
    void recalcularContadores();
};

#endif // SISTEMA_H
