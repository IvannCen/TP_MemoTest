#ifndef COMUN
#define COMUN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//dimensiones de la ventana
#define ANCHOVENTANA 1366
#define ALTOVENTANA 768

//dimensiones de las cartas
#define ANCHOCARTA 130
#define ALTOCARTA 170
#define SEPARACIONCARTA 15

//configuracion de la interfaz
#define ALTOINTERFAZ 80
#define INTERFAZMARGENLATERAL 30
#define INTERFAZMARGENSUPERIOR 20

//recursos
#define CANTIDADIMAGENES 10
#define FUENTE "fnt/jersey.ttf"
#define FUENTEGRANDE 100
#define FUENTEMEDIA 65
#define FUENTECHICA 30

//rutas para las imagenes
#define RUTASETA "img/A/"
#define RUTASETB "img/B/"
#define RUTADORSOA "img/A/dorsoA.png"
#define RUTADORSOB "img/B/dorsoB.png"

//tiempos de delay
#define DELAY 1000 //1000 milisegundos es 1 segundo
#define DELAYCHICO 500 //medio segundo

//limites
#define CANTJUGADORES 2
#define LETRASNOMBRE 4
#define ARCHIVOCONFIG "config.dat"

#endif // COMUN
