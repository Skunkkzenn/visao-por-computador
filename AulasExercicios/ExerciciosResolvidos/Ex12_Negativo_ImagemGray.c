#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

// Converte imagem para uma negativa em grayscale

int main(void){
    IVC *image;
    int i;

    image = vc_read_image("Images/OldClassic/bridge.pgm");

    vc_gray_negative(image);

    if(image == NULL){
        printf("Error -> vc_gray_negative():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    vc_write_image("Images/DesenvolvidasExercicios/vc-negative-gray.pgm", image);

    //Liberta espaço em memória
    vc_image_free(image);

    printf("Press any key to exit...\n");
    getchar();

    return 0;
    
}