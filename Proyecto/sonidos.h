#ifndef SONIDOS
#define SONIDOS

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "configuracion.h"

typedef enum
{
    SONIDO_ERR = -1,
    SONIDO_WAV = 0x00,
    SONIDO_MP3 = 0x01,
    SONIDO_OGG = 0x02,
} tFormatosSnd;

// Definición opaca: Ocultamos los campos internos
typedef struct sSonido tSonido;
typedef struct sMusica tMusica;

tFormatosSnd sonidos_Inicializar(void);
void sonidos_Finalizar(void);

tSonido* sonidos_Cargar(const char *path);
void sonidos_Reproducir(const tSonido *sonido, int cantVeces);
void sonidos_Destruir(tSonido *sonido);

tMusica* sonidos_CargarMusica(const char *path);
void sonidos_ReproducirMusica(const tMusica* musica, int cantVeces);
void sonidos_DestruirMusica(tMusica *musica);

#endif // SONIDOS
