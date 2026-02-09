#include "Tablero.h"
#include "Comun.h"

//esta estructura enum va a asignar valores a los distintos estados donde
//se encuentre el menu. por defecto ESTADO_MENU=0, ESTADO_JUGANDO=1 y asi
typedef enum
{
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_GANO
}EstadoJuego;

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
    TTF_Font* fuenteGrande = TTF_OpenFont(FUENTE, 60);
    TTF_Font* fuenteChica = TTF_OpenFont(FUENTE, 24);

    if(!fuenteGrande || !fuenteChica)
    {
        printf("Error en la fuente ingresada: %s\n", TTF_GetError());
    }
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
                    //si en el menu se apreta ENTER, empieza el juego
                    tableroRellenar(&miTablero); //reinicio las posiciones de las cartas
                    tableroMezclar(&miTablero); //mezclo las cartas en el tablero
                    estadoActual = ESTADO_JUGANDO;
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
            //dibujo el menu
            SDL_Color colorTexto = {255,255,255};
            SDL_Surface* surf = TTF_RenderText_Solid(fuenteGrande, "MEMOTEST - TP", colorTexto);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_Rect rect = {200,200,surf->w,surf->h};
            SDL_RenderCopy(renderer,tex, NULL, &rect);

            //instrucciones en pantalla
            SDL_Surface* surf2 = TTF_RenderText_Solid(fuenteChica, "Presiona ENTER para Jugar", colorTexto);
            SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
            SDL_Rect rect2 = {300,300,surf2->w, surf2->h};
            SDL_RenderCopy(renderer,tex2,NULL,&rect2);

            //libero la memoria temporal de superficies y texturas de texto
            SDL_FreeSurface(surf);
            SDL_FreeSurface(surf2);
            SDL_DestroyTexture(tex);
            SDL_DestroyTexture(tex2);
        }
        else if(estadoActual == ESTADO_JUGANDO)
        {
            //dibujo el juego en si mismo
            tableroDibujar(&miTablero, renderer);
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
            SDL_Rect rect = {300,200,surf->w,surf->h};
            SDL_RenderCopy(renderer,tex,NULL,&rect);

            SDL_Surface* surf2 = TTF_RenderText_Solid(fuenteChica,"Presiona SPACE para volver al Menu", colorVictoria);
            SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
            SDL_Rect rect2 = {300,300,surf2->w,surf2->h};
            SDL_RenderCopy(renderer,tex2,NULL,&rect2);

            SDL_FreeSurface(surf);
            SDL_FreeSurface(surf2);
            SDL_DestroyTexture(tex);
            SDL_DestroyTexture(tex2);
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
