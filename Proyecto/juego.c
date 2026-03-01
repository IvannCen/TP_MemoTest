#include "tablero.h"
#include "menu.h"
#include "ranking.h"
#include <time.h>
#include <stdio.h>

int Juego_Ejecutar(SDL_Window* window, SDL_Renderer* renderer)
{
    TTF_Font* fGrande = TTF_OpenFont(RUTA_FUENTE, TAM_FUENTE_GRANDE);
    TTF_Font* fMedia = TTF_OpenFont(RUTA_FUENTE, TAM_FUENTE_MEDIA);
    TTF_Font* fChica = TTF_OpenFont(RUTA_FUENTE, TAM_FUENTE_CHICA);

    Menu menu;
    menu_Iniciar(&menu, renderer);

    Tablero tablero;
    ConfiguracionJuego config;
    ContextoJuego juego;
    juego.confirmandoSalida = 0;
    juego.opcionSalidaPopup = 1;

    config_Cargar(&config);
    EstadoJuego estado = ESTADO_MENU;

    int tableroCargado = 0;
    int corriendo = 1;
    int jugadorIdx = 0;
    SDL_Event e;

    juego.sndSeleccion = sonidos_Cargar(RUTA_SND_SELECCION);
    juego.sndAcierto = sonidos_Cargar(RUTA_SND_ACIERTO);
    juego.sndFallo = sonidos_Cargar(RUTA_SND_FALLO);
    juego.musMenu = sonidos_CargarMusica(RUTA_MUS_MENU);
    juego.musJuego = sonidos_CargarMusica(RUTA_MUS_JUEGO);

    Mix_Volume(-1, VOL);
    Mix_VolumeMusic(VOL);
    sonidos_ReproducirMusica(juego.musMenu, -1);

    while(corriendo)
    {
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) corriendo = 0;

            switch(estado)
            {
                case ESTADO_MENU:
                {
                    int sel = menu_ManejarOpciones(&menu, &e);
                    if(sel != -1)
                    {
                        if(sel == OPCION_JUGAR)
                        {
                            jugadorIdx = 0;
                            if(config.cantJugadores == 1)
                                ingreso_NombreIniciar(&menu.nombre, "NOMBRE JUGADOR");
                            else
                                ingreso_NombreIniciar(&menu.nombre, "NOMBRE JUGADOR 1");
                            estado = ESTADO_NOMBRE;
                        }
                        else if(sel == OPCION_CONFIGURACION)
                        {
                            menu.opcionSeleccionada=0;
                            estado=ESTADO_CONFIGURACION;
                        }
                        else if(sel == OPCION_ESTADISTICAS)
                            estado = ESTADO_RANKING;
                        else if(sel == OPCION_SALIR)
                            corriendo = 0;
                    }
                    break;
                }
                case ESTADO_CONFIGURACION:
                {
                    int sal = menu_ConfiguracionOpciones(&menu, &e, &config);
                    if(sal)
                    {
                        config_Guardar(&config);
                        menu.opcionSeleccionada=0;
                        estado=ESTADO_MENU;
                    }
                    break;
                }
                case ESTADO_RANKING:
                {
                    if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_ESCAPE)
                        estado=ESTADO_MENU;
                    break;
                }
                case ESTADO_NOMBRE:
                {
                    int res = ingreso_NombreOpciones(&menu.nombre, &e, &juego, jugadorIdx);
                    if(res==1)
                    {
                        if(config.cantJugadores==2 && jugadorIdx==0)
                        {
                            jugadorIdx=1;
                            ingreso_NombreIniciar(&menu.nombre, "NOMBRE JUGADOR 2");
                        }
                        else
                        {
                            juego.turnoJugador=0;
                            juego.puntos[0]=0;
                            juego.puntos[1]=0;
                            juego.cantJugadores=config.cantJugadores;
                            juego.tiempoInicio=SDL_GetTicks();
                            juego.confirmandoSalida=0;
                            juego.opcionSalidaPopup=1;
                            tablero_Iniciar(&tablero, &config);
                            tablero_CargarImagenes(&tablero, renderer, config.idSetImagenes);
                            tablero_Rellenar(&tablero, config.filas, config.columnas);
                            tablero_Mezclar(&tablero);
                            tableroCargado=1;
                            sonidos_ReproducirMusica(juego.musJuego, -1);
                            estado=ESTADO_JUGANDO;
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        }
                    }
                    else if(res==2)
                    {
                        menu.opcionSeleccionada=0;
                        estado=ESTADO_MENU;
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
                            else if(e.key.keysym.sym == SDLK_RETURN)
                            {
                                if(juego.opcionSalidaPopup == 0)
                                {
                                    tablero_Destruir(&tablero);
                                    tableroCargado = 0;
                                    juego.confirmandoSalida = 0;
                                    sonidos_ReproducirMusica(juego.musMenu, -1);
                                    estado = ESTADO_MENU;
                                }
                                else
                                    juego.confirmandoSalida = 0;
                            }
                            else if(e.key.keysym.sym == SDLK_ESCAPE)
                                juego.confirmandoSalida = 0;
                        }
                        else if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN)
                        {
                            int mx = (e.type == SDL_MOUSEMOTION) ? e.motion.x : e.button.x;
                            int my = (e.type == SDL_MOUSEMOTION) ? e.motion.y : e.button.y;
                            SDL_Rect rSi = { (ANCHO_VENTANA/2)-80, (ALTO_VENTANA/2)+20, 50, 40 };
                            SDL_Rect rNo = { (ANCHO_VENTANA/2)+40, (ALTO_VENTANA/2)+20, 50, 40 };

                            if(SDL_PointInRect(&(SDL_Point){mx,my}, &rSi))
                            {
                                juego.opcionSalidaPopup = 0;
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                                {
                                    tablero_Destruir(&tablero);
                                    tableroCargado = 0;
                                    juego.confirmandoSalida = 0;
                                    sonidos_ReproducirMusica(juego.musMenu, -1);
                                    estado = ESTADO_MENU;
                                }
                            }
                            else if(SDL_PointInRect(&(SDL_Point){mx,my}, &rNo))
                            {
                                juego.opcionSalidaPopup = 1;
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                                    juego.confirmandoSalida = 0;
                            }
                        }
                    }
                    else
                    {
                        if(e.type == SDL_KEYDOWN)
                        {
                            if(e.key.keysym.sym == SDLK_ESCAPE)
                            {
                                juego.confirmandoSalida = 1;
                                juego.opcionSalidaPopup = 1;
                            }
                            else
                            {
                                tablero_ManejarTeclado(&tablero, &e, renderer, &juego, fChica);
                                if(tablero_Completo(&tablero))
                                {
                                    SDL_Delay(TIEMPO_DELAY_CORTO);
                                    estado = ESTADO_GANO;
                                }
                            }
                        }
                        else if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                        {
                            tablero_Clic(&tablero, e.button.x, e.button.y, renderer, &juego, fChica);
                            if(tablero_Completo(&tablero))
                            {
                                SDL_Delay(TIEMPO_DELAY_CORTO);
                                estado = ESTADO_GANO;
                            }
                        }
                        else if(e.type == SDL_MOUSEMOTION)
                        {
                            tablero_ManejarHover(&tablero, e.motion.x, e.motion.y);
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
                            ranking_Guardar(juego.nombreJugador[0], juego.puntos[0], tiempo);
                        }
                        if(tableroCargado)
                        {
                            tablero_Destruir(&tablero);
                            tableroCargado = 0;
                        }
                        sonidos_ReproducirMusica(juego.musMenu, -1);
                        estado = ESTADO_MENU;
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        if(estado == ESTADO_MENU)
            menu_Dibujar(&menu, renderer);
        else if(estado == ESTADO_CONFIGURACION)
            menu_ConfiguracionDibujar(&menu, renderer, &config);
        else if(estado == ESTADO_NOMBRE)
            ingreso_NombreDibujar(&menu.nombre, renderer, menu.fondoNombres);
        else if(estado == ESTADO_RANKING)
            ranking_Dibujar(renderer, fMedia, fChica, menu.fondoRanking);
        else if(estado == ESTADO_JUGANDO)
        {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            tablero_Dibujar(&tablero, renderer, mx, my);
            dibujarEstadisticas(renderer, fChica, &juego);
            if(juego.confirmandoSalida)
                dibujarPopupSalida(renderer, menu.fuenteOpciones, "Desea salir?", juego.opcionSalidaPopup);
        }
        else if(estado == ESTADO_GANO)
        {
            if(tableroCargado)
                tablero_Dibujar(&tablero, renderer, 0, 0);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer,0,0,0,200);
            SDL_Rect r={0,0,ANCHO_VENTANA,ALTO_VENTANA};
            SDL_RenderFillRect(renderer,&r);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            dibujarTextoCentrados(renderer, fGrande, "PARTIDA COMPLETADA", 200, COLOR_TITULO);
            if(juego.cantJugadores==1)
            {
                char b[64];
                sprintf(b,"Jugador: %s - Puntos: %d", juego.nombreJugador[0], juego.puntos[0]);
                dibujarTextoCentrados(renderer,fMedia,b,350,COLOR_BLANCO);
            }
            else
            {
                char b[64], g[64];
                sprintf(b,"%s: %d", juego.nombreJugador[0], juego.puntos[0]);
                dibujarTextoCentrados(renderer,fMedia,b,300,COLOR_BLANCO);
                sprintf(b,"%s: %d", juego.nombreJugador[1], juego.puntos[1]);
                dibujarTextoCentrados(renderer,fMedia,b,360,COLOR_BLANCO);
                if(juego.puntos[0]>juego.puntos[1])
                    sprintf(g,"GANADOR: %s", juego.nombreJugador[0]);
                else if(juego.puntos[1]>juego.puntos[0])
                    sprintf(g,"GANADOR: %s", juego.nombreJugador[1]);
                else
                    sprintf(g,"EMPATE!");
                dibujarTextoCentrados(renderer,fMedia,g,450,COLOR_VERDE);
            }
            dibujarTextoCentrados(renderer, fMedia, "Presiona SPACE para volver", 600, COLOR_SELECCION);
        }

        SDL_RenderPresent(renderer);
    }

    menu_Destruir(&menu);
    sonidos_Destruir(juego.sndSeleccion);
    sonidos_Destruir(juego.sndAcierto);
    sonidos_Destruir(juego.sndFallo);
    sonidos_DestruirMusica(juego.musMenu);
    sonidos_DestruirMusica(juego.musJuego);
    TTF_CloseFont(fGrande);
    TTF_CloseFont(fMedia);
    TTF_CloseFont(fChica);
    if(tableroCargado)
        tablero_Destruir(&tablero);
    return 0;
}
