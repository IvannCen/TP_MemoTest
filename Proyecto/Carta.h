#ifndef CARTA
#define CARTA

#include "Comun.h"
#include "Menu.h" //para conocer los niveles de dificultad

typedef struct
{
    int idImagen;   //cada imagen se identifica con un numero.
    int bocaArriba;     //0=boca abajo y 1=boca arriba.
    int encontrada;     //0=no encontrada y 1=encontrada(se hizo una pareja).
    SDL_Rect posicion;  //estructura que contiene las coordenadas de la carta.
}Carta;

//Funcion para inicializar una carta individual
void CartaInicial(Carta* c, int id, int x, int y, int w,int h);

//Funcion para dibujar la carta (renderizado de la carta)
void CartaDibujar(Carta* c, SDL_Renderer* render, SDL_Texture* textura);

//funcion que devuelve un 1 si el clic esta adentro de la coordenada x e y, 0 si no
int cartaAdentro(Carta* c, int x, int y);

//funcion que devuelve la cantidad de cartas segun el nivel de dificultad o nivel
int obtenerCartasPorDificultad(int dificultad);

//funcion para obtener la cantidad de cartas en el modo aventura por niveles
int obtenerCartasPorNivel(int nivel);

#endif // CARTA
