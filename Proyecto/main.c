#include "Tablero.h"
#include "Juego.h"
#include "Menu.h"
#include "Carta.h"

int main(int argc, char *argv[])
{
    srand(time(NULL)); //para generar la aleatoriedad en el programa

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    if(IMG_Init(IMG_INIT_PNG)==0)
        return 1;

    if(TTF_Init()==-1) //inicio las fuentes del juego para las letras
        return 1;


    //Inicializo el subsistema de sonido
    tFormatosSnd formatos = sonidos_inicializar();

    if(formatos == SONIDO_ERR)
    {
        return 1;
    }

    // Usamos tus constantes de Comun.h
    //NOTA: agrego "| SDL_WINDOW_RESIZABLE" para habilitar los botones de maximizar y los
    //que modifican el tamanio de la ventana.
    SDL_Window *window = SDL_CreateWindow("TP Memotest",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          ANCHOVENTANA, ALTOVENTANA,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //funcion inteligente que sirve para el manejo de la pantalla
    SDL_RenderSetLogicalSize(renderer,ANCHOVENTANA,ALTOVENTANA);

    //inicio las fuentes para que se vean en la pantalla
    TTF_Font* fuenteGrande = TTF_OpenFont(FUENTE, FUENTEGRANDE);
    TTF_Font* fuenteChica = TTF_OpenFont(FUENTE, FUENTECHICA);
    TTF_Font* fuenteMedia= TTF_OpenFont(FUENTE, FUENTEMEDIA);

    if(!fuenteGrande || !fuenteChica)
    {
        printf("Error en la fuente ingresada: %s\n", TTF_GetError());
    }

    //sistema del menu
    Menu menuPrincipal;
    menuIniciar(&menuPrincipal, renderer);

    //logica inicial para arrancar en el menu
    EstadoJuego estadoActual = ESTADO_MENU;
    Tablero miTablero;
    ContextoJuego juego;

    //variables del juego
    juego.puntos = 0;
    //variable para ver si hay un tablero en memoria
    int tableroCargado = 0;
    // loop del juego
    int corriendo = 1;
    SDL_Event e;

    // Usamos sonidos_cargar.
    // IMPORTANTE: Asegurate de tener la carpeta /snd con estos archivos
    juego.sndSeleccion = sonidos_cargar(SELECCION);
    juego.sndAcierto = sonidos_cargar(ACIERTO);
    juego.sndFallo = sonidos_cargar(FALLO);
    Mix_Volume(-1, 30); // Reducion del volumen al 30% de forma fija a efectos de no molestar a quien inicie esta demo

    while (corriendo)
    {
        // proceso los ingresos segun el estado en el que se encuentre el juego
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                corriendo = 0;
            }

            switch(estadoActual)
            {
                case ESTADO_MENU:
                {
                    int seleccion = menuManejarOpciones(&menuPrincipal, &e);
                    if(seleccion != -1)
                    {
                        if(seleccion == OPCION_AVENTURA)
                        {
                        menuPrincipal.opcionSeleccionada = 0;
                        estadoActual = ESTADO_DIFICULTAD;
                        }
                        else if(seleccion == OPCION_SALIR)
                        {
                            corriendo = 0;
                        }
                        else if(seleccion == OPCION_COMPETITIVO)
                        {
                            printf("Proximamente Modo Competitivo\n");
                        }
                        else if(seleccion == OPCION_ESTADISTICAS)
                        {
                            printf("Proximamente Ranking\n");
                        }
                        else if(seleccion == OPCION_CONFIGURACION)
                        {
                            printf("Proximamente Configuracion\n");
                        }
                    }
                    break;
                }
                case ESTADO_DIFICULTAD:
                {
                    int dif = menuDificultadOpciones(&menuPrincipal, &e);
                    if(dif != -1)
                    {
                        if(dif==VOLVER)
                        {
                            menuPrincipal.opcionSeleccionada=0;
                            estadoActual = ESTADO_MENU;
                        }
                        else
                        {
                            //guardo la dificultad seleccionada
                            juego.dificultad=dif;
                            ingresoNombreIniciar(&menuPrincipal.nombre);
                            estadoActual = ESTADO_NOMBRE;
                        }
                    }
                    break;
                }
                case ESTADO_NOMBRE:
                {
                    int confirmado = ingresoNombreOpciones(&menuPrincipal.nombre, &e, &juego);
                    if(confirmado)
                    {
                        juego.nivelActual = 1;
                        juego.puntos = 0;
                        juego.tiempoInicio = SDL_GetTicks();//esta funcion resetea el reloj
                        //aca deberia modificar la funcion de obtenerCartas para cada nivel
                        //de dificultad, pero para probar pongo hardcodeado
                        int cartas = obtenerCartasPorDificultad(juego.dificultad);
                        tableroIniciar(&miTablero, cartas);
                        tableroCargarImagenes(&miTablero,renderer);
                        tableroRellenar(&miTablero); //reinicio las posiciones de las cartas
                        tableroMezclar(&miTablero); //mezclo las cartas en el tablero
                        tableroCargado = 1;
                        estadoActual = ESTADO_JUGANDO;
                    }
                    break;
                }
                case ESTADO_JUGANDO:
                {
                    if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                    {
                        //obtengo los puntos obtenidos de la funcion de clic
                        int puntos = tableroClic(&miTablero,e.button.x,e.button.y, renderer, &juego); // tableroClic necesita acceder a juego->sndSeleccion, etc.

                        //sumo los puntos a la variable (o los resto dependiendo del resultado
                        juego.puntos += puntos;

                        //si hay puntaje negativo los evito
                        if(juego.puntos<0)
                        {
                            juego.puntos = 0;
                        }
                        //verifico si hubo una victoria luego del clic
                        if(tableroCompleto(&miTablero))
                        {
                            SDL_Delay(DELAYCHICO); //delay de medio segundo, como una pausa chica
                            estadoActual=ESTADO_GANO;
                        }
                    }
                    break;
                }
                case ESTADO_GANO:
                {
                    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
                    {
                        //aumento el nivel en el que estoy
                        juego.nivelActual++;

                        //elimino el tablero del nivel anterior
                        if(tableroCargado)
                        {
                            tableroDestruir(&miTablero);
                            tableroCargado = 0;
                        }

                        //calculo el nivel de dificultad otra vez para el siguiente nivel
                        int cartas = obtenerCartasPorNivel(juego.nivelActual);

                        if(cartas > 0)
                        {
                            //cargo el siguiente nivel
                            tableroIniciar(&miTablero, cartas);
                            tableroCargarImagenes(&miTablero,renderer);//reinicio las texturas
                            tableroRellenar(&miTablero);
                            tableroMezclar(&miTablero);
                            tableroCargado = 1;
                            estadoActual = ESTADO_JUGANDO;
                        }
                        else
                        {
                            estadoActual=ESTADO_MENU;
                        }
                    }
                    break;
                }
            }
        }

        //Renderizado segun el estado en el que este el juego(Dibujar)
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Fondo Gris oscuro
        SDL_RenderClear(renderer);

        if(estadoActual == ESTADO_MENU)
        {
            menuDibujar(&menuPrincipal, renderer);

        }
        else if(estadoActual == ESTADO_DIFICULTAD)
        {
            menuDificultadDibujar(&menuPrincipal, renderer);
        }
        else if(estadoActual == ESTADO_NOMBRE)
        {
            ingresoNombreDibujar(&menuPrincipal.nombre, renderer);
        }
        else if(estadoActual == ESTADO_JUGANDO)
        {
            //dibujo el juego en si mismo
            tableroDibujar(&miTablero, renderer);
            dibujarEstadisticas(renderer,fuenteChica,&juego);
        }
        else if(estadoActual == ESTADO_GANO)
        {
            if(tableroCargado)
            {
                //dibujo la pantalla de victoria, con el tablero de fondo
                tableroDibujar(&miTablero, renderer);
            }

            //dibujo el fondo transparente de fondo
            //activo un blending para que se acepten transparencias
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            //agrego el color negro para el cuadrado, poniendo 200 de alpha, que le da solides al color
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // Negro con opacidad al 80
            //hago el rectangulo donde va a ir el texto de Ganaste
            SDL_Rect rect = {0, 0, ANCHOVENTANA, ALTOVENTANA};
            SDL_RenderFillRect(renderer, &rect);
            //desactivo el blending para que lo demas se dibuje
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            //color de los textos
            SDL_Color colorVerde = {50, 255, 50};
            SDL_Color colorBlanco = {255, 255, 255};
            SDL_Color colorAmarillo = {255, 255, 0};

            //dibujo el texto encima del tablero
            dibujarTextoCentrados(renderer, fuenteGrande, "NIVEL COMPLETADO", 250, colorVerde);
            char puntajeBuffer[64];
            sprintf(puntajeBuffer, "Puntos Totales: %d", juego.puntos);
            dibujarTextoCentrados(renderer,fuenteMedia,puntajeBuffer,350,colorAmarillo);
            dibujarTextoCentrados(renderer,fuenteMedia,"Presiona SPACE para continuar",500,colorBlanco);
        }
        SDL_RenderPresent(renderer);
    }

    //limpio todas las variables
    menuDestruir(&menuPrincipal);

    sonidos_destruir(juego.sndSeleccion);
    sonidos_destruir(juego.sndAcierto);
    sonidos_destruir(juego.sndFallo);
    sonidos_finalizar();

    TTF_CloseFont(fuenteGrande);
    TTF_CloseFont(fuenteMedia);
    TTF_CloseFont(fuenteChica);
    if(tableroCargado)
    {
        tableroDestruir(&miTablero); //libero el vector dinamico
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); // cierro el TTF
    IMG_Quit();
    SDL_Quit();

    return 0;
}
