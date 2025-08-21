#include "ProductosStore.h"
#include <fstream>
#include <stdexcept>
#include <sstream>

// ESTRATEGIA TEXTO

// ========================================================
// Implementacion ProductosTextoStore (Estrategia Concreta)
// ========================================================
void ProductosTextoStore::guardar(const vector<Producto*>& productos) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw ios_base::failure("Error al abrir productos.txt para guardar.");
    }

    for (Producto* p : productos) {
        // 1) Tipo
        if (dynamic_cast<ProductoAlimenticio*>(p)) {
            archivo << "Alimenticio;";
        } else if (dynamic_cast<ProductoElectronico*>(p)) {
            archivo << "Electronico;";
        } else if (dynamic_cast<ProductoLimpieza*>(p)) {
            archivo << "Limpieza;";
        } else {
            // Si no es ninguno de los tipos conocidos, lo salteamos
            continue;
        }

        // 2) Atributos base
        archivo << p->getCodigo() << ";"
                << p->getNombre() << ";"
                << p->getMarca() << ";"
                << p->getPrecio() << ";"
                << p->getStock() << ";"
                << p->getUmbralStockBajo() << ";";

        // 3) Atributos específicos
        if (auto pa = dynamic_cast<ProductoAlimenticio*>(p)) {
            archivo << pa->getFechaVencimiento() << ";" << (pa->getEsFresco() ? 1 : 0);
        } else if (auto pe = dynamic_cast<ProductoElectronico*>(p)) {
            archivo << pe->getGarantiaMeses() << ";" << pe->getVoltaje();
        } else if (auto pl = dynamic_cast<ProductoLimpieza*>(p)) {
            archivo << pl->getFechaVencimiento() << ";"
                    << pl->getSuperficieUso() << ";"
                    << (pl->getEsToxico() ? 1 : 0);
        }

        archivo << "\n";
    }
}

vector<Producto*> ProductosTextoStore::cargar() {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw ios_base::failure("Error al abrir productos.txt para cargar.");
    }

    vector<Producto*> productos;
    string linea;
    int nroLinea = 0;

    while (getline(archivo, linea)) {
        ++nroLinea;
        if (linea.empty()) continue;

        try {
            stringstream ss(linea);
            string tipo;
            getline(ss, tipo, ';');

            // Campos base en el MISMO orden en que se guardaron
            int codigo, stock, umbral;
            double precio;
            string nombre, marca, tmp;

            getline(ss, tmp, ';');       codigo = stoi(tmp);
            getline(ss, nombre, ';');
            getline(ss, marca,  ';');
            getline(ss, tmp, ';');       precio = stod(tmp);
            getline(ss, tmp, ';');       stock  = stoi(tmp);
            getline(ss, tmp, ';');       umbral = stoi(tmp);

            if (tipo == "Alimenticio") {
                string fecha, sFresco;
                getline(ss, fecha,   ';');
                getline(ss, sFresco, ';'); // puede no haber ';' final; toma el resto de la línea
                bool fresco = (sFresco == "1" || sFresco == "true" || sFresco == "True");
                productos.push_back(new ProductoAlimenticio(codigo, nombre, marca, precio, stock, umbral, fecha, fresco));

            } else if (tipo == "Electronico") {
                string sGarantia, sVolt;
                getline(ss, sGarantia, ';');
                getline(ss, sVolt,     ';');
                int garantia = stoi(sGarantia);
                int voltaje  = stoi(sVolt);
                productos.push_back(new ProductoElectronico(codigo, nombre, marca, precio, stock, umbral, garantia, voltaje));

            } else if (tipo == "Limpieza") {
                string fecha, superficie, sToxico;
                getline(ss, fecha,      ';');
                getline(ss, superficie, ';');
                getline(ss, sToxico,    ';');
                bool toxico = (sToxico == "1" || sToxico == "true" || sToxico == "True");
                productos.push_back(new ProductoLimpieza(codigo, nombre, marca, precio, stock, umbral, fecha, superficie, toxico));

            } else {
                throw invalid_argument("Tipo de producto desconocido: " + tipo);
            }

        } catch (const exception& e) {
            // Evitamos fugas si abortamos por error
            for (auto* p : productos) delete p;
            productos.clear();
            throw invalid_argument("Error al cargar producto en linea " + to_string(nroLinea) + ": " + e.what());
        }
    }

    return productos;
}



// ESTRATEGIA BINARIO

// ========================================================
// Implementacion ProductosBinarioStore (Estrategia Concreta)
// ========================================================

// funciones auxiliares
void escribirString(ofstream& out, const string& str) {
    size_t tam = str.size();
    out.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
    out.write(str.c_str(), tam);
}

string leerString(ifstream& in) {
    size_t tam;
    in.read(reinterpret_cast<char*>(&tam), sizeof(tam));
    char* buffer = new char[tam];
    in.read(buffer, tam);
    string str(buffer, tam);
    delete[] buffer;
    return str;
}

void ProductosBinarioStore::guardar(const vector<Producto*>& productos) {
    ofstream out(nombreArchivo, ios::binary);
    if (!out.is_open()) throw runtime_error("No se pudo abrir el archivo binario para guardar productos.");

    for (Producto* p : productos) {
        string tipo;
        if (dynamic_cast<ProductoAlimenticio*>(p)) tipo = "Alimenticio";
        else if (dynamic_cast<ProductoElectronico*>(p)) tipo = "Electronico";
        else if (dynamic_cast<ProductoLimpieza*>(p)) tipo = "Limpieza";
        else throw runtime_error("Producto desconocido al guardar binario.");

        escribirString(out, tipo);

        int codigo = p->getCodigo();
        int stock = p->getStock();
        int umbral = p->getUmbralStockBajo();
        double precio = p->getPrecio();
        escribirString(out, p->getNombre());
        escribirString(out, p->getMarca());
        out.write(reinterpret_cast<char*>(&codigo), sizeof(codigo));
        out.write(reinterpret_cast<char*>(&precio), sizeof(precio));
        out.write(reinterpret_cast<char*>(&stock), sizeof(stock));
        out.write(reinterpret_cast<char*>(&umbral), sizeof(umbral));

        if (tipo == "Alimenticio") {
            auto* pa = dynamic_cast<ProductoAlimenticio*>(p);
            escribirString(out, pa->getFechaVencimiento());
            char esFresco = pa->getEsFresco() ? 1 : 0;
            out.write(&esFresco, sizeof(esFresco));
        } else if (tipo == "Electronico") {
            auto* pe = dynamic_cast<ProductoElectronico*>(p);
            int garantia = pe->getGarantiaMeses();
            int voltaje = pe->getVoltaje();
            out.write(reinterpret_cast<char*>(&garantia), sizeof(garantia));
            out.write(reinterpret_cast<char*>(&voltaje), sizeof(voltaje));
        } else if (tipo == "Limpieza") {
            auto* pl = dynamic_cast<ProductoLimpieza*>(p);
            escribirString(out, pl->getFechaVencimiento());
            escribirString(out, pl->getSuperficieUso());
            char esToxico = pl->getEsToxico() ? 1 : 0;
            out.write(&esToxico, sizeof(esToxico));
        }
    }

    out.close();
}

vector<Producto*> ProductosBinarioStore::cargar() {
    ifstream in(nombreArchivo, ios::binary);
    if (!in.is_open()) throw runtime_error("No se pudo abrir el archivo binario para cargar productos.");

    vector<Producto*> productos;

    while (in.peek() != EOF) {
        string tipo = leerString(in);
        string nombre = leerString(in);
        string marca = leerString(in);

        int codigo, stock, umbral;
        double precio;

        in.read(reinterpret_cast<char*>(&codigo), sizeof(codigo));
        in.read(reinterpret_cast<char*>(&precio), sizeof(precio));
        in.read(reinterpret_cast<char*>(&stock), sizeof(stock));
        in.read(reinterpret_cast<char*>(&umbral), sizeof(umbral));

        if (tipo == "Alimenticio") {
            string fecha = leerString(in);
            char esFrescoChar;
            in.read(&esFrescoChar, sizeof(esFrescoChar));
            bool esFresco = esFrescoChar != 0;
            productos.push_back(new ProductoAlimenticio(codigo, nombre, marca, precio, stock, umbral, fecha, esFresco));
        } else if (tipo == "Electronico") {
            int garantia, voltaje;
            in.read(reinterpret_cast<char*>(&garantia), sizeof(garantia));
            in.read(reinterpret_cast<char*>(&voltaje), sizeof(voltaje));
            productos.push_back(new ProductoElectronico(codigo, nombre, marca, precio, stock, umbral, garantia, voltaje));
        } else if (tipo == "Limpieza") {
            string fecha = leerString(in);
            string superficie = leerString(in);
            char esToxicoChar;
            in.read(&esToxicoChar, sizeof(esToxicoChar));
            bool esToxico = esToxicoChar != 0;
            productos.push_back(new ProductoLimpieza(codigo, nombre, marca, precio, stock, umbral, fecha, superficie, esToxico));
        } else {
            throw runtime_error("Tipo de producto desconocido al cargar binario.");
        }
    }

    in.close();
    return productos;
}
