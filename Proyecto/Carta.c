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
    if(!c) return;

    // FONDO BLANCO BASE (Para evitar cartas invisibles si falla textura)
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderFillRect(render, &c->posicion);

    if(textura)
    {
        SDL_RenderCopy(render, textura, NULL, &c->posicion);
    }
    else
    {
        // Fallback colores solidos
        if(c->bocaArriba)
            SDL_SetRenderDrawColor(render,0,255,0,255);
        else
            SDL_SetRenderDrawColor(render,0,0,255,255);

        SDL_RenderFillRect(render, &c->posicion);
    }

    // HOVER CYAN
//    if(c->hover && !c->encontrada && !c->bocaArriba)
//    {
//        SDL_SetRenderDrawColor(render, 0, 255, 255, 255); // Cyan
//        SDL_Rect r = c->posicion;
//        SDL_RenderDrawRect(render, &r);
//        r.x++;
//        r.y++;
//        r.w-=2;
//        r.h-=2;
//        SDL_RenderDrawRect(render, &r);
//        r.x++;
//        r.y++;
//        r.w-=2;
//        r.h-=2;
//        SDL_RenderDrawRect(render, &r);
//    }
//    else
//    {
//        SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Borde negro normal
//        SDL_RenderDrawRect(render, &(c->posicion));
//    }

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Borde negro normal
    SDL_RenderDrawRect(render, &(c->posicion));
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
