#include "tablero.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void tablero_Iniciar(Tablero* t, ConfiguracionJuego* config)
{
    int cant;
    int i;
    Carta cVacia;
    SDL_Texture *nullTex = NULL;

    /* Proteccion contra division por cero si config no fue bien cargado */
    if(config->filas <= 0)
        config->filas = 4;

    if(config->columnas <= 0)
        config->columnas = 4;

    cant = config->filas * config->columnas;

    memset(&cVacia, 0, sizeof(Carta)); //(Revisar esto del memset, ya que no es una función que hayamos visto en clase)

    //Guardamos filas y columnas en el tablero basándonos en la configuración
    t->filas = config->filas;
    t->columnas = config->columnas;

    //Inicializamos los TDA pasando la direccion de memoria de los structs
    if(!vectorCrear(&t->cartas, sizeof(Carta)))
    {
        //Uso fprintf en el buffer de stderr para mostrar por consola el error
        fprintf(stderr, "Error al crear el vector de cartas");
        exit(1);
    }

    if(!vectorCrear(&t->imagenes, sizeof(SDL_Texture*)))
    {
        fprintf(stderr, "Error al crear el vector de imsgenes");
        exit(1);
    }

    //Rellenamos con cartas vacías para reservar el tamaño inicial
    for(i = 0; i < cant; i++)
    {
        vectorInsertarAlFinal(&t->cartas, &cVacia);
    }

    //Rellenamos el vector de texturas con punteros nulos seguros
    for(i = 0; i < CANTIDAD_IMAGENES; i++)
    {
        vectorInsertarAlFinal(&t->imagenes, &nullTex);
    }

    //Inicializamos el resto de las variables de estado.
    t->cartaSeleccionada = NULL;
    t->cantidadImagenesCargadas = 0;
    t->dorso = NULL;
    t->texturaFondoJuego = NULL;
    t->rachaActual = 0;
    t->movimientos = 0;
    t->parejasEncontradas = 0;
    t->cursorX = 0;
    t->cursorY = 0;
}

void tablero_Destruir(Tablero* t)
{
    int i;
    SDL_Texture **pTex = NULL;

    if(t->dorso)
    {
        SDL_DestroyTexture(t->dorso);
        t->dorso = NULL;
    }
    if(t->texturaFondoJuego)
    {
        SDL_DestroyTexture(t->texturaFondoJuego);
        t->texturaFondoJuego = NULL;
    }
    for(i = 0; i< CANTIDAD_IMAGENES; i++)
    {
        pTex = (SDL_Texture**)vectorObtenerPuntero(&t->imagenes, i);
        if(pTex && *pTex)
        {
            SDL_DestroyTexture(*pTex);
            *pTex = NULL;
        }
    }

    vectorDestruir(&t->cartas);
    vectorDestruir(&t->imagenes);
}

void tablero_Rellenar(Tablero* t, int filas, int columnas)
{
    int cantidad = vectorCantidadDeElementos(&t->cartas);
    int anchoUtil, altoUtil, anchoCalc, altoCalc, anchoF, altoF, anchoB, altoB, mX, mY;
    int i, c, f, pX, pY, id, imgDisponibles;
    Carta *cPtr = NULL;

    /* Proteccion estructural contra division por cero */
    if(filas <= 0)
        filas = 4;
    if(columnas <= 0)
        columnas = 4;

    /* Si aun no se cargaron imagenes, evitamos dividir por cero usando 1 temporalmente */
    imgDisponibles = (t->cantidadImagenesCargadas > 0) ? t->cantidadImagenesCargadas : 1;

    anchoUtil = ANCHO_VENTANA - (MARGEN_LATERAL * 2);
    altoUtil = ALTO_VENTANA - ALTO_INTERFAZ - MARGEN_SUPERIOR;
    anchoCalc = (anchoUtil - ((columnas - 1) * SEPARACION_CARTA)) / columnas;
    altoCalc = (altoUtil - ((filas - 1) * SEPARACION_CARTA)) / filas;
    anchoF = (anchoCalc > ANCHO_CARTA) ? ANCHO_CARTA : anchoCalc;
    altoF = (altoCalc > ALTO_CARTA) ? ALTO_CARTA : altoCalc;
    anchoB = (columnas * anchoF) + ((columnas - 1) * SEPARACION_CARTA);
    altoB = (filas * altoF) + ((filas - 1) * SEPARACION_CARTA);
    mX = (ANCHO_VENTANA - anchoB) / 2;
    mY = ALTO_INTERFAZ + ((altoUtil - altoB) / 2);

    for(i = 0; i < cantidad; i++)
    {
        c = i % columnas;
        f = i / columnas;
        pX = mX + (c * (anchoF + SEPARACION_CARTA));
        pY = mY + (f * (altoF + SEPARACION_CARTA));

        id = (i / 2) % imgDisponibles;

        cPtr = (Carta*)vectorObtenerPuntero(&t->cartas, i); /* Aca ocurria el crash si imagenes era 0 */

        if(cPtr)
            carta_Inicializar(cPtr, id, pX, pY, anchoF, altoF);
    }
}

void tablero_Dibujar(Tablero* t, SDL_Renderer* render, int mouseX, int mouseY)
{
    int i, cantidad, esCursor;
    Carta *cPtr = NULL;
    SDL_Texture **pTex = NULL;
    SDL_Texture *tex = NULL;

    if(t->texturaFondoJuego)
        SDL_RenderCopy(render, t->texturaFondoJuego, NULL, NULL);
    else
        SDL_SetRenderDrawColor(render, 20, 20, 20, 255);

    if(!t)
        return;

    cantidad = vectorCantidadDeElementos(&t->cartas);

    for(i = 0; i < cantidad; i++)
    {
        cPtr = (Carta*)vectorObtenerPuntero(&t->cartas, i);

        if(cPtr)
        {
            if(cPtr->encontrada || cPtr->bocaArriba)
            {
                pTex = (SDL_Texture**)vectorObtenerPuntero(&t->imagenes, cPtr->idImagen);
                tex = (pTex && *pTex) ? *pTex : t->dorso;
            }
            else
                tex = t->dorso;

            esCursor = 0;
            if(t->columnas > 0) /* Proteccion vital */
            {
                esCursor = (i % t->columnas == t->cursorX && i / t->columnas == t->cursorY);
            }

            if(esCursor)
                cPtr->hover = 1;

            carta_Dibujar(cPtr, render, tex, esCursor);
        }
    }
}

void tablero_ManejarTeclado(Tablero* t, SDL_Event* e, SDL_Renderer* render, ContextoJuego* juego, TTF_Font* font)
{
    int k, cantidad, idx, cx, cy;
    Carta* cPtr = NULL;
    Carta* target = NULL;

    if(e->type == SDL_KEYDOWN)
    {
        cantidad = vectorCantidadDeElementos(&t->cartas);
        for(k = 0; k < cantidad; k++)
        {
            cPtr = (Carta*)vectorObtenerPuntero(&t->cartas, k);
            if(cPtr)
                cPtr->hover = 0;
        }

        switch(e->key.keysym.sym)
        {
            case SDLK_UP:
                if(t->cursorY > 0)
                    t->cursorY--;
                break;
            case SDLK_DOWN:
                if(t->cursorY < t->filas-1)
                    t->cursorY++;
                break;
            case SDLK_LEFT:
                if(t->cursorX > 0)
                    t->cursorX--;
                break;
            case SDLK_RIGHT:
                if(t->cursorX < t->columnas-1)
                    t->cursorX++;
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
            {
                idx = (t->cursorY * t->columnas) + t->cursorX;
                target = (Carta*)vectorObtenerPuntero(&t->cartas, idx);
                if(target)
                {
                    cx = target->posicion.x + target->posicion.w/2;
                    cy = target->posicion.y + target->posicion.h/2;
                    tablero_Clic(t, cx, cy, render, juego, font);
                }
                break;
            }
        }
    }
}

int tablero_Clic(Tablero* t, int x, int y, SDL_Renderer* render, ContextoJuego* juego, TTF_Font* font)
{
    int i = 0, resuelto = 0, cantidad, pts;
    Carta* cActual = NULL;
    Carta* c1 = NULL;
    Carta* c2 = NULL;

    if(!t || !juego)
        return 0;

    cantidad = vectorCantidadDeElementos(&t->cartas);

    while(i < cantidad && !resuelto)
    {
        cActual = (Carta*)vectorObtenerPuntero(&t->cartas, i);
        if(cActual && carta_EstaDentro(cActual, x, y) && !cActual->encontrada && !cActual->bocaArriba)
        {
            resuelto = 1;
            if(!t->cartaSeleccionada)
            {
                sonidos_Reproducir(juego->sndSeleccion, 1);
                cActual->bocaArriba = 1;
                t->cartaSeleccionada = cActual;
            }
            else
            {
                c1 = t->cartaSeleccionada;
                c2 = cActual;
                c2->bocaArriba = 1;
                t->movimientos++;

                tablero_Dibujar(t, render, 0, 0);
                dibujarEstadisticas(render, font, juego);
                SDL_RenderPresent(render);

                if(c1->idImagen == c2->idImagen)
                {
                    sonidos_Reproducir(juego->sndAcierto, 1);
                    c1->encontrada = 1;
                    c2->encontrada = 1;
                    t->parejasEncontradas++;
                    pts = PUNTOS_BASE_ACIERTO + (t->rachaActual * PUNTOS_BONUS_RACHA);
                    *(juego->puntos + juego->turnoJugador) += pts;
                    t->rachaActual++;
                }
                else
                {
                    sonidos_Reproducir(juego->sndFallo, 1);
                    SDL_Delay(TIEMPO_DELAY_ERROR);
                    c1->bocaArriba = 0;
                    c2->bocaArriba = 0;
                    *(juego->puntos + juego->turnoJugador) -= PUNTOS_PENALIZACION;
                    if(*(juego->puntos + juego->turnoJugador) < 0)
                        *(juego->puntos + juego->turnoJugador) = 0;

                    if(juego->cantJugadores == 2)
                        juego->turnoJugador = (juego->turnoJugador == 0) ? 1 : 0;

                    t->rachaActual = 0;
                }
                t->cartaSeleccionada = NULL;
            }
        }
        i++;
    }
    return 0;
}

void tablero_CargarImagenes(Tablero* t, SDL_Renderer* render, int idSet)
{
    int i;
    char buf[100]; //antes era char buf[256], lo modifique
    const char *rDorso = NULL;
    const char *rFondo = NULL;
    const char *carp = NULL;
    SDL_Texture *texLoad = NULL;
    SDL_Texture **pTex = NULL;

    if(!t)
        return;

    rDorso = (idSet == 0) ? RUTA_DORSO_A : RUTA_DORSO_B;
    t->dorso = IMG_LoadTexture(render, rDorso);

    if(!t->dorso)
        fprintf(stderr,"Error Dorso: %s\n", IMG_GetError());

    rFondo = (idSet == 0) ? RUTA_FONDO_JUEGO_C : RUTA_FONDO_JUEGO_L;
    t->texturaFondoJuego = IMG_LoadTexture(render, rFondo);

    carp = (idSet == 0) ? RUTA_SET_A : RUTA_SET_B;

    for(i = 0; i < CANTIDAD_IMAGENES; i++)
    {
        sprintf(buf, "%s%d.png", carp, i);
        texLoad = IMG_LoadTexture(render, buf);

        pTex = (SDL_Texture**)vectorObtenerPuntero(&t->imagenes, i);
        if(pTex)
            *pTex = texLoad;
    }

    t->cantidadImagenesCargadas = CANTIDAD_IMAGENES;
}

void tablero_Mezclar(Tablero* t)
{
    int cantidad = vectorCantidadDeElementos(&t->cartas);
    int i, j, aux;
    Carta *c1 = NULL;
    Carta *c2 = NULL;

    if(!t || cantidad < 2)
        return;

    for(i = 0; i < cantidad; i++)
    {
        j = rand() % cantidad;

        c1 = (Carta*)vectorObtenerPuntero(&t->cartas, i);
        c2 = (Carta*)vectorObtenerPuntero(&t->cartas, j);

        if(c1 && c2)
        {
            aux = c1->idImagen;
            c1->idImagen = c2->idImagen;
            c2->idImagen = aux;
        }
    }
}

void tablero_ManejarHover(Tablero* t, int x, int y)
{
    int i, cantidad;
    Carta* c = NULL;

    /* Vital: evitar crash si todavia no hay columnas definidas (al mover el mouse tempranamente) */
    if(!t || t->columnas <= 0)
        return;

    cantidad = vectorCantidadDeElementos(&t->cartas);

    for(i = 0; i < cantidad; i++)
    {
        c = (Carta*)vectorObtenerPuntero(&t->cartas, i);
        if(c)
        {
            if(carta_EstaDentro(c, x, y))
            {
                t->cursorX = i % t->columnas;
                t->cursorY = i / t->columnas;
                if(!c->encontrada && !c->bocaArriba)
                    c->hover = 1;
            }
            else
                c->hover = 0;
        }
    }
}

int tablero_Completo(Tablero* t)
{
    int i, cantidad;
    Carta* c = NULL;

    if(!t)
        return 0;

    cantidad = vectorCantidadDeElementos(&t->cartas);

    for(i = 0; i < cantidad; i++)
    {
        c = (Carta*)vectorObtenerPuntero(&t->cartas, i);

        if(c && c->encontrada == 0)
            return 0;
    }

    return 1;
}

void dibujarEstadisticas(SDL_Renderer* render, TTF_Font* font, ContextoJuego* juego)
{
    SDL_Color blanco = COLOR_BLANCO;
    SDL_Color gris = COLOR_GRIS;
    SDL_Color verde = COLOR_VERDE;
    SDL_Color c1, c2;
    char buf[100];
    Uint32 seg;

    if(juego->cantJugadores == 1)
    {
        sprintf(buf, "Jugador: %s", *(juego->nombreJugador + 0));
        dibujarTexto(render, font, buf, MARGEN_LATERAL, MARGEN_SUPERIOR, blanco);
        sprintf(buf, "Puntos: %d", *(juego->puntos + 0));
        dibujarTextoCentrados(render, font, buf, MARGEN_SUPERIOR, blanco);
    }
    else
    {
        c1 = (juego->turnoJugador == 0) ? verde : gris;
        c2 = (juego->turnoJugador == 1) ? verde : gris;

        sprintf(buf, "%s: %d", *(juego->nombreJugador + 0), *(juego->puntos + 0));
        dibujarTexto(render, font, buf, (ANCHO_VENTANA/2)-250, MARGEN_SUPERIOR, c1);

        sprintf(buf, "%s: %d", *(juego->nombreJugador + 1), *(juego->puntos + 1));
        dibujarTexto(render, font, buf, (ANCHO_VENTANA/2)+195, MARGEN_SUPERIOR, c2);

        sprintf(buf, "TURNO: %s", *(juego->nombreJugador + juego->turnoJugador));
        dibujarTexto(render, font, buf, MARGEN_LATERAL, MARGEN_SUPERIOR, verde);
    }

    seg = (SDL_GetTicks() - juego->tiempoInicio) / 1000;
    sprintf(buf, "Tiempo: %d", seg);
    dibujarTexto(render, font, buf, ANCHO_VENTANA-MARGEN_LATERAL-100, MARGEN_SUPERIOR, blanco);
    dibujarTexto(render, font, "[ ESC: Salir ]", ANCHO_VENTANA-150, ALTO_VENTANA-50, COLOR_ROJO);
}

void dibujarPopupSalida(SDL_Renderer* render, TTF_Font* font, const char* texto, int opcionSeleccionada)
{
    SDL_Rect over = {0, 0, ANCHO_VENTANA, ALTO_VENTANA};
    SDL_Rect caja = {((ANCHO_VENTANA-400)/2), ((ALTO_VENTANA-200)/2), 400, 200};
    SDL_Color cSi = (opcionSeleccionada == 0) ? COLOR_VERDE : COLOR_GRIS;
    SDL_Color cNo = (opcionSeleccionada == 1) ? COLOR_ROJO : COLOR_GRIS;

    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 200);
    SDL_RenderFillRect(render, &over);
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_NONE);

    SDL_SetRenderDrawColor(render, 50, 50, 50, 255);
    SDL_RenderFillRect(render, &caja);
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderDrawRect(render, &caja);

    dibujarTextoCentrados(render, font, texto, (ALTO_VENTANA/2)-40, COLOR_BLANCO);
    dibujarTexto(render, font, "SI", (ANCHO_VENTANA/2)-80, (ALTO_VENTANA/2)+20, cSi);
    dibujarTexto(render, font, "NO", (ANCHO_VENTANA/2)+40, (ALTO_VENTANA/2)+20, cNo);
}

void dibujarTexto(SDL_Renderer* r, TTF_Font* f, const char* t, int x, int y, SDL_Color c)
{
    SDL_Surface *s = TTF_RenderText_Blended(f, t, c);
    SDL_Texture *tx = NULL;
    SDL_Rect dst;

    if(s)
    {
        tx = SDL_CreateTextureFromSurface(r, s);
        if(tx)
        {
            dst.x = x;
            dst.y = y;
            dst.w = s->w;
            dst.h = s->h;
            SDL_RenderCopy(r, tx, NULL, &dst);
            SDL_DestroyTexture(tx);
        }
        SDL_FreeSurface(s);
    }
}

void dibujarTextoCentrados(SDL_Renderer* r, TTF_Font* f, const char* t, int y, SDL_Color c)
{
    SDL_Surface *s = TTF_RenderText_Blended(f, t, c);
    SDL_Texture *tx = NULL;
    SDL_Rect dst;
    int x;

    if(s)
    {
        tx = SDL_CreateTextureFromSurface(r, s);
        if(tx)
        {
            x = (ANCHO_VENTANA - s->w) / 2;
            dst.x = x;
            dst.y = y;
            dst.w = s->w;
            dst.h = s->h;
            SDL_RenderCopy(r, tx, NULL, &dst);
            SDL_DestroyTexture(tx);
        }
        SDL_FreeSurface(s);
    }
}
