#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

// O programa gera uma imagem no formato PBM (Portable Bitmap), que é um formato de imagem binária. 
// Em imagens binárias, cada pixel pode ter apenas dois valores possíveis
// Representados como 0 (preto) e 1 (branco).

int main(void){
    IVC *image;
    int x, y;
    long int pos;

    // Cria uma nova imagem com as seguintes características:
    // largura = 320 pixels, altura = 280 pixels,
    // 1 canal de cor (tons de cinza), 1 byte por pixel
    image = vc_image_new(320, 280, 1, 1);

    //Verifica se imagem é igual a NULL
    if(image == NULL){
        printf("ERROR -> vc_image_new():\n\tOut of memory!\n");
        getchar();
        return 0;
    }

    // Preenche a imagem com valores de pixel conforme a lógica abaixo:
    // - Define os pixels como 1 (branco) se estiverem na metade superior esquerda ou na metade inferior direita da imagem.
    // - Define os pixels como 0 (preto) caso contrário. 
    for(x=0; x<image->width; x++){
        for(y=0; y<image->height; y++){

            // Calcula a posição do pixel na memória com base nas coordenadas (x, y)
            pos = y * image->bytesperline + x * image->channels;
            
            // Define o valor do pixel conforme a condição especificada
            if((x <= image->width/2) && (y <= image->height/2)) image->data[pos]= 1;
            else if((x > image->width/2) && (y > image->height/2)) image->data[pos] = 1;
            else image->data[pos] = 0;
        }  
    }

    vc_write_image("Images/DesenvolvidasExercicios/vc-0002.pbm", image);

    vc_image_free(image);

    printf("Press any key to exit...\n");
    getchar();

    return 0;
}