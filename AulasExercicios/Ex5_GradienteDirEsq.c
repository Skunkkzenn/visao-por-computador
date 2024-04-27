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
    // 255 canal de cor (tons de cinza), 1 bytes por pixel.
    image = vc_image_new(256, 256, 1, 255);
    if(image == NULL){
        printf("ERROR -> vc_image_new():\n\tOut of memory!\n");
        getchar();
        return 0;
    }

    // Loop externo: Inicia 'x' no último pixel da linha e itera da direita para a esquerda.
    for(x= image->width - 1; x >= 0; x++){
            // Loop interno: Inicia 'y' no último pixel da linha e itera de baixo para cima.
        for(y=image->width - 1; y >= 0; y--){
            // Calcula a posição do pixel no array de dados da imagem.
            posArray = y * image->bytesperline + x * image->channels;
            // Define o valor do pixel como o negativo de 'x'.
            image->data[posArray] = -x;
        }  
    }

    vc_write_image("Images/DesenvolvidasExercicios/GradienteDirEsq.pbm", image);

    vc_image_free(image);

    printf("Press any key to exit...\n");
    getchar();

    return 0;
}