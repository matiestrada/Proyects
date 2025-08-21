#include "Menu.h"
#include "Sistema.h"
#include "Inventario.h"
#include "GestorProveedores.h"
#include "GestorTransacciones.h"

int main() {
    // Se crean las estrategias correspondientes para cargar/guardar la informacion.
    IProductosStore* strategyProductos = new ProductosBinarioStore("productos.dat");
    IProveedoresStore* strategyProveedores = new ProveedoresBinarioStore("proveedores.dat");
    IRemitosStore* strategyRemitos = new RemitosBinarioStore("remitos.dat");

    // Se crean los gestor para administrar los respectivos objetos.
    Inventario inv(strategyProductos);
    GestorProveedores gp(strategyProveedores);
    GestorTransacciones gt(strategyRemitos);

    // Se crea el sistema general de operaciones
    Sistema sistema(inv, gp, gt);

    // Se crea el menu
    Menu menu(sistema);

    // Se inicia el programa llamando al Menu Principal
    menu.MenuPrincipal();

    // Liberacion de memoria
    delete strategyProductos;
    delete strategyProveedores;
    delete strategyRemitos;

    return 0;
}
