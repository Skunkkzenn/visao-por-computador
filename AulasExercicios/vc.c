//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2022/2023
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    VIS�O POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita (no MSVC++) warnings de fun��es n�o seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "vc.h"
#include <math.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//             FUNÇÕES: ALOCAR E LIBERTAR UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Gera nova imagem
//  Alocar memória para uma imagem
IVC *vc_image_new(int width, int height, int channels, int levels)
{
	IVC *image = (IVC *)malloc(sizeof(IVC));

	if (image == NULL)
		return NULL;
	if ((levels <= 0) || (levels > 255))
		return NULL;

	image->width = width;
	image->height = height;
	image->channels = channels;
	image->levels = levels;
	image->bytesperline = image->width * image->channels;
	image->data = (unsigned char *)malloc(image->width * image->height * image->channels * sizeof(char));

	if (image->data == NULL)
	{
		return vc_image_free(image);
	}

	return image;
}

// Libertar memória de uma imagem
IVC *vc_image_free(IVC *image)
{
	if (image != NULL)
	{
		if (image->data != NULL)
		{
			free(image->data);
			image->data = NULL;
		}

		free(image);
		image = NULL;
	}

	return image;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    FUNÇÕES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

char *netpbm_get_token(FILE *file, char *tok, int len)
{
	char *t;
	int c;

	for (;;)
	{
		while (isspace(c = getc(file)))
			;
		if (c != '#')
			break;
		do
			c = getc(file);
		while ((c != '\n') && (c != EOF));
		if (c == EOF)
			break;
	}

	t = tok;

	if (c != EOF)
	{
		do
		{
			*t++ = c;
			c = getc(file);
		} while ((!isspace(c)) && (c != '#') && (c != EOF) && (t - tok < len - 1));

		if (c == '#')
			ungetc(c, file);
	}

	*t = 0;

	return tok;
}

long int unsigned_char_to_bit(unsigned char *datauchar, unsigned char *databit, int width, int height)
{
	int x, y;
	int countbits;
	long int pos, counttotalbytes;
	unsigned char *p = databit;

	*p = 0;
	countbits = 1;
	counttotalbytes = 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//*p |= (datauchar[pos] != 0) << (8 - countbits);

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				*p |= (datauchar[pos] == 0) << (8 - countbits);

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				*p = 0;
				countbits = 1;
				counttotalbytes++;
			}
		}
	}

	return counttotalbytes;
}

void bit_to_unsigned_char(unsigned char *databit, unsigned char *datauchar, int width, int height)
{
	int x, y;
	int countbits;
	long int pos;
	unsigned char *p = databit;

	countbits = 1;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				// datauchar[pos] = (*p & (1 << (8 - countbits))) ? 1 : 0;

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				datauchar[pos] = (*p & (1 << (8 - countbits))) ? 0 : 1;

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				countbits = 1;
			}
		}
	}
}

IVC *vc_read_image(char *filename)
{
	FILE *file = NULL;
	IVC *image = NULL;
	unsigned char *tmp;
	char tok[20];
	long int size, sizeofbinarydata;
	int width, height, channels;
	int levels = 255;
	int v;

	// Abre o ficheiro
	if ((file = fopen(filename, "rb")) != NULL)
	{
		// Efectua a leitura do header
		netpbm_get_token(file, tok, sizeof(tok));

		if (strcmp(tok, "P4") == 0)
		{
			channels = 1;
			levels = 1;
		} // Se PBM (Binary [0,1])
		else if (strcmp(tok, "P5") == 0)
			channels = 1; // Se PGM (Gray [0,MAX(level,255)])
		else if (strcmp(tok, "P6") == 0)
			channels = 3; // Se PPM (RGB [0,MAX(level,255)])
		else
		{
#ifdef VC_DEBUG
			printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM, PGM or PPM file.\n\tBad magic number!\n");
#endif

			fclose(file);
			return NULL;
		}

		if (levels == 1) // PBM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca mem�ria para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL)
				return NULL;

			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height;
			tmp = (unsigned char *)malloc(sizeofbinarydata);
			if (tmp == NULL)
				return 0;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			if ((v = fread(tmp, sizeof(unsigned char), sizeofbinarydata, file)) != sizeofbinarydata)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				free(tmp);
				return NULL;
			}

			bit_to_unsigned_char(tmp, image->data, image->width, image->height);

			free(tmp);
		}
		else // PGM ou PPM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &levels) != 1 || levels <= 0 || levels > 255)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PGM or PPM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca mem�ria para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL)
				return NULL;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			size = image->width * image->height * image->channels;

			if ((v = fread(image->data, sizeof(unsigned char), size, file)) != size)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				return NULL;
			}
		}

		fclose(file);
	}
	else
	{
#ifdef VC_DEBUG
		printf("ERROR -> vc_read_image():\n\tFile not found.\n");
#endif
	}

	return image;
}

int vc_write_image(char *filename, IVC *image)
{
	FILE *file = NULL;
	unsigned char *tmp;
	long int totalbytes, sizeofbinarydata;

	if (image == NULL)
		return 0;

	if ((file = fopen(filename, "wb")) != NULL)
	{
		if (image->levels == 1)
		{
			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height + 1;
			tmp = (unsigned char *)malloc(sizeofbinarydata);
			if (tmp == NULL)
				return 0;

			fprintf(file, "%s %d %d\n", "P4", image->width, image->height);

			totalbytes = unsigned_char_to_bit(image->data, tmp, image->width, image->height);
			printf("Total = %ld\n", totalbytes);
			if (fwrite(tmp, sizeof(unsigned char), totalbytes, file) != totalbytes)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				free(tmp);
				return 0;
			}

			free(tmp);
		}
		else
		{
			fprintf(file, "%s %d %d 255\n", (image->channels == 1) ? "P5" : "P6", image->width, image->height);

			if (fwrite(image->data, image->bytesperline, image->height, file) != image->height)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				return 0;
			}
		}

		fclose(file);

		return 1;
	}
	return 0;
}

// Gerar negativo da imagem Gray
int vc_gray_negative(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// Verificação de Erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL))
		return 0;
	if (channels != 1)
		return 0;

	// Inversão de imagem Gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			data[pos] = 255 - data[pos];
		}
	}
	return 1;
}

// Gerar negativo da imagem RGB
int vc_rgb_negative(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL))
		return 0;
	if (channels != 3)
		return 0;

	// inverte a imagem RGB
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 255 - data[pos];
			data[pos + 1] = 255 - data[pos + 1];
			data[pos + 2] = 255 - data[pos + 2];
		}
	}
	return 1;
}

// Extracao do vermelho de uma imagem
int vc_rgb_get_red_gray(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL))
		return 0;
	if (channels != 3)
		return 0;

	// Extrai componente red
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			// data[pos] = RED
			// data[pos + 1] = Green
			// data[pos + 2] = Blue
			data[pos + 1] = 0;
			data[pos + 2] = 0;
			// Ou então:
			// data[pos + 1] = data[pos];
			// data[pos + 2] = data[pos];
		}
	}
	return 1;
}

// Extração do verde de uma imagem
int vc_rgb_get_green_gray(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL))
		return 0;
	if (channels != 3)
		return 0;

	// Extrai componente red
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 0;
			data[pos + 2] = 0;
		}
	}
	return 1;
}

// Extração do azul de uma imagem
int vc_rgb_get_blue_gray(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL))
		return 0;
	if (channels != 3)
		return 0;

	// Extrai componente red
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 0;
			data[pos + 1] = 0;
		}
	}
	return 1;
}

// Gerar negativo da imagem RGB utilizando o vermelho
int vc_rgb_negative_red(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	// verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL))
		return 0;
	if (channels != 3)
		return 0;

	// inverte a imagem RGB utilizando o vermelho
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 255 - data[pos];
			data[pos + 1] = data[pos + 1];
			data[pos + 2] = data[pos + 2];
		}
	}
	return 1;
}

// Conversão de RGB para Gray
int vc_rgb_to_gray(IVC *src, IVC *dst)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf;

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 3) || (dst->channels != 1))
		return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			rf = (float)datasrc[pos_src];
			gf = (float)datasrc[pos_src + 1];
			bf = (float)datasrc[pos_src + 2];

			datadst[pos_dst] = (unsigned char)((rf * 0.229) + (gf * 0.587) + (bf * 0.114));
		}
	}
	return 1;
}

// Conversão de RGB para HSV
int vc_rgb_to_hsv(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;
	float r, g, b, hue, saturation, value;
	float rgb_max, rgb_min;
	int i, size;

	// Verificação de erros
	if ((width <= 0) || (height <= 0) || (data == NULL))
		return 0;
	if (channels != 3)
		return 0;

	size = width * height * channels;

	for (i = 0; i < size; i = i + channels)
	{
		r = (float)data[i];
		g = (float)data[i + 1];
		b = (float)data[i + 2];

		// Calcula valores máximo e mínimo dos canais de cor R, G e B
		rgb_max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
		rgb_min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

		// Value toma valores entre [0,255]
		value = rgb_max;
		if (value == 0.0f)
		{
			hue = 0.0f;
			saturation = 0.0f;
		}
		else
		{
			// Saturation toma valores entre [0,255]
			saturation = ((rgb_max - rgb_min) / rgb_max) * 255.0f;

			if (saturation == 0.0f)
			{
				hue = 0.0f;
			}
			else
			{
				// Hue toma valores entre [0,360]
				if ((rgb_max == r) && (g >= b))
				{
					hue = 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if ((rgb_max == r) && (b > g))
				{
					hue = 360.0f + 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if (rgb_max == g)
				{
					hue = 120.0f + 60.0f * (b - r) / (rgb_max - rgb_min);
				}
				else
				{
					hue = 240.0f + 60.0f * (r - g) / (rgb_max - rgb_min);
				}
			}
		}

		// Atribui valores entre [0,255]
		data[i] = (unsigned char)(hue / 360.0f * 255.0f);
		data[i + 1] = (unsigned char)(saturation);
		data[i + 2] = (unsigned char)(value);
	}

	return 1;
}

// hmin,hmax = [0, 360]; smin,smax = [0, 100]; vmin,vmax = [0, 100]
int vc_hsv_segmentation(IVC *srcdst, int hmin, int hmax, int smin, int smax, int vmin, int vmax)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;
	int h, s, v; // h=[30, 70] s=[50, 100] v=[60, 100]
	int i, size;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL))
		return 0;
	if (channels != 3)
		return 0;

	size = width * height * channels;

	for (i = 0; i < size; i = i + channels)
	{
		h = (int)(((float)data[i]) / 255.0f * 360.0f);
		s = (int)(((float)data[i + 1]) / 255.0f * 100.0f);
		v = (int)(((float)data[i + 2]) / 255.0f * 100.0f);

		if ((h > hmin) && (h <= hmax) && (s >= smin) && (s <= smax) && (v >= vmin) && (v <= vmax))
		{
			data[i] = 255;
			data[i + 1] = 255;
			data[i + 2] = 255;
		}
		else
		{
			data[i] = 0;
			data[i + 1] = 0;
			data[i + 2] = 0;
		}
	}
	return 1;
}

// Conversão de grayscale para RGB
int vc_scale_gray_to_rgb(IVC *src, IVC *dst)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int x, y;

	// Iterar através de cada pixel
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			unsigned char gray_value = datasrc[y * width + x]; // Direct array access for efficiency

			// Fixa o valor da escala de cinza na faixa de 0 a 255 (assumindo escala de cinza de 8 bits)
			gray_value = (gray_value < 0) ? 0 : (gray_value > 255) ? 255
																   : gray_value;

			// Cálculos de ponto flutuante para melhor precisão
			float g_factor = (float)(gray_value - 64) / 64.0f;
			float b_factor = (float)(gray_value - 192) / 64.0f;

			unsigned char r, g, b;

			if (gray_value <= 64)
			{
				r = 0;
				g = gray_value * 2.5f; // Cálculo mais preciso
				b = 255;
			}
			else if (gray_value <= 128)
			{
				r = 0;
				g = 255.0f;
				b = 255 - g_factor * 255.0f;
			}
			else if (gray_value <= 192)
			{
				r = g_factor * 255.0f;
				g = 255.0f;
				b = 0;
			}
			else
			{
				r = 255;
				g = 255 - b_factor * 255.0f;
				b = 0;
			}

			datadst[(y * width + x) * 3] = r; // Armazena valores RGB de forma eficiente
			datadst[(y * width + x) * 3 + 1] = g;
			datadst[(y * width + x) * 3 + 2] = b;
		}
	}

	return 1;
}

int vc_gray_to_binary(IVC *src, IVC *dst, int threshold)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int x, y;
	long int pos_src, pos_dst;

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 1) || (dst->channels != 1))
		return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			if (datasrc[pos_src] > threshold)
			{
				datadst[pos_dst] = 255;
			}
			else
			{
				datadst[pos_dst] = 0;
			}
		}
	}

	return 1;
}

int vc_gray_to_binary_negative(IVC *src, IVC *dst, int threshold)
{
	unsigned char *dataSrc = (unsigned char *)src->data;
	unsigned char *dataDst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->width * src->channels;
	int channels = src->channels;
	int x, y;
	long int pos;

	// verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if (channels != 1)
		return 0;

	// inverte a imagem gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			if (dataSrc[pos] > threshold)
			{
				dataDst[pos] = 0;
			}
			else
			{
				dataDst[pos] = 255;
			}
		}
	}

	return 1;
}

// threshold automatico pela media global dos pixeis
int vc_gray_to_binary_global_mean(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;
	int sum = 0;
	int mean = 0;

	// verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL))
		return 0;
	if (channels != 1)
		return 0;

	// inverte a imagem gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			sum += data[pos];
		}
	}

	mean = sum / (width * height);
	printf("Mean: %d\n", mean);

	// inverte a imagem gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			if (data[pos] > mean)
			{
				data[pos] = 255;
			}
			else
			{
				data[pos] = 0;
			}
		}
	}

	return 1;
}

int vc_gray_to_binary_midpoint(IVC *src, IVC *dst, int kernel)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int x, y, x2, y2;
	long int pos_src, pos_dst;
	float threshold = 0;
	int max, min;

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 1) || (dst->channels != 1))
		return 0;
	kernel *= 0.5;
	for (y = kernel; y < height - kernel; y++)
	{
		for (x = kernel; x < width - kernel; x++)
		{
			pos_dst = y * bytesperline_dst + x * channels_dst;

			for (max = 0, min = 255, y2 = y - kernel; y2 <= y + kernel; y2++)
			{
				for (x2 = x - kernel; x2 <= x + kernel; x2++)
				{
					pos_src = y2 * bytesperline_src + x2 * channels_src;
					if (datasrc[pos_src] > max)
					{
						max = datasrc[pos_src];
					}
					else if (datadst[pos_src] < min)
					{
						min = datasrc[pos_src];
					}
				}
			}
			threshold = 0.5 * (max + min);

			pos_src = y * bytesperline_src + x * channels_src;
			if (datasrc[pos_src] > threshold)
			{
				datadst[pos_dst] = 255;
			}
			else
			{
				datadst[pos_dst] = 0;
			}
		}
	}

	return 1;
}

int vc_gray_to_binary_bernsen(IVC *src, IVC *dst, int kernel, int cmin)
{
	unsigned char *data_src = (unsigned char *)src->data;
	unsigned char *data_dst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->width;
	int channels = src->channels;
	int x, y, kx, ky;
	int offset = (kernel - 1) / 2;
	long int pos, posk;
	unsigned char threshold, max, min, range;

	// Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL) || (dst->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (channels != 1))
		return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x; // Posição atual

			max = 0;
			min = 255;

			// Encontra o valor máximo e mínimo na vizinhança do kernel
			for (ky = -offset; ky <= offset; ky++)
			{
				for (kx = -offset; kx <= offset; kx++)
				{
					if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
					{
						posk = (y + ky) * bytesperline + (x + kx);
						if (data_src[posk] > max)
						{
							max = data_src[posk];
						}
						if (data_src[posk] < min)
						{
							min = data_src[posk];
						}
					}
				}
			}
			// Calcula o limiar com base no máximo e mínimo na vizinhança
			range = max - min;
			if (range <= cmin)
			{
				threshold = (min + max) / 2;
			}
			else
			{
				threshold = ((min + max) / 2) + (range / 2);
			}

			// Aplica a binarização
			data_dst[pos] = (data_src[pos] > threshold) ? 255 : 0;
		}
	}
	return 1;
}

int vc_gray_to_binary_niblack(IVC *src, IVC *dst, int kernel)
{
	unsigned char *data_src = (unsigned char *)src->data;
	unsigned char *data_dst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->width;
	int channels = src->channels;
	int x, y, kx, ky;
	int offset = (kernel - 1) / 2;
	long int pos, posk;
	float mean, stddev, threshold;

	// Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL) || (dst->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (channels != 1))
		return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x; // Posição atual

			mean = 0;
			stddev = 0;

			// Calcula a média da intensidade dos pixels na vizinhança do kernel
			for (ky = -offset; ky <= offset; ky++)
			{
				for (kx = -offset; kx <= offset; kx++)
				{
					if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
					{
						posk = (y + ky) * bytesperline + (x + kx);
						mean += data_src[posk];
					}
				}
			}
			mean /= kernel * kernel;

			// Calcula o desvio padrão da intensidade dos pixels na vizinhança do kernel
			for (ky = -offset; ky <= offset; ky++)
			{
				for (kx = -offset; kx <= offset; kx++)
				{
					if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
					{
						posk = (y + ky) * bytesperline + (x + kx);
						stddev += pow(data_src[posk] - mean, 2);
					}
				}
			}
			stddev = sqrt(stddev / (kernel * kernel));

			// Calcula o limiar usando a fórmula de Niblack
			threshold = mean + (0.2 * stddev);

			// Aplica a binarização
			data_dst[pos] = (data_src[pos] > threshold) ? 255 : 0;
		}
	}
	return 1;
}

int vc_binary_dilate(IVC *src, IVC *dst, int kernel)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int x, y, x2, y2;
	long int pos_src, pos_dst;
	int verifica;
	kernel = kernel / 2;

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 1) || (dst->channels != 1))
		return 0;

	for (y = kernel; y < height - kernel; y++)
	{
		for (x = kernel; x < width - kernel; x++)
		{
			pos_dst = y * bytesperline_dst + x * channels_dst;

			verifica = 0;

			for (y2 = y - kernel; y2 <= y + kernel; y2++)
			{
				for (x2 = x - kernel; x2 <= x + kernel; x2++)
				{
					pos_src = y2 * bytesperline_src + x2 * channels_src;
					if (datasrc[pos_src] == 255)
					{
						verifica = 1;
					}
				}
			}

			if (verifica == 1)
			{
				datadst[pos_dst] = 255;
			}
			else
			{
				datadst[pos_dst] = 0;
			}
		}
	}

	return 1;
}

int vc_binary_erode(IVC *src, IVC *dst, int kernel)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int x, y, x2, y2;
	long int pos_src, pos_dst;
	int verifica;
	kernel = kernel / 2;

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 1) || (dst->channels != 1))
		return 0;

	for (y = kernel; y < height - kernel; y++)
	{
		for (x = kernel; x < width - kernel; x++)
		{
			pos_dst = y * bytesperline_dst + x * channels_dst;

			verifica = 0;

			for (y2 = y - kernel; y2 <= y + kernel; y2++)
			{
				for (x2 = x - kernel; x2 <= x + kernel; x2++)
				{
					pos_src = y2 * bytesperline_src + x2 * channels_src;
					if (datasrc[pos_src] == 0)
					{
						verifica = 1;
					}
				}
			}

			if (verifica == 1)
			{
				datadst[pos_dst] = 0;
			}
			else
			{
				datadst[pos_dst] = 255;
			}
		}
	}

	return 1;
}

int vc_binary_open(IVC *src, IVC *dst, int kernel)
{
	int verifica = 1;
	IVC *dstTemp = vc_image_new(src->width, src->height, src->channels, src->levels);

	verifica &= vc_binary_erode(src, dstTemp, kernel);
	verifica &= vc_binary_dilate(dstTemp, dst, kernel);

	vc_image_free(dstTemp);

	return verifica;
}

int vc_binary_close(IVC *src, IVC *dst, int kernel)
{
	int verifica = 1;
	IVC *dstTemp = vc_image_new(src->width, src->height, src->channels, src->levels);

	verifica &= vc_binary_dilate(src, dstTemp, kernel);
	verifica &= vc_binary_erode(dstTemp, dst, kernel);

	vc_image_free(dstTemp);

	return verifica;
}

OVC *vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	long int i, size;
	int x, y, a, b; // variáveis auxiliares para percorrer a imagem pixel a pixel (x, y), a & b para percorrer tabela
	long int posX, posA, posB, posC, posD;
	int label = 1; // Etiqueta inicial.
	int labeltable[256] = {0};
	// int labelarea[256] = {0};
	int num, tmplabel;
	OVC *blobs; // Apontador para array de blobs

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 1) || (dst->channels != 1))
		return 0;

	// copiar dados da imagem binaria para imagem grayscale
	memcpy(datadst, datasrc, bytesperline * height);

	// Todos os pixéis de plano de fundo devem obrigatóriamente ter valor 0
	// Todos os pixéis de primeiro plano devem obrigatóriamente ter valor 255
	// Serão atribuídas etiquetas no intervalo [1,254]
	// Este algoritmo está assim limitado a 255 labels
	for (i = 0, size = bytesperline * height; i < size; i++)
	{
		if (datadst[i] != 0)
			datadst[i] = 255;
	}

	// Limpa os rebordos da imagem binária
	// for (y = 0; y<height; y++)
	// {
	// 	datadst[y * bytesperline + 0 * channels] = 0;
	// 	datadst[y * bytesperline + (width - 1) * channels] = 0;
	// }
	// for (x = 0; x<width; x++)
	// {
	// 	datadst[0 * bytesperline + x * channels] = 0;
	// 	datadst[(height - 1) * bytesperline + x * channels] = 0;
	// }

	// Efectua a etiquetagem
	for (y = 1; y < height - 1; y++)
	{
		for (x = 1; x < width - 1; x++)
		{
			// Kernel:
			// A B C
			// D X
			posA = (y - 1) * bytesperline + (x - 1) * channels; // A
			posB = (y - 1) * bytesperline + x * channels;		// B
			posC = (y - 1) * bytesperline + (x + 1) * channels; // C
			posD = y * bytesperline + (x - 1) * channels;		// D
			posX = y * bytesperline + x * channels;				// X

			// Se o pixel for marcado
			if (datadst[posX] != 0)
			{
				// Se A, B, C e D estão a zero
				// printf("Pixel (%d, %d) valor: %d\n", x, y, datadst[posX]); // debug
				if ((datadst[posA] == 0) && (datadst[posB] == 0) && (datadst[posC] == 0) && (datadst[posD] == 0))
				{
					datadst[posX] = label;
					labeltable[label] = label;
					label++;
				}
				else
				{
					num = 255;

					// Se A está marcado
					if (datadst[posA] != 0)
						num = labeltable[datadst[posA]];

					// Se B está marcado e B < num
					if ((datadst[posB] != 0) && (labeltable[datadst[posB]] < num))
						num = labeltable[datadst[posB]];

					// Se C está marcado e C < num
					if ((datadst[posC] != 0) && (labeltable[datadst[posC]] < num))
						num = labeltable[datadst[posC]];

					// Se D está marcado e D < num
					if ((datadst[posD] != 0) && (labeltable[datadst[posD]] < num))
						num = labeltable[datadst[posD]];

					// Atribui a etiqueta ao pixel
					datadst[posX] = num;
					labeltable[num] = num;

					// Actualiza a tabela de etiquetas
					if (datadst[posA] != 0)
					{
						if (labeltable[datadst[posA]] != num)
						{
							tmplabel = labeltable[datadst[posA]];
							for (a = 1; a < label; a++)
							{
								if (labeltable[a] == tmplabel)
									labeltable[a] = num;
							}
						}
					}
					if (datadst[posB] != 0)
					{
						if (labeltable[datadst[posB]] != num)
						{
							tmplabel = labeltable[datadst[posB]];
							for (a = 1; a < label; a++)
							{
								if (labeltable[a] == tmplabel)
									labeltable[a] = num;
							}
						}
					}
					if (datadst[posC] != 0)
					{
						if (labeltable[datadst[posC]] != num)
						{
							tmplabel = labeltable[datadst[posC]];
							for (a = 1; a < label; a++)
							{
								if (labeltable[a] == tmplabel)
									labeltable[a] = num;
							}
						}
					}
					if (datadst[posD] != 0)
					{
						if (labeltable[datadst[posD]] != num)
						{
							tmplabel = labeltable[datadst[posD]];
							for (a = 1; a < label; a++)
							{
								if (labeltable[a] == tmplabel)
									labeltable[a] = num;
							}
						}
					}
				}
			}
		}
	}

	// Volta a etiquetar a imagem
	for (y = 1; y < height - 1; y++)
	{
		for (x = 1; x < width - 1; x++)
		{
			posX = y * bytesperline + x * channels; // Posição actual

			if (datadst[posX] != 0)
			{
				datadst[posX] = labeltable[datadst[posX]];
				// labelarea[datadst[posX]]++;
			}
		}
	}

	// printf("Labels: %d\n", label);

	// Passo 1: Eliminar, da tabela, etiquetas repetidas
	for (a = 1; a < label - 1; a++)
	{
		for (b = a + 1; b < label; b++)
		{
			if (labeltable[a] == labeltable[b])
				labeltable[b] = 0;
		}
	}

	// Passo 2: Conta etiquetas e organiza a tabela de etiquetas, para que não hajam valores vazios (zero) entre etiquetas
	*nlabels = 0;
	for (a = 1; a < label; a++)
	{
		if (labeltable[a] != 0)
		{
			labeltable[*nlabels] = labeltable[a]; // Organiza a tabela de etiquetas
			(*nlabels)++;						  // Conta etiquetas
		}
	}

	// Se não existir blobs
	if (*nlabels == 0)
		return NULL;

	// Cria lista de blobs (objectos) e preenche a etiqueta
	blobs = (OVC *)calloc((*nlabels), sizeof(OVC));
	if (blobs != NULL)
	{
		for (a = 0; a < (*nlabels); a++)
			blobs[a].label = labeltable[a];
	}
	else
		return NULL;

	return blobs;
}

int vc_binary_blob_info(IVC *src, OVC *blobs, int nblobs)
{
	unsigned char *data = (unsigned char *)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, i;
	long int pos;
	int xmin, ymin, xmax, ymax;
	long int sumx, sumy;

	// Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if (channels != 1)
		return 0;

	// Conta área de cada blob
	for (i = 0; i < nblobs; i++)
	{
		xmin = width - 1;
		ymin = height - 1;
		xmax = 0;
		ymax = 0;

		sumx = 0;
		sumy = 0;

		blobs[i].area = 0;

		for (y = 1; y < height - 1; y++)
		{
			for (x = 1; x < width - 1; x++)
			{
				pos = y * bytesperline + x * channels;

				if (data[pos] == blobs[i].label)
				{
					// Área
					blobs[i].area++;

					// Centro de Gravidade
					sumx += x;
					sumy += y;

					// Bounding Box
					if (xmin > x)
						xmin = x;
					if (ymin > y)
						ymin = y;
					if (xmax < x)
						xmax = x;
					if (ymax < y)
						ymax = y;

					// Perímetro
					// Se pelo menos um dos quatro vizinhos não pertence ao mesmo label, então é um pixel de contorno
					if ((data[pos - 1] != blobs[i].label) || (data[pos + 1] != blobs[i].label) || (data[pos - bytesperline] != blobs[i].label) || (data[pos + bytesperline] != blobs[i].label))
					{
						blobs[i].perimeter++;
					}
				}
			}
		}

		// Bounding Box
		blobs[i].x = xmin;
		blobs[i].y = ymin;
		blobs[i].width = (xmax - xmin) + 1;
		blobs[i].height = (ymax - ymin) + 1;

		// Centro de Gravidade
		// blobs[i].xc = (xmax - xmin) / 2;
		// blobs[i].yc = (ymax - ymin) / 2;
		blobs[i].xc = sumx / MAX(blobs[i].area, 1);
		blobs[i].yc = sumy / MAX(blobs[i].area, 1);
	}

	return 1;
}

void draw_box(IVC *image, OVC blob)
{
	int x1 = blob.x, y1 = blob.y;									 // Canto superior esquerdo
	int x2 = blob.x + blob.width - 1, y2 = blob.y + blob.height - 1; // Canto inferior direito
	int width = image->width;
	int channels = image->channels;
	int levels = image->levels;

	/* Desenha caixa completa
	// // Iterar sobre as linhas da caixa
	// for (int y = y1; y <= y2; y++) {
	//     // Calcular o deslocamento para o início da linha
	//     int offset = y * width * channels;

	//     // Desenhar a linha superior
	//     for (int x = x1; x <= x2; x++) {
	//         // Ajustar o deslocamento para o pixel atual
	//         int pixel_offset = offset + x * channels;

	//         // Definir os valores dos canais de cor
	//         for (int c = 0; c < channels; c++) {
	//             // Por exemplo, para uma caixa vermelha:
	//             image->data[pixel_offset + c] = 255; // Vermelho
	//         }
	//     }
	// } */

#pragma region Desenhar caixa em formato de linhas

	// Desenha linhas horizontais
	for (int x = x1; x <= x2; x++)
	{
		// Desenhar a linha superior
		for (int c = 0; c < channels; c++)
		{
			image->data[(y1 * width + x) * channels + c] = levels - 1; // Branco
		}
		// Desenhar a linha inferior
		for (int c = 0; c < channels; c++)
		{
			image->data[(y2 * width + x) * channels + c] = levels - 1; // Branco
		}
	}

	// Desenha linhas verticais
	for (int y = y1; y <= y2; y++)
	{
		// Desenhar a linha esquerda
		for (int c = 0; c < channels; c++)
		{
			image->data[(y * width + x1) * channels + c] = levels - 1; // Branco
		}
		// Desenhar a linha direita
		for (int c = 0; c < channels; c++)
		{
			image->data[(y * width + x2) * channels + c] = levels - 1; // Branco
		}
	}

#pragma endregion

#pragma region Desenhar centro de massa em formato de cruz

	int xc = blob.xc, yc = blob.yc;
	int cross_size = 5;
	for (int i = -cross_size; i <= cross_size; i++)
	{
		// Desenhar linha horizontal
		for (int c = 0; c < channels; c++)
		{
			image->data[((yc + i) * width + xc) * channels + c] = 0; // Preto
		}
		// Desenhar linha vertical
		for (int c = 0; c < channels; c++)
		{
			image->data[(yc * width + xc + i) * channels + c] = 0; // preto
		}
	}
#pragma endregion
}

// Histograma para imagem grayscale
int vc_gray_histogram_show(IVC *src, IVC *dst)
{
	unsigned char *data = (unsigned char *)src->data;
	unsigned char *dataDst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y;
	long int pos;
	int histogram[256] = {0};
	int max = 0;
	int histogram_width = 256; // Largura do histograma

	// Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if (channels != 1)
		return 0;

	// Calcular o histograma
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			histogram[data[pos]]++;
		}
	}

	// normalizar o histograma
	for (int i = 0; i < 256; i++)
	{
		if (histogram[i] > max)
			max = histogram[i];
	}

	// Desenhar o histograma
	int histogram_offset = (width - histogram_width) / 2; // Calcular o deslocamento para centrar o histograma
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			dataDst[pos] = 0; // Branco
		}
	}

	// Desenhar o histograma centrado
	for (x = 0; x < 256; x++)
	{
		int hist_height = (histogram[x] * height) / max;
		for (y = height - 1; y >= height - hist_height; y--)
		{
			pos = y * bytesperline + (x + histogram_offset) * channels;
			dataDst[pos] = 255; // Preto
		}
	}

	// Desenhar linhas verticais nas bordas do histograma
	for (y = 0; y < height; y++)
	{
		// Linha vertical esquerda
		pos = y * bytesperline + histogram_offset * channels;
		dataDst[pos] = 128; // Cinza (outra cor desejada)
		// Linha vertical direita
		pos = y * bytesperline + (histogram_offset + histogram_width - 1) * channels;
		dataDst[pos] = 128; // Cinza (outra cor desejada)
	}

	return 1;
	/*
		// Desenhar o histograma
		// for (y = 0; y<height; y++)
		// {
		// 	for (x = 0; x<width; x++)
		// 	{
		// 		pos = y * bytesperline + x * channels;
		// 		dataDst[pos] = 0; // Branco
		// 	}
		// }

		// Desenhar o histograma
		// for (x = 0; x<256; x++)
		// {
		// 	int hist_height = (histogram[x] * height) / max;
		// 	for (y = height - 1; y >= height - hist_height; y--)
		// 	{
		// 		pos = y * bytesperline + x * channels;
		// 		dataDst[pos] = 255; // Preto
		// 	}
		// }
	*/
}

int vc_gray_histogram_equalization(IVC *src, IVC *dst)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int x, y;
	float min;
	long int pos_src, pos_dst;
	int posicoes[256] = {0};
	float pdf[256] = {0}, cdf[256] = {0};
	float size = width * height;

	for (x = 0; x < height * width; x++)
	{
		posicoes[datasrc[x]]++;
	}

	for (x = 0; x <= 255; x++)
	{
		pdf[x] = (((float)posicoes[x]) / size);
	}

	for (cdf[0] = pdf[0], x = 1; x <= 255; x++)
	{
		cdf[x] = cdf[x - 1] + pdf[x];
	}

	for (min = 0, x = 0; x <= 255; x++)
	{
		if (cdf[x] != 0)
		{
			min = pdf[x];
			break;
		}
	}

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_dst = y * bytesperline_src + x * channels_src;
			datadst[pos_dst] = ((cdf[datasrc[pos_dst]] - min) / (1.0 - min)) * 255.0;
		}
	}
	return 1;
}

int vc_gray_edge_prewitt(IVC *src, IVC *dst, float threshold) {
	unsigned char *data = (unsigned char *)src->data;
	int width = src->width;
	int height = src->height;
	int byteperline = src->width*src->channels;
	int channels = src->channels;
	int x, y;
	long int pos;
	long int posA, posB, posC, posD, posE, posF, posG, posH;
	double mag, mx, my;

	if ((width <= 0) || (height <= 0) || (src->data == NULL)) return 0;
	if (channels != 1) return 0;

	for (y = 1; y < height; y++)
	{
		for (x = 1; x < width; x++)
		{
			pos = y * byteperline + x * channels;

			posA = (y - 1)* byteperline + (x - 1) * channels;
			posB = (y - 1)* byteperline + (x)* channels;
			posC = (y - 1)* byteperline + (x + 1)* channels;
			posD = (y)* byteperline + (x - 1)* channels;
			posE = (y)* byteperline + (x + 1)* channels;
			posF = (y + 1)* byteperline + (x - 1)* channels;
			posG = (y + 1)* byteperline + (x)* channels;
			posH = (y + 1)* byteperline + (x + 1)* channels;
			mx = ((-1 * data[posA]) + (1 * data[posC]) + (-1 * data[posD]) + (1 * data[posE]) + (-1 * data[posF]) + (1 * data[posH])) / 3;
			my = ((-1 * data[posA]) + (1 * data[posF]) + (-1 * data[posB]) + (1 * data[posG]) + (-1 * data[posC]) + (1 * data[posH])) / 3;

			mag = sqrt((mx*mx) + (my * my)) *  (1 / sqrt(2));

			if (mag > threshold)
				dst->data[pos] = 255;
			else
				dst->data[pos] = 0;
		}
	}
	return 1;
}

int vc_gray_edge_sobel(IVC *src, IVC *dst, float threshold)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int x, y, kx, ky;
	int offset = 1;
	long int pos, posA, posB, posC, posD, posE, posF, posG, posH;
	float mx, my, mag;											  

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;
	if ((src->width != dst->width) || (src->height != dst->height))
		return 0;
	if ((src->channels != 1) || (dst->channels != 1))
		return 0;

	for (y = offset; y < height - offset; y++)
	{
		for (x = offset; x < width - offset; x++) 
		{
			pos = y * bytesperline_dst + x * channels_dst; 

			posA = (y - 1) * bytesperline_src + (x - 1) * channels_src;
			posB = (y - 1) * bytesperline_src + x * channels_src;
			posC = (y - 1) * bytesperline_src + (x + 1) * channels_src;
			posD = y * bytesperline_src + (x - 1) * channels_src;
			posE = y * bytesperline_src + (x + 1) * channels_src;
			posF = (y + 1) * bytesperline_src + (x - 1) * channels_src;
			posG = (y + 1) * bytesperline_src + x * channels_src;
			posH = (y + 1) * bytesperline_src + (x + 1) * channels_src;

			mx = ((-1 * datasrc[posA]) + (1 * datasrc[posC]) + (-2 * datasrc[posD]) + (2 * datasrc[posE]) + (-1 * datasrc[posF]) + (1 * datasrc[posH])) / 3;
			my = ((-1 * datasrc[posA]) + (1 * datasrc[posF]) + (-2 * datasrc[posB]) + (2 * datasrc[posG]) + (-1 * datasrc[posC]) + (1 * datasrc[posH])) / 3;

			mag = sqrt((mx * mx) + (my * my)) * (1 / sqrt(2));

			datadst[pos] = (mag > threshold) ? 255 : 0;
		}
	}
	return 1;
}