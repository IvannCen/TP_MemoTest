#ifndef TABLERO
#define TABLERO

#include "Comun.h"
#include "Carta.h"
#include "Juego.h"
#include "sonidos.h"

//hay que revisar la estructura, ya que tiene muchas cosas, que se podrian dividir en estructuras aparte y hacer puntero a estructura
//igual con el tema de las configuraciones y eso del menu y rankings hay que verlo para mandarlo al header correspondiente

/*
typedef struct
{
    Carta* cartas; //puntero a la primera pos del vector de cartas
    int cantidad;   //cantidad de elementos del vec
    SDL_Texture* dorso; // el dorso de las cartas
    SDL_Texture* imagenes[CANTIDADIMAGENES]; //vector dinamico con las imagenes
    int cantidadImagenesCargadas; //cantidad de imagenes cargadas
    Carta* cartaSeleccionada; //dir de la carta seleccionada

    // --- PUNTUACION / RACHAS ---
    int puntaje;
    int racha; // cantidad de aciertos consecutivos
    int puntosPorImagen[CANTIDADIMAGENES]; // puntos base por pareja (idImagen)

    // --- CONFIGURACION DE PARTIDA (para menu config) ---
    int filas;       // 3 o 4
    int cols;        // 4 o 5
    int setFiguras;  // 0 = A, 1 = B

    // --- ESTADISTICAS ---
    int intentos;     // cada vez que comparas 2 cartas
    int aciertos;     // parejas acertadas
    int fallos;       // parejas falladas
    Uint32 inicioMs;  // SDL_GetTicks() al iniciar la partida
    Uint32 finMs;    // tiempo al terminar
int terminado;   // 0 jugando, 1 termino

} Tablero;

*/

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
int tableroClic(Tablero* t, int x, int y, SDL_Renderer* render, ContextoJuego* juego);

//funcion que va a cargar las imagenes
void tableroCargarImagenes(Tablero* t, SDL_Renderer* render);

//funcion para mezclar las cartas en el tablero
void tableroMezclar(Tablero* t);

//////////////////////////////////////////////////////////////////
/////////Funciones mas avanzadas, mas alla del juego base/////////
//////////////////////////////////////////////////////////////////

//funcion para saber si todas las cartas fueron encontradas
int tableroCompleto(Tablero* t);

//despues revisar las funciones porque menu va en otro header para mayor comodidad, y lo de rachas y puntuacion hay que revisar

/*
// ---PUNTUACION ---
int tableroGetPuntaje(Tablero* t);
int tableroGetRacha(Tablero* t);
// bonus: 0.0 si racha=0, 0.2 si racha=1, 0.3 si racha=2, ...
float tableroGetBonusRacha(Tablero* t);

// --- CONFIGURACION (menu) ---
void tableroSetDimensiones(Tablero* t, int filas, int cols);
void tableroSetSetFiguras(Tablero* t, int setFiguras);

// --- ESTADISTICAS ---
int tableroGetIntentos(Tablero* t);
int tableroGetAciertos(Tablero* t);
int tableroGetFallos(Tablero* t);
int tableroGetTiempoSegundos(Tablero* t);

#endif // TABLERO

*/
//funcion para dibujar las estadisticas por nivel
void dibujarEstadisticas(SDL_Renderer* render, TTF_Font* font, ContextoJuego* juego);

//funcion para dibujar los textos directamente antes de escribir las variables
void dibujarTexto(SDL_Renderer* render, TTF_Font* font, const char* texto, int x, int y, SDL_Color color);

//funcion para dibujar los textos pero centrados en la pantalla
void dibujarTextoCentrados(SDL_Renderer* render, TTF_Font* font, const char* texto, int y, SDL_Color color);
#endif // TABLERO

//funcion para dibujar un texto centrado matemáticamente dentro del area especificada
//void dibujarTextoCentradoEnRect(SDL_Renderer* render, TTF_Font* font, const char* texto, SDL_Rect area, SDL_Color color);
