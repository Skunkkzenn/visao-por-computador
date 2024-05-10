#include <stdio.h>
#include "vc.h"

int main(void)
{
    // Declarações das variáveis para as imagens de origem e destino
    IVC *imageSrc;
    IVC *imageDst;

    // Carregar a imagem RGB
    imageSrc = vc_read_image("Images/Additional/flowers.ppm");

    // Verificar se a leitura da imagem foi bem-sucedida
    if (imageSrc == NULL)
    {
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    // Criar uma nova imagem de destino com as mesmas dimensões da imagem de origem
    imageDst = vc_image_new(imageSrc->width, imageSrc->height, 1, 255);

    // Verificar se a criação da imagem de destino foi bem-sucedida
    if (imageDst == NULL)
    {
        printf("Error -> vc_image_new():\n\tUnable to allocate memory for destination image!\n");
        vc_image_free(imageSrc);
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    // Chamar a função para converter RGB para tons de cinza
    if (!vc_rgb_to_gray(imageSrc, imageDst))
    {
        printf("Error -> vc_rgb_to_gray():\n\tConversion failed!\n");
        vc_image_free(imageSrc);
        vc_image_free(imageDst);
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    // Salvar a imagem de destino
    vc_write_image("Images/DesenvolvidasExercicios/ConversaoRGBtoGray.pgm", imageDst);

    // Liberar espaço de memória das imagens
    vc_image_free(imageSrc);
    vc_image_free(imageDst);

    return 0;
}
