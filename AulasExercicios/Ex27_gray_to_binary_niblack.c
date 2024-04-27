#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void)
{
    IVC* imageSrc;
    IVC* imageDst;

    imageSrc = vc_read_image("Images/OldClassic/girlface.pgm");
    if(imageSrc == NULL){
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar();
        return 0;
    }

    imageDst = vc_image_new(imageSrc->width, imageSrc->height, 1, 255);
    if(imageDst == NULL){
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar();
        return 0;
    }

    int kernel = 25;

    vc_gray_to_binary_niblack(imageSrc, imageDst, kernel);
    vc_write_image("Images/DesenvolvidasExercicios/girlface-niblack.pgm", imageDst);

    vc_image_free(imageSrc);
    vc_image_free(imageDst);

    return 0;
}