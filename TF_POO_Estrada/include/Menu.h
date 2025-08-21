#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "Sistema.h"
#include "InputUtils.h"

using namespace std;

class Sistema;

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
