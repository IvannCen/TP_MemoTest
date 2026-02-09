#ifndef TABLERO
#define TABLERO

#include "Comun.h"
#include "Carta.h"

typedef struct
{
    Carta* cartas; //puntero a la primera pos del vector de cartas
    int cantidad;   //cantidad de elementos del vec
    SDL_Texture* dorso; // el dorso de las cartas
    SDL_Texture* imagenes[CANTIDADIMAGENES]; //vector dinamico con las imagenes
    int cantidadImagenesCargadas; //cantidad de imagenes cargadas
    Carta* cartaSeleccionada; //dir de la carta seleccionada
}Tablero;

//inicio el tablero reservando memoria para la cantidad de cartas
void tableroIniciar(Tablero* t, int cant);

//destruyo el tablero, ya que se trata de un vector dinamico
void tableroDestruir(Tablero* t);

//relleno el trablero, configurando las cartas que contiene
void tableroRellenar(Tablero* t);

//dibujo todas las cartas de adentro del tablero
void tableroDibujar(Tablero* t, SDL_Renderer* render);

//funcion que maneja los clics en el tablero
void tableroClic(Tablero* t, int x, int y, SDL_Renderer* render);

//funcion que va a cargar las imagenes
void tableroCargarImagenes(Tablero* t, SDL_Renderer* render);

//funcion para mezclar las cartas en el tablero
void tableroMezclar(Tablero* t);

//////////////////////////////////////////////////////////////////
/////////Funciones mas avanzadas, mas alla del juego base/////////
//////////////////////////////////////////////////////////////////

//funcion para saber si todas las cartas fueron encontradas
int tableroCompleto(Tablero* t);

#endif // TABLERO
