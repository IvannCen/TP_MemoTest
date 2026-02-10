/*
    @author CEJAS-NUÑEZ-NAHUEL

    @date 2026-02-07

*/


#include <stddef.h>
#include "visual.h"
#define PUNTOS 100
#define MULTIPLICADOR 1.5

/* 3X4, 4X4, 4X5 */
#define MAX_FIL 4
#define MAX_COL 5
#define MIN_FIL 3
#define MIN_COL 4

/*  NECESITAREMOS DE VARIAS ESTRUCTURAS:
    NECESITAMOS ENCAPSULAR EL TDA EN UNA SOLA ESTRUCTURA EL CUAL SERA TDA JUEGO.
    TDA JUEGO ENCAPSULA A CARTA, JUGADOR Y EL VECTOR GENERICO PARA PODER APLICAR EL CONCEPTO DE TDA...
    VECTOR AL SER GENERICO(*) NO TIENE MEMORIA FRAGMENTADA...A DIFERENCIA DE (**)(PUNTERO QUE APUNTA UN PUNTERO)

*/
// El TDA Vector Genérico: Maneja un bloque de memoria contiguo

typedef struct {
    void* datos;          // Puntero al bloque de memoria (malloc)
    int capacidad;        // Cantidad de elementos (filas * columnas)
    size_t tamElemento;   // Tamaño en bytes de cada elemento (sizeof(Carta))
} Vector;

typedef struct {
    int idFigura;         // Para saber qué imagen mostrar
    bool resuelta;        // ¿Ya se encontró su par?
    bool descubierta;
} Carta;

typedef struct {
    char nombre[32];
    int puntaje;
    int racha;
    int aciertos;
} Jugador;

typedef struct {
    Vector tablero;       // EL TDA: Aquí viven todas las Cartas
    int filas;
    int columnas;

    Jugador jugadores[2]; // Para modo competitivo (1 o 2 jugadores)
    int turnoActual;      // 0 para Jugador 1, 1 para Jugador 2
    int cantJugadores;    // Configurado desde el menú
    int setFiguras;        // ID del set elegido (Mario, etc.)

    int cartasVolteadas;
    int indicePrimerCarta;

} Juego;

typedef struct
{
    int filas;
    int columnas;
    int setFiguras;
    int cantJugadores;
    char nombreUltimoJugador[32]; // Para que el campo de texto ya aparezca lleno
} Config;


bool CrearVec(Vector *v, int capacidad, size_t tam_elementos);
void *Obtener_Elemento(Vector *v, int indice);
void DestruirVec(Vector *v);
void Mezclar_Tablero(Juego *j);
Juego* Crear_Juego(int fil, int col, int cantJugadores, int figura);
void seleccionar_carta(ContextoSDL* ctx, Juego *j, int f, int c);
void comparar_seleccion(Juego* j, int id1, int id2);
void guardar_configuracion(Config* cfg);
void cargar_configuraciones(Config* cfg);
void registrar_puntaje(Jugador j);
void Dibujar_Tablero(ContextoSDL* ctx, Juego* j);



/*------------------------------*/
/* ----- PROPOSITOS DE DEBUG --------*/
void Debug_Mostrar_Tablero(Juego* j);
