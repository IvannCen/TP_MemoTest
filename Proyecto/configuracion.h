#ifndef CONFIGURACION
#define CONFIGURACION

// --- DIMENSIONES ---
#define ANCHO_VENTANA 1366
#define ALTO_VENTANA 768
#define TITULO_VENTANA "TP Memotest"

// --- INTERFAZ ---
#define ALTO_INTERFAZ 80
#define MARGEN_LATERAL 30
#define MARGEN_SUPERIOR 20

// --- CARTAS ---
#define ANCHO_CARTA 130
#define ALTO_CARTA 170
#define SEPARACION_CARTA 15
#define CARTA_HOVER_OFFSET 10
#define CARTA_SOMBRA_OFFSET 8

// --- RECURSOS: FUENTES ---
#define RUTA_FUENTE "fnt/jersey.ttf"
#define TAM_FUENTE_GRANDE 100
#define TAM_FUENTE_MEDIA 65
#define TAM_FUENTE_CHICA 30
#define TAM_FUENTE_OPCIONES 40
#define TAM_FUENTE_TITULO 80

// --- RECURSOS: IMAGENES ---
#define CANTIDAD_IMAGENES 10
#define RUTA_SET_A "img/A/"
#define RUTA_SET_B "img/B/"
#define RUTA_DORSO_A "img/A/dorsoA.png"
#define RUTA_DORSO_B "img/B/dorsoB.png"

// --- RECURSOS: FONDOS ---
#define RUTA_FONDO_MENU     "img/Fondos/fondoMenu.png"
#define RUTA_FONDO_CONFIG   "img/Fondos/fondoConfig.jpg"
#define RUTA_FONDO_RANKING  "img/Fondos/fondoRankings.jpg"
#define RUTA_FONDO_NOMBRES  "img/Fondos/fondoNombres.jpg"
#define RUTA_FONDO_JUEGO_C  "img/Fondos/fondoJuegoC.jpg"
#define RUTA_FONDO_JUEGO_L  "img/Fondos/fondoJuegoL.jpg"

// --- RECURSOS: SONIDOS ---
#define RUTA_SND_SELECCION  "snd/seleccion.wav"
#define RUTA_SND_ACIERTO    "snd/acierto.wav"
#define RUTA_SND_FALLO      "snd/fallo.wav"
#define RUTA_MUS_MENU       "snd/menu.mp3"
#define RUTA_MUS_JUEGO      "snd/juego.mp3"

// --- JUGABILIDAD ---
#define PUNTOS_BASE_ACIERTO 100
#define PUNTOS_BONUS_RACHA 20
#define PUNTOS_PENALIZACION 20
#define TIEMPO_DELAY_ERROR 1000
#define TIEMPO_DELAY_CORTO 500

// --- LIMITES ---
#define MAX_JUGADORES 2
#define MAX_LETRAS_NOMBRE 4
#define ARCHIVO_CONFIG "config.dat"
#define ARCHIVO_RANKING "ranking.dat"
#define MAX_REGISTROS_RANKING 10

// --- COLORES ---
#define COLOR_NORMAL    (SDL_Color){220, 220, 220, 255}
#define COLOR_SELECCION (SDL_Color){255, 50, 50, 255}
#define COLOR_TITULO    (SDL_Color){255, 215, 0, 255}
#define COLOR_GRIS      (SDL_Color){100, 100, 100, 255}
#define COLOR_BLANCO (SDL_Color){255, 255, 255, 255}
#define COLOR_NEGRO  (SDL_Color){0, 0, 0, 255}
#define COLOR_VERDE  (SDL_Color){0, 255, 0, 255}
#define COLOR_ROJO   (SDL_Color){255, 0, 0, 255}
#define COLOR_AZUL   (SDL_Color){0, 0, 255, 255}

// --- VALORES DEFAULT ---
#define DEFAULT_FILAS 4
#define DEFAULT_COLUMNAS 4
#define DEFAULT_SET 0
#define DEFAULT_JUGADORES 1

// --- VOLUMEN ---
#define VOL 50

#endif // CONFIGURACION
