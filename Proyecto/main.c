/**
  * Apellido(s), nombre(s): [Perez, Juan]
  * DNI: [11111111]
  * Entrega: Si/No
  *
  * Apellido(s), nombre(s): [Gomez, Maria]
  * DNI: [22222222]
  * Entrega: Si/No
  *
  * Apellido(s), nombre(s): [Lopez, Carlos]
  * DNI: [33333333]
  * Entrega: Si/No
  *
  * Apellido(s), nombre(s): [Diaz, Ana]
  * DNI: [44444444]
  * Entrega: Si/No
  *
  */

#include "Tablero.h"
#include "Juego.h"
#include "Menu.h"
#include "Carta.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;
    if(IMG_Init(IMG_INIT_PNG)==0)
        return 1;
    if(TTF_Init()==-1)
        return 1;

    tFormatosSnd formatos = sonidos_inicializar();

    if(formatos == SONIDO_ERR)
        return 1;

    SDL_Window *window = SDL_CreateWindow("TP Memotest", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, ANCHOVENTANA, ALTOVENTANA, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer,ANCHOVENTANA,ALTOVENTANA);

    TTF_Font* fuenteGrande = TTF_OpenFont(FUENTE, FUENTEGRANDE);
    TTF_Font* fuenteChica = TTF_OpenFont(FUENTE, FUENTECHICA);
    TTF_Font* fuenteMedia= TTF_OpenFont(FUENTE, FUENTEMEDIA);

    Menu menuPrincipal;
    menuIniciar(&menuPrincipal, renderer);

    Tablero miTablero;
    Configuracion config;
    ContextoJuego juego;
    juego.confirmandoSalida = 0;
    juego.opcionSalidaPopup = 1;

    cargarConfiguracion(&config);
    EstadoJuego estadoActual = ESTADO_MENU;

    int tableroCargado = 0;
    int corriendo = 1;
    int jugadorIngresando = 0;
    SDL_Event e;

    juego.sndSeleccion = sonidos_cargar(SELECCION_SND);
    juego.sndAcierto = sonidos_cargar(ACIERTO_SND);
    juego.sndFallo = sonidos_cargar(FALLO_SND);

    juego.musMenu = sonidos_cargar_musica(MENU_SND);
    juego.musJuego = sonidos_cargar_musica(JUEGO_SND);

    Mix_Volume(-1, VOL_MAX);
    Mix_VolumeMusic(VOL_MAX);

    sonidos_reproducir_musica(juego.musMenu, -1);

    while(corriendo)
    {
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                corriendo = 0;

            switch(estadoActual)
            {
                case ESTADO_MENU:
                {
                    int seleccion = menuManejarOpciones(&menuPrincipal, &e);
                    if(seleccion != -1)
                    {
                        if(seleccion == OPCION_JUGAR)
                        {
                            if(config.cantJugadores == 1)
                            {
                                jugadorIngresando = 0;
                                ingresoNombreIniciar(&menuPrincipal.nombre,"NOMBRE JUGADOR");
                                estadoActual = ESTADO_NOMBRE;
                            }
                            else
                            {
                                jugadorIngresando = 0;
                                ingresoNombreIniciar(&menuPrincipal.nombre,"NOMBRE JUGADOR 1");
                                estadoActual = ESTADO_NOMBRE;
                            }
                        }
                        else if(seleccion == OPCION_SALIR)
                            corriendo = 0;
                        else if(seleccion == OPCION_CONFIGURACION)
                        {
                            menuPrincipal.opcionSeleccionada = 0;
                            estadoActual = ESTADO_CONFIGURACION;
                        }
                        else if(seleccion == OPCION_ESTADISTICAS)
                            estadoActual = ESTADO_RANKING;
                    }
                    break;
                }
                case ESTADO_CONFIGURACION:
                {
                    int salir = menuConfiguracionOpciones(&menuPrincipal, &e, &config);
                    if(salir)
                    {
                        guardarConfiguracion(&config);
                        menuPrincipal.opcionSeleccionada = 0;
                        estadoActual = ESTADO_MENU;
                    }
                    break;
                }
                case ESTADO_RANKING:
                    {
                        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                            estadoActual = ESTADO_MENU;
                    }
                case ESTADO_NOMBRE:
                {
                    int res = ingresoNombreOpciones(&menuPrincipal.nombre, &e, &juego, jugadorIngresando);
                    if(res == 1)
                    {
                        if(config.cantJugadores == 2 && jugadorIngresando == 0)
                        {
                            //Sigue la musica del menu
                            jugadorIngresando = 1;
                            ingresoNombreIniciar(&menuPrincipal.nombre, "NOMBRE JUGADOR 2");
                        }
                        else
                        {
                            juego.turnoJugador = 0;
                            juego.puntos[0] = 0;
                            juego.puntos[1] = 0;
                            juego.cantJugadores = config.cantJugadores;
                            juego.tiempoInicio = SDL_GetTicks();
                            juego.confirmandoSalida = 0;
                            juego.opcionSalidaPopup = 1;

                            tableroIniciar(&miTablero, &config);
                            tableroCargarImagenes(&miTablero,renderer, config.idSetImagenes);
                            tableroRellenar(&miTablero);
                            tableroMezclar(&miTablero);
                            tableroCargado = 1;

                            sonidos_reproducir_musica(juego.musJuego, -1);

                            estadoActual = ESTADO_JUGANDO;

                            // Limpiar cola de eventos para evitar inputs fantasmas al iniciar
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        }
                    }
                    else if(res == 2)
                    {
                        //Vuelve al menu (sigue la musica del menu)
                        menuPrincipal.opcionSeleccionada = 0;
                        estadoActual = ESTADO_MENU;
                    }
                    break;
                }
                case ESTADO_JUGANDO:
                {
                    if(juego.confirmandoSalida)
                    {
                        if(e.type == SDL_KEYDOWN)
                        {
                            if(e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)
                                juego.opcionSalidaPopup = !juego.opcionSalidaPopup;
                            else if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
                            {
                                if(juego.opcionSalidaPopup == 0) // SI
                                {
                                    if(tableroCargado)
                                    {
                                        tableroDestruir(&miTablero);
                                        tableroCargado = 0;
                                    }
                                    juego.confirmandoSalida = 0;
                                    menuPrincipal.confirmaSalida = 0;

                                    //Cambia la musica a la del menu
                                    sonidos_reproducir_musica(juego.musMenu, -1);

                                    estadoActual = ESTADO_MENU;
                                    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                                }
                                else
                                    juego.confirmandoSalida = 0; // NO
                            }
                            else if(e.key.keysym.sym == SDLK_ESCAPE)
                                juego.confirmandoSalida = 0;
                        }
                        else if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                        {
                            int mx = e.button.x;
                            int my = e.button.y;
                            SDL_Point p = {mx, my};
                            int cY = (ALTOVENTANA-200)/2;
                            SDL_Rect rSi = { (ANCHOVENTANA/2)-80, cY+120, 50, 40 };
                            SDL_Rect rNo = { (ANCHOVENTANA/2)+40, cY+120, 50, 40 };

                            if(SDL_PointInRect(&p, &rSi))
                            {
                                if(tableroCargado)
                                {
                                    tableroDestruir(&miTablero);
                                    tableroCargado = 0;
                                }
                                juego.confirmandoSalida = 0;
                                menuPrincipal.confirmaSalida = 0;

                                //Cambia la musica a la del menu
                                sonidos_reproducir_musica(juego.musMenu, -1);

                                estadoActual = ESTADO_MENU;
                                SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            }
                            else if(SDL_PointInRect(&p, &rNo))
                                juego.confirmandoSalida = 0;
                        }
                        else if(e.type == SDL_MOUSEMOTION)
                        {
                            int mx = e.motion.x, my = e.motion.y;
                            SDL_Point p = {mx, my};
                            int cY = (ALTOVENTANA-200)/2;
                            SDL_Rect rSi = { (ANCHOVENTANA/2)-80, cY+120, 50, 40 };
                            SDL_Rect rNo = { (ANCHOVENTANA/2)+40, cY+120, 50, 40 };
                            if(SDL_PointInRect(&p, &rSi))
                                juego.opcionSalidaPopup = 0;
                            if(SDL_PointInRect(&p, &rNo))
                                juego.opcionSalidaPopup = 1;
                        }
                    }
                    else
                    {
                        if(e.type == SDL_KEYDOWN) {
                            if(e.key.keysym.sym == SDLK_ESCAPE)
                            {
                                juego.confirmandoSalida = 1;
                                juego.opcionSalidaPopup = 1;
                            }
                            else
                            {
                                tableroManejarTeclado(&miTablero, &e, renderer, &juego, fuenteChica);
                                if(tableroCompleto(&miTablero))
                                {
                                    SDL_Delay(DELAYCHICO);
                                    estadoActual=ESTADO_GANO;
                                }
                            }
                        }
                        else if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                        {
                            tableroClic(&miTablero,e.button.x,e.button.y, renderer, &juego, fuenteChica);
                            if(tableroCompleto(&miTablero))
                            {
                                SDL_Delay(DELAYCHICO);
                                estadoActual=ESTADO_GANO;
                            }
                        }
                        else if(e.type == SDL_MOUSEMOTION) {
                            tableroManejarHover(&miTablero, e.motion.x, e.motion.y);
                        }
                    }
                    break;
                }
                case ESTADO_GANO:
                {
                    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
                    {
                        if(juego.cantJugadores == 1)
                        {
                            int tiempo = (SDL_GetTicks() - juego.tiempoInicio) / 1000;
                            rankingGuardar(juego.nombreJugador[0], juego.puntos[0], tiempo);
                        }
                        if(tableroCargado)
                        {
                            tableroDestruir(&miTablero);
                            tableroCargado = 0;
                        }
                        sonidos_reproducir_musica(juego.musMenu, -1);

                        estadoActual=ESTADO_MENU;
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        if(estadoActual == ESTADO_MENU)
            menuDibujar(&menuPrincipal, renderer);
        else if(estadoActual == ESTADO_CONFIGURACION)
            menuConfiguracionDibujar(&menuPrincipal, renderer, &config);
        else if(estadoActual == ESTADO_NOMBRE)
            ingresoNombreDibujar(&menuPrincipal.nombre, renderer, menuPrincipal.fondoNombres);
        else if(estadoActual == ESTADO_RANKING)
            rankingDibujar(renderer, fuenteMedia, fuenteChica, menuPrincipal.fondoRanking);
        else if(estadoActual == ESTADO_JUGANDO)
        {
            int mX, mY;
            SDL_GetMouseState(&mX,&mY);
            tableroDibujar(&miTablero, renderer, mX, mY);
            dibujarEstadisticas(renderer, fuenteChica, &juego);
            if(juego.confirmandoSalida)
            {
                dibujarPopupSalida(renderer, menuPrincipal.fuenteOpciones,
                                   "Abandonar partida?", juego.opcionSalidaPopup);

            }

        }
        else if(estadoActual == ESTADO_GANO)
        {
            if(tableroCargado)
                tableroDibujar(&miTablero, renderer, 0, 0);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_Rect rect = {0, 0, ANCHOVENTANA, ALTOVENTANA};
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            dibujarTextoCentrados(renderer, fuenteGrande, "PARTIDA COMPLETADA", 200, (SDL_Color){50, 255, 50});

            if(juego.cantJugadores == 1)
            {
                char puntajeBuffer[64];
                sprintf(puntajeBuffer, "Jugador: %s - Puntos Totales: %d", juego.nombreJugador[0], juego.puntos[0]);
                dibujarTextoCentrados(renderer,fuenteMedia,puntajeBuffer,350, (SDL_Color){255, 255, 0});
            }
            else
            {
                char buffer1[64], buffer2[64], ganador[64];
                sprintf(buffer1, "%s: %d", juego.nombreJugador[0], juego.puntos[0]);
                sprintf(buffer2, "%s: %d", juego.nombreJugador[1], juego.puntos[1]);
                dibujarTextoCentrados(renderer,fuenteMedia,buffer1,300, (SDL_Color){255, 255, 0});
                dibujarTextoCentrados(renderer,fuenteMedia,buffer2,360, (SDL_Color){255, 255, 0});

                if(juego.puntos[0] > juego.puntos[1])
                    sprintf(ganador, "GANADOR: %s", juego.nombreJugador[0]);
                else if(juego.puntos[1] > juego.puntos[0])
                    sprintf(ganador, "GANADOR: %s", juego.nombreJugador[1]);
                else
                    sprintf(ganador, "EMPATE!");
                dibujarTextoCentrados(renderer,fuenteMedia,ganador,450, (SDL_Color){0, 255, 255});
            }

            dibujarTextoCentrados(renderer,fuenteMedia,"Presiona SPACE para volver",600, (SDL_Color){255, 255, 255});
        }
        SDL_RenderPresent(renderer);
    }

    menuDestruir(&menuPrincipal);

    sonidos_destruir(juego.sndSeleccion);
    sonidos_destruir(juego.sndAcierto);
    sonidos_destruir(juego.sndFallo);

    sonidos_destruir_musica(juego.musMenu);
    sonidos_destruir_musica(juego.musJuego);

    sonidos_finalizar();

    TTF_CloseFont(fuenteGrande);
    TTF_CloseFont(fuenteMedia);
    TTF_CloseFont(fuenteChica);

    if(tableroCargado)
        tableroDestruir(&miTablero);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); IMG_Quit(); SDL_Quit();

    return 0;
}
