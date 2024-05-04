#include <stdio.h>
#include "vc.h"

/*
1º passo: rgb to hsv
2º passo: hsv segmentation
3º passo: dilatação
*/

int main(void) {
    IVC *image[5];

    image[0] = vc_read_image("Images/HSVTestImage01.ppm");
    if (image[0] == NULL) {
        printf("ERROR -> vc_read_image():\n\tFile not found!\n");
        return 0;
    }

    #pragma region Convert image to HSV

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
    vc_hsv_segmentation(image[1], 230, 250, 70, 100, 70, 100);
    vc_write_image("HSVTestImage01_HSV_Segmentation.ppm", image[1]);
    #pragma endregion

    #pragma region Dilatação
    
    int kernel = 15;
    image[2] = vc_read_image("HSVTestImage01_HSV_Segmentation.ppm");
    image[3] = vc_image_new(image[2]->width, image[2]->height, image[2]->channels, image[2]->levels);
    vc_binary_dilate(image[2], image[3], kernel);
    vc_write_image("HSVTestImage01_HSV_Segmentation_Dilated.ppm", image[3]);

    #pragma endregion

    vc_image_free(image[0]);
    vc_image_free(image[1]);
    vc_image_free(image[2]);
    vc_image_free(image[3]);

    return 0;
    
}