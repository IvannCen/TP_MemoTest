#include "Carta.h"

void CartaInicial(Carta* c, int id, int x, int y, int w,int h)
{
    if(!c)
    {
        return;
    }

    c->idImagen = id;
    c->bocaArriba = 0;
    c->encontrada = 0;

    c->posicion.x = x;
    c->posicion.y = y;
    c->posicion.w = w;
    c->posicion.h = h;
}

void CartaDibujar(Carta* c, SDL_Renderer* render, SDL_Texture* textura)
{
    if(!c)
    {
        return;
    }

    if(textura)
    {
        SDL_RenderCopy(render, textura, NULL, &c->posicion);
    }
    else
    {
        //si no hay imagen, se dibuja un color
        if(c->bocaArriba)
        {
            SDL_SetRenderDrawColor(render,0,255,0,255);
        }
        else
        {
           SDL_SetRenderDrawColor(render,0,0,255,255);
        }
        SDL_RenderFillRect(render, &c->posicion);
    }

    //tambien dibujo un borde negro para distinguir a las cartas
    //el color negro en el render es: 0 - 0 - 0 - 255
    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderDrawRect(render, &(c->posicion));
}

int cartaAdentro(Carta* c, int x, int y)
{
    if(!c)
    {
        return 0;
    }

    SDL_Point clic = {x,y};

    return SDL_PointInRect(&clic, &c->posicion);
}
