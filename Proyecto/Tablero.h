#ifndef TABLERO
#define TABLERO

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "configuracion.h"
#include "carta.h"
#include "juego.h"
#include "vector.h"

typedef struct
{
//    Carta* cartas;
//    int cantidad;
    Vector cartas; ///Reemplaza Carta* cartas e int cantidad
    Vector imagenes; ///Reemplaza el vector estatico de imagenes (SDL_Texture* imagenes[CANTIDAD_IMAGENES])
    int filas;
    int columnas;
    SDL_Texture* dorso;
//    SDL_Texture* imagenes[CANTIDAD_IMAGENES];
    SDL_Texture* texturaFondoJuego;
    int cantidadImagenesCargadas;
    Carta* cartaSeleccionada;
    int rachaActual;
    int movimientos;
    int parejasEncontradas;
    int cursorX;
    int cursorY;
}Tablero;

void tablero_Iniciar(Tablero* t, ConfiguracionJuego* config);
void tablero_Destruir(Tablero* t);
void tablero_Rellenar(Tablero* t, int filas, int columnas);
int tablero_Completo(Tablero* t);
int tablero_Clic(Tablero* t, int x, int y, SDL_Renderer* render, ContextoJuego* juego, TTF_Font* font);
void tablero_CargarImagenes(Tablero* t, SDL_Renderer* render, int idSet);
void tablero_Mezclar(Tablero* t);
void tablero_ManejarHover(Tablero* t, int x, int y);
void tablero_ManejarTeclado(Tablero* t, SDL_Event* e, SDL_Renderer* render, ContextoJuego* juego, TTF_Font* font);
void tablero_Dibujar(Tablero* t, SDL_Renderer* render, int mouseX, int mouseY);
void dibujarEstadisticas(SDL_Renderer* render, TTF_Font* font, ContextoJuego* juego);
void dibujarPopupSalida(SDL_Renderer* render, TTF_Font* font, const char* texto, int opcionSeleccionada);
void dibujarTexto(SDL_Renderer* render, TTF_Font* font, const char* texto, int x, int y, SDL_Color color);
void dibujarTextoCentrados(SDL_Renderer* render, TTF_Font* font, const char* texto, int y, SDL_Color color);

#endif // TABLERO
