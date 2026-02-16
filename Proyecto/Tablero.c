#include "Tablero.h"

void tableroIniciar(Tablero* t, Configuracion* config)
{
    int cant = config->filas * config->columnas;

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

    for(int i=0; i<cant; i++)
    {
        t->imagenes[i] = NULL;
    }

    t->rachaActual = 0;
    t->movimientos = 0;
    t->parejasEncontradas = 0;
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

void tableroRellenar(Tablero* t, int filas, int columnas)
{
    //el area donde se pueden poder las ventanas
    // Ancho: Toda la ventana menos un margen chico de costado
    // Alto: Toda la ventana menos el espacio de arriba para los tiempos y eso
    int anchoUtil = ANCHOVENTANA - (INTERFAZMARGENLATERAL * 2); // 20px de margen a los lados
    int altoUtil = ALTOVENTANA - ALTOINTERFAZ - INTERFAZMARGENSUPERIOR;

    //calculo el tamanio de la carta de manera dinamica, para que no sea un numero estatico
    //de la sig manera: (EspacioTotal - (Separaciones)) / Cantidad
    int anchoCalculado = (anchoUtil - ((columnas - 1) * SEPARACIONCARTA)) / columnas;
    int altoCalculado = (altoUtil - ((filas - 1) * SEPARACIONCARTA)) / filas;

    //si hay pocas cartas, el calculo daria cartas muy grandes
    //asi que pongo un limite maximo de tamanio
    // Pero si el calculo da menos, uso el calculado

    int anchoFinal = (anchoCalculado > ANCHOCARTA) ? ANCHOCARTA : anchoCalculado;
    int altoFinal = (altoCalculado > ALTOCARTA) ? ALTOCARTA : altoCalculado;

    //calculo cuanto ocupa todo el bloque completo en la ventana
    int anchoBloque = (columnas * anchoFinal) + ((columnas - 1) * SEPARACIONCARTA);
    int altoBloque = (filas * altoFinal) + ((filas - 1) * SEPARACIONCARTA);

    //margenes para centrar matematicamente en la pantalla
    int margenX = (ANCHOVENTANA - anchoBloque) / 2;
    int margenY = ALTOINTERFAZ + ((altoUtil - altoBloque) / 2);

    for(int i=0; i<t->cantidad; i++)
    {
        int col = i % columnas;
        int fil = i / columnas;

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
        SDL_Texture* texturaActual = NULL;

        if(t->cartas[i].encontrada || t->cartas[i].bocaArriba)
        {
            texturaActual = t->imagenes[t->cartas[i].idImagen];
        }
        else
        {
            texturaActual = t->dorso;
        }

        CartaDibujar(&t->cartas[i], render, texturaActual, mouseX, mouseY);
    }
}

int tableroClic(Tablero* t, int x, int y, SDL_Renderer* render, ContextoJuego* juego)
{
    if(!t || !juego)
    {
        return 0;
    }

    int puntos = 0;
    int i = 0;
    int clicResuelto = 0;

    //recorro mientras haya cartas por revisar y no se haya encontrado un clic
    while(i<t->cantidad && clicResuelto == 0)
    {

        //solo debo procesar las imagenes si se clickea una carta valida
        //y ademas debo permitir solo el clic cuando no este encontrada
        //ya que si se encontro el par no tiene sentido que pueda hacer
        //clic denuevo sobre esa carta y tampoco debe poder hacer clic
        //dos veces sobre una misma carta.
        if(cartaAdentro(&t->cartas[i],x,y) && !t->cartas[i].encontrada && !t->cartas[i].bocaArriba)
        {
            clicResuelto = 1; //proceso el clic

            //primer clic
            if(!t->cartaSeleccionada)
            {
                //reproduzco el sonido seleccionado (1 vez)
                sonidos_reproducir(juego->sndSeleccion, 1);

                t->cartas[i].bocaArriba = 1;
                t->cartaSeleccionada = &t->cartas[i];//guardo la dir
                printf("Seleccion 1: id %d\n", t->cartas[i].idImagen);
            }
            //segundo clic
            else
            {
                Carta* carta1 = t->cartaSeleccionada; //recupero el puntero
                Carta* carta2 = &t->cartas[i]; //puntero actual

                //muestro la carta en pantalla
                carta2->bocaArriba = 1;
                t->movimientos++;

                //refresco la pantalla (de manera forzada)
                //debo forzar el dibujo, ya que cuando probre sin hacer
                //esto la pantalla se congelo antes de mostrar la carta
                //creo que es un problema de la funcion de Delay
                tableroDibujar(t,render,0,0);
                SDL_RenderPresent(render);

                if(carta1->idImagen == carta2->idImagen)
                {
                    printf("Hubo coincidencia\n");
                    sonidos_reproducir(juego->sndAcierto, 1);

                    //asigno los valores
                    carta1->encontrada = 1;
                    carta2->encontrada = 1;
                    t->parejasEncontradas++;
                    puntos = 100 + (t->rachaActual * 20);
                    t->rachaActual++;
                }
                else
                {
                    printf("No hubo coincidencia\n");
                    sonidos_reproducir(juego->sndFallo, 1);

                    SDL_Delay(DELAY);
                    carta1->bocaArriba = 0;
                    carta2->bocaArriba = 0;
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

void tableroCargarImagenes(Tablero* t, SDL_Renderer* render, int idSet)
{
    if(!t)
    {
        return;
    }

    const char* rutaDorso = (idSet == 0) ? RUTADORSOA : RUTADORSOB;
    //primero uso la funcion IMG_LoadTexture para abrir el archivo
    //y para subir el dorso a memoria
    //hago la verificacion de si se pudo cargar con exito
    t->dorso = IMG_LoadTexture(render, rutaDorso);

    if(!t->dorso)
    {
        printf("Error al cargar el archivo 'dorso.png'.SDL_image error: %s\n", IMG_GetError());
    }
    //ahora hago la carga de las imagenes

    const char* carpetaFiguras = (idSet == 0) ? RUTASETA : RUTASETB;
    int i=0;
    int seguirBuscando = 1;
    char nombreArchivo[64];

    while(i<CANTIDADIMAGENES && seguirBuscando)
    {
        //uso sprintf para escribir el numero de la imagen en el
        //nombre del archivo a abrir
        //sprintf(nombreArchivo, "%d.png", i);
        //aca hay que implementar una funcion que dependiendo de los que elija el usuario, va a tomar un
        //pack de textura u otro (tambien con lo del dorso)

        sprintf(nombreArchivo, "%s%d.png",carpetaFiguras, i);

        SDL_Texture* temp = IMG_LoadTexture(render, nombreArchivo);
        if(temp)
        {
            t->imagenes[i] = temp;
            i++;
        }
        else
        {
            seguirBuscando = 0;
        }
    }

    t->cantidadImagenesCargadas = i;
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

//    //dibujo el nivel
//    sprintf(buffer, "Nivel: %d", juego->nivelActual);
//    dibujarTexto(render, font, buffer, INTERFAZMARGENLATERAL, INTERFAZMARGENSUPERIOR, colorBlanco);

    //dibujo los puntos
    sprintf(buffer, "Puntos: %d", juego->puntos);
    dibujarTextoCentrados(render, font, buffer, INTERFAZMARGENSUPERIOR, colorBlanco);

    //dibujo el nombre en pantalla
    sprintf(buffer, "Jugador: %s", juego->nombreJugador);
//    dibujarTextoCentrados(render, font, buffer, INTERFAZMARGENSUPERIOR + 30, colorBlanco);
    dibujarTexto(render, font, buffer, INTERFAZMARGENLATERAL, INTERFAZMARGENSUPERIOR, colorBlanco);

    //dibujo el tiempo
    Uint32 segundos = (SDL_GetTicks() - juego->tiempoInicio) / 1000;
    sprintf(buffer, "Tiempo: %d", segundos);
    dibujarTexto(render, font, buffer, ANCHOVENTANA - INTERFAZMARGENLATERAL - 120, INTERFAZMARGENSUPERIOR, colorBlanco);
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
