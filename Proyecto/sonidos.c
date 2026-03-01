#include "sonidos.h"
#include <stdio.h>
#include <stdlib.h>

#define FREC_MUESTREO 44100
#define BUFFER_AUDIO 1024
#define MONO 1

// Implementación real oculta dentro del .c
struct sSonido
{
    Mix_Chunk* chunk;
};

struct sMusica
{
    Mix_Music* music;
};

tFormatosSnd sonidos_Inicializar(void)
{
    tFormatosSnd formatosSnd = SONIDO_WAV;
    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
    int init = Mix_Init(flags);
    if((init & flags) != flags)
        fprintf(stderr,"Warning Mixer: %s\n", Mix_GetError());

    if(init & MIX_INIT_MP3)
        formatosSnd |= SONIDO_MP3;
    if(init & MIX_INIT_OGG)
        formatosSnd |= SONIDO_OGG;

    if(Mix_OpenAudio(FREC_MUESTREO, MIX_DEFAULT_FORMAT, MONO, BUFFER_AUDIO) < 0)
    {
        fprintf(stderr, "Error Audio: %s\n", Mix_GetError());
        return SONIDO_ERR;
    }
    return formatosSnd;
}

tSonido* sonidos_Cargar(const char *path)
{
    tSonido* s = malloc(sizeof(tSonido));
    if(!s)
        return NULL;
    s->chunk = Mix_LoadWAV(path);
    if(!s->chunk)
    {
        fprintf(stderr, "Error carga sonido %s: %s\n", path, Mix_GetError());
        free(s);
        return NULL;
    }
    return s;
}

void sonidos_Reproducir(const tSonido *sonido, int cantVeces)
{
    if(!sonido || !sonido->chunk)
        return;
    int loops = (cantVeces > 0) ? (cantVeces - 1) : 0;
    if(cantVeces == -1)
        loops = -1;
    Mix_PlayChannel(-1, sonido->chunk, loops);
}

void sonidos_Destruir(tSonido *sonido)
{
    if(!sonido)
        return;
    if(sonido->chunk)
        Mix_FreeChunk(sonido->chunk);
    free(sonido);
}

tMusica* sonidos_CargarMusica(const char *path)
{
    tMusica* m = malloc(sizeof(tMusica));
    if(!m)
        return NULL;
    m->music = Mix_LoadMUS(path);
    if(!m->music)
    {
        fprintf(stderr, "Error carga musica %s: %s\n", path, Mix_GetError());
        free(m);
        return NULL;
    }
    return m;
}

void sonidos_ReproducirMusica(const tMusica* musica, int cantVeces)
{
    if(!musica || !musica->music)
        return;
    int loops = (cantVeces > 0) ? (cantVeces - 1) : 0;
    if(cantVeces == -1)
        loops = -1;
    Mix_PlayMusic(musica->music, loops);
}

void sonidos_DestruirMusica(tMusica *musica)
{
    if(!musica)
        return;
    if(musica->music)
        Mix_FreeMusic(musica->music);
    free(musica);
}

void sonidos_Finalizar(void)
{
    Mix_HaltChannel(-1);
    Mix_HaltMusic();
    Mix_CloseAudio();
    Mix_Quit();
}
