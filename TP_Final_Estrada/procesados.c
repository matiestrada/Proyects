#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include "declaraciones.h"

//Funcion para normalizar datos originales del archivo .wav
void normalizar_datos(fwav* arch_wav)
{
    //Asigno memoria para almacenar los datos normalizados.
    arch_wav->datos_normalizados = (double*)malloc(sizeof(double) * arch_wav->cant_muestras);
    if(arch_wav->datos_normalizados == NULL)
    {
        printf("Error al asignar memoria. (arch_wav->datos_normalizados)");
        exit(1);
    }

    //Declaro valor maximo posible para 2 bytes.
    double valor_max = 0x7FFF;

    //Normalizo los datos dividiendo el valor original por el valor max posible para que quede entre [-1,1]
    for(int i = 0; i < arch_wav->cant_muestras; i++)
    {
        arch_wav->datos_normalizados[i] = (double)arch_wav->datos[i]/valor_max;
    }
}

//Funcion para procesar los datos normalizados y obtener los datos filtrados.
void procesar_datos(fwav* arch_wav, fcoef* coefs)
{
    // Asigno memoria para los datos filtrados
    arch_wav->datos_filtrados = (double*)malloc(sizeof(double) * arch_wav->cant_muestras);
    if(arch_wav->datos_filtrados== NULL)
    {
        printf("Error al asignar memoria. (arch_wav->datos_filtrados)");
        exit(1);
    }

    //Inicializo los datos filtrados a 0 (zero padding).
    for(int n = 0; n < arch_wav->cant_muestras; n++)
    {
        arch_wav->datos_filtrados[n] = 0.0;
    }

    //Declaro variables temporales para almacenar las sumatorias.
    double suma_b, suma_a;

    // Aplico el filtro a cada muestra.
    int tam_filtro = coefs->total / 2;
    for(int n = 0; n < arch_wav->cant_muestras; n++)
    {
        suma_b = 0.0;
        suma_a = 0.0;

        //Calculo la primera suma (b).
        for(int i = 0; i <= n && i < tam_filtro; i++)
        {
            suma_b += arch_wav->datos_normalizados[n - i] * coefs->b[i];
        }
        // Calculo la segunda suma (a).
        for(int i = 1; i <= n && i < tam_filtro; i++)
        {
            suma_a += arch_wav->datos_filtrados[n - i] * coefs->a[i];
        }
        // Aplico la ecuación dada: y[n] = ((b suma) - (a suma)) / a[0].
        arch_wav->datos_filtrados[n] = (suma_b - suma_a) / coefs->a[0];
    }
}

//Funcion para desnormalizar los datos ya filtrados
void desnormalizar_datos(fwav* arch_wav)
{
    //Asigno memoria para almacenar los datos desnormalizados.
    arch_wav->datos_desnormalizados = (int16_t*)malloc(sizeof(int16_t) * arch_wav->cant_muestras);
    if(arch_wav->datos_desnormalizados == NULL)
    {
        printf("Error al asignar memoria. (arch_wav->datos_desnormalizados)");
        exit(1);
    }

    //Declaro valor maximo posible para 2 bytes.
    double valor_max = 0x7FFF;

    //Desnormalizo los datos multiplicando por el valor maximo posible.
    for(int i = 0; i < arch_wav->cant_muestras; i++)
    {
        arch_wav->datos_desnormalizados[i] = (int16_t)(arch_wav->datos_filtrados[i]*valor_max);
    }
}
