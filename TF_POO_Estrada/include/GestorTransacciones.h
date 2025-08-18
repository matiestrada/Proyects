#ifndef GESTORTRANSACCIONES_H
#define GESTORTRANSACCIONES_H

#include <vector>
#include "Remito.h"
#include "Inventario.h"

using namespace std;

// ==========================
// Declaracion GestorTransacciones
// ==========================
class GestorTransacciones {
private:
    vector<Remito*> remitos;  // lista histórica de remitos

public:
    GestorTransacciones();
    ~GestorTransacciones();

    void agregarRemito(Remito* r);

    // Registro de transacciones
    void registrarRemito(Remito* r, Inventario& inventario);

    // Búsquedas
    Remito* buscarRemito(int id) const;
    const vector<Remito*>& getRemitos() const;

    // Mostrar
    void mostrarRemitos() const;
};

#endif // GESTORTRANSACCIONES_H
