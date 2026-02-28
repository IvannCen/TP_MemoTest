#ifndef CARTA
#define CARTA

#include <SDL2/SDL.h>
#include "configuracion.h"

typedef struct
{
    int idImagen;
    int bocaArriba;
    int encontrada;
    int hover;
    SDL_Rect posicion;
}Carta;

void carta_Inicializar(Carta* c, int id, int x, int y, int w, int h);
void carta_Dibujar(Carta* c, SDL_Renderer* render, SDL_Texture* textura, int esCursor);
int carta_EstaDentro(Carta* c, int x, int y);

#endif // CARTA
