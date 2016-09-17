#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

Mat mask(5,5,CV_32F), mask1, frame32f, frameFiltered, result, tilt_shift_image;  

double alfa;
int centro_slider = 50;
int centro_slider_max = 100;

double l1=0, l2=0, altura=0.2;
int altura_slider=10;
int altura_slider_max=100;

double decaimento=1;

int dec_slider = 0;
int dec_slider_max = 100;

Mat image1, image2, blended;
Mat imageTop; 

char TrackbarName[50];

//funçao para o ajuste do centro do foco e para o calculo de alfa
void on_trackbar_centro(int, void*){
 double centro = (double) (centro_slider*image1.size().height)/centro_slider_max ; //calcula onde vai ser o centro do foco de acordo com o valor do slider
 l1 = centro - altura*image1.size().height; //calcula os parametros l1 e l2 a partir do centro
 l2 = centro + altura*image1.size().height;

 
 if(l1<0){ //caso l1 seja menor que 0, ele assume o valor zero
     l1=0;
 }
 if(l2>image1.size().height){ //caso l2 seja maior que a altura da imagem ele assume o valor da altura imagem
   l2 = image1.size().height;
 }

 for(int i=0; i<result.size().height ;i++){ //a formula eh calculada linha a linha
			alfa = 0.5*(tanh((i-l1)/(double)(decaimento+1.0))-tanh((i-l2)/(double)(decaimento+1.0)));
			addWeighted(image1.row(i), alfa, result.row(i), 1.0-alfa,0.0,image2.row(i));
	}
	imshow("Tiltshift",image2); //mostra a imagem na tela
	imwrite("Tiltshift.png",image2); //cria nova imagem
}


//função para ajustar a altura da região com foco
void on_trackbar_altura(int, void* ){
  altura= (double)altura_slider/altura_slider_max;
  on_trackbar_centro(centro_slider, 0);
}

//calcula o decaimento
void on_trackbar_decaimento(int, void*){
  decaimento=dec_slider;
  on_trackbar_centro(centro_slider, 0);
}



int main(int argvc, char** argv){
   
  ///*****Filtro Média*****///
  float media[] = {1,1,1,1,1,
                   1,1,1,1,1,
	           1,1,1,1,1,
                   1,1,1,1,1,
                   1,1,1,1,1}; //o filtro 3x3 não estava embaçando o suficiente pra criar um efeito bom
  
  
  mask = Mat(5, 5, CV_32F, media);  
  scaleAdd(mask, 1/25.0, Mat::zeros(5,5,CV_32F), mask1);
  swap(mask, mask1);


  image1 = imread("blend5.jpg"); //carrega imagem
  image1.copyTo(image2); //criar a copia da imagem1 que será embaçada



 //O filtro é aplicado varias vezes para criar um desfoque suficiente
        image2.convertTo(frame32f, CV_32F); 
	filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(2,2), 0);
	frame32f = frameFiltered;
	filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(2,2), 0);
	frame32f = frameFiltered;
	filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(2,2), 0);
        filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(2,2), 0);
	frame32f = frameFiltered;
	filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(2,2), 0);
	frame32f = frameFiltered;
	filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(2,2), 0);
	frameFiltered.convertTo(result, CV_8U);


  namedWindow("Tiltshift", 1);
  
  sprintf( TrackbarName, "Centro de foco x %d", centro_slider_max );
  createTrackbar( TrackbarName, "Tiltshift",
				  &centro_slider,
				  centro_slider_max,
				  on_trackbar_centro );

  on_trackbar_centro(centro_slider, 0 );
  
  sprintf( TrackbarName, "Decaimento x %d", dec_slider_max );
  createTrackbar( TrackbarName, "Tiltshift",
				  &dec_slider,
				  dec_slider_max,
				  on_trackbar_decaimento );

  on_trackbar_decaimento(dec_slider, 0 );

  sprintf( TrackbarName, "Altura do foco x %d", altura_slider_max );
  createTrackbar( TrackbarName, "Tiltshift",
				  &altura_slider,
				  altura_slider_max,
				  on_trackbar_altura );
  on_trackbar_altura(altura_slider, 0 );

  waitKey(0);
  return 0;
}
