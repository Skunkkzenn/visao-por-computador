#include <stdio.h>
#include <stdlib.h>
#include "vc.h"

/* 
1º passo: rgb to hsv
2º passo: hsv segmentation
3º passo: dilatação
4º passo: rgb to gray
5º passo: gray to binary
6º passo: dilatação
7º passo: labeling
8º passo: labeling info
9º passo: draw boxes e identificar centro de massa
*/


int main(void) {
    
    IVC *image[10];
    OVC *blobs;  

    #pragma region Convert image to HSV

    image[0] = vc_read_image("Images/HSVTestImage01.ppm");

    if (image[0] == NULL) {
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        return 0;
    }

    int validarConversao = vc_rgb_to_hsv(image[0]);

    if (validarConversao != 1) {
        printf("ERROR -> vc_rgb_to_hsv():\n\tConversion failed!\n");
        getchar();
        return 0;
    }

    vc_write_image("HSVTestImage01_HSV.ppm", image[0]);

    #pragma endregion

    #pragma region HSV Segmentation

    image[1] = vc_read_image("HSVTestImage01_HSV.ppm");
    if (image[1] == NULL)
    {
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        getchar();  
        return 0;
    }
    vc_hsv_segmentation(image[1], 230, 250, 70, 100, 70, 100);
    vc_write_image("HSVTestImage01_HSV_Segmentation.ppm", image[1]);

    #pragma endregion

    #pragma region RGB to Gray
    image[2] = vc_read_image("HSVTestImage01_HSV_Segmentation.ppm");
    if (image[2] == NULL)
    {
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        getchar();  
        return 0;
    }

    image[3] = vc_image_new(image[2]->width, image[2]->height, 1, 255);
    if (image[3] == NULL)
    {
        printf("ERROR -> vc_image_new():\n\tMemory not allocated!\n");
        getchar();
        return 0;
    }

    int validarConvert = vc_rgb_to_gray(image[2], image[3]);
    if (validarConvert != 1) {
        printf("ERROR -> vc_rgb_to_gray():\n\tConversion failed!\n");
        getchar();
        return 0;
    }

    vc_write_image("HSVTestImage01_HSV_Segmentation_Gray.pgm", image[3]);
    #pragma endregion

    #pragma region Gray to Binary

    image[4] = vc_read_image("HSVTestImage01_HSV_Segmentation_Gray.pgm");
    if (image[4] == NULL)
    {
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        getchar();  
        return 0;
    }

    image[5] = vc_image_new(image[4]->width, image[4]->height, 1, 255);
    if (image[5] == NULL)
    {
        printf("ERROR -> vc_image_new():\n\tMemory not allocated!\n");
        getchar();
        return 0;
    }

    int thereshold = 128;
    vc_gray_to_binary(image[4], image[5], thereshold);
    vc_write_image("HSVTestImage01_HSV_Segmentation_Binary.pbm", image[5]);

    #pragma endregion

    #pragma region Dilatação

    image[6] = vc_read_image("HSVTestImage01_HSV_Segmentation_Binary.pbm");
    if (image[6] == NULL)
    {
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        getchar();  
        return 0;
    }

    image[7] = vc_image_new(image[6]->width, image[6]->height, 1, 255);
    if (image[7] == NULL)
    {
        printf("ERROR -> vc_image_new():\n\tMemory not allocated!\n");
        getchar();
        return 0;
    }

    int kernel = 2;
    vc_binary_dilate(image[6], image[7], kernel);
    vc_write_image("HSVTestImage01_HSV_Segmentation_Dilated.pbm", image[7]);
    
    #pragma endregion

    #pragma region Labeling

    image[8] = vc_read_image("HSVTestImage01_HSV_Segmentation_Dilated.pbm");
    if (image[8] == NULL)
    {
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        getchar();  
        return 0;
    }

    image[9] = vc_image_new(image[8]->width, image[8]->height, image[8]->channels, image[8]->levels);
    if (image[9] == NULL)
    {
        printf("ERROR -> vc_image_new():\n\tMemory not allocated!\n");
        getchar();
        return 0;
    }

    int nLabels;
    blobs = vc_binary_blob_labelling(image[8], image[9], &nLabels);
    if(blobs != NULL)
    {
        printf("Number of Blobs: %d\n", nLabels);
        for(int i = 0; i < nLabels; i++)
        {
            printf("Blob %d: Area = %d, Perimeter = %d, Label = %d\n", i, blobs[i].area, blobs[i].perimeter, blobs[i].label);
        }
        free(blobs);
    }

    vc_binary_blob_info(image[9], blobs, nLabels);

    #pragma endregion

    #pragma region Draw boxes

    // Iterar sobre os blobs e desenhar caixas ao redor deles
    for(int i = 0; i < nLabels; i++) {
        draw_box(image[8], blobs[i]);
    }
   
    vc_write_image("HSVTestImage01_HSV_Segmentation_Dilated_Labeling.pbm", image[8]);

    #pragma endregion

    // Libertar memória
    vc_image_free(image[0]);
    vc_image_free(image[1]);
    vc_image_free(image[2]);
    vc_image_free(image[3]);
    vc_image_free(image[4]);
    vc_image_free(image[5]);
    vc_image_free(image[6]);
    vc_image_free(image[7]);
    vc_image_free(image[8]);
    vc_image_free(image[9]);

    return 0; 
}