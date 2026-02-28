#include "menu.h"
#include "tablero.h" // Para usar dibujarTextoCentrados
#include <stdio.h>
#include "configuracion.h"

#define INICIOY 300
#define SEPARACION 70
#define CONF_Y_TABLERO 200
#define CONF_Y_SET 280
#define CONF_Y_JUGADORES 360
#define CONF_Y_GUARDAR 480

void menu_Iniciar(Menu* m, SDL_Renderer* renderer)
{
    m->opcionSeleccionada = 0;
    m->confirmaSalida = 0;
    m->opcionSalida = 1;
    m->fuenteTitulo = TTF_OpenFont(RUTA_FUENTE, 80);
    m->fuenteOpciones = TTF_OpenFont(RUTA_FUENTE, 40);
    SDL_Surface* surf = TTF_RenderText_Blended(m->fuenteTitulo, "MEMOTEST MASTER", COLOR_TITULO);
    m->texturaTitulo = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    m->fondoMenu = IMG_LoadTexture(renderer, RUTA_FONDO_MENU);
    m->fondoConfig = IMG_LoadTexture(renderer, RUTA_FONDO_CONFIG);
    m->fondoNombres = IMG_LoadTexture(renderer, RUTA_FONDO_NOMBRES);
    m->fondoRanking = IMG_LoadTexture(renderer, RUTA_FONDO_RANKING);
    m->nombre.fuenteGrande = TTF_OpenFont(RUTA_FUENTE, TAM_FUENTE_GRANDE);
    m->nombre.fuenteChica = TTF_OpenFont(RUTA_FUENTE, TAM_FUENTE_CHICA);
    m->nombre.fuenteMedia = TTF_OpenFont(RUTA_FUENTE, TAM_FUENTE_MEDIA);
    ingreso_NombreIniciar(&m->nombre, "INGRESE NOMBRE");
}

void menu_Destruir(Menu* m)
{
    if(m->texturaTitulo)
        SDL_DestroyTexture(m->texturaTitulo);
    if(m->fondoMenu)
        SDL_DestroyTexture(m->fondoMenu);
    if(m->fondoConfig)
        SDL_DestroyTexture(m->fondoConfig);
    if(m->fondoNombres)
        SDL_DestroyTexture(m->fondoNombres);
    if(m->fondoRanking)
        SDL_DestroyTexture(m->fondoRanking);
    if(m->fuenteTitulo)
        TTF_CloseFont(m->fuenteTitulo);
    if(m->fuenteOpciones)
        TTF_CloseFont(m->fuenteOpciones);
    if(m->nombre.fuenteGrande)
        TTF_CloseFont(m->nombre.fuenteGrande);
    if(m->nombre.fuenteChica)
        TTF_CloseFont(m->nombre.fuenteChica);
    if(m->nombre.fuenteMedia)
        TTF_CloseFont(m->nombre.fuenteMedia);
}

const char* menu_ObtenerTexto(int op)
{
    switch(op)
    {
        case OPCION_JUGAR:
            return "Jugar";
        case OPCION_ESTADISTICAS:
            return "Ranking";
        case OPCION_CONFIGURACION:
            return "Configuracion";
        case OPCION_SALIR:
            return "Salir";
        default:
            return "";
    }
}

int menu_ManejarOpciones(Menu* m, SDL_Event* e)
{
    if(m->confirmaSalida)
    {
        if(e->type==SDL_KEYDOWN)
        {
            if(e->key.keysym.sym==SDLK_LEFT || e->key.keysym.sym==SDLK_RIGHT)
                m->opcionSalida=!m->opcionSalida;
            else if(e->key.keysym.sym==SDLK_RETURN || e->key.keysym.sym==SDLK_KP_ENTER)
                return (m->opcionSalida==0)?OPCION_SALIR : (m->confirmaSalida=0, -1);
            else if(e->key.keysym.sym==SDLK_ESCAPE)
                m->confirmaSalida=0;
        }

        else if(e->type == SDL_MOUSEMOTION || (e->type==SDL_MOUSEBUTTONDOWN && e->button.button==SDL_BUTTON_LEFT))
        {
            int mx = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
            int my = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
            SDL_Rect rSi={(ANCHO_VENTANA/2)-80, (ALTO_VENTANA/2)+20, 50, 40};
            SDL_Rect rNo={(ANCHO_VENTANA/2)+40, (ALTO_VENTANA/2)+20, 50, 40};

            if(SDL_PointInRect(&(SDL_Point){mx,my},&rSi))
            {
                m->opcionSalida = 0;
                if(e->type==SDL_MOUSEBUTTONDOWN)
                    return OPCION_SALIR;
            }
            if(SDL_PointInRect(&(SDL_Point){mx,my},&rNo))
            {
                m->opcionSalida = 1;
                if(e->type==SDL_MOUSEBUTTONDOWN)
                    m->confirmaSalida=0;
            }
        }
        return -1;
    }

    if(e->type==SDL_KEYDOWN)
    {
        switch(e->key.keysym.sym)
        {
            case SDLK_UP:
                m->opcionSeleccionada--;
                if(m->opcionSeleccionada<0)
                    m->opcionSeleccionada=CANTIDAD_OPCIONES-1;
                break;
            case SDLK_DOWN:
                m->opcionSeleccionada++;
                if(m->opcionSeleccionada>=CANTIDAD_OPCIONES)
                    m->opcionSeleccionada=0;
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                if(m->opcionSeleccionada==OPCION_SALIR)
                {
                    m->confirmaSalida=1;
                    m->opcionSalida=1;
                    return -1;
                }
                return m->opcionSeleccionada;
            case SDLK_ESCAPE:
                m->confirmaSalida=1;
                m->opcionSalida=1;
                return -1;
        }
    }
    else if(e->type==SDL_MOUSEMOTION || e->type==SDL_MOUSEBUTTONDOWN)
    {
        int mx = (e->type==SDL_MOUSEMOTION)?e->motion.x:e->button.x;
        int my = (e->type==SDL_MOUSEMOTION)?e->motion.y:e->button.y;
        for(int i=0; i<CANTIDAD_OPCIONES; i++)
        {
            int w,h;
            TTF_SizeText(m->fuenteOpciones, menu_ObtenerTexto(i), &w, &h);
            SDL_Rect r={(ANCHO_VENTANA-w)/2, INICIOY+(i*SEPARACION), w, h};
            if(SDL_PointInRect(&(SDL_Point){mx,my}, &r))
            {
                m->opcionSeleccionada=i;
                if(e->type==SDL_MOUSEBUTTONDOWN && e->button.button==SDL_BUTTON_LEFT)
                {
                    if(i==OPCION_SALIR)
                    {
                        m->confirmaSalida=1;
                        m->opcionSalida=1;
                        return -1;
                    }
                    return i;
                }
            }
        }
    }
    return -1;
}

void menu_Dibujar(Menu* m, SDL_Renderer* renderer)
{
    if(m->fondoMenu)
        SDL_RenderCopy(renderer, m->fondoMenu, NULL, NULL);
    else
        SDL_SetRenderDrawColor(renderer,20,20,20,255);
    int w,h;
    SDL_QueryTexture(m->texturaTitulo, NULL, NULL, &w, &h);
    SDL_Rect r={(ANCHO_VENTANA-w)/2, 100, w, h};
    SDL_RenderCopy(renderer, m->texturaTitulo, NULL, &r);
    for(int i=0; i<CANTIDAD_OPCIONES; i++)
    {
        SDL_Color c = (i==m->opcionSeleccionada) ? COLOR_SELECCION : COLOR_GRIS;
        dibujarTextoCentrados(renderer, m->fuenteOpciones, menu_ObtenerTexto(i), INICIOY+(i*SEPARACION), c);
    }
    if(m->confirmaSalida)
        dibujarPopupSalida(renderer, m->fuenteOpciones, "Desea salir?", m->opcionSalida);
}

int menu_ConfiguracionOpciones(Menu* m, SDL_Event* e, ConfiguracionJuego* config)
{
    const int maxOp = 4;
    if(e->type==SDL_KEYDOWN)
    {
        switch(e->key.keysym.sym)
        {
            case SDLK_UP:
                m->opcionSeleccionada--;
                if(m->opcionSeleccionada<0)
                    m->opcionSeleccionada=maxOp-1;
                break;
            case SDLK_DOWN:
                m->opcionSeleccionada++;
                if(m->opcionSeleccionada>=maxOp)
                    m->opcionSeleccionada=0;
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
            case SDLK_RIGHT:
                if(m->opcionSeleccionada==0)
                {
                    if(config->filas==3)
                    {
                        config->filas=4;
                        config->columnas=4;
                    }
                    else if(config->filas==4 && config->columnas==4)
                    {
                        config->filas=4;
                        config->columnas=5;
                    }
                    else
                    {
                        config->filas=3;
                        config->columnas=4;
                    }
                }
                else if(m->opcionSeleccionada==1)
                    config->idSetImagenes = 1 - config->idSetImagenes;
                else if(m->opcionSeleccionada==2)
                    config->cantJugadores = (config->cantJugadores==1)?2:1;
                else if(m->opcionSeleccionada==3)
                    return 1;
                break;
            case SDLK_ESCAPE:
                return 1;
        }
    }
    else if(e->type==SDL_MOUSEMOTION || e->type==SDL_MOUSEBUTTONDOWN)
    {
        int mx = (e->type==SDL_MOUSEMOTION)?e->motion.x:e->button.x;
        int my = (e->type==SDL_MOUSEMOTION)?e->motion.y:e->button.y;
        char buf[100];
        int posYs[] = {CONF_Y_TABLERO, CONF_Y_SET, CONF_Y_JUGADORES, CONF_Y_GUARDAR};
        for(int i=0; i<maxOp; i++)
        {
            if(i==0)
                sprintf(buf,"Tablero: %dx%d (%d cartas)",config->filas, config->columnas, config->filas*config->columnas);
            else if(i==1)
                sprintf(buf,"Set Grafico: %s",(config->idSetImagenes==0)?"SET A":"SET B");
            else if(i==2)
                sprintf(buf,"Jugadores: %d",config->cantJugadores);
            else
                sprintf(buf,"GUARDAR Y VOLVER");
            int w,h;
            TTF_SizeText(m->fuenteOpciones, buf, &w, &h);
            SDL_Rect r={(ANCHO_VENTANA-w)/2, posYs[i], w, h};
            if(SDL_PointInRect(&(SDL_Point){mx,my}, &r))
            {
                m->opcionSeleccionada = i;
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                {
                    if(i==0)
                    {
                        if(config->filas==3)
                        {
                            config->filas=4;
                            config->columnas=4;
                        }
                        else if(config->filas==4 && config->columnas==4)
                        {
                            config->filas=4;
                            config->columnas=5;
                        }
                        else
                        {
                            config->filas=3;
                            config->columnas=4;
                        }
                    }
                    else if(i==1)
                        config->idSetImagenes = 1 - config->idSetImagenes;
                    else if(i==2)
                        config->cantJugadores = (config->cantJugadores==1)?2:1;
                    else
                        return 1;
                }
            }
        }
    }
    return 0;
}

void menu_ConfiguracionDibujar(Menu* m, SDL_Renderer* renderer, ConfiguracionJuego* config)
{
    if(m->fondoConfig)
        SDL_RenderCopy(renderer, m->fondoConfig, NULL, NULL);
    else
        SDL_SetRenderDrawColor(renderer,20,20,20,255);
    dibujarTextoCentrados(renderer, m->fuenteTitulo, "CONFIGURACION", 50, COLOR_TITULO);
    char buf[100];
    SDL_Color c;

    c = (m->opcionSeleccionada==0)?COLOR_SELECCION:COLOR_NORMAL;
    sprintf(buf,"Tablero: %dx%d (%d cartas)",config->filas,config->columnas,config->filas*config->columnas);
    dibujarTextoCentrados(renderer, m->fuenteOpciones, buf, CONF_Y_TABLERO, c);
    c = (m->opcionSeleccionada==1)?COLOR_SELECCION:COLOR_NORMAL;
    sprintf(buf,"Set Grafico: %s",(config->idSetImagenes==0)?"SET A":"SET B");
    dibujarTextoCentrados(renderer, m->fuenteOpciones, buf, CONF_Y_SET, c);
    c = (m->opcionSeleccionada==2)?COLOR_SELECCION:COLOR_NORMAL;
    sprintf(buf,"Jugadores: %d",config->cantJugadores);
    dibujarTextoCentrados(renderer, m->fuenteOpciones, buf, CONF_Y_JUGADORES, c);
    c = (m->opcionSeleccionada==3)?COLOR_SELECCION:COLOR_NORMAL;
    dibujarTextoCentrados(renderer, m->fuenteOpciones, "GUARDAR Y VOLVER", CONF_Y_GUARDAR, c);
    dibujarTextoCentrados(renderer, m->nombre.fuenteChica, "Clic o Enter para cambiar", 650, COLOR_TITULO);
}

void config_Cargar(ConfiguracionJuego* config)
{
    FILE* f = fopen(ARCHIVO_CONFIG, "rb");
    if(!f)
    {
        config->filas=DEFAULT_FILAS;
        config->columnas=DEFAULT_COLUMNAS;
        config->idSetImagenes=DEFAULT_SET;
        config->cantJugadores=DEFAULT_JUGADORES;
    }
    else
    {
        fread(config, sizeof(ConfiguracionJuego), 1, f);
        fclose(f);
    }
}

void config_Guardar(ConfiguracionJuego* config)
{
    FILE* f = fopen(ARCHIVO_CONFIG, "wb");
    if(f)
    {
        fwrite(config, sizeof(ConfiguracionJuego), 1, f);
        fclose(f);
    }
}

char nextChar(char c)
{
    if(c=='Z')
        return '0';
    if(c=='9')
        return 'A';
    return c+1;
}

char prevChar(char c)
{
    if(c=='A')
        return '9';
    if(c=='0')
        return 'Z';
    return c-1;
}

void ingreso_NombreIniciar(IngresoNombre* ing, const char* titulo)
{
    ing->cursor=0;
    strcpy(ing->nombre,"AAA");
    strcpy(ing->titulo, titulo);
}

int ingreso_NombreOpciones(IngresoNombre* ing, SDL_Event* e, ContextoJuego* juego, int idx)
{
    if(e->type==SDL_KEYDOWN)
    {
        switch(e->key.keysym.sym)
        {
            case SDLK_LEFT:
                ing->cursor--;
                if(ing->cursor<0)
                    ing->cursor=4;
                break;
            case SDLK_RIGHT:
                ing->cursor++;
                if(ing->cursor>4)
                    ing->cursor=0;
                break;
            case SDLK_UP:
                if(ing->cursor<3)
                    ing->nombre[ing->cursor] = nextChar(ing->nombre[ing->cursor]);
                break;
            case SDLK_DOWN:
                if(ing->cursor<3)
                    ing->nombre[ing->cursor] = prevChar(ing->nombre[ing->cursor]);
                break;
            case SDLK_RETURN:
                if(ing->cursor==3)
                {
                    strcpy(juego->nombreJugador[idx], ing->nombre);
                    return 1;
                }
                if(ing->cursor==4)
                    return 2;
                break;
            case SDLK_ESCAPE:
                return 2;
        }
    }
    else if(e->type == SDL_MOUSEWHEEL && ing->cursor < 3)
    {
        if(e->wheel.y > 0)
            ing->nombre[ing->cursor] = nextChar(ing->nombre[ing->cursor]);
        else if(e->wheel.y < 0)
            ing->nombre[ing->cursor] = prevChar(ing->nombre[ing->cursor]);
    }
    else if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
        int my = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
        SDL_Point p = {mx, my};

        int inicio = (ANCHO_VENTANA / 2) - 100;
        for(int i=0; i<3; i++)
        {
            SDL_Rect r = {inicio + (i*SEPARACION), INICIOY, 50, 80};
            if(SDL_PointInRect(&p, &r))
            {
                ing->cursor = i;
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                    ing->nombre[i] = nextChar(ing->nombre[i]);
            }
        }
        int wC, hC;
        TTF_SizeText(ing->fuenteMedia, "CONFIRMAR", &wC, &hC);
        SDL_Rect rC = {(ANCHO_VENTANA-wC)/2, 500, wC, hC};
        if(SDL_PointInRect(&p, &rC))
        {
            ing->cursor = 3;
            if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
            {
                strcpy(juego->nombreJugador[idx], ing->nombre);
                return 1;
            }
        }
        int wV, hV;
        TTF_SizeText(ing->fuenteMedia, "VOLVER", &wV, &hV);
        SDL_Rect rV = {(ANCHO_VENTANA-wV)/2, 580, wV, hV};
        if(SDL_PointInRect(&p, &rV))
        {
            ing->cursor = 4;
            if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                return 2;
        }
    }
    return 0;
}

void ingreso_NombreDibujar(IngresoNombre* ing, SDL_Renderer* renderer, SDL_Texture* fondo)
{
    if(fondo)
        SDL_RenderCopy(renderer, fondo, NULL, NULL);
    else
        SDL_SetRenderDrawColor(renderer,20,20,20,255);
    dibujarTextoCentrados(renderer, ing->fuenteGrande, ing->titulo, 100, COLOR_TITULO);
    int inicio = (ANCHO_VENTANA/2)-100;
    for(int i=0;i<3;i++)
    {
        char l[2]={ing->nombre[i],0};
        SDL_Color c=(i==ing->cursor)?COLOR_SELECCION:COLOR_NORMAL;
        dibujarTexto(renderer, ing->fuenteGrande, l, inicio+(i*SEPARACION), INICIOY, c);
    }
    dibujarTextoCentrados(renderer, ing->fuenteMedia, "CONFIRMAR", 500, (ing->cursor==3)?COLOR_SELECCION:COLOR_NORMAL);
    dibujarTextoCentrados(renderer, ing->fuenteMedia, "VOLVER", 580, (ing->cursor==4)?COLOR_SELECCION:COLOR_NORMAL);
    dibujarTextoCentrados(renderer, ing->fuenteChica, "Flechas/Rueda: Cambiar. Enter: Seleccionar.", 650, COLOR_TITULO);
}
