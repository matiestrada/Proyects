#include "Producto.h"

// =======================
// Implementación Producto
// =======================
Producto::Producto(int codigo, const string& nombre, const string& marca, double precio, int stock, int umbralStockBajo)
    : codigo(codigo), nombre(nombre), marca(marca), precio(precio), stock(stock), umbralStockBajo(umbralStockBajo) {}

Producto::~Producto() {}

// Getters
int Producto::getCodigo() const { return codigo; }
string Producto::getNombre() const { return nombre; }
string Producto::getMarca() const { return marca; }
double Producto::getPrecio() const { return precio; }
int Producto::getStock() const { return stock; }
int Producto::getUmbralStockBajo() const { return umbralStockBajo; }

// Setters
void Producto::setCodigo(int nuevoCodigo) { codigo = nuevoCodigo; }
void Producto::setNombre(const string& nuevoNombre) { nombre = nuevoNombre; }
void Producto::setMarca(const string& nuevaMarca) { marca = nuevaMarca; }
void Producto::setPrecio(double nuevoPrecio) { precio = nuevoPrecio; }
void Producto::setStock(int nuevoStock) { stock = nuevoStock; }
void Producto::setUmbralStockBajo(int nuevoUmbralStockBajo) { umbralStockBajo = nuevoUmbralStockBajo; }



// ==================================
// Implementación ProductoAlimenticio
// ==================================
ProductoAlimenticio::ProductoAlimenticio(int codigo, const string& nombre, const string& marca, double precio, int stock, int umbralStockBajo,
                                         const string& fechaVencimiento, bool esFresco)
    : Producto(codigo, nombre, marca, precio, stock, umbralStockBajo),
      fechaVencimiento(fechaVencimiento), esFresco(esFresco) {}

ProductoAlimenticio::~ProductoAlimenticio() {}

// Getters
string ProductoAlimenticio::getFechaVencimiento() const { return fechaVencimiento; }
bool ProductoAlimenticio::getEsFresco() const { return esFresco; }

// Setters
void ProductoAlimenticio::setFechaVencimiento(const string& nuevaFechaVencimiento) { fechaVencimiento = nuevaFechaVencimiento; }
void ProductoAlimenticio::setEsFresco(bool nuevoEsFresco) { esFresco = nuevoEsFresco; }

void ProductoAlimenticio::mostrarInfo() const {
    cout << "Producto Alimenticio" << "\n"
         << "Codigo: " << codigo << "\n"
         << "Nombre: " << nombre << "\n"
         << "Marca: " << marca << "\n"
         << "Precio: $" << precio << "\n"
         << "Stock: " << stock << "\n"
         << "Umbral de stock bajo: " << umbralStockBajo << "\n"
         << "Fecha de Vencimiento: " << fechaVencimiento << "\n"
         << "Producto Fresco: " << (esFresco ? "Si" : "No") << "\n\n";
}



// ==================================
// Implementación ProductoElectronico
// ==================================
ProductoElectronico::ProductoElectronico(int codigo, const string& nombre, const string& marca, double precio, int stock, int umbralStockBajo,
                                         int garantiaMeses, int voltaje)
    : Producto(codigo, nombre, marca, precio, stock, umbralStockBajo),
      garantiaMeses(garantiaMeses), voltaje(voltaje) {}

ProductoElectronico::~ProductoElectronico() {}

// Getters
int ProductoElectronico::getGarantiaMeses() const { return garantiaMeses; }
int ProductoElectronico::getVoltaje() const { return voltaje; }

// Setters
void ProductoElectronico::setGarantiaMeses(int nuevaGarantiaMeses) { garantiaMeses = nuevaGarantiaMeses; }
void ProductoElectronico::setVoltaje(int nuevoVoltaje) { voltaje = nuevoVoltaje; }

void ProductoElectronico::mostrarInfo() const {
    cout << "Producto Electronico" << "\n"
         << "Codigo: " << codigo << "\n"
         << "Nombre: " << nombre << "\n"
         << "Marca: " << marca << "\n"
         << "Precio: $" << precio << "\n"
         << "Stock: " << stock << "\n"
         << "Umbral de stock bajo: " << umbralStockBajo << "\n"
         << "Meses de garantia: " << garantiaMeses << "\n"
         << "Voltaje: " << voltaje << "V" << "\n\n";
}



// ==================================
// Implementación ProductoLimpieza
// ==================================
ProductoLimpieza::ProductoLimpieza(int codigo, const string& nombre, const string& marca, double precio, int stock, int umbralStockBajo,
                                         const string& fechaVencimiento, const string& superficieUso, bool esToxico)
    : Producto(codigo, nombre, marca, precio, stock, umbralStockBajo),
      fechaVencimiento(fechaVencimiento), superficieUso(superficieUso), esToxico(esToxico) {}

ProductoLimpieza::~ProductoLimpieza() {}

// Getters
string ProductoLimpieza::getFechaVencimiento() const { return fechaVencimiento; }
string ProductoLimpieza::getSuperficieUso() const { return superficieUso; }
bool ProductoLimpieza::getEsToxico() const { return esToxico; }

// Setters
void ProductoLimpieza::setFechaVencimiento(const string& nuevaFechaVencimiento) { fechaVencimiento = nuevaFechaVencimiento; }
void ProductoLimpieza::setSuperficieUso(const string& nuevaSuperficieUso) { superficieUso = nuevaSuperficieUso; }
void ProductoLimpieza::setEsToxico(bool nuevoEsToxico) { esToxico = nuevoEsToxico; }

void ProductoLimpieza::mostrarInfo() const {
    cout << "Producto Limpieza" << "\n"
         << "Codigo: " << codigo << "\n"
         << "Nombre: " << nombre << "\n"
         << "Marca: " << marca << "\n"
         << "Precio: $" << precio << "\n"
         << "Stock: " << stock << "\n"
         << "Umbral de stock bajo: " << umbralStockBajo << "\n"
         << "Fecha de Vencimiento: " << fechaVencimiento << "\n"
         << "Superficie de uso: " << superficieUso << "\n"
         << "Producto Toxico: " << (esToxico ? "Si" : "No") << "\n\n";
}
