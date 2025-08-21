#include "Menu.h"
#include "Sistema.h"
#include "InputUtils.h"
#include <iostream>
using namespace std;

Menu::Menu(Sistema& sist) : sistema(sist) {}

void Menu::MenuPrincipal() {
    // Cargar todo al iniciar
    try {
        sistema.cargarTodos();
        sistema.recalcularContadores(); // asegurar autoincrementales
    } catch (const std::exception& e) {
        cerr << "[ERROR] Al cargar datos: " << e.what() << "\n";
        InputUtils::pausar();
    }

    int opcion = -1;
    do {
        InputUtils::limpiarPantalla();
        cout << "========================================\n";
        cout << "     SISTEMA DE GESTION DE INVENTARIO   \n";
        cout << "========================================\n\n";
        sistema.alertaProductosStockBajo();

        cout << "1) Gestionar Productos\n";
        cout << "2) Gestionar Proveedores\n";
        cout << "3) Gestionar Transacciones\n";
        cout << "0) Salir\n";
        cout << "----------------------------------------\n";
        opcion = InputUtils::leerEntero("Seleccione una opcion: ", true);

        switch (opcion) {
            case 1: MenuProductos(); break;
            case 2: MenuProveedores(); break;
            case 3: MenuTransacciones(); break;
            case 0:
                cout << "\nGuardando informacion antes de salir...\n";
                try { sistema.guardarTodos(); }
                catch (const std::exception& e) { cerr << "[ERROR] " << e.what() << "\n"; }
                cout << "Hasta luego!\n";
                InputUtils::pausar();
                break;
            default:
                cout << "Opcion invalida.\n";
                InputUtils::pausar();
        }
    } while (opcion != 0);
}

void Menu::MenuProductos() {
    int opcion = -1;
    do {
        InputUtils::limpiarPantalla();
        cout << "========== GESTIONAR PRODUCTOS ==========\n";
        cout << "1) Agregar producto\n";
        cout << "2) Modificar producto\n";
        cout << "3) Eliminar producto\n";
        cout << "4) Mostrar lista de productos\n";
        cout << "0) Volver\n";
        cout << "-----------------------------------------\n";
        opcion = InputUtils::leerEntero("Opcion: ", true);

        switch (opcion) {
            case 1: sistema.operacionAgregarProducto(); break;
            case 2: sistema.operacionModificarProducto(); break;
            case 3: sistema.operacionEliminarProducto(); break;
            case 4: sistema.operacionMostrarProductos(); InputUtils::pausar(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; InputUtils::pausar();
        }
    } while (opcion != 0);
}

void Menu::MenuProveedores() {
    int opcion = -1;
    do {
        InputUtils::limpiarPantalla();
        cout << "======== GESTIONAR PROVEEDORES =========\n";
        cout << "1) Agregar proveedor\n";
        cout << "2) Modificar proveedor\n";
        cout << "3) Eliminar proveedor\n";
        cout << "4) Mostrar lista de proveedores\n";
        cout << "0) Volver\n";
        cout << "----------------------------------------\n";
        opcion = InputUtils::leerEntero("Opcion: ", true);

        switch (opcion) {
            case 1: sistema.operacionAgregarProveedor(); break;
            case 2: sistema.operacionModificarProveedor(); break;
            case 3: sistema.operacionEliminarProveedor(); break;
            case 4: sistema.operacionMostrarProveedores(); InputUtils::pausar(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; InputUtils::pausar();
        }
    } while (opcion != 0);
}

void Menu::MenuTransacciones() {
    int opcion = -1;
    do {
        InputUtils::limpiarPantalla();
        cout << "======== GESTIONAR TRANSACCIONES ========\n";
        cout << "1) Registrar Remito de Entrada\n";
        cout << "2) Registrar Remito de Salida\n";
        cout << "3) Mostrar lista de Remitos\n";
        cout << "0) Volver\n";
        cout << "-----------------------------------------\n";
        opcion = InputUtils::leerEntero("Opcion: ", true);

        switch (opcion) {
            case 1: sistema.operacionRegistrarRemitoEntrada(); break;
            case 2: sistema.operacionRegistrarRemitoSalida(); break;
            case 3: sistema.operacionMostrarRemitos(); InputUtils::pausar(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; InputUtils::pausar();
        }
    } while (opcion != 0);
}
