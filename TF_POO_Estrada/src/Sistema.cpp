#include "Sistema.h"
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

// ======================
// Constructor
// ======================
Sistema::Sistema(){
    try {
        cargarDatos();
    } catch (const exception& e) {
        cerr << "[ERROR] Al cargar datos: " << e.what() << "\n";
    }
}

// ======================
// Utilidades de UI/Input
// ======================
void Sistema::limpiarPantalla() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Sistema::pausar() const {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int Sistema::leerEntero(const string& prompt, bool permitirCeroNeg) const {
    while (true) {
        cout << prompt;
        string s; getline(cin, s);
        stringstream ss(s);
        int v; char c;
        if ((ss >> v) && !(ss >> c)) {
            if (!permitirCeroNeg && v <= 0) {
                cout << " Debe ser un entero positivo.\n";
                continue;
            }
            return v;
        }
        cout << " Entrada invalida. Intente nuevamente.\n";
    }
}

double Sistema::leerDouble(const string& prompt) const {
    while (true) {
        cout << prompt;
        string s; getline(cin, s);
        stringstream ss(s);
        double v; char c;
        if ((ss >> v) && !(ss >> c)) return v;
        cout << " Numero invalido. Intente nuevamente.\n";
    }
}

string Sistema::leerLinea(const string& prompt) const {
    cout << prompt;
    string line; getline(cin, line);
    return line;
}

bool Sistema::leerBoolSN(const string& prompt) const {
    while (true) {
        cout << prompt << " (S/N): ";
        string s; getline(cin, s);
        if (s.empty()) continue;
        char ch = static_cast<char>(tolower(s[0]));
        if (ch=='s' || s=="1") return true;
        if (ch=='n' || s=="0") return false;
        cout << " Responda 'S' o 'N'.\n";
    }
}

// ======================
// Persistencia
// ======================
void Sistema::cargarDatos() {
    // Productos
    repo.setEstrategia(new PersistenciaProductos(inventario));
    repo.cargar();

    // Proveedores (asocia productos por código ya cargados)
    repo.setEstrategia(new PersistenciaProveedores(gestorProveedores, inventario));
    repo.cargar();

    // Remitos
    repo.setEstrategia(new PersistenciaRemitos(gestorTransacciones));
    repo.cargar();
}

void Sistema::guardarDatos() {
    // Guardamos todo
    repo.setEstrategia(new PersistenciaProductos(inventario));
    repo.guardar();

    repo.setEstrategia(new PersistenciaProveedores(gestorProveedores, inventario));
    repo.guardar();

    repo.setEstrategia(new PersistenciaRemitos(gestorTransacciones));
    repo.guardar();
}

// ======================
// Menú principal
// ======================
void Sistema::alertaStockBajo() const {
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

void Sistema::menuPrincipal() {
    while (true) {
        limpiarPantalla();
        alertaStockBajo();

        cout << "=========== MENU PRINCIPAL ============\n";
        cout << "1) Gestionar Productos\n";
        cout << "2) Gestionar Proveedores\n";
        cout << "3) Gestionar Transacciones\n";
        cout << "4) Salir\n";
        cout << "======================================\n";

        int op = leerEntero("Elija una opcion: ");
        switch (op) {
            case 1: menuProductos(); break;
            case 2: menuProveedores(); break;
            case 3: menuTransacciones(); break;
            case 4:
                cout << "Guardando informacion...\n";
                try { guardarDatos(); } catch (...) { /* ya mostramos en persistencias si falla */ }
                cout << "Hasta luego!\n";
                return;
            default:
                cout << " Opcion invalida.\n"; pausar();
        }
    }
}

// ======================
// Submenú Productos
// ======================
int Sistema::elegirProveedorId() const {
    cout << "\n=== Proveedores disponibles ===\n";
    gestorProveedores.mostrarProveedores();
    int id = leerEntero("Ingrese ID de proveedor: ");
    if (!gestorProveedores.buscarProveedor(id)) {
        cout << " No existe un proveedor con ID " << id << ".\n";
        return -1;
    }
    return id;
}

int Sistema::elegirTipoProducto() const {
    cout << "\nTipos de producto:\n";
    cout << "1) Alimenticio\n";
    cout << "2) Electronico\n";
    cout << "3) Limpieza\n";
    while (true) {
        int t = leerEntero("Elija tipo (1-3): ");
        if (t>=1 && t<=3) return t;
        cout << " Opcion invalida (1-3).\n";
    }
}

void Sistema::opAgregarProducto() {
    limpiarPantalla();
    cout << "=== Agregar Producto ===\n";
    int idProv = elegirProveedorId();
    if (idProv < 0) { pausar(); return; }

    int tipo = elegirTipoProducto();

    int codigo = leerEntero("Codigo (entero, unico): ", false);
    if (inventario.buscarProducto(codigo)) {
        cout << " Ya existe un producto con ese codigo.\n"; pausar(); return;
    }
    string nombre = leerLinea("Nombre: ");
    string marca  = leerLinea("Marca: ");
    double precio = leerDouble("Precio: ");
    int stock     = leerEntero("Stock: ");
    int umbral    = leerEntero("Umbral de stock bajo: ");

    Producto* nuevo = nullptr;

    try {
        if (tipo == 1) {
            string fv = leerLinea("Fecha de vencimiento (AAAA-MM-DD): ");
            bool fresco = leerBoolSN("¿Es fresco?");
            nuevo = new ProductoAlimenticio(codigo, nombre, marca, precio, stock, umbral, fv, fresco);
        } else if (tipo == 2) {
            int garantia = leerEntero("Garantia (meses): ");
            int voltaje  = leerEntero("Voltaje: ");
            nuevo = new ProductoElectronico(codigo, nombre, marca, precio, stock, umbral, garantia, voltaje);
        } else {
            string fv = leerLinea("Fecha de vencimiento (AAAA-MM-DD): ");
            string sup = leerLinea("Superficie de uso: ");
            bool tox = leerBoolSN("¿Es toxico?");
            nuevo = new ProductoLimpieza(codigo, nombre, marca, precio, stock, umbral, fv, sup, tox);
        }
        inventario.agregarProducto(nuevo);
        if (!gestorProveedores.asociarProductoAProveedor(idProv, nuevo)) {
            cout << " No se pudo asociar el producto al proveedor.\n";
        }
        cout << " Producto agregado exitosamente.\n";
        guardarDatos();
    } catch (const exception& e) {
        delete nuevo;
        cout << "[ERROR] No se pudo crear/agregar el producto: " << e.what() << "\n";
    }
    pausar();
}

void Sistema::editarAtributosProducto(Producto* p) {
    while (true) {
        limpiarPantalla();
        cout << "=== Editar Producto (cod " << p->getCodigo() << ") ===\n";
        cout << "1) Nombre (actual: " << p->getNombre() << ")\n";
        cout << "2) Marca  (actual: " << p->getMarca()  << ")\n";
        cout << "3) Precio (actual: " << p->getPrecio() << ")\n";
        cout << "4) Stock  (actual: " << p->getStock()  << ")\n";
        cout << "5) Umbral stock bajo (actual: " << p->getUmbralStockBajo() << ")\n";

        // Campos específicos por tipo
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

        int op = leerEntero("Elija opcion: ");
        if (op == salirOpt) return;

        try {
            switch (op) {
                case 1: p->setNombre(leerLinea("Nuevo nombre: ")); break;
                case 2: p->setMarca(leerLinea("Nueva marca: ")); break;
                case 3: p->setPrecio(leerDouble("Nuevo precio: ")); break;
                case 4: p->setStock(leerEntero("Nuevo stock: ")); break;
                case 5: p->setUmbralStockBajo(leerEntero("Nuevo umbral: ")); break;
                default:
                    if (pa) {
                        if (op == 6) pa->setFechaVencimiento(leerLinea("Nueva fecha venc.: "));
                        else if (op == 7) pa->setEsFresco(leerBoolSN("¿Es fresco?"));
                        else throw invalid_argument("Opcion invalida.");
                    } else if (pe) {
                        if (op == 6) pe->setGarantiaMeses(leerEntero("Nueva garantia (meses): "));
                        else if (op == 7) pe->setVoltaje(leerEntero("Nuevo voltaje: "));
                        else throw invalid_argument("Opcion invalida.");
                    } else if (pl) {
                        if (op == 6) pl->setFechaVencimiento(leerLinea("Nueva fecha venc.: "));
                        else if (op == 7) pl->setSuperficieUso(leerLinea("Nueva superficie: "));
                        else if (op == 8) pl->setEsToxico(leerBoolSN("¿Es toxico?"));
                        else throw invalid_argument("Opcion invalida.");
                    } else {
                        throw invalid_argument("Opcion invalida.");
                    }
            }
            cout << " Atributo actualizado.\n";
            guardarDatos();
        } catch (const exception& e) {
            cout << "[ERROR] " << e.what() << "\n";
        }
        pausar();
    }
}

void Sistema::opModificarProducto() {
    limpiarPantalla();
    cout << "=== Modificar Producto ===\n";
    int codigo = leerEntero("Codigo del producto a modificar: ", false);
    Producto* p = inventario.buscarProducto(codigo);
    if (!p) {
        cout << " No existe un producto con ese codigo.\n"; pausar(); return;
    }
    editarAtributosProducto(p);
}

void Sistema::opEliminarProducto() {
    limpiarPantalla();
    cout << "=== Eliminar Producto ===\n";
    int codigo = leerEntero("Codigo del producto a eliminar: ", false);
    if (!inventario.buscarProducto(codigo)) {
        cout << " No existe un producto con ese codigo.\n"; pausar(); return;
    }
    if (!leerBoolSN("¿Confirma eliminacion?")) { cout << "Operacion cancelada.\n"; pausar(); return; }

    try {
        if (inventario.eliminarProducto(codigo, gestorProveedores)) {
            cout << " Producto eliminado correctamente.\n";
            guardarDatos();
        } else {
            cout << " No se pudo eliminar el producto.\n";
        }
    } catch (const exception& e) {
        cout << "[ERROR] " << e.what() << "\n";
    }
    pausar();
}

void Sistema::opMostrarProductos() const {
    limpiarPantalla();
    cout << "=== Lista de Productos ===\n";
    inventario.mostrarProductos();
    cout << "\n";
    // no pausa aquí; la hace el menú
}

void Sistema::menuProductos() {
    while (true) {
        limpiarPantalla();
        cout << "====== Gestionar Productos ======\n";
        cout << "1) Agregar producto\n";
        cout << "2) Modificar producto\n";
        cout << "3) Eliminar producto\n";
        cout << "4) Mostrar lista de productos\n";
        cout << "5) Volver\n";
        cout << "=================================\n";
        int op = leerEntero("Opcion: ");
        switch (op) {
            case 1: opAgregarProducto(); break;
            case 2: opModificarProducto(); break;
            case 3: opEliminarProducto(); break;
            case 4: opMostrarProductos(); pausar(); break;
            case 5: return;
            default: cout << " Opcion invalida.\n"; pausar();
        }
    }
}

// ======================
// Submenú Proveedores
// ======================
void Sistema::opAgregarProveedor() {
    limpiarPantalla();
    cout << "=== Agregar Proveedor ===\n";
    int id = leerEntero("ID (entero, unico): ", false);
    if (gestorProveedores.buscarProveedor(id)) {
        cout << " Ya existe proveedor con ese ID.\n"; pausar(); return;
    }
    string nombre = leerLinea("Nombre: ");
    string contacto = leerLinea("Contacto: ");

    try {
        Proveedor* p = new Proveedor(id, nombre, contacto);
        gestorProveedores.agregarProveedor(p);
        cout << " Proveedor agregado.\n";
        guardarDatos();
    } catch (const exception& e) {
        cout << "[ERROR] No se pudo crear/agregar proveedor: " << e.what() << "\n";
    }
    pausar();
}

void Sistema::editarAtributosProveedor(Proveedor* prov) {
    while (true) {
        limpiarPantalla();
        cout << "=== Editar Proveedor (ID " << prov->getId() << ") ===\n";
        cout << "1) Nombre (actual: " << prov->getNombre() << ")\n";
        cout << "2) Contacto (actual: " << prov->getContacto() << ")\n";
        cout << "3) Volver\n";
        int op = leerEntero("Opcion: ");
        if (op == 3) return;

        try {
            switch (op) {
                case 1: prov->setNombre(leerLinea("Nuevo nombre: ")); break;
                case 2: prov->setContacto(leerLinea("Nuevo contacto: ")); break;
                default: cout << " Opción invalida.\n"; pausar(); continue;
            }
            cout << " Atributo actualizado.\n";
            guardarDatos();
        } catch (const exception& e) {
            cout << "[ERROR] " << e.what() << "\n";
        }
        pausar();
    }
}

void Sistema::opModificarProveedor() {
    limpiarPantalla();
    cout << "=== Modificar Proveedor ===\n";
    int id = leerEntero("ID del proveedor: ", false);
    Proveedor* pr = gestorProveedores.buscarProveedor(id);
    if (!pr) { cout << " No existe proveedor con ese ID.\n"; pausar(); return; }
    editarAtributosProveedor(pr);
}

void Sistema::opEliminarProveedor() {
    limpiarPantalla();
    cout << "=== Eliminar Proveedor ===\n";
    int id = leerEntero("ID del proveedor: ", false);
    Proveedor* pr = gestorProveedores.buscarProveedor(id);
    if (!pr) { cout << " No existe proveedor con ese ID.\n"; pausar(); return; }
    if (!pr->getProductosAsociados().empty()) {
        cout << " No se puede eliminar: tiene productos asociados.\n"; pausar(); return;
    }
    if (!leerBoolSN("¿Confirma eliminacion?")) { cout << "Operacion cancelada.\n"; pausar(); return; }

    try {
        gestorProveedores.eliminarProveedor(id);
        cout << " Proveedor eliminado.\n";
        guardarDatos();
    } catch (const exception& e) {
        cout << "[ERROR] " << e.what() << "\n";
    }
    pausar();
}

void Sistema::opMostrarProveedores() const {
    limpiarPantalla();
    cout << "=== Lista de Proveedores ===\n";
    gestorProveedores.mostrarProveedores();
    cout << "\n";
}

void Sistema::menuProveedores() {
    while (true) {
        limpiarPantalla();
        cout << "====== Gestionar Proveedores ======\n";
        cout << "1) Agregar proveedor\n";
        cout << "2) Modificar proveedor\n";
        cout << "3) Eliminar proveedor\n";
        cout << "4) Mostrar lista de proveedores\n";
        cout << "5) Volver\n";
        cout << "===================================\n";
        int op = leerEntero("Opcion: ");
        switch (op) {
            case 1: opAgregarProveedor(); break;
            case 2: opModificarProveedor(); break;
            case 3: opEliminarProveedor(); break;
            case 4: opMostrarProveedores(); pausar(); break;
            case 5: return;
            default: cout << " Opcion invalida.\n"; pausar();
        }
    }
}

// ======================
// Submenú Transacciones
// ======================
static bool validarStockSalida(const Inventario& inv, int codigo, int cantidad) {
    Producto* p = inv.buscarProducto(codigo);
    if (!p) return false;
    return p->getStock() >= cantidad;
}

void Sistema::opRegistrarRemitoEntrada() {
    limpiarPantalla();
    cout << "=== Registrar Remito de ENTRADA ===\n";
    int id = leerEntero("ID de remito (unico): ", false);
    if (gestorTransacciones.buscarRemito(id)) {
        cout << " Ya existe un remito con ese ID.\n"; pausar(); return;
    }
    int d = leerEntero("Dia: ", false);
    int m = leerEntero("Mes: ", false);
    int y = leerEntero("Año: ", false);

    Remito* r = nullptr;
    try {
        r = new RemitoEntrada(id, Fecha(d,m,y));
    } catch (...) {
        cout << "[ERROR] No se pudo crear el remito.\n"; pausar(); return;
    }

    while (true) {
        cout << "\n1) Agregar detalle\n2) Finalizar remito\n";
        int op = leerEntero("Opción: ");
        if (op == 2) break;
        if (op != 1) { cout << " Opcion invalida.\n"; continue; }

        int cod = leerEntero("Codigo de producto: ", false);
        if (!inventario.buscarProducto(cod)) { cout << " Producto inexistente.\n"; continue; }
        int cant = leerEntero("Cantidad (>=1): ", false);
        if (cant <= 0) { cout << " Cantidad invalida.\n"; continue; }

        r->agregarProducto(cod, cant);
        cout << "Detalle agregado.\n";
    }

    try {
        gestorTransacciones.registrarRemito(r, inventario); // actualiza stock
        cout << " Remito de entrada registrado.\n";
        guardarDatos();
    } catch (const exception& e) {
        cout << "[ERROR] " << e.what() << "\n";
        delete r;
    }
    pausar();
}

void Sistema::opRegistrarRemitoSalida() {
    limpiarPantalla();
    cout << "=== Registrar Remito de SALIDA ===\n";
    int id = leerEntero("ID de remito (unico): ", false);
    if (gestorTransacciones.buscarRemito(id)) {
        cout << " Ya existe un remito con ese ID.\n"; pausar(); return;
    }
    int d = leerEntero("Dia: ", false);
    int m = leerEntero("Mes: ", false);
    int y = leerEntero("Año: ", false);

    Remito* r = nullptr;
    try {
        r = new RemitoSalida(id, Fecha(d,m,y));
    } catch (...) {
        cout << "[ERROR] No se pudo crear el remito.\n"; pausar(); return;
    }

    while (true) {
        cout << "\n1) Agregar detalle\n2) Finalizar remito\n";
        int op = leerEntero("Opcion: ");
        if (op == 2) break;
        if (op != 1) { cout << " Opcion invalida.\n"; continue; }

        int cod = leerEntero("Codigo de producto: ", false);
        Producto* p = inventario.buscarProducto(cod);
        if (!p) { cout << " Producto inexistente.\n"; continue; }
        int cant = leerEntero("Cantidad (>=1): ", false);
        if (cant <= 0) { cout << " Cantidad invalida.\n"; continue; }
        if (!validarStockSalida(inventario, cod, cant)) {
            cout << " Stock insuficiente para el producto '" << p->getNombre()
                 << "'. Stock actual: " << p->getStock() << "\n";
            continue;
        }

        r->agregarProducto(cod, cant);
        cout << "Detalle agregado.\n";
    }

    try {
        gestorTransacciones.registrarRemito(r, inventario); // actualiza stock
        cout << " Remito de salida registrado.\n";
        guardarDatos();
    } catch (const exception& e) {
        cout << "[ERROR] " << e.what() << "\n";
        delete r;
    }
    pausar();
}

void Sistema::opMostrarRemitos() const {
    limpiarPantalla();
    cout << "=== Lista de Remitos ===\n";
    gestorTransacciones.mostrarRemitos();
    cout << "\n";
}

void Sistema::menuTransacciones() {
    while (true) {
        limpiarPantalla();
        cout << "====== Gestionar Transacciones ======\n";
        cout << "1) Registrar Remito de Entrada\n";
        cout << "2) Registrar Remito de Salida\n";
        cout << "3) Mostrar lista de Remitos\n";
        cout << "4) Volver\n";
        cout << "=====================================\n";
        int op = leerEntero("Opcion: ");
        switch (op) {
            case 1: opRegistrarRemitoEntrada(); break;
            case 2: opRegistrarRemitoSalida(); break;
            case 3: opMostrarRemitos(); pausar(); break;
            case 4: return;
            default: cout << " Opcion invalida.\n"; pausar();
        }
    }
}

// ======================
// Ejecución
// ======================
void Sistema::ejecutar() {
    menuPrincipal();
}
