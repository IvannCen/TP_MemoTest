#include "Tablero.h"

void tableroIniciar(Tablero* t, Configuracion* config)
{
    int cant = config->filas * config->columnas;
    t->filas = config->filas;
    t->columnas = config->columnas;

    t->cartas = (Carta*)malloc(sizeof(Carta)*cant);
    if(!t->cartas)
    {
        printf("Error al crear el vector dinamico inicial.\n");
        exit(1);
    }
    t->cantidad = cant;
    t->cartaSeleccionada = NULL;
    t->cantidadImagenesCargadas = 0;
    t->dorso = NULL;

    for(int i=0; i<CANTIDADIMAGENES; i++)
    {
        t->imagenes[i] = NULL;
        //Cada figura tiene un valor entre 50 y 200 puntos, en multiplos de 10
        t->puntosPorImagen[i] = (rand()%16 + 5) * 10;
    }

    t->rachaActual = 0;
    t->movimientos = 0;
    t->parejasEncontradas = 0;

    t->cursorX = 0;
    t->cursorY = 0;
}

void tableroDestruir(Tablero* t)
{
    if(t->cartas)
    {
        free(t->cartas);
        t->cartas = NULL;
    }

    if(t->dorso)
    {
        SDL_DestroyTexture(t->dorso);
        t->dorso = NULL;
    }

    for(int i=0; i<CANTIDADIMAGENES; i++)
    {
        if(t->imagenes[i])
        {
            SDL_DestroyTexture(t->imagenes[i]);
            t->imagenes[i] = NULL;
        }
    }
}

void tableroRellenar(Tablero* t)
{
    //el area donde se pueden poder las ventanas
    // Ancho: Toda la ventana menos un margen chico de costado
    // Alto: Toda la ventana menos el espacio de arriba para los tiempos y eso
    int anchoUtil = ANCHOVENTANA - (INTERFAZMARGENLATERAL * 2); // 20px de margen a los lados
    int altoUtil = ALTOVENTANA - ALTOINTERFAZ - INTERFAZMARGENSUPERIOR;

    //calculo el tamanio de la carta de manera dinamica, para que no sea un numero estatico
    //de la sig manera: (EspacioTotal - (Separaciones)) / Cantidad
    int anchoCalculado = (anchoUtil - ((t->columnas - 1) * SEPARACIONCARTA)) / t->columnas;
    int altoCalculado = (altoUtil - ((t->filas - 1) * SEPARACIONCARTA)) / t->filas;

    //si hay pocas cartas, el calculo daria cartas muy grandes
    //asi que pongo un limite maximo de tamanio
    //Pero si el calculo da menos, uso el calculado

    int anchoFinal = (anchoCalculado > ANCHOCARTA) ? ANCHOCARTA : anchoCalculado;
    int altoFinal = (altoCalculado > ALTOCARTA) ? ALTOCARTA : altoCalculado;

    //calculo cuanto ocupa todo el bloque completo en la ventana
    int anchoBloque = (t->columnas * anchoFinal) + ((t->columnas - 1) * SEPARACIONCARTA);
    int altoBloque = (t->filas * altoFinal) + ((t->filas - 1) * SEPARACIONCARTA);

    //margenes para centrar matematicamente en la pantalla
    int margenX = (ANCHOVENTANA - anchoBloque) / 2;
    int margenY = ALTOINTERFAZ + ((altoUtil - altoBloque) / 2);

    for(int i=0; i<t->cantidad; i++)
    {
        int col = i % t->columnas;
        int fil = i / t->columnas;

        int posX = margenX + (col * (anchoFinal + SEPARACIONCARTA));
        int posY = margenY + (fil * (altoFinal + SEPARACIONCARTA));

        int idImagen = (i/2) % t->cantidadImagenesCargadas;

        //mando las variables anchoFinal y altoFinal, no las constantes fijas
        CartaInicial(&t->cartas[i], idImagen, posX, posY, anchoFinal, altoFinal);
    }
}

void tableroDibujar(Tablero* t, SDL_Renderer* render, int mouseX, int mouseY)
{
    if(!t || !t->cartas)
    {
        return;
    }

    for(int i=0;i<t->cantidad;i++)
    {
        SDL_Texture* texturaActual = (t->cartas[i].encontrada || t->cartas[i].bocaArriba) ? t->imagenes[t->cartas[i].idImagen] : t->dorso;

        int esCursor = 0;
        int col = i % t->columnas;
        int fil = i / t->columnas;
        if(col == t->cursorX && fil == t->cursorY)
            esCursor = 1;

        CartaDibujar(&t->cartas[i], render, texturaActual, mouseX, mouseY);

        if(esCursor)
        {
            SDL_SetRenderDrawColor(render,255,255,0,255); //color amarillo
            SDL_Rect r = t->cartas[i].posicion;

            r.x-=3;
            r.y-=3;
            r.w+=6;
            r.h+=6; // Un poco mas grande
            SDL_RenderDrawRect(render, &r);

            r.x--;
            r.y--;
            r.w+=2;
            r.h+=2;
            SDL_RenderDrawRect(render, &r);
        }
    }
}

void tableroManejarTeclado(Tablero* t, SDL_Event* e, SDL_Renderer* render, ContextoJuego* juego)
{
    if(e->type == SDL_KEYDOWN)
    {
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
                // Simular Clic en la carta seleccionada
                int indice = (t->cursorY * t->columnas) + t->cursorX;
                // Llamamos a la logica de clic pasandole el centro de la carta
                int cx = t->cartas[indice].posicion.x + 1;
                int cy = t->cartas[indice].posicion.y + 1;
                int puntos = tableroClic(t, cx, cy, render, juego);
                juego->puntos += puntos;
                if(juego->puntos < 0)
                    juego->puntos = 0;
                break;
            }
        }
    }
}

int tableroClic(Tablero* t, int x, int y, SDL_Renderer* render, ContextoJuego* juego)
{
    if(!t || !juego)
        return 0;

    int puntos = 0;
    int i = 0;
    int clicResuelto = 0;

    while(i<t->cantidad && clicResuelto == 0)
    {
        if(cartaAdentro(&t->cartas[i],x,y) && !t->cartas[i].encontrada && !t->cartas[i].bocaArriba)
        {
            clicResuelto = 1;
            if(!t->cartaSeleccionada)
            {
                sonidos_reproducir(juego->sndSeleccion, 1);
                t->cartas[i].bocaArriba = 1;
                t->cartaSeleccionada = &t->cartas[i];
            }
            else
            {
                Carta* c1 = t->cartaSeleccionada;
                Carta* c2 = &t->cartas[i];
                c2->bocaArriba = 1;
                t->movimientos++;

                // Forzar redibujado
                tableroDibujar(t,render,0,0);
                SDL_RenderPresent(render);

                if(c1->idImagen == c2->idImagen)
                {
                    sonidos_reproducir(juego->sndAcierto, 1);
                    c1->encontrada = 1; c2->encontrada = 1;
                    t->parejasEncontradas++;

                    int puntosBase = t->puntosPorImagen[c1->idImagen];

                    puntos = puntosBase + (t->rachaActual * 20);
                    t->rachaActual++;
                }
                else
                {
                    sonidos_reproducir(juego->sndFallo, 1);
                    SDL_Delay(DELAY);
                    c1->bocaArriba = 0; c2->bocaArriba = 0;
                    puntos = -20;
                    t->rachaActual = 0;
                }
                t->cartaSeleccionada = NULL;
            }
        }
        i++;
    }
    return puntos;
}

void dibujarPopupSalidaJuego(SDL_Renderer* render, TTF_Font* font, int opcionSeleccionada)
{
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, ANCHOVENTANA, ALTOVENTANA};
    SDL_RenderFillRect(render, &overlay);
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_NONE);

    SDL_Rect caja = {(ANCHOVENTANA-400)/2, (ALTOVENTANA-200)/2, 400, 200};
    SDL_SetRenderDrawColor(render, 50, 50, 50, 255);
    SDL_RenderFillRect(render, &caja);
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderDrawRect(render, &caja);

    dibujarTextoCentrados(render, font, "Abandonar partida?", caja.y + 40, (SDL_Color){255,255,255});

    SDL_Color colorSi = (opcionSeleccionada == 0) ? (SDL_Color){0,255,0} : (SDL_Color){150,150,150};
    SDL_Color colorNo = (opcionSeleccionada == 1) ? (SDL_Color){255,0,0} : (SDL_Color){150,150,150};

    dibujarTexto(render, font, "SI", (ANCHOVENTANA/2) - 80, caja.y + 120, colorSi);
    dibujarTexto(render, font, "NO", (ANCHOVENTANA/2) + 40, caja.y + 120, colorNo);
}

void tableroCargarImagenes(Tablero* t, SDL_Renderer* render, int idSet)
{
    if(!t) return;
    const char* rutaDorso = (idSet == 0) ? RUTADORSOA : RUTADORSOB;
    t->dorso = IMG_LoadTexture(render, rutaDorso);
    if(!t->dorso) printf("Error dorso: %s\n", IMG_GetError());

    const char* carpeta = (idSet == 0) ? RUTASETA : RUTASETB;
    char buffer[256];
    // Para el tablero 4x5 (20 cartas) necesitamos 10 imagenes.
    // Iteramos hasta CANTIDADIMAGENES (que es 10 en comun.h)
    for(int i=0; i<CANTIDADIMAGENES; i++) {
        sprintf(buffer, "%s%d.png", carpeta, i);
        t->imagenes[i] = IMG_LoadTexture(render, buffer);
    }
    t->cantidadImagenesCargadas = CANTIDADIMAGENES;
}

void tableroMezclar(Tablero* t)
{
    if(!t)
    {
        return;
    }

    //ciclo para recorrer la cantidad de cartas en el juego
    //uso la cantidad de cartas que hay, en este caso sobre
    //el tablero van a haber 16 cartas, por lo cual uso esa cantidad

    for(int i=0; i<t->cantidad; i++)
    {
        //debo elegir una posicion al azar usando el resto
        int j = rand() % t->cantidad;

        //ahora hago un swap entre los id de las cartas para
        //cambiarlas de lugar, usando una variable auxiliar
        int aux = t->cartas[i].idImagen;
        t->cartas[i].idImagen = t->cartas[j].idImagen;
        t->cartas[j].idImagen = aux;

        //solo cambio su id, no su posicion de x e y, ni el puntero
        //del SDL_Rect, asi que en teoria siguien en su lugar las
        //cartas, solo que con otra foto
    }
}

void tableroManejarHover(Tablero* t, int x, int y)
{
    for(int i=0; i<t->cantidad; i++)
    {
        if(cartaAdentro(&t->cartas[i],x,y))
        {
            // Actualizamos la carta del teclado para que coincida con el mouse
            int col = i % t->columnas;
            int fil = i / t->columnas;
            t->cursorX = col;
            t->cursorY = fil;

            // Tambien marcamos hover
            if(!t->cartas[i].encontrada && !t->cartas[i].bocaArriba)
                t->cartas[i].hover = 1;
        }
        else
        {
            t->cartas[i].hover = 0;
        }
    }
}

int tableroCompleto(Tablero* t)
{
    if(!t)
    {
        return 0;
    }

    if(t->parejasEncontradas == (t->cantidad / 2))
    {
        return 1; //hubo victoria
    }

    return 0; //no hubo victoria
}

void dibujarEstadisticas(SDL_Renderer* render, TTF_Font* font, ContextoJuego* juego)
{
    SDL_Color colorBlanco = {255,255,255};
    char buffer[100];

    //dibujo los puntos
    sprintf(buffer, "Puntos: %d", juego->puntos);
    dibujarTextoCentrados(render, font, buffer, INTERFAZMARGENSUPERIOR, colorBlanco);

    //dibujo el nombre en pantalla
    sprintf(buffer, "Jugador: %s", juego->nombreJugador);
    dibujarTexto(render, font, buffer, INTERFAZMARGENLATERAL, INTERFAZMARGENSUPERIOR, colorBlanco);

    //dibujo el tiempo
    Uint32 segundos = (SDL_GetTicks() - juego->tiempoInicio) / 1000;
    sprintf(buffer, "Tiempo: %d", segundos);
    dibujarTexto(render, font, buffer, ANCHOVENTANA - INTERFAZMARGENLATERAL - 120, INTERFAZMARGENSUPERIOR, colorBlanco);
    dibujarTexto(render, font, "[ ESC: Salir ]", ANCHOVENTANA - 150, ALTOVENTANA - 50, (SDL_Color){255,100,100});
}

void dibujarTexto(SDL_Renderer* render, TTF_Font* font, const char* texto, int x, int y, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Blended(font, texto, color);
    if(surface)
    {
        SDL_Texture* textura = SDL_CreateTextureFromSurface(render, surface);
        if(textura)
        {
            SDL_Rect destinoRect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(render, textura, NULL, &destinoRect);
            SDL_DestroyTexture(textura);
        }
        SDL_FreeSurface(surface);
    }
}

//Revisar si esta funcion es realmente necesaria, podríamos mandar el valor x como parametro
//reutilizando la función dibujarTexto()
void dibujarTextoCentrados(SDL_Renderer* render, TTF_Font* font, const char* texto, int y, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Blended(font, texto, color);
    if(surface)
    {
        SDL_Texture* textura = SDL_CreateTextureFromSurface(render, surface);
        if(textura)
        {
            //calculo la variable para centrar los textos
            int x = (ANCHOVENTANA - surface->w) / 2;
            SDL_Rect destinoRect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(render, textura, NULL, &destinoRect);
            SDL_DestroyTexture(textura);
        }
        SDL_FreeSurface(surface);
    }
}
