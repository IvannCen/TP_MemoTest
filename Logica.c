#include "Logica.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ------- FUNCIONES PARA EL TDA VECTOR ---------*/

/* Inicializa el bloque de memoria para el vector */
bool CrearVec(Vector *v, int capacidad, size_t tam_elementos)
{
    v->datos = malloc(capacidad*tam_elementos);
    if(!v->datos)
        return false;

    v->capacidad = capacidad;
    v->tamElemento = tam_elementos;
    return true;
}

/* Retorna un puntero al elemento en un índice específico */
void *Obtener_Elemento(Vector *v, int indice) /* TOCO TAL POSICION DE X VECTOR Y BUENO NECESITO QUE ELEMENTO ES */
{
    /* VALIDAMOS RANGOS */
    if(indice < 0 || indice >= v->capacidad )
        return NULL;
    /* APLICAMOS ARITMETICA DE PUNTEROS -> BASE + (DESPLAZAMIENTO) */
    return (char*)v->datos + (indice + v->tamElemento);
    /* CHAR YA QUE DEBEMOS MOVERNOS BYTE A BYTE*/
}

/* LIBERAR LA MEMORIA QUE PEDIMOS...IMPORTANTISIMO !!!!!! */
void DestruirVec(Vector *v)
{
    free(v->datos);
    v->datos = NULL;
}

/* ------------------------------------------------------*/

/* ---------- FUNCIONES TDA JUEGO ( LOGICA DEL JUEGO ) -------------*/

Juego* Crear_Juego(int filas, int col, int cantJugadores, int figura)
{
    /*  VALIDAR RANGOS() */
    if (filas < MIN_FIL || filas > MAX_FIL || col < MIN_COL || col > MAX_COL) {
        printf("Error: Dimensiones fuera de rango.\n");
        return NULL;
    }
    /* INICIALIZAMOS MEMORIA DEL JUEGO */
    Juego *juego = (Juego*)malloc(sizeof(Juego));
    if(!juego)
        return NULL;
    /* INICIALIZAMOS CONTENIDOS*/
    juego->filas = filas;
    juego->columnas = col;
    juego->cantJugadores = cantJugadores;
    juego->setFiguras = figura;
    juego->turnoActual = 0;

    /* CANTIDAD DE ELEMENTOS QUE TENDREMOS EN EL TABLERO */
    int TotalCartas = filas*col;

    /*  CREAMOS EL VECTOR TDA: SI FALLA, LIBERAMOS
        SI NO...EL VECTOR SE HA CREADO CORRECTAMENTE*/
    if(!CrearVec(&juego->tablero, TotalCartas, sizeof(Carta)))
    {
        free(juego);
        return NULL;
    }

    printf("Sistema: TDA vector Creado Correctamente");

    /* PIDO MEMORIA PARA LAS CARTAS: CASTEAMOS LA FUNCION YA QUE DEVUELVE VOID*  */
    for(int i = 0; i<TotalCartas; i++) /* TOTAL DE CARTAS */
    {
        Carta *c = (Carta*)Obtener_Elemento(&juego->tablero, i);
        /*  INICIALIZAMOS EN FALSO PARA LOGRAR LA CONFIGURACION
            MAS ADELANTE SE ASIGNARA EN LA MEZCLA !! */
        c->idFigura = -1;
        c->resuelta = false;
        c->descubierta = false;
    }

    printf("Sistema: Juego creado Correctamente");
    return juego;
}

/* DESTRUIMOS EL JUEGO: IMPORTANTISIMO !!!!!!*/
void DestruirJuego(Juego *juego)
{
    DestruirVec(&juego->tablero); /* LLAMO A DESTRUIR VECTOR */
    printf("Sistema: TDA Vector Cerrado Correctamente");
    free(juego); /* LIBERAMOS *juego  */
    printf("Sistema: Memoria Liberadad  Juego Ha Cerrado Correctamente");
}
/*  FUNCION MESCLAR CARTAS...
    LOGICA: SI EL TABLERO TIENE 12 CARTAS, NECESITO 6 PARES DE CARTAS ->[0,0,1,1,2,2,3,3,4,4,5,5,]
    DESORDENAR ESTOS ID PARA QUE NO QUEDEN JUNTOS.
    NECESITAMOS LA BIBLIOTECA TIME.H PARA PODER USAR LA FUNCION RAND()
    NECESITAMOS UN ALGORITMO MUY USADO EN LA INDUSTRIA LLAMADO FISHER-YATES
    COMO NO PUEDO USAR VEC[i] LLAMARE A ELEMENTO OBTENER
    */


void Mezclar_Tablero(Juego *j)
{
    int total_cartas = j->filas * j->columnas;
    int num_parejas = total_cartas / 2;

    /* INICIAMOS EL GENERADOR DE ALEATORIDAD */
    srand(time(NULL));

    /* PRIMERA PASADA: ASIGNAR IDS A LAS PAREJAS */
    for(int i=0; i<total_cartas;i++)
    {
        Carta *c1 = (Carta*)Obtener_Elemento(&j->tablero, i);
        Carta *c2 = (Carta*)Obtener_Elemento(&j->tablero, i+1);

        int id_actual = i/2; /* ESTO GENERA EL 0,0,1,1...*/
        /* ACA YA LE DAMOS LOS ID A LAS FIGURITAS */
        if(c1) /* C1 != NULL*/
            c1->idFigura = id_actual;
         if(c2) /* C2 != NULL*/
            c2->idFigura = id_actual;
    }
        /*  SEGUNDA PASADA: MEZCLAS FISHER-YISHER
            ARRANCAMOS DESDE EL FINAL HACIA ATRAS PARA GENERAR ALETORIDAD */
        for(int i=total_cartas-1; i>0; i--)
        {
            int j_aleatorio = rand() % (i+1);

            /* OBTENEMOS LOS PUNTEROS DE LAS CARTAS PARA LOGRAR INTERCAMBIARLOS */
            Carta *cartaI = (Carta*)Obtener_Elemento(&j->tablero, i);
            Carta *cartaJ = (Carta*)Obtener_Elemento(&j->tablero, j_aleatorio);

            /*  INTERCAMBIO EL ID FIGURA
                BASICAMENTE UN SWAP DE IDs*/
            int tempID = cartaI->idFigura;
            cartaI->idFigura = cartaJ->idFigura;
            cartaJ->idFigura = tempID;
        }
      printf("Sistema: Tablero mezclado con %d parejas \n", num_parejas);
}

void seleccionar_carta(ContextoSDL* ctx, Juego *j, int f, int c)
{
    /* PASAR LAS COORDENADAS DE LA MATRIZ A TDA VECTOR */
    int indice_actual = (f * j->columnas) + c;
    Carta *cartaActual = (Carta*)Obtener_Elemento(&j->tablero, indice_actual);

    if(!cartaActual || cartaActual->descubierta || cartaActual->resuelta )
        return; /* IGNORO EL CLIC */

    if(j->cartasVolteadas >= 2)
        return;

    /* AGREGAMOS SONIDOS DE CLICK´s */

    Mix_PlayChannel(-1, ctx->sfxClick, 0);

    /* VOLTEAMOS LA CARTA */
    cartaActual->descubierta = true;
    j->cartasVolteadas++;

    /*LOGICA DE TURNOS */
    if(j->cartasVolteadas == 1)
    {   /* PRIMERA CARTA QUE VOLTEA*/
        j->indicePrimerCarta = indice_actual;
        printf("Sistema: Primera carta seleccionada (ID: %d)\n", cartaActual->idFigura);
    }
    else if(j->cartasVolteadas == 2)
    {
        /* SEGUNDA CARTA VOLTEADA YA ACA HAY QUE COMPARAR... */
        printf("Sistema: Segunda carta seleccionada (ID: %d). Comparando...\n", cartaActual->idFigura);
        comparar_seleccion(j, j->indicePrimerCarta, indice_actual);
    }

}

void comparar_seleccion(Juego* j, int id1, int id2)
{
    Carta *c1 = (Carta*)Obtener_Elemento(&j->tablero, id1);
    Carta *c2 = (Carta*)Obtener_Elemento(&j->tablero, id2);

    /* DEBEMOS OBTENER EL JUGADOR ACTUAL */
    Jugador *actual = &j->jugadores[j->turnoActual];

    /* SUMAMOS PUNTOS -- ACIERTO */
    if(c1->idFigura == c2->idFigura)
    {
        c1->resuelta = true;
        c2->resuelta = true;

        actual->racha++;
        actual->aciertos++;

        /* CALCULAR PUNTOS: BASE + (Racha*Multiplicador extra)*/
        /*EJEMPLO: 100 + (2 * 1,5 * 100)*/

        actual->puntaje += PUNTOS + (int)(actual->racha * MULTIPLICADOR * 10);
        printf("Sistema: ¡Pareja encontrada! Puntos: %d | Racha: %d\n", actual->puntaje, actual->racha);

        /* LIMPIAMOS LAS CARTAS VOLTEADAS */
        j->cartasVolteadas = 0;
    }
    else
    {
        actual->racha = 0;
        printf("Sistema: No coinciden\n");

        if(j->cantJugadores > 1)
        {
            j->turnoActual = (j->turnoActual + 1) % 2;
        }

    }

}

/* PERSISTENCIA DE ARCHIVOS */
void guardar_configuracion(Config* cfg)
{
    FILE* f = fopen("config.dat", "wb");
    if(f)
    {
        fwrite(cfg, sizeof(Config), 1, f);
        fclose(f);
        printf("Sistema: Configuracion guardada en el disco\n");
    }
    /*ELSE: SI LA CONFIGURACION FALLA O ALGO TENEMOS QUE FRENAR LA EJECUCION DE ALGUNA MANERA...*/
}
/* SI EL ARCHIVO NO EXISTE: PORQUE EL JUEGO SE CARGO POR PRIMERA VEZ, CARGAMOS POR DEFECTO */
void cargar_configuraciones(Config* cfg)
{
    FILE* f = fopen("config.dat", "rb");
    if(f)
    {
        fread(cfg, sizeof(Config), 1, f);
        fclose(f);
    }
    else
    {
        /* VALORES POR DEFECTO */
        cfg->filas = MIN_FIL;
        cfg->columnas = MIN_COL;
        cfg->setFiguras = 1;
        strcpy(cfg->nombreUltimoJugador, "Jugador 1");
    }
}
void registrar_puntaje(Jugador j)
{
    FILE* f = fopen("ranking.dat", "ab"); /* AGREGAMOS AL FINAL CON AB */
    if(f)
    {
        fwrite(&j, sizeof(Jugador), 1, f);
        fclose(f);
    }
}


void Dibujar_Tablero(ContextoSDL* ctx, Juego* j)
{
    int anchoCarta = 120;
    int altoCarta = 160;
    int margen = 20;

    for(int f=0; f<j->filas; f++)
    {
        for(int c=0; c<j->columnas; c++)
        {
            int indice = f * j->columnas + c;
            Carta* carta = (Carta*)Obtener_Elemento(&j->tablero, indice);

            /* CALCULAMOS LA POSICION X  E  Y EN LA PANTALLA */
            SDL_Rect rectDestino = {
                margen + c * (anchoCarta + margen), // X
                margen + f * (altoCarta + margen),  // Y
                anchoCarta, altoCarta               // W, H
            };


            if (carta->descubierta || carta->resuelta)
                {
                    SDL_RenderCopy(ctx->renderizador, ctx->figuras[carta->idFigura], NULL, &rectDestino);
                }
            else
                {
                    SDL_SetRenderDrawColor(ctx->renderizador, 255, 165, 0, 255);
                    SDL_RenderFillRect(ctx->renderizador, &rectDestino);
                }
        }
    }
}

/* ------------------------------------------------------------------------- */

/* FUNCIONES STATIC PARA PROBAR COMO FUNCIONA ALGO: */

void Debug_Mostrar_Tablero(Juego* j)
 {
    printf("\n--- ESTADO DEL TABLERO ---\n");
    for (int f = 0; f < j->filas; f++) {
        for (int c = 0; c < j->columnas; c++) {
            // Calculamos el índice lineal para el TDA Vector
            int indice = f * j->columnas + c;
            Carta* carta = (Carta*)Obtener_Elemento(&j->tablero, indice);

            printf("[%02d] ", carta->idFigura);
        }
        printf("\n");
    }
    printf("--------------------------\n");
}
