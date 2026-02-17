#include "sonidos.h"
#include <stdio.h>
#include <stdlib.h>

#define MIXER_CANAL_LIBRE -1 // Reproduce en el primer canal libre disponible
#define BUFFER_AUDIO 1024    // Tamanio del buffer de memoria
#define MONO 1

tFormatosSnd sonidos_inicializar(void)
{
    tFormatosSnd formatosSnd = SONIDO_WAV; // Soporte para WAV ya incluido
    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
    int init = Mix_Init(flags); // Inicializa SDL_mixer, devuelve los flags inicializados exitosamente o 0 si falla

    //Mascara de bits con la inicializacion y los formatos cargados, si la inicializacion fallo devuelve error
    if((init & flags) != flags)
        fprintf(stderr,"Error: %s\n", Mix_GetError());

    //Si la inicializacion coincide con MP3, lo asigna al formato de sonido. Lo mismo pasa con OGG, se suma a los formatos de sonido.
    if(init & MIX_INIT_MP3)
        formatosSnd |= SONIDO_MP3; //Hacer formatosSnd |= SONIDO_MP3 es lo mismo que hacer formatosSnd = formatosSnd |(OR) SONIDO_MP3

    if(init & MIX_INIT_OGG)
        formatosSnd |= SONIDO_OGG;

    //Abre el dispositivo de audio en formato mono
    if(Mix_OpenAudio(FREC_MUESTREO, MIX_DEFAULT_FORMAT, MONO, BUFFER_AUDIO)<0)
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
    if(!sonido->chunk)
    {
        fprintf(stderr, "Fallo la carga del sonido \"%s:\" %s\n", path, Mix_GetError());
        free(sonido);
        return NULL;
    }

    return sonido;
}

void sonidos_reproducir(const tSonido *sonido, int cantVeces)
{
    if(!sonido->chunk)
        return;

    int loops;

    if(cantVeces == -1)
        loops = -1; // -1 para reproducir de forma infinita
    else
        loops = (cantVeces > 0) ? (cantVeces - 1) : 0; // En mixer 0 para una vez, 1 para dos, etc.. y no acepta valores menores a -1

    if(Mix_PlayChannel(MIXER_CANAL_LIBRE, sonido->chunk, loops) == -1)
        fprintf(stderr, "No se pudo reproducir el sonido: %s\n", Mix_GetError());
}

void sonidos_destruir(tSonido *sonido)
{
    if(!sonido)
        return;

    if(sonido->chunk)
        Mix_FreeChunk(sonido->chunk);

    free(sonido);
}

tMusica* sonidos_cargar_musica(const char *path)
{
    tMusica* m = malloc(sizeof(tMusica));
    if(!m)
        return NULL;

    m->music = Mix_LoadMUS(path);
    if(!m->music)
    {
        fprintf(stderr, "Fallo la carga de la musica \"%s:\" %s\n", path, Mix_GetError());
        free(m);
        return NULL;
    }
    return m;
}

void sonidos_reproducir_musica(const tMusica* musica, int cantVeces)
{
    if(!musica->music)
        return;

    int loops;

    if(cantVeces == -1)
        loops = -1; // -1 para reproducir de forma infinita
    else
        loops = (cantVeces > 0) ? (cantVeces - 1) : 0; // En mixer 0 para una vez, 1 para dos, etc.. y no acepta valores menores a -1

    if(Mix_PlayMusic(musica->music, loops) == -1)
        fprintf(stderr, "No se pudo reproducir el track: %s\n", Mix_GetError());
}

void sonidos_destruir_musica(tMusica *musica)
{
    if(!musica)
        return;

    if(musica->music)
        Mix_FreeMusic(musica->music);

    free(musica);
}

void sonidos_finalizar(void)
{
    Mix_HaltChannel(-1); //Se usa -1 para detener todos los canales
    Mix_HaltMusic();
    Mix_CloseAudio();
    Mix_Quit();
}
