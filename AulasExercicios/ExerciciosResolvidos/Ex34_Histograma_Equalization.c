#include <stdio.h>
#include <stdlib.h>
#include "vc.h"

int main(void){
    IVC* image[2];

    image[1] = vc_read_image("Images/Histograma/barbara2.pgm");
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

    vc_gray_histogram_equalization(image[1], image[2]);
    int valida = vc_write_image("Images/Histograma/barbara2_equalization.pgm", image[2]);
    if(valida == 0){
        printf("ERROR -> vc_write_image():\n\tFile not created!\n");
        getchar();
        return 0;
    }

    vc_image_free(image[1]);
    vc_image_free(image[2]);
}