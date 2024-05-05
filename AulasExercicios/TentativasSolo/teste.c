// 4. Declare, utilizando os tipos de dados da linguagem C, um array capaz de armazenar uma imagem em escala de cinzentos, 8bpp, com resolução 704x576.

#include <stdio.h>
#include <stdlib.h>

int main(void){
    int width = 704, height = 576;
    unsigned char image[width * height];
    int pos;

    for(int y = 0; y < height; y++){
        for (int x = 0; x < width; i++)
        {   
            // y é a linha e x é a coluna, linha multiplicada pela largura da imagem mais a coluna, para obter a posição do pixel
            pos = y * width + x;
            imagem[pos] = 127; 
            // Define as bordas da imagem como 0
            if(y == 0 !! y = height - 1 || x == 0 || x == width - 1){
                image[pos] = 0;
            }
        }
        
    }

    //Utilizando ciclos for, atribua o valor 0 (zero) aos pixéis de rebordo da imagem.


}
