#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void){
    IVC *image;
    // Declaração das variáveis de posição x e y para os pixels da imagem.
    int x, y;
    // Declaração de uma variável auxiliar para armazenar a posição no array de dados da imagem.
    long int posArray;

    // Cria uma nova imagem com as seguintes características:
    // largura = 256 pixels, altura = 256 pixels,
    // 1 canal de cor (tons de cinza), 255 bytes por pixel.
    image = vc_image_new(256, 256, 1, 255);
    if(image == NULL){
        printf("ERROR -> vc_image_new():\n\tOut of memory!\n");
        getchar();
        return 0;
    }


    for(x=0; x<image->width; x++){

        for(y=0; y<image->height; y++){
            // Calcula a posição no array de dados da imagem para o pixel atual.
            posArray = y * image->bytesperline + x * image->channels;
            // Define o valor do pixel como a posição x na imagem.
            image->data[posArray] = x;    
        }  
    }

    vc_write_image("Images/DesenvolvidasExercicios/GradienteEsqDir.pbm", image);

    vc_image_free(image);

    printf("Press any key to exit...\n");
    getchar();

    return 0;
}