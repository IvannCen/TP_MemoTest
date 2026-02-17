#ifndef MENU
#define MENU

#include "Comun.h"
#include "Juego.h"

#define SETA 0
#define FILAS 4
#define COLUMNAS 4
#define CANTJUGADORES 1

//defino las opciones del menu enumeradas
typedef enum
{
    OPCION_AVENTURA,
    OPCION_COMPETITIVO,
    OPCION_ESTADISTICAS,
    OPCION_CONFIGURACION,
    OPCION_SALIR,
    CANTIDADOPCIONES //esta variable va a valer 5 automatico
}OpcionesMenu;

//menu de dificultad
typedef enum
{
    FACIL,
    NORMAL,
    DIFICIL,
    VOLVER,
    CANTIDADIFICULTAD
}OpcionesDificultad;

//ingreso de nombre
typedef struct
{
    int cursor; //0,1,2 (letras) o 3 (boton confirmar), 4 (Volver)
    char nombre[LETRASNOMBRE]; //nombre temporal "AAA"
    TTF_Font* fuenteGrande;
    TTF_Font* fuenteChica;
    TTF_Font* fuenteMedia;
}IngresoNombre;

//struct del menu
typedef struct
{
    int opcionSeleccionada; //una de las 5 opciones (de 0 a 4)
    TTF_Font* fuenteTitulo;
    TTF_Font* fuenteOpciones;
    SDL_Texture* texturaTitulo; //guardo la textura para no generarla en cada frame del menu
    IngresoNombre nombre;
    int confirmaSalida; //0 = no muestra el cartel, 1 = muestra el cartel
    int opcionSalida; //0 = si, 1 = no;
}Menu;

//funciones del menu
void menuIniciar(Menu* m, SDL_Renderer* renderer);
void menuDestruir(Menu* m);

//funciones del manejo de flechas: "-1" si no eligio nada, o la opcion si se presiona enter
int menuManejarOpciones(Menu* m, SDL_Event* e);
void menuDibujar(Menu* m, SDL_Renderer* renderer);

//menu de configuracion
//void menuDificultadDibujar(Menu* m, SDL_Renderer* renderer);
//int menuDificultadOpciones(Menu* m, SDL_Event* e);
int obtenerYConfig(int opcion);
int menuConfiguracionOpciones(Menu* m, SDL_Event* e, Configuracion* config);
void menuConfiguracionDibujar(Menu* m, SDL_Renderer* renderer, Configuracion* config);
void cargarConfiguracion(Configuracion* config);
void guardarConfiguracion(Configuracion* config);

//ingreso de nombres
void ingresoNombreIniciar(IngresoNombre* ing); //funcion que resetea el ingreso a AAA
void ingresoNombreDibujar(IngresoNombre* ing, SDL_Renderer* renderer);
//devuelve: 0(nada), 1(confirmar/jugar), 2(volver al menu)
int ingresoNombreOpciones(IngresoNombre* ing, SDL_Event* e, ContextoJuego* juego);

//funciones extra
char siguienteCaracter(char c);
char anteriorCaracter(char c);
const char* menuObtenerTexto(int opcion); //conecta el enum con el texto
//const char* menuDificultadObtenerTexto(int opcion); //idem con dificultad (ESTO SE VA A MODIFICAR CUANDO CAMBIEMOS LA CONFIG)

#endif // MENU
