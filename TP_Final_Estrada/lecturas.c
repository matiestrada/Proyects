#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include "declaraciones.h"

//Funcion para verificar el formato del archivo .wav y a su vez leer sus datos y almacenarlos en el struct
fwav* leer_archivo_audio(const char* fname)
{
    //Declaracion de puntero a estructura.
    fwav* arch_wav = (fwav*)malloc(sizeof(fwav));
    if(arch_wav == NULL)
    {
        printf("Error al asignar memoria. (arch_wav)");
        exit(1);
    }

    //Apertura de archivo .wav
    FILE* fp = fopen(fname, "rb");
    if(fp == NULL)
    {
        perror("No se pudo abrir el archivo. (archivo .wav)");
        free(arch_wav);
        exit(1);
    }

    //Verificacion encabezado "RIFF"
    char encabezado[5] = {'\0'};
    fread(encabezado, sizeof(char), 4, fp);
    if(strcmp(encabezado, "RIFF") != 0)
    {
        printf("Error. Formato de archivo no valido. (RIFF)");
        free(arch_wav);
        fclose(fp);
        exit(1);
    }
    //Lectura longitud (en bytes) de los datos que siguen
    fread(&(arch_wav->tam_arch), sizeof(uint32_t), 1, fp);

    //Verificacion encabezado "WAVE"
    fread(encabezado, sizeof(char), 4, fp);
    if(strcmp(encabezado, "WAVE") != 0)
    {
        printf("Error. Formato de archivo no valido. (WAVE)");
        free(arch_wav);
        fclose(fp);
        exit(1);
    }

    //Verificacion encabezado "fmt "
    fread(encabezado, sizeof(char), 4, fp);
    if(strcmp(encabezado, "fmt ") != 0)
    {
        printf("Error. Formato de archivo no valido. (fmt )");
        free(arch_wav);
        fclose(fp);
        exit(1);
    }

    //Lectura longitud del resto del grupo Format
    fread(&(arch_wav->tam_format), sizeof(uint32_t), 1, fp);

    //Lectura Formato de audio
    fread(&(arch_wav->format_audio), sizeof(uint16_t), 1, fp);
    if(arch_wav->format_audio != 1)
    {
        printf("Error. Formato de audio incorrecto.");
        free(arch_wav);
        fclose(fp);
        exit(1);
    }

    //Lectura numero de canales
    fread(&(arch_wav->num_canales), sizeof(uint16_t), 1, fp);

    //Lectura frecuencia de muestreo
    fread(&(arch_wav->frec_muestreo), sizeof(uint32_t), 1, fp);

    //Lectura bytes por segundo
    fread(&(arch_wav->bytes_by_seg), sizeof(uint32_t), 1, fp);

    //Lectura bytes por muestra
    fread(&(arch_wav->bytes_by_muestra), sizeof(uint16_t), 1, fp);

    //Lectura bits por muestra
    fread(&(arch_wav->bits_by_muestra), sizeof(uint16_t), 1, fp);

    //Verificacion encabezado "data"
    fread(encabezado, sizeof(char), 4, fp);
    if(strcmp(encabezado, "data") != 0)
    {
        printf("Error. Formato de archivo no valido. (data)");
        free(arch_wav);
        fclose(fp);
        exit(1);
    }

    //Lectura longitud (en bytes) de los datos
    fread(&(arch_wav->tam_datos), sizeof(uint32_t), 1, fp);

    //Calculos de cantidad de muestras y duracion del audio.
    arch_wav->cant_muestras = arch_wav->tam_datos / arch_wav->bytes_by_muestra;
    arch_wav->duracion = (float)arch_wav->cant_muestras / (float)arch_wav->frec_muestreo;

    //Asignacion de memoria para almacenar los datos y lectura de los mismos.
    arch_wav->datos = (int16_t*)malloc(sizeof(arch_wav->bytes_by_muestra) * arch_wav->cant_muestras);
    if(arch_wav->datos == NULL)
    {
        printf("Error al asignar memoria. (arch_wav->datos)");
        free(arch_wav);
        fclose(fp);
        exit(1);
    }
    fread(arch_wav->datos, sizeof(int16_t), arch_wav->cant_muestras, fp);

    fclose(fp);
    return arch_wav;
}

//Funcion para leer los coeficientes del archivo .txt y almacenarlos en el struct
fcoef* leer_archivo_coefs(const char* fname)
{
    //Declaracion de puntero a estructura.
    fcoef* coefs = (fcoef*)malloc(sizeof(fcoef));
    if(coefs == NULL)
    {
        printf("Error al asignar memoria. (coefs)");
        exit(1);
    }

    //Apertura archivo .txt
    FILE* fp = fopen(fname, "rt");
    if(fp == NULL)
    {
        perror("No se pudo abrir el archivo. (archivo .txt)");
        free(coefs);
        exit(1);
    }

    //Primero obtengo todos los valores del archivo.
    int size = 5; //tamaño inicial del array.
    double* valores = (double*)malloc(sizeof(double)*size); //array para almacenar todos los coeficientes.
    if(valores == NULL)
    {
        printf("Error al asignar memoria. (valores)");
        free(coefs);
        fclose(fp);
        exit(1);
    }
    double current; //variable para almacenar el valor actual.
    int cnt = 0, flag = 0;
    while(fscanf(fp, "%lf", &current) != EOF && flag == 0)
    {
        valores[cnt++] = current;
        if(cnt == size)
        {
            size *= 2;
            valores = (double*)realloc(valores, sizeof(double)*size); //duplico el tamaño del arreglo para poder almacenar mas valores.
        }
        if(fgetc(fp) != ',') //leo la coma para que avance el indicador de archivo, en caso de no leer la coma significa que se llego al final del archivo.
        {
            flag = 1;
        }
    }

    //Ahora reservo memoria para los dos vectores y divido los valores mitad y mitad.
    coefs->a = (double*)malloc(sizeof(double) * cnt/2);
    coefs->b = (double*)malloc(sizeof(double) * cnt/2);
    if(coefs->a == NULL || coefs->b == NULL)
    {
        printf("Error al asignar memoria. (coefs->a o coefs->b)");
        free(coefs);
        free(valores);
        fclose(fp);
        exit(1);
    }

    //Ciclo para dividir los valores.
    for(int i = 0; i < cnt/2; i++)
    {
        coefs->a[i] = valores[i];
        coefs->b[i] = valores[cnt/2 + i];
    }

    coefs->total = cnt; //Guardo la cantidad total de coeficientes.

    free(valores);
    fclose(fp);
    return coefs;
}
