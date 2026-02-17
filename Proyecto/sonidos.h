#ifndef SONIDOS_H_INCLUDED
#define SONIDOS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define FREC_MUESTREO 44100
#define VOL_MAX 50

#define SELECCION_SND   "snd/seleccion.wav"
#define ACIERTO_SND     "snd/acierto.wav"
#define FALLO_SND       "snd/fallo.wav"
#define MENU_SND        "snd/menu.mp3"
#define JUEGO_SND       "snd/juego.mp3"

//Formatos de audio soportados
typedef enum
{
    SONIDO_ERR = -1,
    SONIDO_WAV = 0x00,
    SONIDO_MP3 = 0x01,
    SONIDO_OGG = 0x02,
} tFormatosSnd;

//Se usa Mix_Chunk porque son efectos cortos cargados desde la unidad de almacenamiento
typedef struct
{
    Mix_Chunk* chunk;
    //int esTono;   //Por ahora no vamos a generar ningún tono generado por buffer de audio, pero lo dejo en caso de que cambiemos de opinion
} tSonido;

typedef struct
{
    Mix_Music* music;
} tMusica;

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

//Carga un archivo de musica
tMusica* sonidos_cargar_musica(const char *path);

//Reproduce un track "n" veces, determinado por el parametro cantVeces
void sonidos_reproducir_musica(const tMusica* musica, int cantVeces);

//Liberar memoria asociada a la musica
void sonidos_destruir_musica(tMusica *musica);

//Finaliza correctamente SDL_mixer, cerrando primero los canales de audio, luego cerrando el mixer y finalmente desinicializandolo
//Finaliza tanto los sonidos como la musica
void sonidos_finalizar(void);

#endif // SONIDOS_H_INCLUDED
