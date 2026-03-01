#include "carta.h"

void carta_Inicializar(Carta* c, int id, int x, int y, int w, int h)
{
    if(!c)
        return;
    c->idImagen = id;
    c->bocaArriba = 0;
    c->encontrada = 0;
    c->hover = 0;
    c->posicion.x = x;
    c->posicion.y = y;
    c->posicion.w = w;
    c->posicion.h = h;
}

void carta_Dibujar(Carta* c, SDL_Renderer* render, SDL_Texture* textura, int esCursor)
{
    if(!c)
        return;
    SDL_Rect rectVisual = c->posicion;
    int estaLevantada = (c->hover || esCursor) && !c->encontrada && !c->bocaArriba;

    if(estaLevantada)
    {
        rectVisual.y -= CARTA_HOVER_OFFSET;
        SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 80);
        SDL_Rect rectSombra = c->posicion;
        rectSombra.x += 4; rectSombra.w -= 8; rectSombra.h = 8; rectSombra.y = c->posicion.y + c->posicion.h - 4;
        SDL_RenderFillRect(render, &rectSombra);
        SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_NONE);
    }

    if(textura)
    {
        SDL_SetTextureBlendMode(textura, SDL_BLENDMODE_BLEND);
        if(c->encontrada)
            SDL_SetTextureAlphaMod(textura, 150);
        else
            SDL_SetTextureAlphaMod(textura, 255);
        SDL_RenderCopy(render, textura, NULL, &rectVisual);
    }
    else
    {
        if(c->bocaArriba)
            SDL_SetRenderDrawColor(render,0,255,0,255);
        else
            SDL_SetRenderDrawColor(render,0,0,255,255);
        SDL_RenderFillRect(render, &rectVisual);
    }

    if(esCursor)
    {
        SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
        SDL_Rect r = rectVisual;
        r.x-=3;
        r.y-=3;
        r.w+=6;
        r.h+=6;
        SDL_RenderDrawRect(render, &r);
        r.x--;
        r.y--;
        r.w+=2;
        r.h+=2;
        SDL_RenderDrawRect(render, &r);
    }
    else if(c->hover && !c->encontrada && !c->bocaArriba)
    {
        SDL_SetRenderDrawColor(render, 0, 255, 255, 255);
        SDL_RenderDrawRect(render, &rectVisual);
        SDL_Rect r = rectVisual;
        r.x++;
        r.y++;
        r.w-=2;
        r.h-=2;
        SDL_RenderDrawRect(render, &r);
    }
    else
    {
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderDrawRect(render, &rectVisual);
    }
}

int carta_EstaDentro(Carta* c, int x, int y)
{
    if(!c)
        return 0;
    SDL_Point p = {x,y};
    return SDL_PointInRect(&p, &c->posicion);
}
