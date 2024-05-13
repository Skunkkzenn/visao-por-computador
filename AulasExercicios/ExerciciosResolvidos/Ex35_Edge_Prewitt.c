#include <stdio.h>
#include <stdlib.h>
#include "vc.h"

int main(void)  {
    IVC* image[2];

    image[1] = vc_read_image("Images/Histograma/barbara.pgm");
    if(image[1] == NULL){
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        getchar();
        return 0;
    }


    image[2] = vc_image_new(image[1]->width, image[1]->height, image[1]->channels, image[1]->levels);
    if(image[2] == NULL){
        printf("ERROR -> vc_image_new():\n\tMemory not allocated!\n");
        getchar();
        return 0;
    }

    int valida = vc_gray_edge_prewitt(image[1], image[2], 12);
    if(valida == 0){
        printf("ERROR -> vc_gray_edge_prewitt():\n\tMemory not allocated!\n");
        getchar();
        return 0;
    }
   
    int validaEscrita = vc_write_image("Images/Histograma/barbara_edge_prewitt.pgm", image[2]);
    if(validaEscrita == 0){
        printf("ERROR -> vc_write_image():\n\tFile not created!\n");
        getchar();
        return 0;
    }

    vc_image_free(image[1]);
    vc_image_free(image[2]);

    return 0;
}