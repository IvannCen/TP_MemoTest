#include "Tablero.h"
#include "Comun.h"


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
        t->puntosPorImagen[i] = 0;
    }

    t->puntaje = 0;
    t->racha = 0;

    // --- NUEVO: valores por defecto para configuracion ---
    t->filas = 4;
    t->cols = 4;
    t->setFiguras = 0;

    // --- NUEVO: estadisticas ---
    t->intentos = 0;
    t->aciertos = 0;
    t->fallos = 0;
    t->inicioMs = 0;
    t->finMs = 0;
    t->terminado = 0;
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

void tableroRellenar(Tablero* t)
{
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

    int i = 0;
    int clicResuelto = 0;

    //recorro mientras haya cartas por revisar y no se haya encontrado un clic
    while(i<t->cantidad && clicResuelto == 0)
    {
        //obtengo el puntero real a la carta del vector
        Carta* cartaActual = &t->cartas[i];

        //solo debo procesar las imagenes si se clickea una carta valida
        //y ademas debo permitir solo el clic cuando no este encontrada
        //ya que si se encontro el par no tiene sentido que pueda hacer
        //clic denuevo sobre esa carta y tampoco debe poder hacer clic
        //dos veces sobre una misma carta.
        if(cartaAdentro(cartaActual,x,y) && !cartaActual->encontrada)
        {
            if(!cartaActual->bocaArriba)
            {
                clicResuelto = 1; //proceso el clic

                //primer clic
                if(!t->cartaSeleccionada)
                {
                    cartaActual->bocaArriba = 1;
                    t->cartaSeleccionada = cartaActual;//guardo la dir
                    printf("Seleccion 1: id %d (dir: %p)\n", cartaActual->idImagen, (void*)cartaActual);
                }
                //segundo clic
                else
                {
                    Carta* carta1 = t->cartaSeleccionada; //recupero el puntero
                    Carta* carta2 = cartaActual; //puntero actual

                    //muestro la carta en pantalla
                    carta2->bocaArriba = 1;


                    //refresco la pantalla (de manera forzada)
                    //debo forzar el dibujo, ya que cuando probre sin hacer
                    //esto la pantalla se congelo antes de mostrar la carta
                    //creo que es un problema de la funcion de Delay
                    //refresco forzado para mostrar la segunda carta antes del delay
                    SDL_SetRenderDrawColor(render,50,50,50,255);
                    SDL_RenderClear(render);
                    tableroDibujar(t,render);
                    SDL_RenderPresent(render);


                    //hago la comparacion de punteros para ver si hubo coinicidencia
                    printf("Comparando ID %d con ID %d..\n", carta1->idImagen, carta2->idImagen);

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
                }
            }
        }
        i++;
    }
}

void tableroCargarImagenes(Tablero* t, SDL_Renderer* render)
{
    if(!t) return;

    //primero uso la funcion IMG_LoadTexture para abrir el archivo
    //y para subir el dorso a memoria
    //hago la verificacion de si se pudo cargar con exito
    if(t->setFiguras == 0)
        t->dorso = IMG_LoadTexture(render, "A/dorso.png");
    else
        t->dorso = IMG_LoadTexture(render, "B/dorso.png");
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

        // NUEVO: dos sets de figuras
        // Requiere carpetas A/ y B/ con 0.png, 1.png, 2.png...
        if(t->setFiguras == 0)
            sprintf(nombreArchivo, "A/%d.png", i);
        else
            sprintf(nombreArchivo, "B/%d.png", i);

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
    if(!t) return 0;

    for(int i=0; i<t->cantidad; i++)
    {
        //si hay cartas que no fueron encontradas, el juego sigue
        if(t->cartas[i].encontrada==0)
        {
            printf("Falta la carta %d (ID %d)\n", i, t->cartas[i].idImagen);
            return 0;//devuelvo 0 porque el juego no termino
        }
    }
    // Si llega aca es victoria
    if(!t->terminado)   // <-- IMPORTANTE (para que no lo resetee varias veces)
    {
        printf("Victoria detectada\n");
        t->terminado = 1;
        t->finMs = SDL_GetTicks();
    }
    return 1;
}
