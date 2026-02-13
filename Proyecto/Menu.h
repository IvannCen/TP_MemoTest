#ifndef MENU
#define MENU

#include "Comun.h"

//defino las opciones del menu enumeradas
typedef enum
{
    OPCION_AVENTURA = 0,
    OPCION_COMPETITIVO,
    OPCION_ESTADISTICAS,
    OPCION_CONFIGURACION,
    OPCION_SALIR,
    CANTIDADOPCIONES //esta variable va a valer 5 automatico
}OpcionesMenu;

typedef struct
{
    int opcionSeleccionada; //una de las 5 opciones (de 0 a 4)
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteOpciones;
    SDL_Texture* texturaTitulo; //guardo la textura para no generarla en cada frame del menu
}Menu;

//funciones del menu
void menuIniciar(Menu* m, SDL_Renderer* renderer);
void menuDestruir(Menu* m);

//funciones del manejo de flechas: "-1" si no eligio nada, o la OPCION si se presiona Enter
int menuManejarOpciones(Menu* m, SDL_Event* e);
void menuDibujar(Menu* m, SDL_Renderer* renderer);

#endif // MENU
