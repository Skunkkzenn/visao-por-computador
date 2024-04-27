#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void)
    {
        IVC *image;
        
        // Carregar a imagem RGB
        image = vc_read_image("Images/DesenvolvidasExercicios/ConversaoRGBtoHSV.ppm");

        vc_hsv_segmentation(image, 30, 70, 50, 100, 60, 100);

        if (image == NULL)
        {
            printf("Error -> vc_read_image():\n\tFile not found!\n");
            getchar(); // Espera por caracter para finalizar a sessão
            return 0;
        }

        vc_write_image("Images/DesenvolvidasExercicios/HSV-segmentation.ppm", image);

        // Liberar espaço de memória da imagem
        vc_image_free(image);

        return 0;
    }