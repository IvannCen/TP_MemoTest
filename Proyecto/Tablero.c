#include "Tablero.h"
#include "Comun.h"

//hay que revisar estas funciones

/*
static void tableroResetPuntuacion(Tablero* t)
{
    t->puntaje = 0;
    t->racha = 0;

    // Si hay N imagenes cargadas, asigno puntos base a esas parejas
    // (ejemplo: 50..200 en saltos de 10)
    for(int i=0; i<CANTIDADIMAGENES; i++)
        t->puntosPorImagen[i] = 0;

    for(int i=0; i<t->cantidadImagenesCargadas; i++)
    {
        int base = 50 + (rand() % 16) * 10; // 50..200
        t->puntosPorImagen[i] = base;
    }
}

float tableroGetBonusRacha(Tablero* t)
{
    if(!t) return 0.0f;
    if(t->racha <= 0) return 0.0f;

    // racha=1 -> 0.2
    // racha=2 -> 0.3
    // racha=3 -> 0.4 ...
    return 0.1f * (t->racha + 1);
}

int tableroGetPuntaje(Tablero* t)
{
    if(!t) return 0;
    return t->puntaje;
}

int tableroGetRacha(Tablero* t)
{
    if(!t) return 0;
    return t->racha;
}

// -----------------------------
// NUEVO: CONFIGURACION + STATS
// -----------------------------
void tableroSetDimensiones(Tablero* t, int filas, int cols)
{
    if(!t) return;
    t->filas = filas;
    t->cols = cols;
}

void tableroSetSetFiguras(Tablero* t, int setFiguras)
{
    if(!t) return;
    t->setFiguras = setFiguras; // 0=A, 1=B
}

int tableroGetIntentos(Tablero* t)
{
    if(!t) return 0;
    return t->intentos;
}

int tableroGetAciertos(Tablero* t)
{
    if(!t) return 0;
    return t->aciertos;
}

int tableroGetFallos(Tablero* t)
{
    if(!t) return 0;
    return t->fallos;
}

int tableroGetTiempoSegundos(Tablero* t)
{
    if(!t) return 0;
    if(t->inicioMs == 0) return 0;

    Uint32 ahora = SDL_GetTicks();

    if(t->terminado && t->finMs != 0)
        ahora = t->finMs;

    return (int)((ahora - t->inicioMs) / 1000);
}

*/

/*
para recordar, en esta funcion estoy iniciando el tablero que se
va a ver en pantalla, donde cant es la cantidad de cartas que se
pueden seleccionar por pantalla. Despues tengo que modificar ese
cant para que haya mas niveles de dificultad, ya que si lo dejo
estatico solo tendria un nivel de 16 cartas. despues tengo que
modificarlo
*/

/*
tanto en iniciar como en destruir tuve que agregar un iniciador
y destructor de imagenes, poniendo todo en null porque sino
al momento de cerrar el programa me tiraba error porque estaba
queriendo leer basura que habia en el vector antes de iniciar el
programa, porque en el ejemplo solo puse 8 imagenes y el vector
podia tener 10 imagenes, por eso lo demas que sobraba tenia basura
y por eso habia que poner todo en null antes de empezar y al
destruir las cartas. ademas es por un problema del comando de la
biblioteca, porque SDL_DestroyTexture no funciona sobre basura,
por eso tengo que poner en null todo por los espacios sobrantes
*/
void tableroIniciar(Tablero* t, int cant)
{
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
        //t->puntosPorImagen[i] = 0;
    }

//    t->puntaje = 0;
//    t->racha = 0;
//
//     --- NUEVO: valores por defecto para configuracion ---
//    t->filas = 4;
//    t->cols = 4;
//    t->setFiguras = 0;
//
//     --- NUEVO: estadisticas ---
//    t->intentos = 0;
//    t->aciertos = 0;
//    t->fallos = 0;
//    t->inicioMs = 0;
//    t->finMs = 0;
//    t->terminado = 0;
}

void tableroDestruir(Tablero* t)
{
    if(t->cartas)
    {
        free(t->cartas);
        t->cartas = NULL;
        t->cantidad = 0;
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

/*
cuando saco los else if de centrado de cartas el juego sigue corriendo sin centrar las cartas
por lo cual habria que ver la manera de que cuando sacamos los else if solo haya los niveles que dejamos puestos
para poder probar en un solo nivel y no en los 7 que hay
*/

void tableroRellenar(Tablero* t)
{
    //lo de las rachas esta copado, habria que hacer una funcion aparte y ademas modificar las rachas, ya que solo se queda en 0.2, y deberia ir subiendo
    //esta bueno, solo hay que revisar y modificar un par de cosas

    /*
    // Cada vez que se arranca una partida nueva, reseteo puntuacion + rachas
    tableroResetPuntuacion(t);

    // --- NUEVO: reset estadisticas ---
    t->intentos = 0;
    t->aciertos = 0;
    t->fallos = 0;
    t->inicioMs = SDL_GetTicks();
    t->finMs = 0;
    t->terminado = 0;

    for(int i=0; i<t->cantidad; i++)
    {
        int col = i % t->cols;
        int fil = i / t->cols;

        // ancho total del tablero
        int anchoTablero = t->cols * ANCHOCARTA +
                           (t->cols - 1) * SEPARACIONCARTA;

        // alto total del tablero
        int altoTablero = t->filas * ALTOCARTA +
                          (t->filas - 1) * SEPARACIONCARTA;

        // centrado horizontal
        int inicioX = (ANCHOVENTANA - anchoTablero) / 2;

        // centrado vertical debajo del HUD
        int inicioY = HUD_ALTO +
                      (ALTOVENTANA - HUD_ALTO - altoTablero) / 2;

        int posX = inicioX + col * (ANCHOCARTA + SEPARACIONCARTA);
        int posY = inicioY + fil * (ALTOCARTA + SEPARACIONCARTA);

        int idImagen = (i/2) % t->cantidadImagenesCargadas;

        CartaInicial(&t->cartas[i], idImagen,
                     posX, posY,
                     ANCHOCARTA, ALTOCARTA);

    */
    int columnas;
    if(t->cantidad == 6)
    {
        columnas = 3; //nivel 1: con 6 cartas, entonces el tablero queda de 3x2 (queda centrado)
    }
    else if(t->cantidad == 12)
    {
        columnas = 4; //nivel 2: con 12 cartas, entonces el tablero queda de 3x4 (queda centrado)
    }
    else if(t->cantidad == 20)
    {
        columnas = 5; //nivel 3: con 20 cartas, entonces el tablero queda de 4x5 (queda centrado)
    }
    else if(t->cantidad == 30)
    {
        columnas = 6; //nivel 4: con 30 cartas, entonces el tablero queda de 5x6 (queda centrado)
    }
    else if(t->cantidad == 42)
    {
        columnas = 7; //nivel 5: con 42 cartas, entonces el tablero queda de 6x7 (queda centrado)
    }

    // Calculamos las filas necesarias con la cantidad y las columnas
    int filas = t->cantidad / columnas;

    //el area donde se pueden poder las ventanas
    // Ancho: Toda la ventana menos un margen chico de costado
    // Alto: Toda la ventana menos el espacio de arriba para los tiempos y eso
    int anchoUtil = ANCHOVENTANA - (INTERFAZMARGENLATERAL * 2); // 20px de margen a los lados
    int altoUtil = ALTOVENTANA - ALTOINTERFAZ - INTERFAZMARGENSUPERIOR;

    //calculo el tama�o de la carta de manera dinamica, para que no sea un numero estatico
    //de la sig manera: (EspacioTotal - (Separaciones)) / Cantidad
    int anchoCalculado = (anchoUtil - ((columnas - 1) * SEPARACIONCARTA)) / columnas;
    int altoCalculado = (altoUtil - ((filas - 1) * SEPARACIONCARTA)) / filas;

    //si hay pocas cartas, el calculo daria cartas muy grandes
    //asi que pongo un limite maximo de tama�o
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


//dibujo todas las cartas de adentro del tablero
void tableroDibujar(Tablero* t, SDL_Renderer* render)
{
    if(!t || !t->cartas) return;

    for(int i=0; i<t->cantidad; i++)
    {
        SDL_Texture* texturaActual = NULL;

        if(t->cartas[i].encontrada || t->cartas[i].bocaArriba)
        {
            int id = t->cartas[i].idImagen;
            texturaActual = t->imagenes[id];
        }
        else
        {
            texturaActual = t->dorso;
        }

        CartaDibujar(&t->cartas[i], render, texturaActual);
    }
}

/*
a esta funcion tengo que agregarle funciones de SDL para el delay
cuando ocurre que no se selecciono el par correcto, y ademas para
forzar el dibujo con el render. por lo cual tengo que modificar
el prototipo para que reciba el render para dibujar no solo la
imagen seleccionada, sino tambien la segunda imagen que se elija
*/

//lo mismo que en el main (creo) no les gusta a los profes las funciones estaticas, habria que revisar

/*

static void aplicarAcierto(Tablero* t, int idImagen)
{
    int base = 0;
    if(idImagen >= 0 && idImagen < CANTIDADIMAGENES)
        base = t->puntosPorImagen[idImagen];

    // aumento racha primero
    t->racha++;

    float bonus = tableroGetBonusRacha(t);      // 0.2, 0.3, ...
    float total = (float)base * (1.0f + bonus); // base + base*bonus

    // redondeo a int
    int suma = (int)(total + 0.5f);
    t->puntaje += suma;

    printf("ACIERTO: base=%d, racha=%d, bonus=%.1f, suma=%d, puntaje=%d\n",
           base, t->racha, bonus, suma, t->puntaje);
}

static void aplicarFallo(Tablero* t)
{
    t->racha = 0;
    printf("FALLO: racha reseteada a 0\n");
}

void tableroClic(Tablero* t, int x, int y, SDL_Renderer* render)
{
    if(!t) return;
}

*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//aca dejo el codigo porque esta repetido y lo dejo para revisar (pertenece a la funcion de abajo)


//hay que revisar esta parte porque no nos decidimos todavia si mostrar estadisticas al final en el modo aventura y estdisticas por partida en el competitivo
                    //hay que ver esas cosas antes de hacer algo

                    /*
                    // --- NUEVO: estadisticas (un intento por cada comparacion de 2 cartas) ---
                    t->intentos++;

                    if(carta1->idImagen == carta2->idImagen)
                    {
                        printf("Hubo coincidencia\n");
                        //asigno los valores
                        carta1->encontrada = 1;
                        carta2->encontrada = 1;

                        // --- NUEVO: estadisticas ---
                        t->aciertos++;

                        // --- PUNTUACION + RACHA ---
                        aplicarAcierto(t, carta1->idImagen);
                    }
                    else
                    {
                        printf("No hubo coincidencia\n");

                        // --- NUEVO: estadisticas ---
                        t->fallos++;

                        SDL_Delay(DELAY);
                        carta1->bocaArriba = 0;
                        carta2->bocaArriba = 0;

                        // --- FALLO: reset racha ---
                        aplicarFallo(t);
                    }

                    t->cartaSeleccionada = NULL;

                    */
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int tableroClic(Tablero* t, int x, int y, SDL_Renderer* render)
{
    if(!t)
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
                t->cartas[i].bocaArriba = 1;
                t->cartaSeleccionada = &t->cartas[i];//guardo la dir
                printf("Seleccion 1: id %d (dir: %p)\n", t->cartas[i].idImagen, (void*)&t->cartas[i]);
            }
            //segundo clic
            else
            {
                Carta* carta1 = t->cartaSeleccionada; //recupero el puntero
                Carta* carta2 = &t->cartas[i]; //puntero actual

                //muestro la carta en pantalla
                carta2->bocaArriba = 1;

                //refresco la pantalla (de manera forzada)
                //debo forzar el dibujo, ya que cuando probre sin hacer
                //esto la pantalla se congelo antes de mostrar la carta
                //creo que es un problema de la funcion de Delay
                //SDL_SetRenderDrawColor(render,50,50,50,255);
                //SDL_RenderClear(render);
                tableroDibujar(t,render);
                SDL_RenderPresent(render);

                //hago la comparacion de punteros para ver si hubo coinicidencia
                printf("Comparando ID %d con ID %d..\n", carta1->idImagen, carta2->idImagen);

                if(carta1->idImagen == carta2->idImagen)
                {
                    printf("Hubo coincidencia\n");

                    //asigno los valores
                    carta1->encontrada = 1;
                    carta2->encontrada = 1;
                    puntos = 100;

                }
                else
                {
                    printf("No hubo coincidencia\n");
                    SDL_Delay(DELAY);
                    carta1->bocaArriba = 0;
                    carta2->bocaArriba = 0;
                    puntos = -20;
                }

                t->cartaSeleccionada = NULL;
            }
        }
        i++;
    }
    return puntos;
}

void tableroCargarImagenes(Tablero* t, SDL_Renderer* render)
{
    if(!t)
    {
        return;
    }

    //primero uso la funcion IMG_LoadTexture para abrir el archivo
    //y para subir el dorso a memoria
    //hago la verificacion de si se pudo cargar con exito
    t->dorso = IMG_LoadTexture(render, "dorso.png");

    /*
    if(t->setFiguras == 0)
        t->dorso = IMG_LoadTexture(render, "A/dorso.png");
    else
        t->dorso = IMG_LoadTexture(render, "B/dorso.png");
    */

    if(!t->dorso)
    {
        printf("Error al cargar el archivo 'dorso.png'.SDL_image error: %s\n", IMG_GetError());
    }
    //ahora hago la carga de las imagenes

    int i=0;
    int seguirBuscando = 1;
    char nombreArchivo[64];

    while(i<CANTIDADIMAGENES && seguirBuscando == 1)
    {
        //uso sprintf para escribir el numero de la imagen en el
        //nombre del archivo a abrir
        //sprintf(nombreArchivo, "%d.png", i);

        sprintf(nombreArchivo, "B/%d.png", i);

        /*
        // NUEVO: dos sets de figuras
        // Requiere carpetas A/ y B/ con 0.png, 1.png, 2.png...
        if(t->setFiguras == 0)
            sprintf(nombreArchivo, "A/%d.png", i);
        else
            sprintf(nombreArchivo, "B/%d.png", i);

        */
        SDL_Texture* temp = IMG_LoadTexture(render, nombreArchivo);
        if(temp)
        {
            t->imagenes[i] = temp;
            i++;
            printf("Cargada: %s\n", nombreArchivo);
        }
        else
        {
            seguirBuscando = 0;
        }
    }

    t->cantidadImagenesCargadas = i;
    printf("Total de imagenes cargadas: %d\n", t->cantidadImagenesCargadas);
}

void tableroMezclar(Tablero* t)
{
    if(!t) return;

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

    for(int i=0; i<t->cantidad; i++)
    {
        //si hay cartas que no fueron encontradas, el juego sigue
        if(t->cartas[i].encontrada==0)
        {
            printf("Falta la carta %d (ID %d)\n", i, t->cartas[i].idImagen);
            return 0;//devuelvo 0 porque el juego no termino
        }
    }

    /*
    // Si llega aca es victoria
    if(!t->terminado)   // <-- IMPORTANTE (para que no lo resetee varias veces)
    {
        printf("Victoria detectada\n");
        t->terminado = 1;
        t->finMs = SDL_GetTicks();
    }
    */
    printf("Victoria detectada\n");
    return 1;
}

void dibujarEstadisticas(SDL_Renderer* render, TTF_Font* font, ContextoJuego* juego)
{
    SDL_Color colorBlanco = {255,255,255};
    char buffer[50];
//    int anchoSector;
//    SDL_Rect areaSector;
//    Uint32 segundos;

//    //Dividimos el ancho de la pantalla en 4 sectores iguales
//    anchoSector = ANCHOVENTANA/4; //quizás sea mejor definir una macro que sea para la cant de sectores/columnas
//
//    areaSector.y = INTERFAZMARGENSUPERIOR;
//    areaSector.w = ALTOINTERFAZ;
//    areaSector.x = anchoSector;

    //dibujo el nivel
    sprintf(buffer, "Nivel: %d", juego->nivelActual);
    dibujarTexto(render, font, buffer, INTERFAZMARGENLATERAL, INTERFAZMARGENSUPERIOR, colorBlanco);

    //dibujo los puntos
    sprintf(buffer, "Puntos: %d", juego->puntos);
    dibujarTexto(render, font, buffer, ((ANCHOVENTANA/2)/2)-50, INTERFAZMARGENSUPERIOR, colorBlanco);

    //dibujo el nombre en pantalla
    sprintf(buffer, "Jugador: %s", juego->nombreJugador);
    dibujarTexto(render, font, buffer, (((ANCHOVENTANA/2)/2)+(ANCHOVENTANA/2))-50, INTERFAZMARGENSUPERIOR, colorBlanco);

    //dibujo el tiempo
    Uint32 segundos = (SDL_GetTicks() - juego->tiempoInicio) / 1000;
    sprintf(buffer, "Tiempo: %d", segundos);
    dibujarTexto(render, font, buffer, ANCHOVENTANA - INTERFAZMARGENLATERAL - 120, INTERFAZMARGENSUPERIOR, colorBlanco);

//    //Dibujamos el sector 1 (nivel)
//    areaSector.x = 0; //anchoSector * 0
//    sprintf(buffer, "Nivel: %d", juego->nivelActual);
//    dibujarTextoCentradoEnRect(render, font, buffer, areaSector, colorBlanco);
//
//    //Dibujamos el sector 2 (puntos)
//    areaSector.x = anchoSector; //anchoSector * 1 (se mueve un sector a la derecha)
//    sprintf(buffer, "Puntos: %d", juego->puntos);
//    dibujarTextoCentradoEnRect(render, font, buffer, areaSector, colorBlanco);
//
//    //Dibujamos el sector 3 (nombre del jugador)
//    areaSector.x = anchoSector * 2;
//    sprintf(buffer, "Jugador: %s", juego->nombreJugador);
//    dibujarTextoCentradoEnRect(render, font, buffer, areaSector, colorBlanco);
//
//    //Dibujamos el sector 4 (tiempo)
//    areaSector.x = anchoSector * 3;
//    segundos = (SDL_GetTicks() - juego->tiempoInicio) / 1000;
//    sprintf(buffer, "Tiempo: %d", (int)segundos);
//    dibujarTextoCentradoEnRect(render, font, buffer, areaSector, colorBlanco);
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

//void dibujarTextoCentradoEnRect(SDL_Renderer* render, TTF_Font* font, const char* texto, SDL_Rect area, SDL_Color color)
//{
//    SDL_Surface* surface;
//    SDL_Texture* textura;
//    SDL_Rect destinoRect;
//
//    if (!render || !font || !texto)
//    {
//        return;
//    }
//
//    surface = TTF_RenderText_Blended(font, texto, color);
//    if (!surface)
//    {
//        return;
//    }
//
//    textura = SDL_CreateTextureFromSurface(render, surface);
//    if (!textura)
//    {
//        SDL_FreeSurface(surface);
//        return;
//    }
//
//    /*
//       Logica de centrado matematico:   X = inicioCaja + (anchoCaja - anchoTexto) / 2
//                                        Y = inicioCaja + (altoCaja - altoTexto) / 2
//    */
//
//    destinoRect.w = surface->w;
//    destinoRect.h = surface->h;
//    destinoRect.x = area.x + (area.w - surface->w) / 2;
//    destinoRect.y = area.y + (area.h - surface->h) / 2;
//
//    SDL_RenderCopy(render, textura, NULL, &destinoRect);
//
//    SDL_DestroyTexture(textura);
//    SDL_FreeSurface(surface);
//}
