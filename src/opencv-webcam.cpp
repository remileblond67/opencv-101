#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int
main ()
{
  char key = ' ';
  // Flux WebCam
  CvCapture *webcamCapture;
  // Image capturée par la WebCam
  IplImage *webcamImage;
  // Nom de la fenêtre d'affichage de l'image de la WebCam
  char winWebCamName[] = "Image WebCam";

  webcamCapture = cvCreateCameraCapture (CV_CAP_ANY);

  if (!webcamCapture)
    {
      printf ("Impossible de récupérer le flux vidéo de la caméra !\n");
      return 1;
    }

  while (key != 'q' && key != 'Q')
    {
      webcamImage = cvQueryFrame (webcamCapture);
      cvShowImage (winWebCamName, webcamImage);
      key = cvWaitKey (5);
    }

  cvReleaseCapture(&webcamCapture);
  cvDestroyWindow(winWebCamName);
  return 0;
}
