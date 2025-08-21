#include "GestorProveedores.h"
#include <iostream>

// el constructor recibe como parametro la estrategia creada en el main
GestorProveedores::GestorProveedores(IProveedoresStore* s) : store(s) {}

// se eliminan los proveedores y se limpia el vector
GestorProveedores::~GestorProveedores() {
    for (auto p : proveedores) delete p;
    proveedores.clear();
}

void GestorProveedores::agregarProveedor(Proveedor* p) {
    if (p) proveedores.push_back(p);
}

// el metodo contempla la idea de que no se puede eliminar un proveedor si tiene productos asociados
// ya que cada producto esta asociado si o si a un proveedor, no pueden quedar libres.
void GestorProveedores::eliminarProveedor(int id) {
    for (auto it = proveedores.begin(); it != proveedores.end(); ++it) {
        if ((*it)->getId() == id) {
            // Verificar si tiene productos asociados
            if (!(*it)->getCodigosProductos().empty()) {
                std::cout << "No se puede eliminar el proveedor porque tiene productos asociados." << std::endl;
                return;
            }

            // Si no tiene productos asociados, se elimina
            delete *it;
            proveedores.erase(it);
            std::cout << "Proveedor eliminado correctamente." << std::endl;
            return;
        }
    }

    std::cout << "Proveedor con el ID especificado no encontrado." << std::endl;
}



Proveedor* GestorProveedores::buscarProveedor(int id) const {
    for (auto p : proveedores) {
        if (p->getId() == id) return p;
    }
    return nullptr;
}

const vector<Proveedor*>& GestorProveedores::getProveedores() const {
    return proveedores;
}

// metodo que gestiona la asociacion de productos
// ya que cada producto debe estar asociado a un unico proveedor
//lo verifica antes de llamar al metodo de la clase Proveedor para asociar dicho producto
bool GestorProveedores::asociarProductoAProveedor(int idProveedor, Producto* producto) {
    // Verificar si ya está en otro proveedor
    for (auto& prov : proveedores) {
        for (auto* p : prov->getProductosAsociados()) {
            if (p == producto) {
                cout << "El producto ya pertenece al proveedor: " << prov->getNombre() << endl;
                return false;
            }
        }
    }
    // Si no está, lo asociamos
    Proveedor* proveedor = buscarProveedor(idProveedor);
    if (proveedor) {
        proveedor->asociarProducto(producto);
        return true;
    }
    return false;
}


void GestorProveedores::desasociarProductoDeProveedor(int codigoProducto) {
    for (auto& proveedor : proveedores) {
        proveedor->desasociarProducto(codigoProducto);
    }
}

// metodo para setear una nueva estrategia en caso de querer modificarla durante la ejecucion.
void GestorProveedores::setEstrategia(IProveedoresStore* str) { store = str; }

// metodos para realizar la persistencia de los proveedores con el patron strategy
void GestorProveedores::guardar() {
    if(store) store->guardar(proveedores);
}

// recibe referencia al inventario como parametro
//para poder asociar los productos, cargados previamente, a cada proveedor segun los codigos en el DTO que devuelve la estrategia concreta.
void GestorProveedores::cargar(Inventario& inventario) {
    proveedores.clear(); // limpiamos antes de cargar

    vector<ProveedorDTO> dtos = store->cargar();

    for (const auto& dto : dtos) {
        Proveedor* p = new Proveedor(dto.id, dto.nombre, dto.contacto);

        // Resolver punteros a productos
        for (int cod : dto.codigosProductos) {
            Producto* prod = inventario.buscarProducto(cod);
            if (prod) {
                p->asociarProducto(prod);
            }
        }
        proveedores.push_back(p);
    }
}

void GestorProveedores::mostrarProveedores() const {
    for (auto p : proveedores) p->mostrarInfo();
}
