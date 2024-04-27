#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"
#include <math.h>

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
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar();
        return 0;
    }

    int kernel = 25;
    int cmin = 15;

    vc_gray_to_binary_bernsen(imagemSrc, imagemDst, kernel, cmin);
    vc_write_image("Images/DesenvolvidasExercicios/girlface-bernsen.pgm", imagemDst);

    vc_image_free(imagemSrc);
    vc_image_free(imagemDst);

    return 0;
}