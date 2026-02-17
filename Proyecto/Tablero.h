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
    int filas;
    int columnas;

    SDL_Texture* dorso; // el dorso de las cartas
    SDL_Texture* imagenes[CANTIDADIMAGENES]; //vector dinamico con las imagenes
    int cantidadImagenesCargadas; //cantidad de imagenes cargadas

    Carta* cartaSeleccionada; //dir de la carta seleccionada
    int rachaActual; //cant de aciertos consecutivos
    int movimientos; //cont de intentos para las estadisticas
    int parejasEncontradas; //cont para saber si gano

    int puntosPorImagen[CANTIDADIMAGENES]; //le asignamos valores aleatorios por cada figura

    int cursorX;
    int cursorY;
}Tablero;

////////////////////////////////////////////FUNCIONES QUE MANEJAN LA MEMORIA////////////////////////////////////////////

//inicio el tablero reservando memoria para la cantidad de cartas
void tableroIniciar(Tablero* t, Configuracion* config);
//destruyo el tablero, ya que se trata de un vector dinamico
void tableroDestruir(Tablero* t);

////////////////////////////////////////////FUNCIONES QUE MANEJAN LA LOGICA DEL JUEGO////////////////////////////////////////////

//relleno el trablero, configurando las cartas que contiene
void tableroRellenar(Tablero* t);
//funcion para saber si todas las cartas fueron encontradas
int tableroCompleto(Tablero* t);

//funcion que maneja los clics en el tablero
int tableroClic(Tablero* t, int x, int y, SDL_Renderer* render, ContextoJuego* juego);

//funcion que va a cargar las imagenes
void tableroCargarImagenes(Tablero* t, SDL_Renderer* render, int idSet);
//funcion para mezclar las cartas en el tablero
void tableroMezclar(Tablero* t);

//funcion para actualizar el estado de hover de las cartas segun la posicion del mouse
void tableroManejarHover(Tablero* t, int x, int y);

//funcion que maneja el teclado en el juego
void tableroManejarTeclado(Tablero* t, SDL_Event* e, SDL_Renderer* render, ContextoJuego* juego);

////////////////////////////////////////////FUNCIONES DE RENDERIZADO////////////////////////////////////////////

//dibujo todas las cartas de adentro del tablero
void tableroDibujar(Tablero* t, SDL_Renderer* render, int mouseX, int mouseY);
//funcion para dibujar las estadisticas por nivel
void dibujarEstadisticas(SDL_Renderer* render, TTF_Font* font, ContextoJuego* juego);
//dibujar el tablero del popup
void dibujarPopupSalidaJuego(SDL_Renderer* render, TTF_Font* font, int opcionSeleccionada);

////////////////////////////////////////////FUNCIONES DE INGRESO DE TEXTO POR PANTALLA////////////////////////////////////////////

//funcion para dibujar los textos directamente antes de escribir las variables
void dibujarTexto(SDL_Renderer* render, TTF_Font* font, const char* texto, int x, int y, SDL_Color color);

//funcion para dibujar los textos pero centrados en la pantalla
void dibujarTextoCentrados(SDL_Renderer* render, TTF_Font* font, const char* texto, int y, SDL_Color color);

#endif // TABLERO
