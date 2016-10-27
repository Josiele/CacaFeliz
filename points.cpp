#include <iostream>
#include <opencv2/opencv.hpp>

#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

int top_slider = 10;
int top_slider_max = 200;

//STEP define o passo usado para varrer a imagem de referência
int STEP = 1;
int STEP_max = 8;

int JITTER = 2;
int JITTER_max = 15;

int RAIO = 1;
int RAIO_max = 8;

Mat cannyPoints;
Mat image, border;

void on_trackbar_canny(int, void*){
  
  int width, height;
  int x, y, gray;

  Vec3b val;

  //arrays de índices que servirão para identificar elementos da imagem de referência
  vector<int> yrange;
  vector<int> xrange;

  width = image.size().width;
  height = image.size().height;

  Canny(image, border, top_slider, 3*top_slider);
  imshow("canny", border);

  if(STEP < 1){
   STEP = 1;
  }

  if(JITTER < 1){
   JITTER = 1;
  }


  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

//xrange e yrange são preenchidos com valores sequenciais iniciando em 0 e,
//em seguida, esses valores recebem um ganho igual a STEP e um deslocamento STEP/2

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  cannyPoints = Mat(height,width, CV_8UC3, Scalar(255,255,255));


  //a função a seguir é usada para embaralhar aleatoriamente
  random_shuffle(xrange.begin(), xrange.end());

  //Os loops a seguir servem para fazer as variáveis i e j assumirem,
  // a cada passada no loop, os valores dos arrays xrange e yrange de forma consecutiva.
  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){

      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);



      val[0] = (int)(image.at<Vec3b>(x,y)[2]);
      val[1] = (int)(image.at<Vec3b>(x,y)[1]);
      val[2] = (int)(image.at<Vec3b>(x,y)[0]);

//A funcao a seguir eh usada para tracar um circulo de raio
// especificado em um ponto determinado pelo usuario

      circle(cannyPoints,
             cv::Point(y,x),
             RAIO,
             CV_RGB(val[0],val[1],val[2]),
             -1,
             CV_AA);
    }
  }
  for(int i=0; i<height; i++ ){
        for(int j=0; j<width; j++){
           if(border.at<uchar>(i,j)>0){
		val[0] = (int)(image.at<Vec3b>(i,j)[2]);
                val[1] = (int)(image.at<Vec3b>(i,j)[1]);
                val[2] = (int)(image.at<Vec3b>(i,j)[0]);

              circle(cannyPoints,
                     cv::Point(j,i),
                     RAIO,
                     CV_RGB(val[0],val[1],val[2]),
                     -1,
                     CV_AA);
           }
        }
     }
  imshow("Exer. 11.1 - cannyPoints", cannyPoints);
}

int main(int argc, char**argv){

  image = imread(argv[1],CV_LOAD_IMAGE_COLOR);
 
  namedWindow("Exer. 11.1 - cannyPoints",1);

  cvMoveWindow( "Exer. 11.1 - cannyPoints", image.cols, 0 );


  createTrackbar("Threshold inferior", "Exer. 11.1 - cannyPoints",
                &top_slider,
                top_slider_max,
                on_trackbar_canny);

  createTrackbar("STEP", "Exer. 11.1 - cannyPoints",
                &STEP,
                STEP_max,
                on_trackbar_canny);

  createTrackbar("JITTER", "Exer. 11.1 - cannyPoints",
                &JITTER,
                JITTER_max,
                on_trackbar_canny);

  createTrackbar("RAIO", "Exer. 11.1 - cannyPoints",
                &RAIO,
                RAIO_max,
                on_trackbar_canny);


  on_trackbar_canny(top_slider, 0 );

  imwrite("Exer. 11.1 - cannyPoints.jpg", cannyPoints);

  waitKey();
  return 0;
}
