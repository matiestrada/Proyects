#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "Sistema.h"
#include "InputUtils.h" // Header que contiene el namespace de utilidades de entrada auxiliares

using namespace std;

class Sistema; // Foward declaration

class Menu{
private:
    Sistema& sistema;
public:
    Menu(Sistema& sist);
    virtual ~Menu() = default;

    void MenuPrincipal();
    void MenuProductos();
    void MenuProveedores();
    void MenuTransacciones();
};

#endif // MENU_H
