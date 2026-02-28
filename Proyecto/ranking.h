#ifndef RANKING
#define RANKING

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "configuracion.h"

typedef struct
{
    char nombre[MAX_LETRAS_NOMBRE];
    int puntos;
    int tiempo;
}RegistroRanking;

void ranking_Guardar(const char* nombre, int puntos, int tiempo);
void ranking_Dibujar(SDL_Renderer* render, TTF_Font* fuenteTitulo, TTF_Font* fuenteLista, SDL_Texture* fondo);

#endif // RANKING
