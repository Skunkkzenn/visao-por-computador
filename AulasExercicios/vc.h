//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2022/2023
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    VISÃO POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#define VC_DEBUG


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct {
	unsigned char *data;
	int width, height;
	int channels;			// Binário/Cinzentos=1; RGB=3
	int levels;				// Binário=1; Cinzentos [1,255]; RGB [1,255]
	int bytesperline;		// width * channels
} IVC;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROTÓTIPOS DE FUNÇÕES
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// FUNCOES: ALOCAR E LIBERTAR UMA IMAGEM
IVC *vc_image_new(int width, int height, int channels, int levels);
IVC *vc_image_free(IVC *image);

// FUNCOES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
IVC *vc_read_image(char *filename);
int vc_write_image(char *filename, IVC *image);

int vc_gray_negative(IVC *image);
int vc_rgb_negative(IVC *srcdst);

int vc_rgb_to_hsv(IVC *srcdst);
int vc_hsv_segmentation(IVC *srcdst, int hmin, int hmax, int smin, int smax, int vmin, int vmax);

int vc_rgb_get_red_gray(IVC *image);
int vc_rgb_get_green_gray(IVC *srcdst);
int vc_rgb_get_blue_gray(IVC *srcdst);
int vc_rgb_get_red_to_gray(IVC *srcdst);

int vc_rgb_to_gray(IVC *src, IVC *dst);
int vc_scale_gray_to_rgb(IVC *src, IVC *dst);

//Binarios
int vc_gray_to_binary(IVC *src, IVC *dst, int threshold);
int vc_gray_to_binary_negative(IVC *src, IVC *dst, int threshold);
int vc_gray_to_binary_global_mean(IVC *srcdst);
int vc_gray_to_binary_midpoint(IVC *src, IVC *dst, int kernel);
int vc_gray_to_binary_bernsen(IVC *src, IVC *dst, int kernel, int cmin);
int vc_gray_to_binary_niblack(IVC *src, IVC *dst, int kernel);
int vc_binary_dilate(IVC *src, IVC *dst, int kernel);
int vc_binary_erode(IVC *src, IVC *dst, int kernel);

int vc_binary_open(IVC *src, IVC *dst, int kernel);
int vc_binary_close(IVC *src, IVC *dst, int kernel);
int vc_binary_blob_labelling(IVC *src, IVC *dst);