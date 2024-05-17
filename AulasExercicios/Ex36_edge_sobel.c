#include <stdio.h>
#include <stdlib.h>
#include "vc.h"

int main(void){
    IVC *image[2];

    image[1] = vc_read_image("./Images/Classic/barbara.pgm");
    if(image[1] == NULL) {
        printf("Erro: Não foi possível abrir a imagem.\n");
        exit(1);
    }

    image[2] = vc_image_new(image[1]->width, image[1]->height, image[1]->channels, image[1]->levels);
    if(image[2] == NULL) {
        printf("Erro: Não foi possível alocar memória para a imagem.\n");
        exit(1);
    }

    int valida = vc_gray_edge_sobel(image[1], image[2], 0.8);
    if(valida == 0) {
        printf("Erro: Não foi possível alocar memória para a imagem.\n");
        getchar();
        exit(1);
    }

    int validaEscrita = vc_write_image("barbara_edge_sobel.pgm", image[2]);
    if(validaEscrita == 0) {
        printf("Erro: Não foi possível escrever a imagem.\n");
        getchar();
        exit(1);
    }

    vc_image_free(image[1]);
    vc_image_free(image[2]);

    return 0;
}