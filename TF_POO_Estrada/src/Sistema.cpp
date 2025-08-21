#include "Sistema.h"
#include "InputUtils.h"
#include <limits>
#include <sstream>
#include <algorithm>

using namespace std;

Sistema::Sistema(Inventario& inv, GestorProveedores& gp, GestorTransacciones& gt)
    : inventario(inv), gestorProveedores(gp), gestorTransacciones(gt) {}

/* ===================== Persistencia ===================== */
void Sistema::cargarTodos() {
    // Orden: Productos -> Proveedores -> Remitos
    inventario.cargar();
    gestorProveedores.cargar(inventario);
    gestorTransacciones.cargar();

    // tras cargar, recalculamos contadores
    recalcularContadores();
}

void Sistema::guardarTodos() {
    // Orden: Productos -> Proveedores -> Remitos
    inventario.guardar();
    gestorProveedores.guardar();
    gestorTransacciones.guardar();
}

/* =========== Recalcular autoincrementales =========== */
void Sistema::recalcularContadores() {
    // Productos
    int maxCodigo = 0;
    for (auto* p : inventario.getProductos())
        maxCodigo = std::max(maxCodigo, p->getCodigo());
    ultimoCodigoProducto = maxCodigo;

    // Proveedores
    int maxProv = 0;
    for (auto* pr : gestorProveedores.getProveedores())
        maxProv = std::max(maxProv, pr->getId());
    ultimoIdProveedor = maxProv;

    // Remitos
    int maxRem = 0;
    for (auto* r : gestorTransacciones.getRemitos())
        maxRem = std::max(maxRem, r->getId());
    ultimoIdRemito = maxRem;
}

/* ===================== Alerta stock bajo ===================== */
void Sistema::alertaProductosStockBajo() const {
    auto bajos = inventario.getProductosStockBajo();
    cout << "========== ALERTA: Stock Bajo ==========\n";
    if (bajos.empty()) {
        cout << "No hay productos con stock bajo.\n";
    } else {
        for (auto* p : bajos) {
            cout << "Codigo: " << p->getCodigo()
                 << " | Nombre: " << p->getNombre()
                 << " | Stock: " << p->getStock() << "\n";
        }
    }
    cout << "========================================\n\n";
}

/* ===================== Productos ===================== */
void Sistema::operacionAgregarProducto() {
    InputUtils::limpiarPantalla();
    cout << "=== Agregar Producto ===\n";

    //Se muestra lista de proveedores para que el usuario elija a cual pertenece.
    cout << "\n=== Proveedores disponibles ===\n";
    operacionMostrarProveedores();
    int idProv = InputUtils::leerEntero("Ingrese ID de proveedor: ");
    auto* prov = gestorProveedores.buscarProveedor(idProv);
    if (!prov) {
        cout << " No existe un proveedor con ID " << idProv << ".\n";
        InputUtils::pausar();
        return;
    }

    //Se muestra las opciones de tipos de productos disponibles para que el usuario elija.
    cout << "\nTipos de producto:\n";
    cout << "1) Alimenticio\n";
    cout << "2) Electronico\n";
    cout << "3) Limpieza\n";
    int tipo = -1;
    while (tipo<1 || tipo>3) {
        tipo = InputUtils::leerEntero("Elija tipo (1-3): ");
        if (tipo<1 || tipo>3)
        cout << " Opcion invalida (1-3).\n";
    }

    int codigo = ++ultimoCodigoProducto;
    cout << "(Codigo asignado automaticamente: " << codigo << ")\n";

    string nombre = InputUtils::leerLinea("Nombre: ");
    string marca  = InputUtils::leerLinea("Marca: ");
    double precio = InputUtils::leerDouble("Precio: ");
    int stock     = InputUtils::leerEntero("Stock: ");
    int umbral    = InputUtils::leerEntero("Umbral de stock bajo: ");

    Producto* nuevo = nullptr;
    try {
        if (tipo == 1) {
            string fv = InputUtils::leerLinea("Fecha de vencimiento (AAAA-MM-DD): ");
            bool fresco = InputUtils::leerBoolSN("¿Es fresco?");
            nuevo = new ProductoAlimenticio(codigo, nombre, marca, precio, stock, umbral, fv, fresco);
        } else if (tipo == 2) {
            int garantia = InputUtils::leerEntero("Garantia (meses): ");
            int voltaje  = InputUtils::leerEntero("Voltaje: ");
            nuevo = new ProductoElectronico(codigo, nombre, marca, precio, stock, umbral, garantia, voltaje);
        } else {
            string fv = InputUtils::leerLinea("Fecha de vencimiento (AAAA-MM-DD): ");
            string sup = InputUtils::leerLinea("Superficie de uso: ");
            bool tox = InputUtils::leerBoolSN("¿Es toxico?");
            nuevo = new ProductoLimpieza(codigo, nombre, marca, precio, stock, umbral, fv, sup, tox);
        }
        inventario.agregarProducto(nuevo);
        if (!gestorProveedores.asociarProductoAProveedor(idProv, nuevo)) {
            cout << " No se pudo asociar el producto al proveedor.\n";
        }
        cout << " Producto agregado exitosamente.\n";
        guardarTodos();
    } catch (const exception& e) {
        delete nuevo;
        cout << "[ERROR] No se pudo crear/agregar el producto: " << e.what() << "\n";
    }
    InputUtils::pausar();
}

void Sistema::operacionModificarProducto() {
    InputUtils::limpiarPantalla();
    cout << "=== Modificar Producto ===\n";
    int codigo = InputUtils::leerEntero("Codigo del producto a modificar: ", false);
    Producto* p = inventario.buscarProducto(codigo);
    if (!p) {
        cout << " No existe un producto con ese codigo.\n"; InputUtils::pausar(); return;
    }

        while (true) {
        InputUtils::limpiarPantalla();
        cout << "=== Editar Producto (cod " << p->getCodigo() << ") ===\n";
        cout << "1) Nombre (actual: " << p->getNombre() << ")\n";
        cout << "2) Marca  (actual: " << p->getMarca()  << ")\n";
        cout << "3) Precio (actual: " << p->getPrecio() << ")\n";
        cout << "4) Stock  (actual: " << p->getStock()  << ")\n";
        cout << "5) Umbral stock bajo (actual: " << p->getUmbralStockBajo() << ")\n";

        auto* pa = dynamic_cast<ProductoAlimenticio*>(p);
        auto* pe = dynamic_cast<ProductoElectronico*>(p);
        auto* pl = dynamic_cast<ProductoLimpieza*>(p);

        int base = 5;
        if (pa) {
            cout << ++base << ") Fecha venc. (actual: " << pa->getFechaVencimiento() << ")\n";
            cout << ++base << ") Es fresco (actual: " << (pa->getEsFresco() ? "Si" : "No") << ")\n";
        } else if (pe) {
            cout << ++base << ") Garantia meses (actual: " << pe->getGarantiaMeses() << ")\n";
            cout << ++base << ") Voltaje (actual: " << pe->getVoltaje() << ")\n";
        } else if (pl) {
            cout << ++base << ") Fecha venc. (actual: " << pl->getFechaVencimiento() << ")\n";
            cout << ++base << ") Superficie uso (actual: " << pl->getSuperficieUso() << ")\n";
            cout << ++base << ") Es toxico (actual: " << (pl->getEsToxico() ? "Si" : "No") << ")\n";
        }
        int salirOpt = ++base;
        cout << salirOpt << ") Volver\n";

        int op = InputUtils::leerEntero("Elija opcion: ");
        if (op == salirOpt) return;

        try {
            switch (op) {
                case 1: p->setNombre(InputUtils::leerLinea("Nuevo nombre: ")); break;
                case 2: p->setMarca(InputUtils::leerLinea("Nueva marca: ")); break;
                case 3: p->setPrecio(InputUtils::leerDouble("Nuevo precio: ")); break;
                case 4: p->setStock(InputUtils::leerEntero("Nuevo stock: ")); break;
                case 5: p->setUmbralStockBajo(InputUtils::leerEntero("Nuevo umbral: ")); break;
                default:
                    if (pa) {
                        if (op == 6) pa->setFechaVencimiento(InputUtils::leerLinea("Nueva fecha venc.: "));
                        else if (op == 7) pa->setEsFresco(InputUtils::leerBoolSN("¿Es fresco?"));
                        else throw invalid_argument("Opcion invalida.");
                    } else if (pe) {
                        if (op == 6) pe->setGarantiaMeses(InputUtils::leerEntero("Nueva garantia (meses): "));
                        else if (op == 7) pe->setVoltaje(InputUtils::leerEntero("Nuevo voltaje: "));
                        else throw invalid_argument("Opcion invalida.");
                    } else if (pl) {
                        if (op == 6) pl->setFechaVencimiento(InputUtils::leerLinea("Nueva fecha venc.: "));
                        else if (op == 7) pl->setSuperficieUso(InputUtils::leerLinea("Nueva superficie: "));
                        else if (op == 8) pl->setEsToxico(InputUtils::leerBoolSN("¿Es toxico?"));
                        else throw invalid_argument("Opcion invalida.");
                    } else {
                        throw invalid_argument("Opcion invalida.");
                    }
            }
            cout << " Atributo actualizado.\n";
            guardarTodos();
        } catch (const exception& e) {
            cout << "[ERROR] " << e.what() << "\n";
        }
       InputUtils::pausar();
    }
}

void Sistema::operacionEliminarProducto() {
    InputUtils::limpiarPantalla();
    cout << "=== Eliminar Producto ===\n";
    int codigo = InputUtils::leerEntero("Codigo del producto a eliminar: ", false);
    if (!inventario.buscarProducto(codigo)) {
        cout << " No existe un producto con ese codigo.\n"; InputUtils::pausar(); return;
    }
    if (!InputUtils::leerBoolSN("¿Confirma eliminacion?")) { cout << "Operacion cancelada.\n"; InputUtils::pausar(); return; }

    if (inventario.eliminarProducto(codigo, gestorProveedores)) {
        cout << " Producto eliminado correctamente.\n";
        guardarTodos();
    } else {
        cout << " No se pudo eliminar el producto.\n";
    }
    InputUtils::pausar();
}

void Sistema::operacionMostrarProductos() const {
    InputUtils::limpiarPantalla();
    cout << "=== Lista de Productos ===\n";
    inventario.mostrarProductos();
    cout << "\n";
}

/* ===================== Proveedores ===================== */
void Sistema::operacionAgregarProveedor() {
    InputUtils::limpiarPantalla();
    cout << "=== Agregar Proveedor ===\n";
    int id = ++ultimoIdProveedor;
    cout << "(ID asignado automaticamente: " << id << ")\n";
    string nombre = InputUtils::leerLinea("Nombre: ");
    string contacto = InputUtils::leerLinea("Contacto: ");

    try {
        Proveedor* p = new Proveedor(id, nombre, contacto);
        gestorProveedores.agregarProveedor(p);
        cout << " Proveedor agregado.\n";
        guardarTodos();
    } catch (const exception& e) {
        cout << "[ERROR] No se pudo crear/agregar proveedor: " << e.what() << "\n";
    }
    InputUtils::pausar();
}

void Sistema::operacionModificarProveedor() {
    InputUtils::limpiarPantalla();
    cout << "=== Modificar Proveedor ===\n";
    int id = InputUtils::leerEntero("ID del proveedor: ", false);
    Proveedor* prov = gestorProveedores.buscarProveedor(id);
    if (!prov) { cout << " No existe proveedor con ese ID.\n"; InputUtils::pausar(); return; }

        while (true) {
        InputUtils::limpiarPantalla();
        cout << "=== Editar Proveedor (ID " << prov->getId() << ") ===\n";
        cout << "1) Nombre (actual: " << prov->getNombre() << ")\n";
        cout << "2) Contacto (actual: " << prov->getContacto() << ")\n";
        cout << "3) Volver\n";
        int op = InputUtils::leerEntero("Opcion: ");
        if (op == 3) return;

        try {
            switch (op) {
                case 1: prov->setNombre(InputUtils::leerLinea("Nuevo nombre: ")); break;
                case 2: prov->setContacto(InputUtils::leerLinea("Nuevo contacto: ")); break;
                default: cout << " Opcion invalida.\n"; InputUtils::pausar(); continue;
            }
            cout << " Atributo actualizado.\n";
            guardarTodos();
        } catch (const exception& e) {
            cout << "[ERROR] " << e.what() << "\n";
        }
        InputUtils::pausar();
    }
}

void Sistema::operacionEliminarProveedor() {
    InputUtils::limpiarPantalla();
    cout << "=== Eliminar Proveedor ===\n";
    int id = InputUtils::leerEntero("ID del proveedor: ", false);
    Proveedor* pr = gestorProveedores.buscarProveedor(id);
    if (!pr) { cout << " No existe proveedor con ese ID.\n"; InputUtils::pausar(); return; }

    if (!pr->getProductosAsociados().empty()) {
        cout << " No se puede eliminar: tiene productos asociados.\n"; InputUtils::pausar(); return;
    }
    if (!InputUtils::leerBoolSN("¿Confirma eliminacion?")) { cout << "Operacion cancelada.\n"; InputUtils::pausar(); return; }

    gestorProveedores.eliminarProveedor(id);
    cout << " Proveedor eliminado.\n";
    guardarTodos();
    InputUtils::pausar();
}

void Sistema::operacionMostrarProveedores() const {
    cout << "=== Lista de Proveedores ===\n";
    gestorProveedores.mostrarProveedores();
    cout << "\n";
}

/* ===================== Transacciones ===================== */
static bool validarStockSalidaLocal(const Inventario& inv, int codigo, int cantidad) {
    Producto* p = inv.buscarProducto(codigo);
    if (!p) return false;
    return p->getStock() >= cantidad;
}

void Sistema::operacionRegistrarRemitoEntrada() {
    InputUtils::limpiarPantalla();
    cout << "=== Registrar Remito de ENTRADA ===\n";
    int id = ++ultimoIdRemito;
    cout << "(ID de remito asignado automaticamente: " << id << ")\n";
    int d = InputUtils::leerEntero("Dia: ", false);
    int m = InputUtils::leerEntero("Mes: ", false);
    int y = InputUtils::leerEntero("Año: ", false);

    Remito* r = nullptr;
    try {
        r = new RemitoEntrada(id, Fecha(d,m,y));
    } catch (...) {
        cout << "[ERROR] No se pudo crear el remito.\n"; InputUtils::pausar(); return;
    }

    cout << "\nProductos disponibles (codigo - nombre - stock):\n";
    inventario.mostrarProductos();

    while (true) {
        cout << "\n1) Agregar detalle\n2) Finalizar remito\n";
        int op = InputUtils::leerEntero("Opcion: ");
        if (op == 2) break;
        if (op != 1) { cout << " Opcion invalida.\n"; continue; }

        int cod = InputUtils::leerEntero("Codigo de producto: ", false);
        if (!inventario.buscarProducto(cod)) { cout << " Producto inexistente.\n"; continue; }
        int cant = InputUtils::leerEntero("Cantidad (>=1): ", false);
        if (cant <= 0) { cout << " Cantidad invalida.\n"; continue; }

        r->agregarProducto(cod, cant);
        cout << "Detalle agregado.\n";
    }

    try {
        gestorTransacciones.registrarRemito(r, inventario); // actualiza stock (suma)
        cout << " Remito de entrada registrado.\n";
        guardarTodos();
    } catch (const exception& e) {
        cout << "[ERROR] " << e.what() << "\n";
        delete r;
    }
    InputUtils::pausar();
}

void Sistema::operacionRegistrarRemitoSalida() {
    InputUtils::limpiarPantalla();
    cout << "=== Registrar Remito de SALIDA ===\n";
    int id = ++ultimoIdRemito;
    cout << "(ID de remito asignado automaticamente: " << id << ")\n";
    int d = InputUtils::leerEntero("Dia: ", false);
    int m = InputUtils::leerEntero("Mes: ", false);
    int y = InputUtils::leerEntero("Año: ", false);

    Remito* r = nullptr;
    try {
        r = new RemitoSalida(id, Fecha(d,m,y));
    } catch (...) {
        cout << "[ERROR] No se pudo crear el remito.\n"; InputUtils::pausar(); return;
    }

    cout << "\nProductos disponibles (codigo - nombre - stock):\n";
    inventario.mostrarProductos();

    while (true) {
        cout << "\n1) Agregar detalle\n2) Finalizar remito\n";
        int op = InputUtils::leerEntero("Opcion: ");
        if (op == 2) break;
        if (op != 1) { cout << " Opcion invalida.\n"; continue; }

        int cod = InputUtils::leerEntero("Codigo de producto: ", false);
        Producto* p = inventario.buscarProducto(cod);
        if (!p) { cout << " Producto inexistente.\n"; continue; }
        int cant = InputUtils::leerEntero("Cantidad (>=1): ", false);
        if (cant <= 0) { cout << " Cantidad invalida.\n"; continue; }
        if (!validarStockSalidaLocal(inventario, cod, cant)) {
            cout << " Stock insuficiente para '" << p->getNombre()
                 << "'. Stock actual: " << p->getStock() << "\n";
            continue;
        }

        r->agregarProducto(cod, cant);
        cout << "Detalle agregado.\n";
    }

    try {
        gestorTransacciones.registrarRemito(r, inventario); // actualiza stock (resta)
        cout << " Remito de salida registrado.\n";
        guardarTodos();
    } catch (const exception& e) {
        cout << "[ERROR] " << e.what() << "\n";
        delete r;
    }
    InputUtils::pausar();
}

void Sistema::operacionMostrarRemitos() const {
    InputUtils::limpiarPantalla();
    cout << "=== Lista de Remitos ===\n";
    gestorTransacciones.mostrarRemitos();
    cout << "\n";
}
