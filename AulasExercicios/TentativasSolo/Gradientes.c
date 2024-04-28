#include <stdio.h>
#include "vc.h"


#pragma region Apontamentos

    // Posição atual do pixel na imagem
    // pos = y * image->bytesperline + x * image->channels; 

#pragma endregion

int main(void){

    IVC* image;
    int pos; // armaena a posição do pixel na imagem

    image = vc_image_new(256, 256, 1, 255);
    if(image == NULL) {
        printf("ERROR -> vc_image_new():\n\tOut of memory!\n");
        getchar();
        return 0;
    }

    for(int y = 0; y < image->height; y++){
        for(int x = 0; x < image->width; x++){
            pos = y * image->bytesperline + x * image->channels;
            // image->data[pos] = x; // Gradiente da esquerda para a direita
            // image->data[pos] = -x; // Gradiente da direita para a esquerda
            // image->data[pos] = y; // Gradiente de cima para baixo
            // image->data[pos] = -y; // Gradiente de baixo para cima
            // image->data[pos] = (x + image->width - y) / 2; // Gradiente da direita para a esquerda e de cima para baixo
            // image->data[pos] = - (x + image->width - y) / 2; // Gradiente da esquerda para a direita e de baixo para cima
            // image->data[pos] = (x + y) / 2; // Gradiente da esquerda para a direita e de cima para baixo
            // image->data[pos] = - (x + y) / 2; // Gradiente da direita para a esquerda e de baixo para cima
        }
    }

    vc_write_image("gradiente.pbm", image);
    vc_image_free(image);

    return 0;
}