/**
  * Apellido(s), nombre(s): Centurion Bisquert, Ivan Jose
  * DNI: 42200016
  * Entrega: Si
  *
  * Apellido(s), nombre(s): Pepek, Matias German
  * DNI: 45807109
  * Entrega: Si
  *
  * Apellido(s), nombre(s): Chiaravalle, Matias Ezequiel
  * DNI: 44714975
  * Entrega: Si
  *
  * Apellido(s), nombre(s): Cejas Nuñez, Nahuel Facundo
  * DNI: 45285201
  * Entrega: No
  */

#include "configuracion.h"
#include "juego.h"
#include "sonidos.h"
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        return 1;
    if(IMG_Init(IMG_INIT_PNG)==0)
        return 1;
    if(TTF_Init()==-1)
        return 1;

    sonidos_Inicializar();

    SDL_Window *window = SDL_CreateWindow(TITULO_VENTANA,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          ANCHO_VENTANA, ALTO_VENTANA,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!window)
        return 1;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
        return 1;

    SDL_RenderSetLogicalSize(renderer, ANCHO_VENTANA, ALTO_VENTANA);

    Juego_Ejecutar(window, renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    sonidos_Finalizar();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
