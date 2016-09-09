#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histOld;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

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
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));

  double comparold=0;

  cap >> image;
  split (image, planes);
  calcHist(&planes[0], 1, 0, Mat(), histOld, 1,
           &nbins, &histrange,
           uniform, acummulate);

  while(1){
    cap >> image;
    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);  

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
    }

    double compar = compareHist(histR, histOld, CV_COMP_INTERSECT);

    //CV_COMP_INTERSECT
    if(compar>(comparold+50) || compar<(comparold-50)){
        cout<<"Moved"<<endl;
    }

    histImgR.copyTo(image(Rect(0, 0,nbins, histh)));

    imshow("image", image);
    if(waitKey(30) >= 0) break;

    histOld=histR;
    comparold=compar;
  }
  return 0;
}
