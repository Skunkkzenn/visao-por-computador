#include <stdio.h>
#include "vc.h"

int main(void)
{
    IVC *image;
    int i;

    // Lê imagem PBM para inverter as posições;
    image = vc_read_image("vc-0002.pbm");

    if (image == NULL)
    {
        printf("Error -> vc_read_image():\n\tFile not found!");
    }

    // Itera sobre todos os pixels da imagem.
    // Inverte os valores dos pixels, trocando 0 por 1 e 1 por 0.
    for (i = 0; i < image->bytesperline * image->height; i += image->channels)
    {
        // Verifica o valor do pixel atual.
        if (image->data[i] == 1)
        {   // Se o pixel for 1, troca para 0.
            image->data[i] = 0;
        }
        else
        {   // Se o pixel for 0, troca para 1.
            image->data[i] = 1;
        }
    }

    //Escreve nova imagem
    vc_write_image("Images/DesenvolvidasExercicios/vc-0002-iverted.pbm", image);

    //Liberta memória de imagem criada
    vc_image_free(image);

    printf("Press any key to exit.");
    getchar();

    return 0;
}