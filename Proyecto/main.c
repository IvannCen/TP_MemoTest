#include "Tablero.h"
#include "Comun.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//todo esto que esta comentado hay que revisarlo porque la estructura de los estados esta ya en el Juego.h por lo cual esta de mas ponerlo
//y las funciones estaticas deberian estar en una header aparte si se quiere ya que a los profes no les gusta las funciones de ese estilo
//ademas habiamos quedado que el tema del nombre se iba a poner al momento de elegir el modo de juego, no al iniciar porque sino no se puede crear rankings por jugador

/*
#define CONFIG_PATH "config.bin"

#define MAX_NOMBRE 24

//esta estructura enum va a asignar valores a los distintos estados donde
//se encuentre el menu. por defecto ESTADO_MENU=0, ESTADO_JUGANDO=1 y asi
typedef enum
{
    ESTADO_MENU,
    ESTADO_MENU_CONFIG,
    ESTADO_INGRESO_NOMBRE,
    ESTADO_JUGANDO,
    ESTADO_GANO
} EstadoJuego;

static int punto_en_rect(int x, int y, SDL_Rect r)
{
    return (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h);
}

static void render_texto(SDL_Renderer* renderer, TTF_Font* fuente, const char* txt,
                         SDL_Color color, int x, int y)
{
    SDL_Surface* s = TTF_RenderText_Solid(fuente, txt, color);
    if(!s) return;
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
    if(!t)
    {
        SDL_FreeSurface(s);
        return;
    }
    SDL_Rect r = { x, y, s->w, s->h };
    SDL_RenderCopy(renderer, t, NULL, &r);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

static void recrear_tablero(Tablero* t, SDL_Renderer* renderer, const Config* cfg)
{
    // destruyo y creo de nuevo segun dimensiones + set
    tableroDestruir(t);

    int cantCartas = cfg->filas * cfg->cols;
    tableroIniciar(t, cantCartas);

    tableroSetDimensiones(t, cfg->filas, cfg->cols);
    tableroSetSetFiguras(t, cfg->setFiguras);

    tableroCargarImagenes(t, renderer);

    // cuando arranca partida real, esto se hace luego del nombre:
    // tableroRellenar(t);
    // tableroMezclar(t);
}

*/
#include "Juego.h"


int main(int argc, char *argv[])
{
    // 1. INICIALIZACION
//    if (SDL_Init(SDL_INIT_VIDEO) < 0)
//    {
//        printf("Error en SDL: %s\n", SDL_GetError());
//        return 1;
//    }

    srand(time(NULL)); //para generar la aleatoriedad en el programa

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return 1;
    }
    if(IMG_Init(IMG_INIT_PNG)==0)
    {
        return 1;
    }
    if(TTF_Init()==-1) //inicio las fuentes del juego para las letras
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
    TTF_Font* fuenteGrande = TTF_OpenFont(FUENTE, 50);
    TTF_Font* fuenteChica = TTF_OpenFont(FUENTE, 16);

    if(!fuenteGrande || !fuenteChica)
    {
        printf("Error en la fuente ingresada: %s\n", TTF_GetError());
    }

    /*
    // --- CONFIG persistente ---
    Config cfg;
    config_cargar(&cfg, CONFIG_PATH);

    // --- VARIABLES PARA NOMBRE DEL JUGADOR ---
    char nombreJugador[MAX_NOMBRE + 1];
    int nombreLen = 0;
    int nombreError = 0;
    nombreJugador[0] = '\0';

    */

    //logica inicial para arrancar en el menu
    EstadoJuego estadoActual = ESTADO_MENU;
    Tablero miTablero;

    /*
    // Creo tablero con la config cargada
    tableroIniciar(&miTablero, cfg.filas * cfg.cols);    // Pedir memoria (malloc)
    tableroSetDimensiones(&miTablero, cfg.filas, cfg.cols);
    tableroSetSetFiguras(&miTablero, cfg.setFiguras);
    tableroCargarImagenes(&miTablero, renderer); //cargo las imagenes
    ContextoJuego juego;
    */
    
    //variables del juego
    juego.nivelActual = 1;
    juego.puntos = 0;

    //variable para ver si hay un tablero en memoria
    int tableroCargado = 0;

//     2. LOGICA (El TDA Tablero se encarga de todo)
//
//    EstadoJuego estadoActual = ESTADO_MENU; //el juego arranca en el menu
//
//    Tablero miTablero;
//    tableroIniciar(&miTablero, 16);    // Pedir memoria (malloc)
//    tableroCargarImagenes(&miTablero, renderer); //cargo las imagenes

//    tableroCargarImagenes(&miTablero, renderer); //cargo las imagenes
//
//    tableroRellenar(&miTablero);  // Calcular posiciones
//
//    //despues de poner en orden las cartas en el tablero, llamo a la funcion
//    //que las va a mezclar, cambiando sus imagenes de lugar. Si o si tengo
//    //primero que rellenar el tablero antes de mezclar, ya que en la funcion
//    //de rellenar estoy asignando los id a las imagenes y eso es lo que uso
//    //en mezclar. si lo hiciera al reves estaria mezclando basura, y al llamar
//    //a rellenar despues solo lo ordenaria al original y no habria mezcla
//
//    tableroMezclar(&miTablero);

    /*
    // Botones menu config
    SDL_Rect btnDim, btnSet, btnJug, btnSave, btnPlay;
    */
    
    // loop del juego
    int corriendo = 1;
    SDL_Event e;
    int w = ANCHOVENTANA;
    int h = ALTOVENTANA;

    while (corriendo)
    {
        //aca aplico la logica para gestionar el tiempo
        Uint32 tiempoActual = 0;
        if(estadoActual==ESTADO_JUGANDO)
        {
            //calculo el tiempo dividiendo por mil para que este en segundos
            tiempoActual = (SDL_GetTicks() - juego.tiempoInicio) / 1000;
        }

        // proceso los ingresos segun el estado en el que se encuentre el juego
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                corriendo = 0;
            }

//            if(e.type == SDL_MOUSEBUTTONDOWN)
//            {
//                if(e.button.button == SDL_BUTTON_LEFT)
//                {
//                    int x = e.button.x;
//                    int y = e.button.y;
//
//                    tableroClic(&miTablero, x, y, renderer);
//                }
//            }
            

//hay que revisar porque el ingreso del nombre no debe ser al inicio del menu, sino al momento de entrar en los modos de juego
//aparte se cambia por completo el menu de esta manera, y ademas toda la logica de ingreso y variables para el mismo deben estar en una funcion
//sino se carga el main de miles de cosas y no es la idea (por favor revisar)
            /*
            //ingresos en el menu
            if(estadoActual == ESTADO_MENU)
            {
                if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
                {
                    // ENTER: paso a menu de configuracion
                    estadoActual = ESTADO_MENU_CONFIG;
                }
            }

            // Menu grafico de configuracion
            else if(estadoActual == ESTADO_MENU_CONFIG)
            {

                // Si la ventana es resizable, esto es mejor:
                SDL_GetWindowSize(window, &w, &h);

                int bw = 360;
                int bh = 60;
                int gap = 22;

                int totalH = bh*5 + gap*4;
                int startX = (w - bw) / 2;
                int startY = (h - totalH) / 2 + 40; // +40 para dejar lugar al titulo

                btnDim  = (SDL_Rect)
                {
                    startX, startY + (bh+gap)*0, bw, bh
                };
                btnSet  = (SDL_Rect)
                {
                    startX, startY + (bh+gap)*1, bw, bh
                };
                btnJug  = (SDL_Rect)
                {
                    startX, startY + (bh+gap)*2, bw, bh
                };
                btnSave = (SDL_Rect)
                {
                    startX, startY + (bh+gap)*3, bw, bh
                };
                btnPlay = (SDL_Rect)
                {
                    startX, startY + (bh+gap)*4, bw, bh
                };
                if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                {
                    estadoActual = ESTADO_MENU;
                }

                if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                {
                    int x = e.button.x;
                    int y = e.button.y;

                    if(punto_en_rect(x,y,btnDim))
                    {
                        // ciclo: 3x4 -> 4x4 -> 4x5 -> 3x4
                        if(cfg.filas == 3 && cfg.cols == 4)
                        {
                            cfg.filas = 4;
                            cfg.cols = 4;
                        }
                        else if(cfg.filas == 4 && cfg.cols == 4)
                        {
                            cfg.filas = 4;
                            cfg.cols = 5;
                        }
                        else
                        {
                            cfg.filas = 3;
                            cfg.cols = 4;
                        }
                    }
                    else if(punto_en_rect(x,y,btnSet))
                    {
                        cfg.setFiguras = 1 - cfg.setFiguras;
                    }
                    else if(punto_en_rect(x,y,btnJug))
                    {
                        cfg.jugadores = (cfg.jugadores == 1) ? 2 : 1;
                    }
                    else if(punto_en_rect(x,y,btnSave))
                    {
                        config_guardar(&cfg, CONFIG_PATH);
                        // refresco tablero con config guardada (solo imagenes/dimensiones)
                        recrear_tablero(&miTablero, renderer, &cfg);
                    }
                    else if(punto_en_rect(x,y,btnPlay))
                    {
                        // aplico config al tablero antes de jugar
                        recrear_tablero(&miTablero, renderer, &cfg);

                        // paso a pedir nombre por SDL (NO consola)
                        nombreLen = 0;
                        nombreJugador[0] = '\0';
                        nombreError = 0;
                        SDL_StartTextInput();
                        estadoActual = ESTADO_INGRESO_NOMBRE;
                    }
                }
            }

            //ingresos en la pantalla de nombre
            else if(estadoActual == ESTADO_INGRESO_NOMBRE)
            {
                if(e.type == SDL_TEXTINPUT)
                {
                    char c = e.text.text[0];

                    if(nombreLen < MAX_NOMBRE)
                    {
                        if((c >= 'a' && c <= 'z') ||
                                (c >= 'A' && c <= 'Z') ||
                                (c >= '0' && c <= '9') ||
                                c == ' ' || c == '_' || c == '-')
                        {
                            nombreJugador[nombreLen++] = c;
                            nombreJugador[nombreLen] = '\0';
                            nombreError = 0;
                        }
                    }
                }

                if(e.type == SDL_KEYDOWN)
                {
                    if(e.key.keysym.sym == SDLK_BACKSPACE && nombreLen > 0)
                    {
                        nombreLen--;
                        nombreJugador[nombreLen] = '\0';
                    }
                    else if(e.key.keysym.sym == SDLK_RETURN)
                    {
                        if(nombreLen > 0)
                        {
                            SDL_StopTextInput();

                            // ahora si enpieza el juego
                            tableroRellenar(&miTablero); //reinicio las posiciones de las cartas
                            tableroMezclar(&miTablero); //mezclo las cartas en el tablero
                            estadoActual = ESTADO_JUGANDO;
                        }
                        else
                        {
                            nombreError = 1;
                        }
                    }
                    else if(e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        SDL_StopTextInput();
                        estadoActual = ESTADO_MENU_CONFIG;
                    }
                    //aca arranca una nueva partida
                    juego.nivelActual = 1;
                    juego.puntos = 0;
                    juego.tiempoInicio = SDL_GetTicks();//esta funcion resetea el reloj

                    //inicio el nivel 1
                    int cartas = obtenerCartasPorNivel(juego.nivelActual);

                    //si en el menu se apreta ENTER, empieza el juego
                    tableroIniciar(&miTablero, cartas);
                    tableroCargarImagenes(&miTablero,renderer);
                    tableroRellenar(&miTablero); //reinicio las posiciones de las cartas
                    tableroMezclar(&miTablero); //mezclo las cartas en el tablero
                    tableroCargado = 1;
                    estadoActual = ESTADO_JUGANDO;
                }
            }
            */

            if(estadoActual == ESTADO_MENU)
            {
                if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
                {
                    //aca arranca una nueva partida
                    juego.nivelActual = 1;
                    juego.puntos = 0;
                    juego.tiempoInicio = SDL_GetTicks();//esta funcion resetea el reloj

                    //inicio el nivel 1
                    int cartas = obtenerCartasPorNivel(juego.nivelActual);

                    //si en el menu se apreta ENTER, empieza el juego
                    tableroIniciar(&miTablero, cartas);
                    tableroCargarImagenes(&miTablero,renderer);
                    tableroRellenar(&miTablero); //reinicio las posiciones de las cartas
                    tableroMezclar(&miTablero); //mezclo las cartas en el tablero
                    tableroCargado = 1;
                    estadoActual = ESTADO_JUGANDO;
                }
            }


            //ingresos en el juego
            else if(estadoActual == ESTADO_JUGANDO)
            {
                if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                {
                    //obtengo los puntos obtenidos de la funcion de clic
                    int puntos = tableroClic(&miTablero,e.button.x,e.button.y, renderer);

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
//                        printf("Nivel completado!\n");
//                        juego.puntos += 500; // Bonus por nivel
                        printf("Nivel %d Completado en %d segundos\n", juego.nivelActual, tiempoActual);
                        SDL_Delay(DELAYCHICO); //delay de medio segundo, como una pausa chica
                        estadoActual=ESTADO_GANO;
                    }
                }
            }

            //ingresos de cuando se gano el juego
            else if(estadoActual == ESTADO_GANO)
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

                    if(cartas > 0 && juego.nivelActual <= CANTIDADNIVELES)
                    {
                        //cargo el siguiente nivel
                        tableroIniciar(&miTablero, cartas);
                        tableroCargarImagenes(&miTablero,renderer);//reinicio las texturas
                        tableroRellenar(&miTablero);
                        tableroMezclar(&miTablero);
                        tableroCargado = 1;

                        //aca es una decision a tomar, ya que podriamos o dejar el tiempo que siga corriendo
                        //o podriamos reiniciarlo, eso depende de como queremos que sea el tiempo para las
                        //estadisticas de cada jugador
                        //juego.tiempoInicio = SDL_GetTicks();//Reinicia el reloj

                        estadoActual = ESTADO_JUGANDO;
                    }
                    else
                    {
                        //ya no hay mas niveles, se vuelve al menu de inicio
                        printf("Juego Completado\n");
                        estadoActual=ESTADO_MENU;
                    }
                }
            }
        }

        //Renderizado segun el estado en el que este el juego(Dibujar)
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Fondo Gris oscuro
        SDL_RenderClear(renderer);

        if(estadoActual == ESTADO_MENU)
        {
            //revisar esta parte porque el menu debe estar en otro header para no sobrecargar el main

            /*
//            //dibujo el menu
            SDL_Color colorTexto = {255,255,255};
            SDL_Surface* surf = TTF_RenderText_Solid(fuenteGrande, "MEMOTEST - TP", colorTexto);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            //SDL_Rect rect = {200,200,surf->w,surf->h};
//            SDL_RenderCopy(renderer,tex, NULL, &rect);
//
//            //instrucciones en pantalla
//            SDL_Surface* surf2 = TTF_RenderText_Solid(fuenteChica, "Presiona ENTER para Jugar", colorTexto);
//            SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
//            SDL_Rect rect2 = {300,300,surf2->w, surf2->h};
//            SDL_RenderCopy(renderer,tex2,NULL,&rect2);

            SDL_Rect rect;
            rect.w = surf->w;
            rect.h = surf->h;
            rect.x = (ANCHOVENTANA - rect.w) / 2;
            rect.y = (ALTOVENTANA - rect.h) / 2 - 50;

            SDL_RenderCopy(renderer, tex, NULL, &rect);

            SDL_Surface* surf2 = TTF_RenderText_Solid(fuenteChica, "ENTER: Configurar y jugar", colorTexto);
            SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);

            SDL_Rect rect2;
            rect2.w = surf2->w;
            rect2.h = surf2->h;
            rect2.x = (ANCHOVENTANA - rect2.w) / 2;
            rect2.y = rect.y + rect.h + 20;

            SDL_RenderCopy(renderer, tex2, NULL, &rect2);

            SDL_FreeSurface(surf);
            SDL_FreeSurface(surf2);
            SDL_DestroyTexture(tex);
            SDL_DestroyTexture(tex2);
            */

            //dibujo el menu
            SDL_Color colorTexto = {255,255,0};
            dibujarTextoCentrados(renderer,fuenteGrande,"MEMOTEST - TP",200, colorTexto);
            dibujarTextoCentrados(renderer,fuenteChica,"Presiona ENTER para comenzar",400,colorTexto);

        }
        //lo mismo, hay que cambiar la configuracion del menu para que quede todo en otro header y .c antes de sobrecargar el main
        /*
        else if(estadoActual == ESTADO_MENU_CONFIG)
        {
            SDL_Color blanco = {255,255,255};

            // titulo
            render_texto(renderer, fuenteGrande, "CONFIGURACION", blanco, (ANCHOVENTANA - 420)/2, 80);

            // botones
            SDL_SetRenderDrawColor(renderer, 80,80,80,255);
            SDL_RenderFillRect(renderer, &btnDim);
            SDL_RenderFillRect(renderer, &btnSet);
            SDL_RenderFillRect(renderer, &btnJug);
            SDL_RenderFillRect(renderer, &btnSave);
            SDL_RenderFillRect(renderer, &btnPlay);

            SDL_SetRenderDrawColor(renderer, 200,200,200,255);
            SDL_RenderDrawRect(renderer, &btnDim);
            SDL_RenderDrawRect(renderer, &btnSet);
            SDL_RenderDrawRect(renderer, &btnJug);
            SDL_RenderDrawRect(renderer, &btnSave);
            SDL_RenderDrawRect(renderer, &btnPlay);

            char tDim[64];
            sprintf(tDim, "Dimensiones: %dx%d", cfg.filas, cfg.cols);

            char tSet[64];
            sprintf(tSet, "Set de figuras: %s", (cfg.setFiguras==0) ? "A" : "B");

            char tJug[64];
            sprintf(tJug, "Jugadores: %d", cfg.jugadores);

            render_texto(renderer, fuenteChica, tDim, blanco, btnDim.x + 20, btnDim.y + 20);
            render_texto(renderer, fuenteChica, tSet, blanco, btnSet.x + 20, btnSet.y + 20);
            render_texto(renderer, fuenteChica, tJug, blanco, btnJug.x + 20, btnJug.y + 20);
            render_texto(renderer, fuenteChica, "Guardar configuracion", blanco, btnSave.x + 20, btnSave.y + 20);
            render_texto(renderer, fuenteChica, "Jugar", blanco, btnPlay.x + 20, btnPlay.y + 20);

            render_texto(renderer, fuenteChica, "ESC: volver", blanco, 20, ALTOVENTANA - 40);
        }
        else if(estadoActual == ESTADO_INGRESO_NOMBRE)
        {

            SDL_GetWindowSize(window, &w, &h);

            SDL_Color blanco = {255,255,255};

            // --- titulo centrado ---
            const char* titulo = "Ingrese su nombre y presione ENTER";
            SDL_Surface* s1 = TTF_RenderText_Solid(fuenteChica, titulo, blanco);
            SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);

            // caja input centrada
            int cajaW = 600;
            int cajaH = 70;
            SDL_Rect caja = { (w - cajaW)/2, (h - cajaH)/2, cajaW, cajaH };

            // titulo arriba de la caja
            SDL_Rect r1 = { (w - s1->w)/2, caja.y - 60, s1->w, s1->h };
            SDL_RenderCopy(renderer, t1, NULL, &r1);

            // --- caja input ---
            SDL_SetRenderDrawColor(renderer, 80,80,80,255);
            SDL_RenderFillRect(renderer, &caja);
            SDL_SetRenderDrawColor(renderer, 200,200,200,255);
            SDL_RenderDrawRect(renderer, &caja);

            // --- texto escrito (o placeholder) ---
            if(nombreLen > 0)
            {
                SDL_Surface* s2 = TTF_RenderText_Solid(fuenteGrande, nombreJugador, blanco);
                SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);

                SDL_Rect r2 = { caja.x + 10, caja.y + (caja.h - s2->h)/2, s2->w, s2->h };
                SDL_RenderCopy(renderer, t2, NULL, &r2);

                SDL_FreeSurface(s2);
                SDL_DestroyTexture(t2);
            }
            else
            {
                SDL_Surface* s2 = TTF_RenderText_Solid(fuenteChica, "(escriba aqui)", blanco);
                SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);

                SDL_Rect r2 = { caja.x + 10, caja.y + (caja.h - s2->h)/2, s2->w, s2->h };
                SDL_RenderCopy(renderer, t2, NULL, &r2);

                SDL_FreeSurface(s2);
                SDL_DestroyTexture(t2);
            }

            // --- error centrado debajo ---
            if(nombreError)
            {
                SDL_Color rojo = {255,80,80};
                SDL_Surface* s3 = TTF_RenderText_Solid(fuenteChica, "Nombre invalido (no puede estar vacio)", rojo);
                SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer, s3);

                SDL_Rect r3 = { (w - s3->w)/2, caja.y + caja.h + 20, s3->w, s3->h };
                SDL_RenderCopy(renderer, t3, NULL, &r3);

                SDL_FreeSurface(s3);
                SDL_DestroyTexture(t3);
            }

            // --- hint ESC abajo a la izquierda ---
            SDL_Surface* s4 = TTF_RenderText_Solid(fuenteChica, "ESC: volver", blanco);
            SDL_Texture* t4 = SDL_CreateTextureFromSurface(renderer, s4);
            SDL_Rect r4 = { 20, h - 40, s4->w, s4->h };
            SDL_RenderCopy(renderer, t4, NULL, &r4);

            SDL_FreeSurface(s1);
            SDL_FreeSurface(s4);
            SDL_DestroyTexture(t1);
            SDL_DestroyTexture(t4);
        }

        */
        else if(estadoActual == ESTADO_JUGANDO)
        {
            //dibujo el juego en si mismo
            tableroDibujar(&miTablero, renderer);
            
            //esta idea esta buena, habria que modificarla para que queden centrados los datos pero esta copado
            
            /*
            // -----------------------------
            // HUD: PUNTAJE Y RACHA
            // -----------------------------
            SDL_Color blanco = {255,255,255};

            char hud1[120];
            sprintf(hud1, "Puntaje: %d", tableroGetPuntaje(&miTablero));
            render_texto(renderer, fuenteChica, hud1, blanco, 20, 20);

            char hud2[120];
            sprintf(hud2, "Racha: %d  (Bonus: %.1f)",
                    tableroGetRacha(&miTablero),
                    tableroGetBonusRacha(&miTablero));
            render_texto(renderer, fuenteChica, hud2, blanco, 20, 45);

            // Estadisticas
            char hudS[200];
            sprintf(hudS, "Intentos: %d | Aciertos: %d | Fallos: %d | Tiempo: %ds",
                    tableroGetIntentos(&miTablero),
                    tableroGetAciertos(&miTablero),
                    tableroGetFallos(&miTablero),
                    tableroGetTiempoSegundos(&miTablero));
            render_texto(renderer, fuenteChica, hudS, blanco, 20, 70);

            // (opcional) mostrar nombre del jugador
            char hud3[120];
            sprintf(hud3, "Jugador: %s", nombreJugador);
            render_texto(renderer, fuenteChica, hud3, blanco, 20, 95);

            // (opcional) mostrar config actual
            char hud4[120];
            sprintf(hud4, "Tablero: %dx%d | Set: %s | Jugadores: %d",
                    cfg.filas, cfg.cols, (cfg.setFiguras==0)?"A":"B", cfg.jugadores);
            render_texto(renderer, fuenteChica, hud4, blanco, 20, 120);
            */

            //dibujo las estadisticas del nivel
            dibujarEstadisticas(renderer, fuenteChica, &juego);
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
            dibujarTextoCentrados(renderer, fuenteGrande,"NIVEL COMPLETADO",200,colorVerde);
            
            //hay que revisar porque son distintos niveles y deberia continuar, ademas es depende del modo de juego que se este jugando
            //ademas lo mismo, habria que hacer funciones aparte para que hagan esto sin rellenar el main tanto
            
            /*
            //centrado en la pantalla
//            SDL_Rect rect = {300,200,surf->w,surf->h};
//            SDL_RenderCopy(renderer,tex,NULL,&rect);
//
//            SDL_Surface* surf2 = TTF_RenderText_Solid(fuenteChica,"Presiona SPACE para volver al Menu", colorVictoria);
//            SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
//            SDL_Rect rect2 = {300,300,surf2->w,surf2->h};
//            SDL_RenderCopy(renderer,tex2,NULL,&rect2);

            SDL_Rect rect;
            rect.w = surf->w;
            rect.h = surf->h;
            rect.x = (ANCHOVENTANA - rect.w) / 2;
            rect.y = (ALTOVENTANA - rect.h) / 2 - 40;

            SDL_RenderCopy(renderer, tex, NULL, &rect);

            // --- TEXTO INFERIOR ---
            SDL_Surface* surf2 = TTF_RenderText_Solid(
                                     fuenteChica,
                                     "Presiona SPACE para volver al Menu",
                                     colorVictoria
                                 );

            SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);

            SDL_Rect rect2;
            rect2.w = surf2->w;
            rect2.h = surf2->h;
            rect2.x = (ANCHOVENTANA - rect2.w) / 2;
            rect2.y = rect.y + rect.h + 20;

            SDL_RenderCopy(renderer, tex2, NULL, &rect2);

            // puntaje final + stats
            char finalTxt[220];
            sprintf(finalTxt, "Puntaje: %d | Intentos: %d | Tiempo: %ds",
                    tableroGetPuntaje(&miTablero),
                    tableroGetIntentos(&miTablero),
                    tableroGetTiempoSegundos(&miTablero));

            SDL_Surface* surf3 = TTF_RenderText_Solid(fuenteChica, finalTxt, colorVictoria);
            SDL_Texture* tex3 = SDL_CreateTextureFromSurface(renderer, surf3);
            SDL_Rect rect3 = {(ANCHOVENTANA - surf3->w)/2, rect2.y + rect2.h + 20, surf3->w, surf3->h};
            SDL_RenderCopy(renderer, tex3, NULL, &rect3);

            SDL_FreeSurface(surf);
            SDL_FreeSurface(surf2);
            SDL_FreeSurface(surf3);
            SDL_DestroyTexture(tex);
            SDL_DestroyTexture(tex2);
            SDL_DestroyTexture(tex3);

            */

            char puntajeBuffer[50];
            sprintf(puntajeBuffer, "Puntos Totales: %d", juego.puntos);
            dibujarTextoCentrados(renderer, fuenteChica, puntajeBuffer, 350, colorAmarillo);
            dibujarTextoCentrados(renderer,fuenteChica,"Presiona SPACE para continuar",500,colorBlanco);
        }
        SDL_RenderPresent(renderer);
    }

    //limpio todas las variables
    TTF_CloseFont(fuenteGrande);
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
