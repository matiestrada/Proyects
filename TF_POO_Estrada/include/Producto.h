#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <iostream>
#include <string>

using namespace std;

// ==================================
// Declaracion Producto
// ==================================
class Producto{
protected:
    int codigo;
    string nombre;
    string marca;
    double precio;
    int stock;
    int umbralStockBajo; //umbral para emitir las alertas en caso de que el stock sea bajo

public:
    //Constructor y destructor
    Producto(int codigo, const string& nombre, const string& marca, double precio, int stock, int umbralStockBajo);
    virtual ~Producto();

    //Getters
    int getCodigo() const;
    string getNombre() const;
    string getMarca() const;
    double getPrecio() const;
    int getStock() const;
    int getUmbralStockBajo() const;

    //Setters
    void setCodigo(int nuevoCodigo);
    void setNombre(const string& nuevoNombre);
    void setMarca(const string& nuevaMarca);
    void setPrecio(double nuevoPrecio);
    void setStock(int nuevoStock);
    void setUmbralStockBajo(int nuevoUmbralStockBajo);

    virtual void mostrarInfo() const = 0;
};



// ==================================
// Declaracion ProductoAlimenticio
// ==================================
class ProductoAlimenticio : public Producto{
private:
    string fechaVencimiento;
    bool esFresco; //true = SI , false = NO

public:
    //Constructor y destructor
    ProductoAlimenticio(int codigo, const string& nombre, const string& marca, double precio, int stock, int umbralStockBajo,
                        const string& fechaVencimiento, bool esFresco);
    virtual ~ProductoAlimenticio();

    //Getters
    string getFechaVencimiento() const;
    bool getEsFresco() const;

    //Setters
    void setFechaVencimiento(const string& nuevaFechaVencimiento);
    void setEsFresco(bool nuevoEsFresco);

    virtual void mostrarInfo() const override;
};



// ==================================
// Declaracion ProductoElectronico
// ==================================
class ProductoElectronico : public Producto{
private:
    int garantiaMeses;
    int voltaje;

public:
    //Constructor y destructor
    ProductoElectronico(int codigo, const string& nombre, const string& marca, double precio, int stock, int umbralStockBajo,
                        int garantiaMeses, int voltaje);
    virtual ~ProductoElectronico();

    //Getters
    int getGarantiaMeses() const;
    int getVoltaje() const;

    //Setters
    void setGarantiaMeses(int nuevaGarantiaMeses);
    void setVoltaje(int nuevoVoltaje);

    virtual void mostrarInfo() const override;
};



// ==================================
// Declaracion ProductoLimpieza
// ==================================
class ProductoLimpieza : public Producto{
private:
    string fechaVencimiento;
    string superficieUso; //Cocina, Piso, Cuero, Madera, Ropa, Ambiente...
    bool esToxico; //true = SI , false = NO

public:
    //Constructor y destructor
    ProductoLimpieza(int codigo, const string& nombre, const string& marca, double precio, int stock, int umbralStockBajo,
                        const string& fechaVencimiento, const string& superficieUso, bool esToxico);
    virtual ~ProductoLimpieza();

    //Getters
    string getFechaVencimiento() const;
    string getSuperficieUso() const;
    bool getEsToxico() const;

    //Setters
    void setFechaVencimiento(const string& nuevaFechaVencimiento);
    void setSuperficieUso(const string& nuevaSuperficieUso);
    void setEsToxico(bool nuevoEsFresco);

    virtual void mostrarInfo() const override;
};

#endif // PRODUCTO_H
