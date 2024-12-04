#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include "declaraciones.h"

//Funcion para imprimir en pantalla la informacion solicitada.
void print_wav(fwav* arch_wav)
{
    printf("******* Datos del archivo acorde.wav *******\n\n\n");
    printf("  a. Frecuencia de muestreo: %uHz\n\n", arch_wav->frec_muestreo);
    printf("  b. Numero de canales: %u\n\n", arch_wav->num_canales);
    if(arch_wav->num_canales == 1)
    {
        printf("  c. Caracteristica: %s\n\n", "Mono");
    }
    else
    {
        printf("  c. Caracteristica: %s\n\n", "Stereo");
    }
    printf("  d. Cantidad de bits por muestra: %u\n\n", arch_wav->bits_by_muestra);
    printf("  e. Cantidad total de muestras: %u\n\n", arch_wav->cant_muestras);
    printf("  f. Duracion en segundos: %f\n\n", arch_wav->duracion);
}

//Funcion para crear el archivo filtrado.wav con los datos ya desnormalizados.
void crear_archivo_filtrado(fwav* arch_wav, const char* fname)
{
    //Apertura del archivo en modo escritura
    FILE* fp = fopen(fname, "wb");
    if(fp == NULL)
    {
        perror("No se pudo abrir el archivo. (Archivo filtrado.wav");
        exit(1);
    }

    //Escritura de los grupos RIFF, Format y data
    fwrite("RIFF", sizeof(char), 4, fp);
    fwrite(&(arch_wav->tam_arch), sizeof(uint32_t), 1, fp);
    fwrite("WAVE", sizeof(char), 4, fp);
    fwrite("fmt ", sizeof(char), 4, fp);
    fwrite(&(arch_wav->tam_format), sizeof(uint32_t), 1, fp);
    fwrite(&(arch_wav->format_audio), sizeof(uint16_t), 1, fp);
    fwrite(&(arch_wav->num_canales), sizeof(uint16_t), 1, fp);
    fwrite(&(arch_wav->frec_muestreo), sizeof(uint32_t), 1, fp);
    fwrite(&(arch_wav->bytes_by_seg), sizeof(uint32_t), 1, fp);
    fwrite(&(arch_wav->bytes_by_muestra), sizeof(uint16_t), 1, fp);
    fwrite(&(arch_wav->bits_by_muestra), sizeof(uint16_t), 1, fp);
    fwrite("data", sizeof(char), 4, fp);
    fwrite(&(arch_wav->tam_datos), sizeof(uint32_t), 1, fp);

    //Escritura de los valores muestra.
    fwrite(arch_wav->datos_desnormalizados, sizeof(int16_t), arch_wav->cant_muestras, fp);

    fclose(fp);
}

//Funcion para crear el archivo .csv
void crear_archivo_csv(fwav* arch_wav, const char* fname)
{
    //Apertura del archivo en modo escritura.
    FILE* fp = fopen(fname, "w");
    if(fp == NULL)
    {
        perror("No se pudo abrir el archivo. (Archivo .csv)");
        exit(1);
    }

    //Escritura de valores originales normalizados y valores filtrados normalizados.
    fprintf(fp, "Numero de muestra;Valor original;Valor filtrado\n");
    for(int i = 0; i < arch_wav->cant_muestras; i++)
    {
        fprintf(fp, "%d;%.10lf;%.10lf\n", i+1, arch_wav->datos_normalizados[i], arch_wav->datos_filtrados[i]);
    }

    fclose(fp);
}
