#include "Tablero.h"
#include "Comun.h"

//esta estructura enum va a asignar valores a los distintos estados donde
//se encuentre el menu. por defecto ESTADO_MENU=0, ESTADO_JUGANDO=1 y asi
typedef enum
{
    ESTADO_MENU,
    ESTADO_INGRESO_NOMBRE,   // <-- NUEVO ESTADO
    ESTADO_JUGANDO,
    ESTADO_GANO
} EstadoJuego;

int main(int argc, char *argv[])
{
    // 1. INICIALIZACION
//    if (SDL_Init(SDL_INIT_VIDEO) < 0)
//    {
//        printf("Error en SDL: %s\n", SDL_GetError());
//        return 1;
//    }

    srand(time(NULL)); //para generar la aleatoriedad en el programa

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); //inicio las fuentes del juego para las letras

    // Usamos tus constantes de Comun.h
    SDL_Window *window = SDL_CreateWindow("TP Memotest",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          ANCHOVENTANA, ALTOVENTANA,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //inicio las fuentes para que se vean en la pantalla
    TTF_Font* fuenteGrande = TTF_OpenFont(FUENTE, 50);
    TTF_Font* fuenteChica = TTF_OpenFont(FUENTE, 16);

    if(!fuenteGrande || !fuenteChica)
    {
        printf("Error en la fuente ingresada: %s\n", TTF_GetError());
    }

    // --- VARIABLES PARA NOMBRE DEL JUGADOR (NUEVO) ---
    #define MAX_NOMBRE 24
    char nombreJugador[MAX_NOMBRE + 1];
    int nombreLen = 0;
    int nombreError = 0;
    nombreJugador[0] = '\0';

    // 2. LOGICA (El TDA Tablero se encarga de todo)

    EstadoJuego estadoActual = ESTADO_MENU; //el juego arranca en el menu

    Tablero miTablero;
    tableroIniciar(&miTablero, 16);    // Pedir memoria (malloc)
    tableroCargarImagenes(&miTablero, renderer); //cargo las imagenes

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

    // 3. GAME LOOP
    int corriendo = 1;
    SDL_Event e;

    while (corriendo)
    {
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

            //ingresos en el menu
            if(estadoActual == ESTADO_MENU)
            {
                if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
                {
                    //si en el menu se apreta ENTER, ahora pasa a pedir nombre (NO consola)
                    nombreLen = 0;
                    nombreJugador[0] = '\0';
                    nombreError = 0;

                    SDL_StartTextInput();
                    estadoActual = ESTADO_INGRESO_NOMBRE;
                }
            }

            //ingresos en la pantalla de nombre (NUEVO)
            else if(estadoActual == ESTADO_INGRESO_NOMBRE)
            {
                if(e.type == SDL_TEXTINPUT)
                {
                    // tomo solo el primer caracter (ASCII simple)
                    char c = e.text.text[0];

                    if(nombreLen < MAX_NOMBRE)
                    {
                        // filtro minimo
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

                            // ahora si empieza el juego
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
                        estadoActual = ESTADO_MENU;
                    }
                }
            }

            //ingresos en el juego
            else if(estadoActual == ESTADO_JUGANDO)
            {
                if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT)
                {
                    tableroClic(&miTablero,e.button.x,e.button.y, renderer);
                    //verifico si hubo una victoria luego del clic
                    if(tableroCompleto(&miTablero))
                    {
                        printf("Juego Terminado\n");
                        estadoActual=ESTADO_GANO;
                    }
                }
            }

            //ingresos de cuando se gano el juego
            else if(estadoActual == ESTADO_GANO)
            {
                if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
                {
                    //si se pulsa el espacio, se vuelve al menu principal
                    estadoActual = ESTADO_MENU;
                }
            }
        }

        //Renderizado segun el estado en el que este el juego(Dibujar)
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Fondo Gris
        SDL_RenderClear(renderer);

        if(estadoActual == ESTADO_MENU)
        {
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

            // --- SUBTITULO ---
            SDL_Surface* surf2 = TTF_RenderText_Solid(fuenteChica, "Presiona ENTER para Jugar", colorTexto);
            SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);

            SDL_Rect rect2;
            rect2.w = surf2->w;
            rect2.h = surf2->h;
            rect2.x = (ANCHOVENTANA - rect2.w) / 2;
            rect2.y = rect.y + rect.h + 20;

            SDL_RenderCopy(renderer, tex2, NULL, &rect2);

            //libero la memoria temporal de superficies y texturas de texto
            SDL_FreeSurface(surf);
            SDL_FreeSurface(surf2);
            SDL_DestroyTexture(tex);
            SDL_DestroyTexture(tex2);
        }
        else if(estadoActual == ESTADO_INGRESO_NOMBRE)
        {
            SDL_Color blanco = {255,255,255};

            SDL_Surface* s1 = TTF_RenderText_Solid(fuenteChica, "Ingrese su nombre y presione ENTER", blanco);
            SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_Rect r1 = {(ANCHOVENTANA - s1->w)/2, 160, s1->w, s1->h};
            SDL_RenderCopy(renderer, t1, NULL, &r1);

            // caja input
            SDL_Rect caja = {220, 220, 560, 70};
            SDL_SetRenderDrawColor(renderer, 80,80,80,255);
            SDL_RenderFillRect(renderer, &caja);
            SDL_SetRenderDrawColor(renderer, 200,200,200,255);
            SDL_RenderDrawRect(renderer, &caja);

            // texto escrito
            if(nombreLen > 0)
            {
                SDL_Surface* s2 = TTF_RenderText_Solid(fuenteGrande, nombreJugador, blanco);
                SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);
                SDL_Rect r2 = {caja.x + 10, caja.y + 10, s2->w, s2->h};
                SDL_RenderCopy(renderer, t2, NULL, &r2);
                SDL_FreeSurface(s2);
                SDL_DestroyTexture(t2);
            }
            else
            {
                SDL_Surface* s2 = TTF_RenderText_Solid(fuenteChica, "(escriba aqui)", blanco);
                SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);
                SDL_Rect r2 = {caja.x + 10, caja.y + 22, s2->w, s2->h};
                SDL_RenderCopy(renderer, t2, NULL, &r2);
                SDL_FreeSurface(s2);
                SDL_DestroyTexture(t2);
            }

            if(nombreError)
            {
                SDL_Color rojo = {255,80,80};
                SDL_Surface* s3 = TTF_RenderText_Solid(fuenteChica, "Nombre invalido (no puede estar vacio)", rojo);
                SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer, s3);
                SDL_Rect r3 = {(ANCHOVENTANA - s3->w)/2, 310, s3->w, s3->h};
                SDL_RenderCopy(renderer, t3, NULL, &r3);
                SDL_FreeSurface(s3);
                SDL_DestroyTexture(t3);
            }

            SDL_Surface* s4 = TTF_RenderText_Solid(fuenteChica, "ESC: volver", blanco);
            SDL_Texture* t4 = SDL_CreateTextureFromSurface(renderer, s4);
            SDL_Rect r4 = {20, ALTOVENTANA - 40, s4->w, s4->h};
            SDL_RenderCopy(renderer, t4, NULL, &r4);

            SDL_FreeSurface(s1);
            SDL_FreeSurface(s4);
            SDL_DestroyTexture(t1);
            SDL_DestroyTexture(t4);
        }
        else if(estadoActual == ESTADO_JUGANDO)
        {
            //dibujo el juego en si mismo
            tableroDibujar(&miTablero, renderer);

            // -----------------------------
            // HUD: PUNTAJE Y RACHA
            // -----------------------------
            SDL_Color blanco = {255,255,255};

            char hud1[80];
            sprintf(hud1, "Puntaje: %d", tableroGetPuntaje(&miTablero));
            SDL_Surface* s1 = TTF_RenderText_Solid(fuenteChica, hud1, blanco);
            SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_Rect r1 = {20, 20, s1->w, s1->h};
            SDL_RenderCopy(renderer, t1, NULL, &r1);
            SDL_FreeSurface(s1);
            SDL_DestroyTexture(t1);

            char hud2[80];
            sprintf(hud2, "Racha: %d  (Bonus: %.1f)",
                    tableroGetRacha(&miTablero),
                    tableroGetBonusRacha(&miTablero));

            SDL_Surface* s2 = TTF_RenderText_Solid(fuenteChica, hud2, blanco);
            SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);
            SDL_Rect r2 = {20, 50, s2->w, s2->h};
            SDL_RenderCopy(renderer, t2, NULL, &r2);
            SDL_FreeSurface(s2);
            SDL_DestroyTexture(t2);

            // (opcional) mostrar nombre del jugador
            char hud3[80];
            sprintf(hud3, "Jugador: %s", nombreJugador);
            SDL_Surface* s3 = TTF_RenderText_Solid(fuenteChica, hud3, blanco);
            SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer, s3);
            SDL_Rect r3 = {20, 80, s3->w, s3->h};
            SDL_RenderCopy(renderer, t3, NULL, &r3);
            SDL_FreeSurface(s3);
            SDL_DestroyTexture(t3);
        }
        else if(estadoActual == ESTADO_GANO)
        {
            //dibujo la pantalla de victoria, con el tablero de fondo
            //tableroDibujar(&miTablero, renderer);

            //dibujo el texto encima del tablero
            SDL_Color colorVictoria = {255,215,0}; //color dorado
            SDL_Surface* surf = TTF_RenderText_Solid(fuenteGrande,"GANASTE",colorVictoria);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer,surf);

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

            // (opcional) mostrar puntaje final
            char finalTxt[80];
            sprintf(finalTxt, "Puntaje final: %d", tableroGetPuntaje(&miTablero));
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
        }

        SDL_RenderPresent(renderer);
    }

    //limpio todas las variables
    TTF_CloseFont(fuenteGrande);
    TTF_CloseFont(fuenteChica);
    tableroDestruir(&miTablero); //libero el vector dinamico
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); // cierro el TTF
    IMG_Quit();
    SDL_Quit();

    return 0;
}
