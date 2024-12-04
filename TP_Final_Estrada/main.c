#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include "declaraciones.h"

int main(int argc, char* argv[])
{
    //Validacion del programa (parametros y archivos).
    validar_programa(argc, argv[1], argv[2]);

    //Verificacion de archivo .wav y lectura de datos.
    fwav* arch_wav = leer_archivo_audio(argv[1]);

    //Impresion en consola de los datos adquiridos
    print_wav(arch_wav);

    //Normalizacion de datos
    normalizar_datos(arch_wav);

    //Lectura de coeficientes filtro digital
    fcoef* coefs = leer_archivo_coefs(argv[2]);

    //Procesado de datos originales
    procesar_datos(arch_wav, coefs);

    //Desnormalizacion de datos filtrados
    desnormalizar_datos(arch_wav);

    //Creacion de archivo filtrado .wav
    crear_archivo_filtrado(arch_wav, "Filtrado.wav");

    //Creacion de archivo .csv
    crear_archivo_csv(arch_wav, "Datos_finales.csv");

    //Reproduccion archivos de audio
    printf("\n\n******* Reproduccion archivo original acorde.wav *******\n");
    system("pause");
    sndPlaySound(argv[1], 0);
    printf("\n\n******* Reproduccion archivo filtrado Filtrado.wav *******\n");
    system("pause");
    sndPlaySound("Filtrado.wav", 0);

    //Liberacion de memoria
    free_wav(arch_wav);
    free_coefs(coefs);

    return 0;
}
