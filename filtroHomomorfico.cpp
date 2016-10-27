#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

#define RADIUS 20

using namespace cv;
using namespace std;

Mat filter, tmp;
int dft_N, dft_M;
int gamah_slider = 20;
int gamah_slider_max = 50;

int gamal_slider = 5;
int gamal_slider_max = 100;

int c_slider = 5;
int c_slider_max = 100;

int d0_slider = 80;
int d0_slider_max = 1000;

char TrackbarName[50];

void on_trackbar_config(int, void*){
	int M,N;
	float D2, gamah, gamal, d0;
	M = dft_M;
	N = dft_N;
	gamah = gamah_slider/10.0;
	gamal = gamal_slider/10.0;
	d0 = d0_slider/10.0;

	tmp = Mat(dft_M, dft_N, CV_32F);
	for(int i=0; i<dft_M ;i++){
		for(int j=0; j<dft_N ;j++){
				D2 = ((float)i-M/2.0)*((float)i-M/2.0) + ((float)j-N/2.0)*((float)j-N/2.0);
				tmp.at<float>(i,j) = (gamah-gamal)*(1.0-exp(-1.0*(float)c_slider*(D2/(d0*d0))))+ gamal;
			}
	}
	imshow("Filtro.png",tmp);
  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);

}

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cÃ³pias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

int main(int argc , char** argv){
  VideoCapture cap;   
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, mag;
  Mat image, imagegray; 
  Mat_<float> realInput, zeros;
  vector<Mat> planos;

  // guarda tecla capturada
  char key;
	if(argc != 2){
		printf("ERRO\n");
		exit(-1);
	}
	image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE); // carrega a imagem

	log(realInput, realInput);
  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));
	
  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // a funÃ§Ã£o de transferÃªncia (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();	
	on_trackbar_config(1,0);
	
  for(;;){

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();

    // cria a compoente real
    realInput = Mat_<float>(padded); 

		// calcula o logarítmo da imagem
   //log(realInput, realInput);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);


    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
   // calcula a exponencial da imagem
    cv::exp(planos[0], planos[0]);

		
    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("filtrada", planos[0]);

    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!
 
  	sprintf( TrackbarName, "Gamah x %d", gamah_slider_max/10);
  	createTrackbar( TrackbarName, "filtrada",
				  &gamah_slider,
				  gamah_slider_max,
				  NULL); //funcao

	sprintf( TrackbarName, "Gamal x %d", gamal_slider_max/10);
  	createTrackbar( TrackbarName, "filtrada",
				  &gamal_slider,
				  gamal_slider_max,
				  NULL); //funcao

	sprintf( TrackbarName, "C x %d", c_slider_max/10);
  	createTrackbar( TrackbarName, "filtrada",
				  &c_slider,
				  c_slider_max,
				  NULL); //funcao

		sprintf( TrackbarName, "D0 x %d", d0_slider_max/10);
  	createTrackbar( TrackbarName, "filtrada",
				  &d0_slider,
				  d0_slider_max,
				  NULL); //funcao 

		on_trackbar_config(d0_slider,0);
  }
  return 0;
}

