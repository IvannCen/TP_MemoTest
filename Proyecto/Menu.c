#include "Menu.h"

void menuIniciar(Menu* m, SDL_Renderer* renderer)
{
    m->opcionSeleccionada = 0; //arranca desde la primer opcion del menu

    //cargo las fuentes y ajusto los tamaños como quiera
    m->fuenteTitulo = TTF_OpenFont(FUENTE, 80);
    m->fuenteOpciones = TTF_OpenFont(FUENTE, 40);

    //creo los titulos una sola vez
    SDL_Color color = {255,215,0};
    SDL_Surface* surf = TTF_RenderText_Solid(m->fuenteTitulo, "MEMOTEST MASTER", color);
    m->texturaTitulo = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void menuDestruir(Menu* m)
{
    if(m->texturaTitulo)
    {
        SDL_DestroyTexture(m->texturaTitulo);
    }
    if(m->fuenteTitulo)
    {
        TTF_CloseFont(m->fuenteTitulo);
    }
    if(m->fuenteOpciones)
    {
        TTF_CloseFont(m->fuenteOpciones);
    }
}

int menuManejarOpciones(Menu* m, SDL_Event* e)
{
    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
            m->opcionSeleccionada--;
            if(m->opcionSeleccionada < 0)
            {
                m->opcionSeleccionada = CANTIDADOPCIONES - 1; //vuelve a la opcion de abajo del todo
            }
            break;
        case SDLK_DOWN:
            m->opcionSeleccionada++;
            if(m->opcionSeleccionada >= CANTIDADOPCIONES)
            {
                m->opcionSeleccionada = 0; //vuelve a la primera opcion
            }
            break;
        case SDLK_RETURN: //este es el enter normal
        case SDLK_KP_ENTER: //enter del pad numerico
            return m->opcionSeleccionada; //devuelvo la opcion si se selecciona el enter
        }
    }

    return -1; //devuelvo -1 si sigue en el menu y no eligio nada
}

void menuDibujar(Menu* m, SDL_Renderer* renderer)
{
    //lo dibujo al menu centrado en la pantalla
    int w,h;
    SDL_QueryTexture(m->texturaTitulo, NULL, NULL, &w, &h);
    SDL_Rect recTitulo = {(ANCHOVENTANA - w)/2, 100, w, h};
    SDL_RenderCopy(renderer, m->texturaTitulo, NULL, &recTitulo);

    //dibujo las opciones en pantalla
    const char* opciones[] = {"Aventura","Competitivo","Ranking","Configuracion","Salir"};

    //estos los pongo estaticos pero podrian ser macros, vamos viendo
    int inicioY = 300; //altura donde esta la primer opcion para tener referencia
    int separacion = 60; //espacion entre opciones

    for(int i=0;i<CANTIDADOPCIONES;i++)
    {
        SDL_Color color;

        //si es la opcion seleccionada es verde o rojo, sino blanco
        if(i == m->opcionSeleccionada)
        {
            //color verde
            color.r = 50;
            color.g = 255;
            color.b = 50;
            color.a = 255;
        }
        else
        {
            //color gris
            color.r = 200;
            color.g = 200;
            color.b = 200;
            color.a = 255;
        }

        //ahora renderizo el color
        SDL_Surface* surf = TTF_RenderText_Solid(m->fuenteOpciones, opciones[i], color);
        SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, surf);

        //lo centro en el centro de manera horizontal
        SDL_Rect rect = {(ANCHOVENTANA - surf->w)/2, inicioY + (i*separacion), surf->w, surf->h};
        SDL_RenderCopy(renderer, textura, NULL, &rect);

        SDL_FreeSurface(surf);
        SDL_DestroyTexture(textura);
    }
}
