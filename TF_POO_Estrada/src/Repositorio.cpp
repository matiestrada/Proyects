#include "Repositorio.h"

// ========================================
// Implementacion Persistencia (Estrategia)
// ========================================
Persistencia::Persistencia(){}

Persistencia::~Persistencia() {}



// =====================================
// Implementacion Repositorio (Contexto)
// =====================================
Repositorio::Repositorio(Persistencia* estr) : estrategia(estr) {}

void Repositorio::setEstrategia(Persistencia* estr) { estrategia.reset(estr); }

void Repositorio::guardar() {
    if (estrategia)
        estrategia->guardar();
}

void Repositorio::cargar() {
    if (estrategia)
        estrategia->cargar();
}



// ==========================================================
// Implementacion PersistenciaProductos (Estrategia Concreta)
// ==========================================================
PersistenciaProductos::PersistenciaProductos(Inventario& inv, const string& archivo) : inventario(inv), nombreArchivo(archivo) {}

PersistenciaProductos::~PersistenciaProductos() {}

void PersistenciaProductos::guardar() const {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) return;

        for (Producto* p : inventario.getProductos()) {
            // Guardamos tipo de producto primero
            if (dynamic_cast<ProductoAlimenticio*>(p))
                archivo << "Alimenticio;";
            else if (dynamic_cast<ProductoElectronico*>(p))
                archivo << "Electronico;";
            else if (dynamic_cast<ProductoLimpieza*>(p))
                archivo << "Limpieza;";
            else
                continue;

            // Campos comunes
            archivo << p->getCodigo() << ";"
                    << p->getNombre() << ";"
                    << p->getMarca() << ";"
                    << p->getPrecio() << ";"
                    << p->getStock() << ";"
                    << p->getUmbralStockBajo() << ";";

            // Campos específicos
            if (ProductoAlimenticio* pa = dynamic_cast<ProductoAlimenticio*>(p)) {
                archivo << pa->getFechaVencimiento() << ";"
                        << pa->getEsFresco();
            } else if (ProductoElectronico* pe = dynamic_cast<ProductoElectronico*>(p)) {
                archivo << pe->getGarantiaMeses() << ";"
                        << pe->getVoltaje();
            } else if (ProductoLimpieza* pl = dynamic_cast<ProductoLimpieza*>(p)) {
                archivo << pl->getFechaVencimiento() << ";"
                        << pl->getSuperficieUso() << ";"
                        << pl->getEsToxico();
            }
            archivo << "\n";
        }

        archivo.close();
}

void PersistenciaProductos::cargar() const {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) return;

        inventario.vaciarInventario(); // eliminamos productos actuales

        string linea;
        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string tipo;
            getline(ss, tipo, ';');

            int codigo, stock, umbral;
            double precio;
            string nombre, marca;

            getline(ss, linea, ';'); codigo = stoi(linea);
            getline(ss, nombre, ';');
            getline(ss, marca, ';');
            getline(ss, linea, ';'); precio = stod(linea);
            getline(ss, linea, ';'); stock = stoi(linea);
            getline(ss, linea, ';'); umbral = stoi(linea);

            if (tipo == "Alimenticio") {
                string fecha; bool fresco;
                getline(ss, fecha, ';');
                getline(ss, linea, ';'); fresco = (linea == "1");
                inventario.agregarProducto(new ProductoAlimenticio(codigo, nombre, marca, precio, stock, umbral, fecha, fresco));
            } else if (tipo == "Electronico") {
                int garantia, voltaje;
                getline(ss, linea, ';'); garantia = stoi(linea);
                getline(ss, linea, ';'); voltaje = stoi(linea);
                inventario.agregarProducto(new ProductoElectronico(codigo, nombre, marca, precio, stock, umbral, garantia, voltaje));
            } else if (tipo == "Limpieza") {
                string fecha, superficie; bool toxico;
                getline(ss, fecha, ';');
                getline(ss, superficie, ';');
                getline(ss, linea, ';'); toxico = (linea == "1");
                inventario.agregarProducto(new ProductoLimpieza(codigo, nombre, marca, precio, stock, umbral, fecha, superficie, toxico));
            }
        }

        archivo.close();
}



// ============================================================
// Implementacion PersistenciaProveedores (Estrategia Concreta)
// ============================================================
PersistenciaProveedores::PersistenciaProveedores(GestorProveedores& gp, Inventario& inv, const string& archivo) : gestor(gp), inventario(inv), nombreArchivo(archivo) {}

PersistenciaProveedores::~PersistenciaProveedores() {}

void PersistenciaProveedores::guardar() const {
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return;

    const vector<Proveedor*>& proveedores = gestor.getProveedores();
    size_t cantProveedores = proveedores.size();
    archivo.write(reinterpret_cast<const char*>(&cantProveedores), sizeof(size_t));

    for (Proveedor* prov : proveedores) {
        // Guardamos id
        int id = prov->getId();
        archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));

        // Guardamos nombre
        size_t lenNombre = prov->getNombre().size();
        archivo.write(reinterpret_cast<const char*>(&lenNombre), sizeof(size_t));
        archivo.write(prov->getNombre().c_str(), lenNombre);

        // Guardamos contacto
        size_t lenContacto = prov->getContacto().size();
        archivo.write(reinterpret_cast<const char*>(&lenContacto), sizeof(size_t));
        archivo.write(prov->getContacto().c_str(), lenContacto);

        // Guardamos productos asociados (por código)
        const vector<Producto*>& productos = prov->getProductosAsociados();
        size_t cantProductos = productos.size();
        archivo.write(reinterpret_cast<const char*>(&cantProductos), sizeof(size_t));
        for (Producto* p : productos) {
            int codigo = p->getCodigo();
            archivo.write(reinterpret_cast<const char*>(&codigo), sizeof(int));
        }
    }

    archivo.close();
}

void PersistenciaProveedores::cargar() const {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return;

    size_t cantProveedores;
    archivo.read(reinterpret_cast<char*>(&cantProveedores), sizeof(size_t));

    for (size_t i = 0; i < cantProveedores; i++) {
        // Leemos id
        int id;
        archivo.read(reinterpret_cast<char*>(&id), sizeof(int));

        // Leemos nombre
        size_t lenNombre;
        archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(size_t));
        string nombre(lenNombre, '\0');
        archivo.read(&nombre[0], lenNombre);

        // Leemos contacto
        size_t lenContacto;
        archivo.read(reinterpret_cast<char*>(&lenContacto), sizeof(size_t));
        string contacto(lenContacto, '\0');
        archivo.read(&contacto[0], lenContacto);

        // Creamos proveedor
        Proveedor* prov = new Proveedor(id, nombre, contacto);

        // Leemos productos asociados
        size_t cantProductos;
        archivo.read(reinterpret_cast<char*>(&cantProductos), sizeof(size_t));
        for (size_t j = 0; j < cantProductos; j++) {
            int codigo;
            archivo.read(reinterpret_cast<char*>(&codigo), sizeof(int));

            // Buscamos producto en inventario
            Producto* prod = inventario.buscarProducto(codigo);
            if (prod)
                prov->asociarProducto(prod);
        }

        // Agregamos al gestor
        gestor.agregarProveedor(prov);
    }

    archivo.close();
}



// ========================================================
// Implementacion PersistenciaRemitos (Estrategia Concreta)
// ========================================================
PersistenciaRemitos::PersistenciaRemitos(GestorTransacciones& g, const string& archivo) : gestor(g), nombreArchivo(archivo) {}
PersistenciaRemitos::~PersistenciaRemitos() {}

void PersistenciaRemitos::guardar() const {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) return;

    for (Remito* r : gestor.getRemitos()) {
        // tipo
        archivo << r->tipoRemito() << ";";

        // id
        archivo << r->getId() << ";";

        // fecha
        archivo << r->getFecha().getDay() << "-"
                << r->getFecha().getMonth() << "-"
                << r->getFecha().getYear() << ";";

        // detalles
        const auto& detalles = r->getDetalles();
        for (size_t i = 0; i < detalles.size(); i++) {
            archivo << detalles[i].codigoProducto << "," << detalles[i].cantidad;
            if (i < detalles.size() - 1) archivo << "|";
        }
        archivo << "\n";
    }

    archivo.close();
}

void PersistenciaRemitos::cargar() const {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) return;

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);

        string tipo, idStr, fechaStr, detallesStr;
        getline(ss, tipo, ';');
        getline(ss, idStr, ';');
        getline(ss, fechaStr, ';');
        getline(ss, detallesStr, ';');

        int id = stoi(idStr);

        // parsear fecha
        int d, m, y;
        char sep;
        stringstream sf(fechaStr);
        sf >> d >> sep >> m >> sep >> y;
        Fecha fecha(d, m, y);

        Remito* r = nullptr;
        if (tipo == "Entrada") {
            r = new RemitoEntrada(id, fecha);
        } else if (tipo == "Salida") {
            r = new RemitoSalida(id, fecha);
        }

        // parsear detalles
        stringstream sd(detallesStr);
        string det;
        while (getline(sd, det, '|')) {
            stringstream sdet(det);
            string codStr, cantStr;
            getline(sdet, codStr, ',');
            getline(sdet, cantStr, ',');
            int codigo = stoi(codStr);
            int cantidad = stoi(cantStr);
            r->agregarProducto(codigo, cantidad);
        }

        gestor.agregarRemito(r);
    }

    archivo.close();
}
