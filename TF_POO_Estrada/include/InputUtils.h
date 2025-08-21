#ifndef INPUTUTILS_H_INCLUDED
#define INPUTUTILS_H_INCLUDED

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>

namespace InputUtils {

    inline void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }

    inline void pausar() {
        std::cout << "\nPresione ENTER para continuar...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    inline int leerEntero(const std::string& prompt, bool permitirCeroNeg = true) {
        int valor;
        while (true) {
            std::cout << prompt;
            if (std::cin >> valor) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (!permitirCeroNeg && valor <= 0) {
                    std::cout << "El valor debe ser positivo.\n";
                } else {
                    return valor;
                }
            } else {
                std::cout << "Entrada invalida. Intente de nuevo.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    inline double leerDouble(const std::string& prompt) {
        double valor;
        while (true) {
            std::cout << prompt;
            if (std::cin >> valor) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return valor;
            } else {
                std::cout << "Entrada invalida. Intente de nuevo.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    inline std::string leerLinea(const std::string& prompt) {
        std::string linea;
        std::cout << prompt;
        std::getline(std::cin, linea);
        return linea;
    }

    inline bool leerBoolSN(const std::string& prompt) {
        while (true) {
            std::string input;
            std::cout << prompt << " (S/N): ";
            std::getline(std::cin, input);
            if (input == "S" || input == "s" || input == "1") return true;
            if (input == "N" || input == "n" || input == "0") return false;
            std::cout << "Entrada invalida. Responda con S o N.\n";
        }
    }

}


#endif // INPUTUTILS_H_INCLUDED
