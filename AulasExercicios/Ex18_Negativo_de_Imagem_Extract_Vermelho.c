#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"
// Gerar negativo da imagem RGB - Extração do Vermelhor
int main(void){
    IVC *image;
    int i;

    image = vc_read_image("Images/Additional/cablecar.ppm");

    vc_rgb_get_red_to_gray(image);

    if(image == NULL){
        printf("Error -> vc_gray_negative():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    vc_write_image("Images/DesenvolvidasExercicios/vc_rgb_get_red_to_gray.ppm", image);

    //Liberta espaço em memória
    vc_image_free(image);

    
    printf("Press any key to exit...\n");
    getchar();

    return 0; 

}