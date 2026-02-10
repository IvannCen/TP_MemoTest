#include "visual.h"
#include <stdio.h>

// Definición de colores
const SDL_Color COLOR_NEON_NARANJA = {255, 165, 0, 255};
const SDL_Color COLOR_FONDO_SEGA   = {20, 20, 40, 255};
const SDL_Color COLOR_BLANCO       = {255, 255, 255, 255};

bool Inicializar_SDL(ContextoSDL* ctx)
{

    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "0");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        return false;
    }

    ctx->ventana = SDL_CreateWindow(
        "Memoria TP - Estetica Arcade 90s",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        ANCHO_PANTALLA, ALTO_PANTALLA,
        SDL_WINDOW_SHOWN
    );

    ctx->renderizador = SDL_CreateRenderer(ctx->ventana, -1, SDL_RENDERER_SOFTWARE);

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_ClearError();

    ctx->corriendo = true;

    return true;
}

void Cerrar_SDL(ContextoSDL* ctx)
{
    if (ctx->renderizador)
        SDL_DestroyRenderer(ctx->renderizador);
    if (ctx->ventana)
        SDL_DestroyWindow(ctx->ventana);

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Cargar_Assets(ContextoSDL* ctx, char *categoria)
{
    char ruta[100];

    /* 1. Cargar las 10 figuras del set elegido */
    for (int i = 0; i < MAX_FIGURAS; i++)
        {
            /* Construye la ruta: "imagenes/cars/img1.jpg" */
            sprintf(ruta, "imagenes/%s/img%d.jpg", categoria, i + 1);
            ctx->figuras[i] = IMG_LoadTexture(ctx->renderizador, ruta);

            if(!ctx->figuras[i])
            {
                printf("Error: No se encontro %s\n", ruta);
                return false;
            }
        }
    ctx->fuenteArcade = TTF_OpenFont("tipografia/ARCADE_N.TTF", 12);
    ctx->sfxClick = Mix_LoadWAV("sonidos/click.mp3");
    ctx->sfxWinner = Mix_LoadWAV("sonidos/winner.mp3");
    ctx->sfxRacha = Mix_LoadWAV("sonidos/racha.mp3");

    return true;

}

void Escribir_Text(ContextoSDL* ctx, char*texto, int x, int y, SDL_Color color, bool centrado)
{
    SDL_Surface* surface = TTF_RenderText_Solid(ctx->fuenteArcade, texto, color);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx->renderizador, surface);

    SDL_Rect rect = { centrado ? (512/2 - surface->w/2) : x, y, surface->w, surface->h };
    SDL_RenderCopy(ctx->renderizador, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}




/* -------------------------------------------------*/
/* DEBUG Y APRENDIZAJE DE LAS FUNCIONES SDL*/

#define ALTO 600
#define ANCHO 400

/*  SDL2: INICIALIZAR -> ABRIR VENTANA -> LOOP PRINCIPAL -> EVENTOS -> DIBUJOS -> CIERRO
    DEBERAS IR CAPTURANDO EVENTOS EN UN BUCLE.
    SDL EVENT* ES UNA ESTRUCURA GIGANTE QUE CAPTURA TODO TIPO DE EVENTO COMO CLICK, MOUSE, TECLADO, ETC...
    CLAVE: SDL POLL EVENT -> REVISA SI HAY EVENTOS PENDIENTES YA QUE AQUI SE OPERA POR COLA...PRIMERO ESTE, LUEGO ESTE Y ASI..
    EJEM: evento.type = MOUSEBUTTONDOWN
    e.button.button = SDL_BUTTON_LEFT / SDL_BUTTON_RIGHT
    COORDENADAS: e.button.x / e.button.y
    SDL RECT -> POSICION Y TAMAÑO(rect.x, rect.y, rect.w, rect.h)
    USADO PARA DIBUJAR SPRAYS, DETECTA COLISIONES...
    evento.type == SDL_KEYDOWN ME DETECTA SI APRETE UNA TECLA!
    con: printf("Detectamos la tecla[%d]\n",evento.key.keysym.sym); ME IMPRIME QUE TECLA TOCO
    evento.key.keysym.sym == SDLK_RETURN DETECTA EL ENTER(IDEAL PARA MENU)
    PRIMERO DETECTO QUE TIPO DE EVENTO FUE, SI MOUSE O TECLADO Y RECIEN AHI OBSERVO QUE SI FUE CLICK LEFT O ETC.
    LAS COORDENADAS ARRANCAR DESDE LA ESQUINA SUPERIOR IZQUIERDA COMO UNA MATRIZ: PARA HACER UNA CUADRADP SE ELIGEN SU COORDENADAS
    Y ANCHO Y ALTURA.
    OSEA...DESDE LA COORDENADA X,Y LE DAMOS ALTURA Y ANCHO..
    SDL_Rect es solo datos
    SDL_RenderFillRect es el dibujo
    SDL_GetKeyboardState(NULL) IDEAL PARA MOVIMIENTOS FLUIDOS Y RAPIDOS USADA PARA DETECTAS COSAS EN TIEMPO REAL -> teclado[SDL_SCANCODE_D]


*/

void ProbarSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* ventana = SDL_CreateWindow("Ventana", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ALTO, ANCHO, 0);

    SDL_Renderer* render = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    int corriendo = 1;
    SDL_Event evento;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 150;
    rect.h = 100;

    /*SDL_Rect rect1;
    rect1.w = 100;
    rect1.h = 100;
    rect1.x = (ALTO - rect1.w) / 2;
    rect1.y = (ANCHO - rect1.h) / 2;*/

    const Uint8* teclado = SDL_GetKeyboardState(NULL);
    int velocidad = 5;
    int arrastre = 0;

    while(corriendo)
    {
        while(SDL_PollEvent(&evento)) /* CUANDO ENTRAMOS POR PRIMERA VEZ, NO HABRA NINGUN EVENTO...NADA. ASIQUE SALTARA DIRECTO A HACER EL DIBUJO*/
        {
            if(evento.type == SDL_QUIT)
            {
                printf("Sistema: Se ha oprimido SKIP, Cerrando ventana...\n");
                corriendo = 0; /* NOS QUEDAMOS SIN EVENTOS Y while(corriendo) EVALUA: */
            }
            if(evento.type == SDL_MOUSEBUTTONDOWN)
            {
                /* MUEVO EL CUADRADO A COMO SE ME DE LA GANA CON MOUSEX E MOUSEY DETECTANDO BOTONES */
                int mouseX = evento.button.x;
                int mouseY = evento.button.y;

                printf("Sistema: Detectamos un click!\n");
                if(evento.button.button == SDL_BUTTON_LEFT)
                {
                    /* EL MOUSE QUEDA EN EL MEDIO */
                    rect.x = mouseX - rect.w / 2;
                    rect.y = mouseY - rect.h / 2;
                    printf("Click Izquierdo! Se movio a: [%d][%d]\n",evento.button.x, evento.button.y);
                }
                if(evento.button.button == SDL_BUTTON_RIGHT)
                {
                    printf("Click Derecho en [%d][%d]\n", evento.button.x, evento.button.y);
                }

                /* GEOMETRIA PARA DETECTA EL RECT */
                if(mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h)
                    {
                        printf("Click dentro del cuadrado\n");
                    }
                printf("\n");
            }
            if(evento.type == SDL_KEYDOWN)
            {
                printf("Detectamos una tecla[%d]\n",evento.key.keysym.sym);
                if(evento.key.keysym.sym == SDLK_KP_ENTER || evento.key.keysym.sym == SDLK_RETURN)
                    printf("Se ha detectado un Enter\n");
                printf("\n");

            }

        }
        /* MOVIMIENDO EL CUADRADO CON TECLAS */
                if (teclado[SDL_SCANCODE_W])
                    rect.y -= velocidad;
                if (teclado[SDL_SCANCODE_S])
                    rect.y += velocidad;
                if (teclado[SDL_SCANCODE_A])
                    rect.x -= velocidad;
                if (teclado[SDL_SCANCODE_D])
                    rect.x += velocidad;
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);

        SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        SDL_RenderFillRect(render, &rect);

       /* SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        SDL_RenderFillRect(render, &rect1);
        */
        SDL_RenderPresent(render);

    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}


