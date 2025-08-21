#ifndef GESTORTRANSACCIONES_H
#define GESTORTRANSACCIONES_H

#include <vector>
#include "Remito.h"
#include "Inventario.h"
#include "RemitosStore.h"

using namespace std;

// ==========================
// Declaracion GestorTransacciones
// ==========================
class GestorTransacciones {
private:
    vector<Remito*> remitos;  // lista histórica de remitos
    IRemitosStore* store; // estrategia de persistencia

public:
    GestorTransacciones(IRemitosStore* s);
    ~GestorTransacciones();

    // Registro de transacciones
    void agregarRemito(Remito* r);
    void registrarRemito(Remito* r, Inventario& inventario); // usa el inventario para actualizar el stock de los productos del remito

    // Búsquedas
    Remito* buscarRemito(int id) const;
    const vector<Remito*>& getRemitos() const;

    // Operaciones de persistencia
    void setEstrategia(IRemitosStore* str);
    void guardar();
    void cargar();

    // Mostrar
    void mostrarRemitos() const;
};

#endif // GESTORTRANSACCIONES_H
