#ifndef TABLERO
#define TABLERO

#include "Comun.h"
#include "Carta.h"
#include "Juego.h"
#include "sonidos.h"

typedef struct
{
    Carta* cartas; //puntero a la primera pos del vector de cartas
    int cantidad;   //cantidad de elementos del vec
    SDL_Texture* dorso; // el dorso de las cartas
    SDL_Texture* imagenes[CANTIDADIMAGENES]; //vector dinamico con las imagenes
    int cantidadImagenesCargadas; //cantidad de imagenes cargadas
    Carta* cartaSeleccionada; //dir de la carta seleccionada
}Tablero;

//funciones que manejan la memoria
//inicio el tablero reservando memoria para la cantidad de cartas
void tableroIniciar(Tablero* t, int cant);
//destruyo el tablero, ya que se trata de un vector dinamico
void tableroDestruir(Tablero* t);

//funciones que manejan la logica del juego
//relleno el trablero, configurando las cartas que contiene
void tableroRellenar(Tablero* t);
//funcion para saber si todas las cartas fueron encontradas
int tableroCompleto(Tablero* t);
//funcion que maneja los clics en el tablero
int tableroClic(Tablero* t, int x, int y, SDL_Renderer* render, ContextoJuego* juego);
//funcion que va a cargar las imagenes
void tableroCargarImagenes(Tablero* t, SDL_Renderer* render);
//funcion para mezclar las cartas en el tablero
void tableroMezclar(Tablero* t);

//funciones de renderizado
//dibujo todas las cartas de adentro del tablero
void tableroDibujar(Tablero* t, SDL_Renderer* render);
//funcion para dibujar las estadisticas por nivel
void dibujarEstadisticas(SDL_Renderer* render, TTF_Font* font, ContextoJuego* juego);

//funciones de ingreso de texto por pantalla
//funcion para dibujar los textos directamente antes de escribir las variables
void dibujarTexto(SDL_Renderer* render, TTF_Font* font, const char* texto, int x, int y, SDL_Color color);
//funcion para dibujar los textos pero centrados en la pantalla
void dibujarTextoCentrados(SDL_Renderer* render, TTF_Font* font, const char* texto, int y, SDL_Color color);
#endif // TABLERO

//funcion para dibujar un texto centrado matemáticamente dentro del area especificada
//void dibujarTextoCentradoEnRect(SDL_Renderer* render, TTF_Font* font, const char* texto, SDL_Rect area, SDL_Color color);
