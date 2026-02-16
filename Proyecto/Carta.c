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
        //esta funcion copia una textura (imagen) sobre el renderer (lienzo)
        //los parametros son: el renderer, la textura, NULL(para usar toda la imagen) y la posicion y tamaño destino
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

    //funcion matematica que devuelve true si el punto esta en el rect
    return SDL_PointInRect(&clic, &c->posicion);
}

int obtenerCartasPorDificultad(int dificultad)
{
    switch(dificultad)
    {
    case FACIL:
        return 12; //3x4
    case NORMAL:
        return 16; //4x4
    case DIFICIL:
        return 20; //4x5
    default:
        return 16; //por defecto el nivel normal
    }
}
int obtenerCartasPorNivel(int nivel)
{
    switch(nivel)
    {
    case 1:
        return 12; //3x4
    case 2:
        return 16; //4x4
    case 3:
        return 20; //4x5
    case 4:
        return 30; //6x5
    case 5:
        return 42; //7x6
    default:
        return 0; //el 0 es que el nivel termino.
    }
}
