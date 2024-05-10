#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vc.h"

int main(void)
{
    IVC *imagesrc;
    IVC *imagedst;

    // Carregar a imagem RGB
    imagesrc = vc_read_image("cranio_pet_img/PET-Alzheimer.ppm");
    if (imagesrc == NULL)
    {
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    // Alocar memória para a imagem de destino (tons de cinza)
    imagedst = vc_image_new(imagesrc->width, imagesrc->height, 1, 255);
    // Verificar se a criação da imagem de destino foi bem-sucedida
    if (imagedst == NULL)
    {
        printf("Error -> vc_image_new():\n\tFailed to create destination image!\n");
        vc_image_free(imagesrc);
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    // Converter a imagem RGB para tons de cinza
    if(!vc_rgb_to_gray(imagesrc, imagedst)){
        printf("Error -> vc_rgb_to_gray():\n\tConversion failed!\n");
        vc_image_free(imagesrc);
        vc_image_free(imagedst);
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    int threshold = 64;

    // Binarizar a imagem em tons de cinza
    vc_gray_to_binary(imagedst, threshold);

    // Salvar a imagem binarizada
    vc_write_image("Images/DesenvolvidasExercicios/cranio.pgm", imagedst);

    // Liberar espaço de memória das imagens
    vc_image_free(imagesrc);
    vc_image_free(imagedst);

    return 0;
}