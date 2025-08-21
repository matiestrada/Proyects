#include "Menu.h"
#include "Sistema.h"
#include "Inventario.h"
#include "GestorProveedores.h"
#include "GestorTransacciones.h"

int main() {
    IProductosStore* strategyProductos = new ProductosBinarioStore("productos.dat");
    IProveedoresStore* strategyProveedores = new ProveedoresBinarioStore("proveedores.dat");
    IRemitosStore* strategyRemitos = new RemitosBinarioStore("remitos.dat");

    Inventario inv(strategyProductos);
    GestorProveedores gp(strategyProveedores);
    GestorTransacciones gt(strategyRemitos);

    Sistema sistema(inv, gp, gt);
    Menu menu(sistema);

    menu.MenuPrincipal();

    delete strategyProductos;
    delete strategyProveedores;
    delete strategyRemitos;

    return 0;
}
