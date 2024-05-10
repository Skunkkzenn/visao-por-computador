#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void)
{
    IVC* image;

    image = vc_read_image("Images/OldClassic/girlface.pgm");

    if (image == NULL)
    {
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar();
        return 0;
    }
    
    vc_gray_to_binary_global_mean(image);

    vc_write_image("Images/DesenvolvidasExercicios/girlface-binary-mean-convert.pgm", image);

    vc_image_free(image);

    return 0;
}