/*
    @author CEJAS-NUÑEZ-NAHUEL

    @date 2026-02-07

*/
/* LIBRERIAS SDL2, SDL2 IMAGE, SDL2 TTF, SDL2 MIXER */

#ifndef VISUAL_H

#define VISUAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
extern const SDL_Color COLOR_NEON_NARANJA;
extern const SDL_Color COLOR_FONDO_SEGA;
extern const SDL_Color COLOR_BLANCO;

/* DIMENSIONES DE LA PANTALLA */
#define ANCHO_PANTALLA 512
#define ALTO_PANTALLA 384
/*  ------------------------- */
#define MAX_FIGURAS 10
/* ESTRUCTURAS SDL */


typedef struct {
    SDL_Window* ventana;
    SDL_Renderer* renderizador;
    TTF_Font* fuenteArcade;
    SDL_Texture* figuras[10];
    Mix_Chunk* sfxClick;
    Mix_Chunk* sfxWinner;
    Mix_Chunk* sfxRacha;
    bool corriendo;
} ContextoSDL;


bool Inicializar_SDL(ContextoSDL *ctx);
void Cerrar_SDL(ContextoSDL* ctx);
bool Cargar_Assets(ContextoSDL* ctx, char *categoria);
void Escribir_Text(ContextoSDL* ctx, char*texto, int x, int y, SDL_Color color, bool centrado);
void ProbarSDL();


#endif

