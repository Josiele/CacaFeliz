#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histEq;
  int nbins = 256;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  Mat img_hist_equalized;

  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }
  
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw,  CV_BGR2GRAY, Scalar(0,0,0));
  Mat histImgEq(histh, histw,  CV_BGR2GRAY, Scalar(0,0,0));
  while(1){
    cap >> image;
    
    cvtColor(image, image, CV_BGR2GRAY); //change the color image to grayscale image

    calcHist(&image, 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    histImgR.setTo(Scalar(0));
    
     for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(255), 1, 8, 0);
      }

     ///***********EQUALIZA**************///
    equalizeHist(image, img_hist_equalized);
 
     ///*****Calcula o histograma da imagem equalizada*****///
    calcHist(&img_hist_equalized, 1, 0, Mat(), histEq, 1,
             &nbins, &histrange,
             uniform, acummulate);

     ///*****Normaliza o histograma da equalizada pra caber no numelo de rows de histImgEq******///
    normalize(histEq, histEq, 0, histImgEq.rows, NORM_MINMAX, -1, Mat());
    histImgEq.setTo(Scalar(0));
     
     for(int i=0; i<nbins; i++){
         line(histImgEq, Point(i, histh),
           Point(i, cvRound(histEq.at<float>(i))),
           Scalar(255), 1, 8, 0);
     }

    ///*****Desenha o histograma no frame da imagem original*****//
    histImgR.copyTo(image(Rect(0, 0,nbins, histh)));

    ///*****Desenha o histograma da imagem equalizada na nova imagem*****//
    histImgEq.copyTo(img_hist_equalized(Rect(0, 0,nbins, histh)));

    ///*****Exibe as duas imagens*****///
    imshow("image", image);
    imshow("img_hist_equalized", img_hist_equalized);
    
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
