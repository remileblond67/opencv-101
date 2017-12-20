#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int
main ()
{
  char key = ' ';
  // Flux WebCam
  VideoCapture webcamCapture(0);
  // Image capturée par la WebCam
  Mat webcamImage;
  // Nom de la fenêtre d'affichage de l'image de la WebCam
  char winWebCamName[] = "Image WebCam";

  // Ouverture du flux de la WebCam
  webcamCapture.open(0);
  if (!webcamCapture.isOpened())
  {
    std::cout << "Pas de flux vidéo. Merci de connecter une webcam" << std::endl ;
    return -1;
  }

  while (key != 'q' && key != 'Q')
    {
      webcamCapture >> webcamImage;
      webcamCapture.retrieve(webcamImage, CV_CAP_OPENNI_BGR_IMAGE);
      resize(webcamImage, webcamImage, Size(800,600));
      imshow(winWebCamName, webcamImage);
      key = cvWaitKey (5);
    }

  return 0;
}
