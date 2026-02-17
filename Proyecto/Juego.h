#ifndef JUEGO
#define JUEGO

#include <SDL2/SDL.h>
#include "sonidos.h"
#include "Comun.h"

typedef struct
{
    int filas;
    int columnas;
    int idSetImagenes;
    int cantJugadores;
}Configuracion;

typedef struct
{
    int puntos[CANTJUGADORES];
    Uint32 tiempoInicio; //tiempo para el cronometro
    int turnoJugador; //para el modo competitivo
    char nombreJugador[CANTJUGADORES][LETRASNOMBRE]; //tres caracteres como juegos retro: AAA
    int cantJugadores;

    int confirmandoSalida; //0=jugando, 1=mostrar popup "¿Desea Salir?"
    int opcionSalidaPopup; //0=SI, 1=NO

    //Sonidos, efectos cortos
    tSonido* sndSeleccion;
    tSonido* sndAcierto;
    tSonido* sndFallo;

    //Musica
    tMusica* musMenu;
    tMusica* musJuego;
}ContextoJuego;

//esta estructura enum va a asignar valores a los distintos estados donde
//se encuentre el menu. por defecto ESTADO_MENU=0, ESTADO_DIFICULTAD=1 y asi
typedef enum
{
    ESTADO_MENU,
    ESTADO_CONFIGURACION,
    ESTADO_NOMBRE,
    ESTADO_JUGANDO,
    ESTADO_GANO,
    ESTADO_RANKING
}EstadoJuego;

#endif // JUEGO
