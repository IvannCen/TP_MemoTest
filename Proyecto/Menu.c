#include "Menu.h"
#include "Tablero.h"

void menuIniciar(Menu* m, SDL_Renderer* renderer)
{
    m->opcionSeleccionada = 0; //arranca desde la primer opcion del menu

    //cargo las fuentes y ajusto los tamaños como quiera
    m->fuenteTitulo = TTF_OpenFont(FUENTE, 80);
    m->fuenteOpciones = TTF_OpenFont(FUENTE, 40);

    //creo los titulos una sola vez
    SDL_Color color = {255,215,0};
    SDL_Surface* surf = TTF_RenderText_Blended(m->fuenteTitulo, "MEMOTEST MASTER", color);
    m->texturaTitulo = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    //inicializo tambien los ingresos del nombre
    m->nombre.fuenteGrande = TTF_OpenFont(FUENTE, FUENTEGRANDE); //letras del nombre
    m->nombre.fuenteChica = TTF_OpenFont(FUENTE, FUENTECHICA); //boton de confirmar
    m->nombre.fuenteMedia = TTF_OpenFont(FUENTE, FUENTEMEDIA);
    ingresoNombreIniciar(&m->nombre);
}

void menuDestruir(Menu* m)
{
    if(m->texturaTitulo)
        SDL_DestroyTexture(m->texturaTitulo);

    if(m->fuenteTitulo)
        TTF_CloseFont(m->fuenteTitulo);

    if(m->fuenteOpciones)
        TTF_CloseFont(m->fuenteOpciones);

    if(m->nombre.fuenteGrande)
        TTF_CloseFont(m->nombre.fuenteGrande);

    if(m->nombre.fuenteChica)
        TTF_CloseFont(m->nombre.fuenteChica);
}

int menuManejarOpciones(Menu* m, SDL_Event* e)
{
    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
            m->opcionSeleccionada--;
            if(m->opcionSeleccionada < 0)
                m->opcionSeleccionada = CANTIDADOPCIONES - 1; //vuelve a la opcion de abajo del todo

            break;
        case SDLK_DOWN:
            m->opcionSeleccionada++;
            if(m->opcionSeleccionada >= CANTIDADOPCIONES)
                m->opcionSeleccionada = 0; //vuelve a la primera opcion

            break;
        case SDLK_RETURN: //este es el enter normal
        case SDLK_KP_ENTER: //enter del pad numerico
            return m->opcionSeleccionada; //devuelvo la opcion si se selecciona el enter
        }
    }

    return -1; //devuelvo -1 si sigue en el menu y no eligio nada
}

void menuDibujar(Menu* m, SDL_Renderer* renderer)
{
    //lo dibujo al menu centrado en la pantalla
    int w,h;
    SDL_QueryTexture(m->texturaTitulo, NULL, NULL, &w, &h);
    SDL_Rect recTitulo = {(ANCHOVENTANA - w)/2, 100, w, h};
    SDL_RenderCopy(renderer, m->texturaTitulo, NULL, &recTitulo);

    //dibujo las opciones en pantalla
    const char* opciones[] = {"Aventura","Competitivo","Ranking","Configuracion","Salir"};

    //estos los pongo estaticos pero podrian ser macros, vamos viendo
    int inicioY = 300; //altura donde esta la primer opcion para tener referencia
    int separacion = 70; //espacion entre opciones

    for(int i=0;i<CANTIDADOPCIONES;i++)
    {
        SDL_Color color = (i==m->opcionSeleccionada) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
        dibujarTextoCentrados(renderer, m->fuenteOpciones, opciones[i], inicioY + (i * separacion), color);
    }
}

int menuDificultadOpciones(Menu* m, SDL_Event* e)
{
    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
            m->opcionSeleccionada--;
            if(m->opcionSeleccionada<0)
                m->opcionSeleccionada = CANTIDADIFICULTAD - 1;

            break;
        case SDLK_DOWN:
            m->opcionSeleccionada++;
            if(m->opcionSeleccionada >= CANTIDADIFICULTAD)
                m->opcionSeleccionada = 0;

            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            return m->opcionSeleccionada; //retorna 0 para facil, 1 normal, 2 dificil, 3 volver
            break;
        }
    }
    return -1;
}

void menuDificultadDibujar(Menu* m, SDL_Renderer* renderer)
{
    SDL_Color colorTitulo = {255,215,0};
    dibujarTextoCentrados(renderer,m->fuenteTitulo,"SELECCIONA DIFICULTAD",100,colorTitulo);

    const char* opciones[]={"FACIL","NORMAL","DIFICIL","VOLVER"};

    //estos los pongo estaticos pero podrian ser macros, vamos viendo
    int inicioY = 300; //altura donde esta la primer opcion para tener referencia
    int separacion = 70; //espacion entre opciones

    for(int i=0;i<CANTIDADIFICULTAD;i++)
    {
        SDL_Color color = (i==m->opcionSeleccionada) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
        dibujarTextoCentrados(renderer,m->fuenteOpciones,opciones[i],inicioY + (i * separacion),color);
    }
}

//ingreso del nombre en el juego

void ingresoNombreIniciar(IngresoNombre* ing)
{
    ing->cursor = 0;
    strcpy(ing->nombre,"AAA");
}

//funcion auxiliar para los caracteres
char siguienteCaracter(char c)
{
    if(c=='Z')
        return '0';

    if(c=='9')
        return 'A';

    return c+1;
}

char anteriorCaracter(char c)
{
    if(c=='A')
        return '9';

    if(c=='0')
        return 'Z';

    return c-1;
}

int ingresoNombreOpciones(IngresoNombre* ing, SDL_Event* e, ContextoJuego* juego)
{
    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_LEFT:
            ing->cursor--;
            if(ing->cursor<0)
                ing->cursor=3; //es el boton de confirmar

            break;
        case SDLK_RIGHT:
            ing->cursor++;
            if(ing->cursor>3)
                ing->cursor=0;

            break;
        case SDLK_UP:
            if(ing->cursor<3)
                ing->nombre[ing->cursor] = siguienteCaracter((ing->nombre[ing->cursor]));

            break;
        case SDLK_DOWN:
            if(ing->cursor<3)
                ing->nombre[ing->cursor] = anteriorCaracter((ing->nombre[ing->cursor]));

            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            if(ing->cursor == 3)
            {
                strcpy(juego->nombreJugador, ing->nombre);
                return 1; //1=confirmado
            }


            break;
        }
    }
    return 0; //0 = seguir adelante
}

void ingresoNombreDibujar(IngresoNombre* ing, SDL_Renderer* renderer)
{
    SDL_Color colorTitulo = {255,215,0};
    dibujarTextoCentrados(renderer,ing->fuenteGrande,"INGRESE NOMBRE",100,colorTitulo);

    int inicio = (ANCHOVENTANA / 2) - 100;
    int inicioY = 300; //altura donde esta la primer opcion para tener referencia
    int separacion = 70; //espacion entre opciones

    for(int i=0;i<3;i++)
    {
        char letra[2] = {ing->nombre[i], '\0'};

        //si el cursor esta en la letra, es de color verde, sino blanco
        SDL_Color color = (i==ing->cursor) ? (SDL_Color){0,255,0} : (SDL_Color){255,255,255};
        dibujarTexto(renderer,ing->fuenteGrande,letra,inicio+(i*separacion),inicioY,color);
    }

    //dibujo el boton de confirmar
    SDL_Color colorBoton = (ing->cursor == 3) ? (SDL_Color){0,255,0} : (SDL_Color){100, 100, 100};
    dibujarTextoCentrados(renderer,ing->fuenteMedia,"CONFIRMAR",500,colorBoton);

    //instrucciones en pantalla
    dibujarTextoCentrados(renderer,ing->fuenteChica,"Utilice las fechas del teclado para moverse entre caracteres",600,colorTitulo);
    dibujarTextoCentrados(renderer,ing->fuenteChica,"Luego presione ENTER en el boton confirmar",650,colorTitulo);
}
