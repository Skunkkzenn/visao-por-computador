#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void)
    {
        IVC *image;

        // Carregar a imagem RGB
        image = vc_read_image("ImagensNegativa/HSVTestImage01.ppm");
        if (image == NULL)
        {
            printf("Error -> vc_read_image():\n\tFile not found!\n");
            getchar(); // Espera por caracter para finalizar a sessão
            return 0;
        }

        // Converter a imagem para HSV
        int validaConversao = vc_rgb_to_hsv(image);

        if (validaConversao != 1)
        {
            printf("Error -> vc_rgb_to_hsv():\n\tConversion failed!\n");
            getchar(); // Espera por caracter para finalizar a sessão
            return 0;
        }

        vc_write_image("Images/DesenvolvidasExercicios/ConversaoRGBtoHSV.ppm", image);

        // Liberar espaço de memória da imagem
        vc_image_free(image);

        return 0;
    }