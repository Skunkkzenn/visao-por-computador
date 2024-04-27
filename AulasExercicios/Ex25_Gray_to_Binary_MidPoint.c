#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void)
{
    IVC *imagemSrc;
    IVC *imagemDst;

    imagemSrc = vc_read_image("Images/OldClassic/girlface.pgm");
    if (imagemSrc == NULL)
    {
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar();
        return 0;
    }

    imagemDst = vc_image_new(imagemSrc->width, imagemSrc->height, 1, 255);
    if (imagemDst == NULL)
    {
        printf("Error -> vc_image_new():\n\tFailed to create destination image!\n");
        vc_image_free(imagemSrc);
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    int kernel = 25;

    vc_gray_to_binary_midpoint(imagemSrc, imagemDst, kernel);
    vc_write_image("Images/DesenvolvidasExercicios/girlface-midpoint.pgm", imagemDst);

    vc_image_free(imagemSrc);
    vc_image_free(imagemDst);

}