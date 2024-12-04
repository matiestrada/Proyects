#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include "declaraciones.h"

//Funcion para validar parametros del main, verificar existencia de ambos archivos: fname1 = acorde.wav ; fname2 = coeficientes.txt
void validar_programa(int num, const char* fname1, const char* fname2)
{
    if(num != 3)
    {
        printf("ERROR. Cantidad de parametros incorrecta.");
        exit(-1);
    }
    FILE* fp1 = fopen(fname1, "rb");
    FILE* fp2 = fopen(fname2, "rt");
    if(fp1 == NULL || fp2 == NULL)
    {
        perror("Uno o ambos archivos no se pudieron abrir.");
    }
    fclose(fp1);
    fclose(fp2);
}

//Funcion para liberar memoria de la estructura del archivo .wav
void free_wav(fwav* arch_wav)
{
    free(arch_wav->datos);
    free(arch_wav->datos_normalizados);
    free(arch_wav->datos_filtrados);
    free(arch_wav->datos_desnormalizados);
    free(arch_wav);
}

//Funcion para liberar memoria de la estructura del archivo .txt
void free_coefs(fcoef* coefs)
{
    free(coefs->a);
    free(coefs->b);
    free(coefs);
}
