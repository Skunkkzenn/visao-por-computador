#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

//Programa que realiza a negação de uma imagem em escala de cinza PGM (Portable Gray Map)

//Abertura de Imagem e Gravação em Novo Ficheiro
int main(void){
    IVC *image;
    int i;

    //Abre imagem com função vinda do vc.h & vc.c
    image = vc_read_image("Images/FLIR/flir-01.pgm");

    //Erro na abertura de imagem
    if(image == NULL){
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    //  Um loop que itera sobre todos os pixels da imagem. A cada iteração, inverte o valor do pixel 
    // (o primeiro byte de cada canal, assumindo que a imagem tenha um único canal de cor).
    for (i = 0; i < image->bytesperline*image->height; i+=image->channels)
    {
        image->data[i] = 255 - image->data[i];
    }

    // Chama a função vc_write_image para gravar a imagem modificada em um novo arquivo chamado "vc-0001.pgm".
    vc_write_image("Images/DesenvolvidasExercicios/vc-0001.pgm", image);

    //Liberta espaço em memória
    vc_image_free(image);

    printf("Press any key to exit...\n");
    getchar();

    return 0;
    
}