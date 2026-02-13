#ifndef JUEGO
#define JUEGO

typedef struct
{
    int nivelActual;
    int puntos;
    Uint32 tiempoInicio; //tiempo para el cronometro
    int dificultad; //0=Facil, 1=Normal, 2=Dificil
    char nombreJugador[4]; //tres caracteres como juegos retro: AAA
}ContextoJuego;

//esta estructura enum va a asignar valores a los distintos estados donde
//se encuentre el menu. por defecto ESTADO_MENU=0, ESTADO_DIFICULTAD=1 y asi
typedef enum
{
    ESTADO_MENU,
    ESTADO_DIFICULTAD,
    ESTADO_NOMBRE,
    ESTADO_JUGANDO,
    ESTADO_GANO
}EstadoJuego;

#endif // JUEGO
