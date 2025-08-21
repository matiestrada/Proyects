#ifndef REMITOSSTORE_H
#define REMITOSSTORE_H

#include <iostream>
#include <vector>
#include "Remito.h"

using namespace std;

// ================================================
// Declaracion IRemitosStore (Estrategia Abstracta)
// ================================================
class IRemitosStore {
public:
    virtual ~IRemitosStore() = default;

    // Guarda los productos en un archivo (texto o binario según la implementación)
    virtual void guardar(const vector<Remito*>& remitos) = 0;

    // Carga los productos desde un archivo
    virtual vector<Remito*> cargar() = 0;
};


// ===================================================
// Declaracion RemitosTextoStore (Estrategia Concreta)
// ===================================================
class RemitosTextoStore : public IRemitosStore {
private:
    string nombreArchivo;
public:
    RemitosTextoStore(const string& archivo) : nombreArchivo(archivo) {}
    void guardar(const vector<Remito*>& remitos) override;
    vector<Remito*> cargar() override;
};


// =====================================================
// Declaracion RemitosBinarioStore (Estrategia Concreta)
// =====================================================
class RemitosBinarioStore : public IRemitosStore {
private:
    string nombreArchivo;
public:
    RemitosBinarioStore(const string& archivo) : nombreArchivo(archivo) {}
    void guardar(const vector<Remito*>& remitos) override;
    vector<Remito*> cargar() override;
};


#endif // REMITOSSTORE_H
