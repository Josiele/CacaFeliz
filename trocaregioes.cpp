#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  Vec3b val;
  int width, height;
  int x=0,y=0;

  
  ///*****Abre a imagem colocada como parametro****///
  image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu a imagem, verifique se o nome esta correto" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  ///*****Armazena a largura e a altura da imagem aberta*****///
  width=image.size().width; 
  height=image.size().height;
  
  cout<<"largura:"<<width<<endl;
  cout<<"altura:"<<height<<endl;

  
  ///*****Cria uma nova matriz de mesma altura e largura que a imagem previamente aberta*****///
  Mat imagemtrocada(height, width, CV_LOAD_IMAGE_GRAYSCALE);


  ///*****Gera numero aleatorio*****///
  srand((unsigned)time(NULL));
  int numeroAleatorio= rand()%(height);
 

  //O algoritmo começa a percorrer a imagem original a partir de um ponto aleatorio
  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      x=(i+numeroAleatorio)%(height);//quando x for maior que a altura da imagem, ele começa a percorrer a imagem a partir da primeira linha novamente
      y=(j+numeroAleatorio)%(width); //quando y for maior que a largura da imagem, ele começa a percorrer a imagem a partir da primeira coluna novamente
      imagemtrocada.at<uchar>(i,j)=image.at<uchar>(x,y);  //a imagem trocada começa a ser desenhada a partir do ponto aleatorio    
    }
  }
  
  imshow("janela", imagemtrocada); //mostra a nova imagem 
  imwrite("Output.png",imagemtrocada); //salva a nova imagem em um novo arquivo
  waitKey();
  return 0;
}
