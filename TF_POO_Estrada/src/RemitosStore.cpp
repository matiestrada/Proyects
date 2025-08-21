#include "RemitosStore.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

// ESTRATEGIA TEXTO

// ======================================================
// Implementacion RemitosTextoStore (Estrategia Concreta)
// ======================================================
void RemitosTextoStore::guardar(const vector<Remito*>& remitos) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open())
        throw ios_base::failure("Error al abrir archivo de remitos para guardar.");

    for (Remito* r : remitos) {
        archivo << r->tipoRemito() << ";"
                << r->getId() << ";"
                << r->getFecha().getDay() << ";"
                << r->getFecha().getMonth() << ";"
                << r->getFecha().getYear() << ";";

        // Guardamos los detalles
        const auto& detalles = r->getDetalles();
        for (size_t i = 0; i < detalles.size(); ++i) {
            archivo << detalles[i].codigoProducto << ":" << detalles[i].cantidad;
            if (i < detalles.size() - 1)
                archivo << ",";
        }
        archivo << "\n";
    }
}

vector<Remito*> RemitosTextoStore::cargar() {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open())
        throw ios_base::failure("Error al abrir archivo de remitos para cargar.");

    vector<Remito*> remitos;
    string linea;

    while (getline(archivo, linea)) {
        try {
            stringstream ss(linea);
            string tipo;
            getline(ss, tipo, ';');

            string token;
            int id, day, month, year;
            getline(ss, token, ';'); id = stoi(token);
            getline(ss, token, ';'); day = stoi(token);
            getline(ss, token, ';'); month = stoi(token);
            getline(ss, token, ';'); year = stoi(token);

            Fecha fecha(day, month, year);
            Remito* r = nullptr;
            if (tipo == "Entrada")
                r = new RemitoEntrada(id, fecha);
            else if (tipo == "Salida")
                r = new RemitoSalida(id, fecha);
            else
                continue;

            // Detalles
            getline(ss, token, ';');
            stringstream ssDetalles(token);
            string detalleStr;
            while (getline(ssDetalles, detalleStr, ',')) {
                size_t pos = detalleStr.find(':');
                if (pos != string::npos) {
                    int codigo = stoi(detalleStr.substr(0, pos));
                    int cantidad = stoi(detalleStr.substr(pos + 1));
                    r->agregarProducto(codigo, cantidad);
                }
            }
            remitos.push_back(r);
        } catch (const exception& e) {
            throw invalid_argument(string("Error al cargar remito: ") + e.what());
        }
    }

    return remitos;
}


// ESTRATEGIA BINARIO

// ========================================================
// Implementacion RemitosBinarioStore (Estrategia Concreta)
// ========================================================
void RemitosBinarioStore::guardar(const vector<Remito*>& remitos) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open())
        throw ios_base::failure("Error al abrir archivo binario de remitos para guardar.");

    size_t n = remitos.size();
    archivo.write(reinterpret_cast<const char*>(&n), sizeof(n));

    for (Remito* r : remitos) {
        // Guardamos tipo
        string tipo = r->tipoRemito();
        size_t lenTipo = tipo.size();
        archivo.write(reinterpret_cast<const char*>(&lenTipo), sizeof(lenTipo));
        archivo.write(tipo.c_str(), lenTipo);

        // ID y fecha
        int id = r->getId();
        archivo.write(reinterpret_cast<const char*>(&id), sizeof(id));
        int day = r->getFecha().getDay();
        int month = r->getFecha().getMonth();
        int year = r->getFecha().getYear();
        archivo.write(reinterpret_cast<const char*>(&day), sizeof(day));
        archivo.write(reinterpret_cast<const char*>(&month), sizeof(month));
        archivo.write(reinterpret_cast<const char*>(&year), sizeof(year));

        // Detalles
        const auto& detalles = r->getDetalles();
        size_t nDetalles = detalles.size();
        archivo.write(reinterpret_cast<const char*>(&nDetalles), sizeof(nDetalles));
        for (const auto& d : detalles) {
            archivo.write(reinterpret_cast<const char*>(&d.codigoProducto), sizeof(d.codigoProducto));
            archivo.write(reinterpret_cast<const char*>(&d.cantidad), sizeof(d.cantidad));
        }
    }
}

vector<Remito*> RemitosBinarioStore::cargar() {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open())
        throw ios_base::failure("Error al abrir archivo binario de remitos para cargar.");

    vector<Remito*> remitos;

    size_t n;
    archivo.read(reinterpret_cast<char*>(&n), sizeof(n));

    for (size_t i = 0; i < n; ++i) {
        // Tipo
        size_t lenTipo;
        archivo.read(reinterpret_cast<char*>(&lenTipo), sizeof(lenTipo));
        string tipo(lenTipo, ' ');
        archivo.read(&tipo[0], lenTipo);

        // ID y fecha
        int id, day, month, year;
        archivo.read(reinterpret_cast<char*>(&id), sizeof(id));
        archivo.read(reinterpret_cast<char*>(&day), sizeof(day));
        archivo.read(reinterpret_cast<char*>(&month), sizeof(month));
        archivo.read(reinterpret_cast<char*>(&year), sizeof(year));
        Fecha fecha(day, month, year);

        Remito* r = nullptr;
        if (tipo == "Entrada") r = new RemitoEntrada(id, fecha);
        else if (tipo == "Salida") r = new RemitoSalida(id, fecha);
        else continue;

        // Detalles
        size_t nDetalles;
        archivo.read(reinterpret_cast<char*>(&nDetalles), sizeof(nDetalles));
        for (size_t j = 0; j < nDetalles; ++j) {
            int codigo, cantidad;
            archivo.read(reinterpret_cast<char*>(&codigo), sizeof(codigo));
            archivo.read(reinterpret_cast<char*>(&cantidad), sizeof(cantidad));
            r->agregarProducto(codigo, cantidad);
        }

        remitos.push_back(r);
    }

    return remitos;
}
