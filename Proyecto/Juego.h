#ifndef JUEGO
#define JUEGO

#include <SDL2/SDL.h>
#include "configuracion.h"
#include "sonidos.h"

typedef enum
{
    ESTADO_MENU,
    ESTADO_CONFIGURACION,
    ESTADO_NOMBRE,
    ESTADO_JUGANDO,
    ESTADO_GANO,
    ESTADO_RANKING
}EstadoJuego;

typedef struct
{
    int filas;
    int columnas;
    int idSetImagenes;
    int cantJugadores;
}ConfiguracionJuego;

typedef struct
{
    int puntos[MAX_JUGADORES];
    Uint32 tiempoInicio;
    int turnoJugador;
    char nombreJugador[MAX_JUGADORES][MAX_LETRAS_NOMBRE];
    int cantJugadores;
    int confirmandoSalida;
    int opcionSalidaPopup;
    tSonido* sndSeleccion;
    tSonido* sndAcierto;
    tSonido* sndFallo;
    tMusica* musMenu;
    tMusica* musJuego;
}ContextoJuego;

int Juego_Ejecutar(SDL_Window* window, SDL_Renderer* renderer);

#endif // JUEGO
