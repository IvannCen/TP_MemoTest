#ifndef MENU
#define MENU

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "configuracion.h"
#include "juego.h"

typedef enum
{
    OPCION_JUGAR,
    OPCION_ESTADISTICAS,
    OPCION_CONFIGURACION,
    OPCION_SALIR,
    CANTIDAD_OPCIONES
}OpcionesMenu;

typedef struct
{
    int cursor;
    char nombre[MAX_LETRAS_NOMBRE];
    char titulo[50];
    TTF_Font* fuenteGrande;
    TTF_Font* fuenteChica;
    TTF_Font* fuenteMedia;
}IngresoNombre;

typedef struct
{
    int opcionSeleccionada;
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteOpciones;
    SDL_Texture* texturaTitulo;
    SDL_Texture* fondoMenu;
    SDL_Texture* fondoConfig;
    SDL_Texture* fondoNombres;
    SDL_Texture* fondoRanking;
    IngresoNombre nombre;
    int confirmaSalida;
    int opcionSalida;
}Menu;

void menu_Iniciar(Menu* m, SDL_Renderer* renderer);
void menu_Destruir(Menu* m);
int menu_ManejarOpciones(Menu* m, SDL_Event* e);
void menu_Dibujar(Menu* m, SDL_Renderer* renderer);
int menu_ConfiguracionOpciones(Menu* m, SDL_Event* e, ConfiguracionJuego* config);
void menu_ConfiguracionDibujar(Menu* m, SDL_Renderer* renderer, ConfiguracionJuego* config);
void config_Cargar(ConfiguracionJuego* config);
void config_Guardar(ConfiguracionJuego* config);
void ingreso_NombreIniciar(IngresoNombre* ing, const char* titulo);
void ingreso_NombreDibujar(IngresoNombre* ing, SDL_Renderer* renderer, SDL_Texture* fondo);
int ingreso_NombreOpciones(IngresoNombre* ing, SDL_Event* e, ContextoJuego* juego, int indiceJugador);

#endif // MENU
