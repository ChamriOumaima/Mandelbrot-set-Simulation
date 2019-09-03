#include "gd.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

typedef struct {
  double r, i;
}complex;


gdImagePtr image;
int largeur, hauteur;
int noir;
int* couleurs;


double fractale(double centre_x, double centre_y, double zoom){

  complex origin, z, c, nouveau;
  double distance;
  int x, y;
  int couleur, exit;
  origin.r = centre_x;
  origin.i = centre_y;
  // définition de 100 thread qui vont dessiner l'ensemble
  omp_set_num_threads(100);
  #pragma omp parallel
{
#pragma omp for private(x,y,couleur,distance,z,c,nouveau,exit) schedule(dynamic)
  for (y = 0; y < hauteur; y++) {
    for (x = 0; x < largeur; x++) {

#pragma omp critical
// omp critical est utilisée pour que le bloc soit exécuté par un seul thread 
// Calcul le nombre complexe situé en (x,y)    
      c.r = (origin.r + ( (double) (x-(largeur/2)) / (double) largeur ) ) / zoom;
      c.i = (origin.i + ( (double) (y-(hauteur/2)) / (double) hauteur ) ) / zoom;
      z.r = z.i = 0.0; // Point de départ
      couleur = 0;  // le max d'itèrations c'est le nombre de couleur
      exit = 0;

      while( (couleur<256) && (exit==0) ){

        nouveau.r = z.r * z.r - z.i * z.i + c.r;
        nouveau.i = 2 * z.r * z.i + c.i;
        z.r = nouveau.r;
        z.i = nouveau.i;        
        distance = nouveau.r*nouveau.r + nouveau.i*nouveau.i;
        couleur++;

        if (distance > 4) {

          exit = 1;
        }
      }

      // Mise à jour de la couleur
    if (couleur<=256){
 			// le cas où la suite U diverge pour ce point (x,y) c a d il existe une distande > 4 
        gdImageSetPixel(image, x,y, couleurs[couleur]);

      }else{
      		// la suite converge donc toute les distances de ce point sont inf à 4 
        gdImageSetPixel(image, x,y, noir); 

      }

    }

  }
}

  return 0;

}
int main (int argc, char const *argv[]){

  char nom_fichier[50];
  FILE *fichier; 
  int n;
  largeur = 3000;
  hauteur = 3000;

// Création de l'image
  //gdImageCreate is called to create palette-based images, with no more than 256 colors.
image = gdImageCreate(largeur, hauteur);
noir = gdImageColorAllocate(image, 0, 0, 0);

// Création et allocation du tableau de couleurs

couleurs = (int*) malloc(256*sizeof(int));
for(n=0;n<42;n++){    
couleurs[n] = gdImageColorAllocate(image, 40, n * 4, 20);
couleurs[n + 42] = gdImageColorAllocate(image, (255 - n * 4), 255, 0);
couleurs[n + 84] = gdImageColorAllocate(image, 0, 255, n * 4);
couleurs[n + 126] = gdImageColorAllocate(image, 0, (255 - n * 4), 255);
couleurs[n + 168] = gdImageColorAllocate(image, n * 4, 0, 255);
couleurs[n + 210] = gdImageColorAllocate(image, 255, 0, (255 - n * 4));

}

// lancement de la partie dessin 
fractale(-0.1,0,0.4);

// enregistrement de l'image dans le fichier 
sprintf(nom_fichier,"Mandelbrot_%d.png",largeur);
fichier = fopen(nom_fichier, "wb");
gdImagePng(image, fichier);
fclose(fichier);
gdImageDestroy(image);
free(couleurs);

return(0);

}