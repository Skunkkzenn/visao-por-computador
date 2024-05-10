#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void)
{
    IVC *imgSrc;
    IVC *imgDst;

    imgSrc = vc_read_image("Images/DesenvolvidasExercicios/cranio.pgm");
    if (imgSrc == NULL)
    {
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    imgDst = vc_image_new(imgSrc->width, imgSrc->height, 1, 255);
    if (imgDst == NULL)
    {
        printf("Error -> vc_image_new():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    int kernel = 10;

    vc_binary_erode(imgSrc, imgDst, kernel);

    vc_write_image("Images/DesenvolvidasExercicios/cranio-erode.pgm", imgDst);
    vc_image_free(imgSrc);
    vc_image_free(imgDst);

    return 0;
}