#include "sonidos.h"
#include <stdio.h>
#include <stdlib.h>

#define MIXER_CANAL_LIBRE -1 // Reproduce en el primer canal libre disponible
#define BUFFER_AUDIO 1024    // Tamanio del buffer de memoria
#define ESTEREO 2            // 2 canales, sonido estereo

tFormatosSnd sonidos_inicializar(void)
{
    tFormatosSnd formatosSnd = SONIDO_WAV; // Soporte para WAV ya incluido
    int flags = MIX_INIT_MP3 | MIX_INIT_OGG; //(Originalmente, en el ejemplo se usa int32_t, ver que es mejor)
    int init = Mix_Init(flags); // Inicializa SDL_mixer, devuelve los flags inicializados exitosamente o 0 si falla

    //Mascara de bits con la inicializacion y los formatos cargados, si la inicializacion fallo devuelve error
    if ((init & flags) != flags)
    {
        fprintf(stderr,"Error: %s\n", Mix_GetError());
    }

    //Si la inicializacion coincide con MP3, lo asigna al formato de sonido. Lo mismo pasa con OGG, se suma a los formatos de sonido.
    if (init & MIX_INIT_MP3)
    {
        formatosSnd |= SONIDO_MP3; //Hacer formatosSnd |= SONIDO_MP3 es lo mismo que hacer formatosSnd = formatosSnd |(OR) SONIDO_MP3
    }

    if (init & MIX_INIT_OGG)
    {
        formatosSnd |= SONIDO_OGG;
    }

    //Abre el dispositivo de audio en formato estereo
    if(Mix_OpenAudio(FREC_MUESTREO, MIX_DEFAULT_FORMAT, ESTEREO, BUFFER_AUDIO)<0)
    {
        fprintf(stderr, "Error: No se pudo abrir el dispositivo de audio: %s\n", Mix_GetError());
        Mix_Quit();
        return SONIDO_ERR;
    }

    return formatosSnd;
}

tSonido* sonidos_cargar(const char *path)
{
    tSonido* sonido = malloc(sizeof(tSonido));
    if(!sonido)
        return NULL;

    sonido->chunk = Mix_LoadWAV(path);
    if (!sonido->chunk)
    {
        fprintf(stderr, "Fallo la carga del sonido \"%s:\" %s\n", path, Mix_GetError());
        free(sonido);
        return NULL;
    }

    return sonido;
}

void sonidos_reproducir(const tSonido *sonido, int cantVeces) //(const tSonido *sonido, int32_t cantVeces)
{
    if (!sonido->chunk)
    {
        return;
    }

    int loops; //el tipo de dato era originalmente int32_t

    if (cantVeces == -1)
    {
        loops = -1; // -1 para reproducir de forma infinita
    }
    else
    {
        loops = (cantVeces > 0) ? (cantVeces - 1) : 0; // En mixer 0 para una vez, 1 para dos, etc.. y no acepta valores menores a -1
    }

    if (Mix_PlayChannel(MIXER_CANAL_LIBRE, sonido->chunk, loops) == -1)
    {
        fprintf(stderr, "No se pudo reproducir el sonido: %s\n", Mix_GetError());
    }
}

void sonidos_destruir(tSonido *sonido)
{
    if (!sonido)
    {
        return;
    }

    /*
        Comentado porque no utilizamos tonos, en caso de querer usarse la logica es la siguiente:

        if (sonido->esTono)
        {
            if (sonido->chunk)
            {
                if (sonido->chunk->abuf)
                {
                    free(sonido->chunk->abuf);
                }
                free(sonido->chunk);
            }
        }
        else
        {
            if (sonido->chunk)
            {
                Mix_FreeChunk(sonido->chunk);
            }
        }
    */
    if (sonido->chunk)
    {
        Mix_FreeChunk(sonido->chunk);
    }
    free(sonido);
}

void sonidos_finalizar(void)
{
    Mix_HaltChannel(-1); //Se usa -1 para detener todos los canales
    Mix_CloseAudio();
    Mix_Quit();
}
