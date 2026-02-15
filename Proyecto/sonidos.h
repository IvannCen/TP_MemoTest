#ifndef SONIDOS_H_INCLUDED
#define SONIDOS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define FREC_MUESTREO 44100

#define SELECCION   "snd/click.mp3"
#define ACIERTO     "snd/winner.mp3"
#define FALLO       "snd/racha.mp3"   //CARGAR SONIDOS, ESTOS LOS USAMOS DE PRUEBA

//Formatos de audio soportados
typedef enum
{
    SONIDO_ERR = -1,
    SONIDO_WAV =  0,
    SONIDO_MP3 =  1,
    SONIDO_OGG =  2,
} tFormatosSnd;

//Se usa Mix_Chunk porque son efectos cortos cargados desde la unidad de almacenamiento
typedef struct
{
    Mix_Chunk* chunk;
    //int esTono;   //Por ahora no vamos a generar ningún tono generado por buffer de audio, pero lo dejo en caso de que cambiemos de opinion
} tSonido;

tFormatosSnd sonidos_inicializar(void);

/*
    IMPORTANTE!!!!: los sonidos deberian estar dentro de una estructura tJuego definida en la logica de "juego.h"
    donde tJuego es la estructura principal del juego que contiene el estado del mismo y sus recursos (para este caso,
    deberia tener los campos tSonido* seleccion, tSonido* acierto y tSonido* fallo

    (VER JUEGO.H EN SDL_EJEMPLO_AVANZADO)
*/
//Carga un archivo de audio desde el almacenamiento
tSonido* sonidos_cargar(const char *path);

//Reproduce un sonido "n" veces, determinado por el parametro cantVeces
void sonidos_reproducir(const tSonido *sonido, int cantVeces); //(const tSonido *sonido, int32_t cantVeces)

//Libera memoria asociada al sonido
void sonidos_destruir(tSonido *sonido);

//Finaliza correctamente SDL_mixer, cerrando primero los canales de audio, luego cerrando el mixer y finalmente desinicializandolo
void sonidos_finalizar(void);

#endif // SONIDOS_H_INCLUDED
