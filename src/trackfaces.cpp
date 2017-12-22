#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

String face_cascade_name = "res/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "res/haarcascade_eye_tree_eyeglasses.xml";
String window_name = "Capture - reconnaissance de visages";

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

/* Retourne l'image de la WebCam */
Mat lectureWebcam(VideoCapture capture);

/* Recherche de visages dans une image */
std::vector<Rect> rechercheVisages(Mat image);

/* Recherche des yeux dans une image de visage */
std::vector<Rect> rechercheYeux (Mat imageVisage);


int main (void) {
  // Flux WebCam
  VideoCapture webcamCapture(0);
  // Image capturée par la WebCam
  Mat webcamImage;
  // Image de présentation (avec texte et repérage)
  Mat presImage;
  // Image de chaque visage
  Mat imageVisage;

  std::vector<Rect> faces;
  std::vector<Rect> eyes;

  Scalar couleur_titre = Scalar(255,255,255);
  Scalar couleur_visage = Scalar(255,0,255);
  Scalar couleur_yeux = Scalar(255,0,0);

  int key;

  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
  if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

  // Ouverture du flux de la WebCam
  webcamCapture.open(0);
  if (!webcamCapture.isOpened())
  {
    std::cout << "Pas de flux vidéo. Merci de connecter une webcam" << std::endl ;
    return -1;
  }
  // Caracteristiques du flux de la WebCam
  printf("Résolution webcam : %0.f x %0.f à %0.f fps\n", webcamCapture.get(CV_CAP_PROP_FRAME_WIDTH), webcamCapture.get(CV_CAP_PROP_FRAME_HEIGHT), webcamCapture.get(CV_CAP_PROP_FPS));

  while (webcamCapture.grab()) {
    webcamImage = lectureWebcam(webcamCapture);
    webcamImage.copyTo(presImage);
    faces = rechercheVisages(webcamImage);
    // Pour chaque visage trouvé
    for (size_t i = 0; i < faces.size(); i++) {
      // Recherche des yeux dans le visage
      eyes = rechercheYeux(webcamImage(faces[i]));
      for( size_t j = 0; j < eyes.size(); j++ )
      {
        // Marquage des yeux
        Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
        int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
        circle( presImage, eye_center, radius, couleur_yeux, 4, 8, 0 );
      }
      // Marquage du visage
      Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
      Point txt_center( faces[i].x + faces[i].width/2, faces[i].y );
      Point pt1 (faces[i].x, faces[i].y );
      Point pt2 (faces[i].x+faces[i].width, faces[i].y + faces[i].height);
      Point pt_text (faces[i].x, faces[i].y-10 );

      putText(presImage, "Visage "+ std::to_string(i), pt_text, FONT_HERSHEY_SIMPLEX, 0.5, couleur_visage, 2);
      //ellipse( webcamImage, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, couleur_visage, 4, 8, 0);
      rectangle(presImage, pt1, pt2, couleur_visage, 4, 8, 0);
    }
    // Affichage dans l'image du nombre de visages détectés
    putText(presImage, "Nombre de visages : " + std::to_string (faces.size()), Point(10,25), FONT_HERSHEY_SIMPLEX, 1, couleur_titre, 2);
    // Affichage de l'image contenant les marquages
    imshow(window_name, presImage);
    key = waitKey(1);
    if( (char)key == 27 ) { break; } // escape
    //printf("Id image traitée : %0.f\n", webcamCapture.get(CV_CAP_PROP_POS_MSEC));
  }
  return 0;
}

/* Retourne l'image de la WebCam */
Mat lectureWebcam(VideoCapture capture) {
  Mat webcamImage;
  capture >> webcamImage;
  capture.retrieve(webcamImage, CV_CAP_OPENNI_BGR_IMAGE);
  return webcamImage;
}

/* Recherche de visages dans une image */
std::vector<Rect> rechercheVisages(Mat image) {
  std::vector<Rect> faces;
  Mat simpleImage;

  // Simplication de l'image
  cvtColor(image, simpleImage, CV_BGR2GRAY);
  equalizeHist(simpleImage, simpleImage);

  // Recherche de visages
  face_cascade.detectMultiScale(simpleImage, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, Size(30, 30), Size(200,200));

  return faces;
}

/* Recherche des yeux dans une image de visage */
std::vector<Rect> rechercheYeux (Mat imageVisage) {
  std::vector<Rect> eyes;
  eyes_cascade.detectMultiScale(imageVisage, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30,30));
  return eyes;
}
