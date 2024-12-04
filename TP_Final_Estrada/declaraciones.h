#ifndef DECLARACIONES_H_INCLUDED
#define DECLARACIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>

/***Estructura para almacenar la informacion del archivo acorde.wav***/
typedef struct _fwav
{
    uint32_t tam_arch; //Longitud (en bytes) de los datos que siguen -> lontigud del archivo - 8.
    uint32_t tam_format; //Longitud del resto del grupo Format (siempre 0x10).
    uint16_t format_audio; //Siempre 01. Formato de audio.
    uint16_t num_canales; //Número de canales (01 -> Mono ó 02 -> Stereo).
    uint32_t frec_muestreo; //Frecuencia de muestreo (en Hz).
    uint32_t bytes_by_seg; //Bytes por segundo.
    uint16_t bytes_by_muestra; //Bytes por muestra (1 -> 8 bits Mono;  02 -> 8 bits Stereo/16 bits Mono; 4 -> 16 bits Stereo).
    uint16_t bits_by_muestra; //Bits por muestra.
    uint32_t tam_datos; //Longitud (en bytes) de los datos (total muestras * bytes por muestra).
    uint32_t cant_muestras; //Cantidad total de muestras -> tam_datos/bytes_by_muestra.
    float duracion; //duracion en segundos del audio -> cant_muestras/frec_muestreo.
    int16_t *datos; //array con todos los datos.
    double *datos_normalizados; //array con los datos normalizados.
    double *datos_filtrados; //array con los datos filtrados.
    int16_t *datos_desnormalizados; //array con los datos finales para crear el archivo filtrado.wav
}fwav;

/***Estructura para almacenar la informacion del archivo coeficientes.txt***/
typedef struct _fcoef
{
    double *a;
    double *b;
    int total; //Cantidad total de coeficientes en el archivo (a+b).
}fcoef;


/***DECLARACIONES DE FUNCIONES***/

/*Archivo validar_liberar.c*/
void validar_programa(int, const char*, const char*); //Funcion para validar parametros del main, verificar existencia de ambos archivos: fname1 = acorde.wav ; fname2 = coeficientes.txt
void free_wav(fwav*); //Funcion para liberar memoria de la estructura del archivo .wav
void free_coefs(fcoef*); //Funcion para liberar memoria de la estructura del archivo .txt
/*Archivo lecturas.c*/
fwav* leer_archivo_audio(const char*); //Funcion para verificar el formato del archivo .wav y a su vez leer sus datos y almacenarlos en el struct
fcoef* leer_archivo_coefs(const char*); //Funcion para leer los coeficientes del archivo .txt y almacenarlos en el struct
/*Archivo escrituras.c*/
void print_wav(fwav*); //Funcion para imprimir en pantalla la informacion solicitada.
void crear_archivo_filtrado(fwav*, const char*); //Funcion para crear el archivo filtrado.wav con los datos ya desnormalizados.
void crear_archivo_csv(fwav*, const char*); //Funcion para crear el archivo .csv
/*Archivo procesados.c*/
void normalizar_datos(fwav*); //Funcion para normalizar datos originales del archivo .wav
void procesar_datos(fwav*, fcoef*); //Funcion para procesar los datos normalizados y obtener los datos filtrados.
void desnormalizar_datos(fwav*); //Funcion para desnormalizar los datos ya filtrados

#endif // DECLARACIONES_H_INCLUDED
