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
    c->hover = 0;

    c->posicion.x = x;
    c->posicion.y = y;
    c->posicion.w = w;
    c->posicion.h = h;
}

void CartaDibujar(Carta* c, SDL_Renderer* render, SDL_Texture* textura, int mouseX, int mouseY)
{
    if(!c)
    {
        return;
    }

    if(textura)
    {
        //esta funcion copia una textura (imagen) sobre el renderer (lienzo)
        //los parametros son: el renderer, la textura, NULL(para usar toda la imagen) y la posicion y tama�o destino
        SDL_RenderCopy(render, textura, NULL, &c->posicion);
    }
    else
    {
        //si no hay imagen, se dibuja un color
        if(c->bocaArriba)
        {
            SDL_SetRenderDrawColor(render,0,255,0,255); //color verde
        }
        else
        {
           SDL_SetRenderDrawColor(render,0,0,255,255); //azul (para el dorso)
        }
        //esta funcion pinta un rectangulo solido con el color actual
        SDL_RenderFillRect(render, &c->posicion);
    }

    //solo aplico el hover si la carta no fue revelada (es decir, no fue encontrada ni dada vuelta)
    if(c->hover && !c->encontrada && !c->bocaArriba)
    {
        SDL_SetRenderDrawColor(render, 0, 255, 255, 255); //color cyan

        //Dibujamos un borde mas grueso
        SDL_Rect r = c->posicion;
        SDL_RenderDrawRect(render, &r); //rectangulo 1, externo

        r.x++;
        r.y++;
        r.w-=2;
        r.h-=2;
        SDL_RenderDrawRect(render, &r); //rectangulo 2,

        r.x++;
        r.y++;
        r.w-=2;
        r.h-=2;
        SDL_RenderDrawRect(render, &r); //Segundo borde interno
    }

    else
    {
        //Borde blanco
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        SDL_RenderDrawRect(render, &(c->posicion));
    }
}

int cartaAdentro(Carta* c, int x, int y)
{
    if(!c)
    {
        return 0;
    }

    SDL_Point clic = {x,y};

    //funcion matematica que devuelve true si el punto esta en el rect
    return SDL_PointInRect(&clic, &c->posicion);
}