#include <stdio.h>
#include "vc.h" 

int main(void)
{
    IVC *image_src;
    IVC *image_dst;

    // Carregar a imagem em escala de cinza
    image_src = vc_read_image("Images/OldClassic/bridge.pgm");

    if (image_src == NULL)
    {
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    // Aloca memória para a imagem de destino RGB
    image_dst = vc_image_new(image_src->width, image_src->height, 3, 255); // 3 canais (RGB), 255 níveis de intensidade

    if (image_dst == NULL)
    {
        printf("Error -> vc_image_new():\n\tMemory allocation error!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        vc_image_free(image_src); // Libera a memória da imagem de origem antes de sair
        return 0;
    }

    // Chama a função para converter a imagem de escala de cinza para RGB
    if (!vc_scale_gray_to_rgb(image_src, image_dst))
    {
        printf("Error -> vc_scale_gray_to_rgb():\n\tConversion failed!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        vc_image_free(image_src); // Libera a memória da imagem de origem antes de sair
        vc_image_free(image_dst); // Libera a memória da imagem de destino antes de sair
        return 0;
    }

    // Escreve a imagem resultante em um arquivo
    vc_write_image("Images/DesenvolvidasExercicios/scale_gray_to_rgb.ppm", image_dst); // Substitua "caminho_para_sua_imagem_de_saida.ppm" pelo caminho para salvar a imagem de saída

    // Libera a memória das imagens
    vc_image_free(image_src);
    vc_image_free(image_dst);

    return 0;
}