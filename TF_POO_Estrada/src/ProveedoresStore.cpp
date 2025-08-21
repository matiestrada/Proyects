#include "ProveedoresStore.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

// ESTRATEGIA TEXTO

// ==========================================================
// Implementacion ProveedoresTextoStore (Estrategia Concreta)
// ==========================================================

void ProveedoresTextoStore::guardar(const vector<Proveedor*>& proveedores) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw ios_base::failure("Error al abrir archivo de proveedores para guardar.");
    }

    for (const Proveedor* prov : proveedores) {
        archivo << prov->getId() << ";"
                << prov->getNombre() << ";"
                << prov->getContacto() << ";";

        // Guardamos los códigos de productos separados por coma
        const vector<Producto*>& productos = prov->getProductosAsociados();
        for (size_t i = 0; i < productos.size(); i++) {
            archivo << productos[i]->getCodigo();
            if (i < productos.size() - 1)
                archivo << ",";
        }
        archivo << "\n";
    }
    archivo.close();
}

vector<ProveedorDTO> ProveedoresTextoStore::cargar() {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw ios_base::failure("Error al abrir archivo de proveedores para cargar.");
    }

    vector<ProveedorDTO> proveedoresDTO;
    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        ProveedorDTO dto;
        string parte;

        // ID
        getline(ss, parte, ';');
        dto.id = stoi(parte);

        // Nombre
        getline(ss, dto.nombre, ';');

        // Contacto
        getline(ss, dto.contacto, ';');

        // Codigos de productos (separados por coma)
        getline(ss, parte, ';');
        stringstream codigosSS(parte);
        string codigoStr;
        while (getline(codigosSS, codigoStr, ',')) {
            if (!codigoStr.empty())
                dto.codigosProductos.push_back(stoi(codigoStr));
        }

        proveedoresDTO.push_back(dto);
    }

    archivo.close();
    return proveedoresDTO;
}

// ESTRATEGIA BINARIO

// ============================================================
// Implementacion ProveedoresBinarioStore (Estrategia Concreta)
// ============================================================

void ProveedoresBinarioStore::guardar(const vector<Proveedor*>& proveedores) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        throw ios_base::failure("Error al abrir archivo binario de proveedores para guardar.");
    }

    for (const Proveedor* prov : proveedores) {
        // ID
        int id = prov->getId();
        archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));

        // Nombre
        int lenNombre = prov->getNombre().size();
        archivo.write(reinterpret_cast<const char*>(&lenNombre), sizeof(int));
        archivo.write(prov->getNombre().c_str(), lenNombre);

        // Contacto
        int lenContacto = prov->getContacto().size();
        archivo.write(reinterpret_cast<const char*>(&lenContacto), sizeof(int));
        archivo.write(prov->getContacto().c_str(), lenContacto);

        // Productos asociados
        const vector<Producto*>& productos = prov->getProductosAsociados();
        int cantProd = productos.size();
        archivo.write(reinterpret_cast<const char*>(&cantProd), sizeof(int));

        for (const Producto* p : productos) {
            int codigo = p->getCodigo();
            archivo.write(reinterpret_cast<const char*>(&codigo), sizeof(int));
        }
    }

    archivo.close();
}

vector<ProveedorDTO> ProveedoresBinarioStore::cargar() {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        throw ios_base::failure("Error al abrir archivo binario de proveedores para cargar.");
    }

    vector<ProveedorDTO> proveedoresDTO;

    while (archivo.peek() != EOF) {
        ProveedorDTO dto;

        // ID
        int id;
        archivo.read(reinterpret_cast<char*>(&id), sizeof(int));
        if (archivo.eof()) break; // por seguridad
        dto.id = id;

        // Nombre
        int lenNombre;
        archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(int));
        string nombre(lenNombre, '\0');
        archivo.read(&nombre[0], lenNombre);
        dto.nombre = nombre;

        // Contacto
        int lenContacto;
        archivo.read(reinterpret_cast<char*>(&lenContacto), sizeof(int));
        string contacto(lenContacto, '\0');
        archivo.read(&contacto[0], lenContacto);
        dto.contacto = contacto;

        // Productos asociados
        int cantProd;
        archivo.read(reinterpret_cast<char*>(&cantProd), sizeof(int));
        for (int i = 0; i < cantProd; i++) {
            int codigo;
            archivo.read(reinterpret_cast<char*>(&codigo), sizeof(int));
            dto.codigosProductos.push_back(codigo);
        }

        proveedoresDTO.push_back(dto);
    }

    archivo.close();
    return proveedoresDTO;
}
