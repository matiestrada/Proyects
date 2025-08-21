#include "Proveedor.h"
#include "Producto.h"

// ========================
// Implementacion Proveedor
// ========================

Proveedor::Proveedor(int id, const string& nombre, const string& contacto)
    : id(id), nombre(nombre), contacto(contacto) {}

// se limpia el vector de productos asociados
Proveedor::~Proveedor() {
    productosAsociados.clear();
}

// Getters
int Proveedor::getId() const { return id; }
string Proveedor::getNombre() const { return nombre; }
string Proveedor::getContacto() const { return contacto; }
const vector<Producto*>& Proveedor::getProductosAsociados() const { return productosAsociados; }

//getter solamente de los codigos de los productos asociados para poder realizar la persistencia de la informacion
vector<int> Proveedor::getCodigosProductos() const {
    vector<int> codigos;
    for (auto* p : productosAsociados) {
        if (p) codigos.push_back(p->getCodigo());
    }
    return codigos;
}

// Setters
void Proveedor::setId(int nuevoId) { id = nuevoId; }
void Proveedor::setNombre(const string& nuevoNombre) { nombre = nuevoNombre; }
void Proveedor::setContacto(const string& nuevoContacto) { contacto = nuevoContacto; }

// Asociacion de productos
void Proveedor::asociarProducto(Producto* producto) {
    if (producto) {
        productosAsociados.push_back(producto); //se agrega el puntero al producto al final del vector
    }
}

// desasociar el producto del proveedor, no se elimina el producto
void Proveedor::desasociarProducto(int codigoProducto) {
    for (auto it = productosAsociados.begin(); it != productosAsociados.end(); ++it) {
        if ((*it)->getCodigo() == codigoProducto) {
            productosAsociados.erase(it);
            return;
        }
    }
}

void Proveedor::mostrarInfo() const {
    cout << "Proveedor [" << id << "]\n";
    cout << "  Nombre: " << nombre << "\n";
    cout << "  Contacto: " << contacto << "\n";
    cout << "  Productos asociados: " << productosAsociados.size() << "\n";

    if (!productosAsociados.empty()) {
        cout << "    Lista de productos:\n";
        for (auto p : productosAsociados) {
            cout << "      - " << p->getNombre()
                 << " (Codigo: " << p->getCodigo() << ")\n";
        }
    }
}
