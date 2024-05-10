#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void){
    IVC *image;
    //especifica posições
    int x, y;
    //var auxiliar, posição do array
    long int posArray;

    image = vc_image_new(256, 256, 1, 255);
    if(image == NULL){
        printf("ERROR -> vc_image_new():\n\tOut of memory!\n");
        getchar();
        return 0;
    }

    for(x = 0; x < image->width; x++){
        for(y = 0; y < image->height; y++){
            posArray = y * image->bytesperline + x * image->channels;
            // Calcula a intensidade do pixel com base na distância do canto superior direito
            image->data[posArray] = (x + image->width - y) / 2;   
        }  
    }
    
    vc_write_image("Images/DesenvolvidasExercicios/GradienteTransversalEsqInfDirSup.pbm", image);

    vc_image_free(image);

    printf("Press any key to exit...\n");
    getchar();

    return 0;
}