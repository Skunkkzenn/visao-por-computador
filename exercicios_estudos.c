#include <stdio.h>
#include <vc.h>
#define width 704
#define height 576


/* Declare, utilizando os tipos de dados da linguagem C, um
 array capaz de armazenar uma imagem em escala de
 cinzentos, 8bpp, com resolução 704x576. */

int main() {

    unsigned char imagem[width * height]; // 8bpp = 1 byte por pixel

    /* Utilizando ciclos for, atribua o valor 127 a todos os pixéis da
    imagem anterior. */

    for (int i = 0; i < height; i++)
    {   
        for (int j = 0; j < width; i++)
        {   // Primeira execução: imagem[0, 0] = 127;  Segunda execução: imagem[0, 1] = 127...
            // No caso obtemos a posição do pixel na imagem através da fórmula: i * WIDTH + j
            imagem[i * width + j] = 127;

            // Atribuir 0 aos pixeis da borda da imagem
            if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
            {
                imagem[i * width + j] = 0;
            }
        }

    }
    
    return 0;
}