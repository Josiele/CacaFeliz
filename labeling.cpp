#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects=0;
  int comburaco=0, semburaco=0;

  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  //Eliminar bolhas da borda
  //Primeiro for elimina as bolhas das bordas de cima e de baixo
  for(int i=0; i<height; i=i+(height-1)){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
                p.x=j;
                p.y=i;
                floodFill(image,p,0);
          }
      }
  }
  //Segundo for elimina as bolhas da borda da esquerda
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j=j+width){
      if(image.at<uchar>(i,j) == 255){
                p.x=j;
                p.y=i;
                floodFill(image,p,0);
          }
      }
    //Terceiro for elimina as bolhas da borda da direita
    for(int k=width-1; k<width; k=k+width){
      if(image.at<uchar>(i,k) == 255){
                p.x=k;
                p.y=i;
                floodFill(image,p,0);
          }
      }
  }
  //final de tirar as bolhas das bordas

  int count=0;//Acro que esse contador eh a resposta da primeira pergunta da secao 4

  //busca objetos com e sem buracos presentes(desconsiderando os das bordas) mantendo a cor das bolhas
  nobjects=200;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		// achou um objeto
		nobjects++;
		p.x=j;
		p.y=i;
                floodFill(image,p,nobjects);
                count=count+(nobjects-200);
                nobjects=200;
	  }
	}
  }
  cout << "Total de bolhas: "<< count <<"\n";


  //mudar a cor do fundo
  p.x=0;
  p.y=0;
  floodFill(image,p,100);

  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){
                // achou um objeto
		if(image.at<uchar>(i,j-1)!=50) { //se o pixel anterior for tiver label 50 significa que ele ja tinha contado um buraco antes
			nobjects++;
               		p.x=j-1; //pega opixel anterior que pertence a bolha
                        p.y=i;
                        floodFill(image,p,50); //as bolhas que tiverem buraco vao ter a cor 50.
		}
		p.x=j;
		p.y=i;
                floodFill(image,p,1); //Os buracos terao cor 1
          }
        }
  }
  comburaco=nobjects;
  semburaco=count-comburaco;
  cout << "Bolhas com buraco: "<< comburaco <<"\n";
  cout << "Bolhas sem burracos: "<< semburaco <<"\n";

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
