#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void){
    IVC *image;
    int i;

    image = vc_read_image("Images/Additional/flower.ppm");

    vc_rgb_negative(image);

    if(image == NULL){
         printf("Error -> vc_rgb_negative():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    vc_write_image("Images/DesenvolvidasExercicios/vc-negative-rgb.pgm", image);

    //Liberta espaço de memória criado.
    vc_image_free(image);

    printf("Press any key to exit...\n");
    getchar();

    return 0;
}