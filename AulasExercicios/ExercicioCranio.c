/*
    Passos:
    1) Binarização da imagem para obter o crânio
        Thr = ???
    2) Binarização da imagem para obter o cérebro
        Thr = ???
*/
#include <stdio.h>
#include "vc.h"

int main(void)
{
    IVC *image[7];

#pragma region Binarização da imagem para obter o crânio e cerebro
    image[0] = vc_read_image("cranio_pet_img/brain.pgm");
    if (image[0] == NULL)
    {
        printf("Error -> vc_read_image():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    image[1] = vc_image_new(image[0]->width, image[0]->height, image[0]->channels, image[0]->levels);
    if (image[1] == NULL)
    {
        printf("Error -> vc_image_new():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }
    int threshold = 180;
    vc_gray_to_binary(image[0], image[1], threshold);
    vc_write_image("Images/DesenvolvidasExercicios/skull_segmented.pbm", image[1]);
#pragma endregion

#pragma region Binarização da imagem, remover o crânio e obter o cérebro
    image[2] = vc_image_new(image[0]->width, image[0]->height, image[0]->channels, image[0]->levels);

    if (image[2] == NULL)
    {
        printf("Error -> vc_image_new():\n\tFile not found!\n");
        getchar(); // Espera por caracter para finalizar a sessão
        return 0;
    }

    threshold = 90;
    vc_gray_to_binary(image[0], image[2], threshold);
    vc_write_image("Images/DesenvolvidasExercicios/brain_skull_segmented.pbm", image[2]);

#pragma endregion 


#pragma region subtração pixel a pixel
    image[3] = vc_image_new(image[0]->width, image[0]->height, image[0]->channels, image[0]->levels);
    
    // Realiza a operação de subtração pixel a pixel
    for (int y = 0; y < image[0]->height; y++)
    {
        for (int x = 0; x < image[0]->width; x++)
        {
            int pos = y * image[0]->bytesperline + x * image[0]->channels;

            // igualar a zero os pixéis da imagem brain_skull_segmented.pbm
            // para os quais imagem skull_segmented.pbm regista intensidade 255.
            // image[1] = skull_segmented.pbm
            // image[2] = brain_skull_segmented.pbm
            
            if (image[1]->data[pos] == 255)
            {
                image[3]->data[pos] = 0;
            }
            else // Se o pixel na imagem do crânio for preto (0)
            {
                image[3]->data[pos] = image[2]->data[pos]; // Mantém o pixel na imagem do cérebro e do crânio na imagem resultante
            }

        }
    }
    vc_write_image("Images/DesenvolvidasExercicios/brain_segmented.pbm", image[3]);

#pragma endregion

#pragma region Erosão remover bordas

    image[4] = vc_image_new(image[0]->width, image[0]->height, image[0]->channels, image[0]->levels);
    vc_binary_erode(image[3], image[4], 4);
    vc_write_image("Images/DesenvolvidasExercicios/brain_segmented_eroded.pbm", image[4]);

#pragma endregion

#pragma region Dilatação para recuperar a imagem do cerebro original

    image[5] = vc_image_new(image[0]->width, image[0]->height, image[0]->channels, image[0]->levels);
    vc_binary_dilate(image[4], image[5], 5);
    vc_write_image("Images/DesenvolvidasExercicios/brain_segmented_eroded_dilated.pbm", image[5]);

#pragma endregion

    image[6] = vc_image_new(image[0]->width, image[0]->height, image[0]->channels, image[0]->levels);
    vc_binary_close(image[5], image[6], 4);
    vc_write_image("Images/DesenvolvidasExercicios/brain_segmented_eroded_dilated_closed.pbm", image[6]);

    image[7] = vc_image_new(image[0]->width, image[0]->height, image[0]->channels, image[0]->levels);

    for (int y = 0; y < image[0]->height; y++)
    {
        for (int x = 0; x < image[0]->width; x++)
        {
            int pos = y * image[0]->bytesperline + x * image[0]->channels;

    // Então gera a imagem final (brain_final.pgm). Vai a cada pixel da máscara e verifica se a intensidade é 0 ou 255. 
    // Se for 0, coloca o pixel da imagem brain_final.pgm também a zero. 
    // Se for 255, então coloca o pixel da imagem brain_final.pgm 
    // Igual ao pixel correspondente na imagem original (brain.pgm).

            if (image[6]->data[pos] == 0)
            {
                image[7]->data[pos] = 0;
            }
            else if (image[6]->data[pos] == 255)
            {
                image[7]->data[pos] = image[0]->data[pos];
            }
             
            {
                image[3]->data[pos] = image[2]->data[pos]; 
            }

        }
    }

    vc_write_image("Images/DesenvolvidasExercicios/brain_final.pgm", image[7]);

#pragma region Libertar memória

    vc_image_free(image[0]);
    vc_image_free(image[1]);
    vc_image_free(image[2]);
    vc_image_free(image[3]);
    vc_image_free(image[4]);
    vc_image_free(image[5]);
    vc_image_free(image[6]);
    vc_image_free(image[7]);

#pragma endregion

    return 0;
}