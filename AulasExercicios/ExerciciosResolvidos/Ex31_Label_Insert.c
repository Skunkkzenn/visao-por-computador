#include <stdio.h>
#include <stdlib.h>
#include "vc.h"

int main(void)
{
    IVC *image[2];

    image[0] = vc_read_image("Images/Labelling/labelling-1.pgm");
    if (image[0] == NULL)
    {
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        getchar();
        return 0;
    }

    image[1] = vc_image_new(image[0]->width, image[0]->height, image[0]->channels, image[0]->levels);
    if (image[1] == NULL)
    {
        printf("ERROR -> vc_image_new():\n\tMemory not allocated!\n");
        getchar();
        return 0;
    }

    int nBlobs;
    OVC *blobs = vc_binary_blob_labelling(image[0], image[1], &nBlobs);
    if(blobs != NULL)
    {
        printf("Number of Blobs: %d\n", nBlobs);
        for(int i = 0; i < nBlobs; i++)
        {
            printf("Blob %d: Area = %d, Perimeter = %d, Label = %d\n", i, blobs[i].area, blobs[i].perimeter, blobs[i].label);
        }
        free(blobs);
    }

    vc_write_image("Images/Labelling/labelling-1-output.pgm", image[1]);

    vc_image_free(image[0]);
    vc_image_free(image[1]);

    return 0;
}