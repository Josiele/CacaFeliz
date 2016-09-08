#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;
  int x1, x2, y1, y2;

  image= imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu bolhas.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  cout<<"Insira as coordenadas do ponto P1(x1,y1): ";
  cin>>x1>>y1;
  cout<<"Insira as coordenadas do ponto P2(x2,y2): ";
  cin>>x2>>y2;

  if(x1<x2 && y1<y2){
      for(int i=x1;i<x2;i++){
        for(int j=y1;j<y2;j++){
          image.at<uchar>(i,j)=255- image.at<uchar>(i,j);
        }
      }
  }else if(x1>x2 && y1<y2){
      for(int i=x2;i<x1;i++){
        for(int j=y1;j<y2;j++){
          image.at<uchar>(i,j)=255- image.at<uchar>(i,j);
        }
      }
  }else if(x1<x2 && y1>y2){
      for(int i=x1;i<x2;i++){
        for(int j=y2;j<y1;j++){
          image.at<uchar>(i,j)=255- image.at<uchar>(i,j);;
        }
      }
  }else{
      for(int i=x2;i<x1;i++){
        for(int j=y2;j<y1;j++){
          image.at<uchar>(i,j)=255-image.at<uchar>(i,j);
        }
      }
  }

  imshow("janela", image);  
  waitKey();

  return 0;
}
