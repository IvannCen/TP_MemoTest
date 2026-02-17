#include "Menu.h"
#include "Tablero.h"

void menuIniciar(Menu* m, SDL_Renderer* renderer)
{
    m->opcionSeleccionada = 0; //arranca desde la primer opcion del menu
    m->confirmaSalida = 0; //arranca desactivado
    m->opcionSalida = 1; //por defecto, arranca en no

    //cargo las fuentes y ajusto los tamanios como quiera
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
    ingresoNombreIniciar(&m->nombre, "INGRESE NOMBRE");
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

    if(m->nombre.fuenteMedia)
        TTF_CloseFont(m->nombre.fuenteMedia);
}

int menuManejarOpciones(Menu* m, SDL_Event* e)
{
    if (m->confirmaSalida)
    {
        if(e->type == SDL_KEYDOWN)
        {
            if(e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_RIGHT)
                m->opcionSalida = !m->opcionSalida;
            else if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_KP_ENTER)
                return (m->opcionSalida == 0) ? OPCION_SALIR : (m->confirmaSalida = 0, -1);
            else if(e->key.keysym.sym == SDLK_ESCAPE)
                m->confirmaSalida = 0;
        }
        else if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
        {
            int mx = e->button.x, my = e->button.y;
            SDL_Point p = {mx, my};
            SDL_Rect rectSi = { (ANCHOVENTANA/2) - 80, (ALTOVENTANA/2) + 20, 50, 40 };
            SDL_Rect rectNo = { (ANCHOVENTANA/2) + 40, (ALTOVENTANA/2) + 20, 50, 40 };

            if(SDL_PointInRect(&p, &rectSi))
                return OPCION_SALIR;

            if(SDL_PointInRect(&p, &rectNo))
                m->confirmaSalida = 0;
        }
        return -1;
    }

    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
            m->opcionSeleccionada--;
            if(m->opcionSeleccionada < 0)
                m->opcionSeleccionada = CANTIDADOPCIONES - 1;
            break;

        case SDLK_DOWN:
            m->opcionSeleccionada++;
            if(m->opcionSeleccionada >= CANTIDADOPCIONES)
                m->opcionSeleccionada = 0;
            break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            if (m->opcionSeleccionada == OPCION_SALIR)
            {
                m->confirmaSalida = 1;
                m->opcionSalida = 1;
                return -1;
            }
            return m->opcionSeleccionada;

        case SDLK_ESCAPE:
            m->confirmaSalida = 1;
            m->opcionSalida = 1;
            return -1;
        }
    }
    else if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
        int my = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
        SDL_Point p = {mx, my};

        for(int i = 0; i < CANTIDADOPCIONES; i++)
        {
            int w, h;
            TTF_SizeText(m->fuenteOpciones, menuObtenerTexto(i), &w, &h);
            SDL_Rect rect = {(ANCHOVENTANA - w) / 2, INICIOY + (i * SEPARACION), w, h};

            if(SDL_PointInRect(&p, &rect))
            {
                m->opcionSeleccionada = i;
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                {
                    if (i == OPCION_SALIR)
                    {
                        m->confirmaSalida = 1;
                        m->opcionSalida = 1;
                        return -1;
                    }
                    return i;
                }
            }
        }
    }
    return -1;
}

void menuDibujar(Menu* m, SDL_Renderer* renderer)
{
    int w,h;
    SDL_QueryTexture(m->texturaTitulo, NULL, NULL, &w, &h);
    SDL_Rect recTitulo = {(ANCHOVENTANA - w)/2, 100, w, h};
    SDL_RenderCopy(renderer, m->texturaTitulo, NULL, &recTitulo);

    for(int i=0; i<CANTIDADOPCIONES; i++)
    {
        SDL_Color color = (i == m->opcionSeleccionada) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
        dibujarTextoCentrados(renderer, m->fuenteOpciones, menuObtenerTexto(i), INICIOY + (i * SEPARACION), color);
    }

    if (m->confirmaSalida)
    {
        dibujarPopupSalida(renderer, m->fuenteOpciones, "Desea salir?", m->opcionSalida);
    }
}

int menuConfiguracionOpciones(Menu* m, SDL_Event* e, Configuracion* config)
{
    const int maxOpciones = 4; // 0:Tablero, 1:Set, 2:Jugadores, 3:Guardar

    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
            m->opcionSeleccionada--;
            if(m->opcionSeleccionada < 0)
                m->opcionSeleccionada = maxOpciones - 1;
            break;

        case SDLK_DOWN:
            m->opcionSeleccionada++;
            if(m->opcionSeleccionada >= maxOpciones) m->opcionSeleccionada = 0;
            break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
        case SDLK_RIGHT:
            if(m->opcionSeleccionada == 0)
            {
                if(config->filas == 3 && config->columnas == 4)
                {
                    config->filas=4;
                    config->columnas=4;
                }
                else if(config->filas == 4 && config->columnas == 4)
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
            else if(m->opcionSeleccionada == 1)
                config->idSetImagenes = 1 - config->idSetImagenes;
            else if(m->opcionSeleccionada == 2)
                config->cantJugadores = (config->cantJugadores == 1) ? 2 : 1;
            else if(m->opcionSeleccionada == 3)
                return 1;
            break;

        case SDLK_ESCAPE:
            return 1;
        }
    }
    else if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
        int my = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
        SDL_Point p = {mx, my};

        char buffer[100];
        // Revisamos cada boton con su coordenada Y exacta
        for(int i=0; i<maxOpciones; i++)
        {
            int posY = 0;
            if(i==0)
            {
                sprintf(buffer, "Tablero: %d x %d (%d cartas)",config->filas, config->columnas, config->filas*config->columnas);
                posY = CONF_Y_TABLERO;
            }
            else if(i==1)
            {
                sprintf(buffer, "Set Grafico: %s",(config->idSetImagenes == 0) ? "SET A (Clasico)" : "SET B (Alternativo)");
                posY = CONF_Y_SET;
            }
            else if(i==2)
            {
                sprintf(buffer, "Jugadores: %d",config->cantJugadores);
                posY = CONF_Y_JUGADORES;
            }
            else if(i==3)
            {
                sprintf(buffer, "GUARDAR Y VOLVER");
                posY = CONF_Y_GUARDAR;
            }

            int w, h;
            TTF_SizeText(m->fuenteOpciones, buffer, &w, &h);
            SDL_Rect rect = {(ANCHOVENTANA - w) / 2, posY, w, h};

            if(SDL_PointInRect(&p, &rect))
            {
                m->opcionSeleccionada = i;
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                {
                    if(i == 0)
                    {
                        if(config->filas == 3 && config->columnas == 4)
                        {
                            config->filas=4;
                            config->columnas=4;
                        }
                        else if(config->filas == 4 && config->columnas == 4)
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
                    else if(i == 1)
                        config->idSetImagenes = 1 - config->idSetImagenes;
                    else if(i == 2)
                        config->cantJugadores = (config->cantJugadores == 1) ? 2 : 1;
                    else if(i == 3)
                        return 1;
                }
            }
        }
    }
    return 0;
}

void menuConfiguracionDibujar(Menu* m, SDL_Renderer* renderer, Configuracion* config)
{
    SDL_Color oro = {255,215,0};
    dibujarTextoCentrados(renderer, m->fuenteTitulo, "CONFIGURACION", 50, oro);

    char buffer[100];
    SDL_Color color;

    // OPCION 0: TABLERO
    color = (m->opcionSeleccionada == 0) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
    sprintf(buffer, "Tablero: %d x %d (%d cartas)",config->filas, config->columnas, config->filas*config->columnas);
    dibujarTextoCentrados(renderer,m->fuenteOpciones,buffer, CONF_Y_TABLERO, color);

    // OPCION 1: SET
    color = (m->opcionSeleccionada == 1) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
    sprintf(buffer, "Set Grafico: %s",(config->idSetImagenes == 0) ? "SET A (Clasico)" : "SET B (Alternativo)");
    dibujarTextoCentrados(renderer,m->fuenteOpciones,buffer, CONF_Y_SET, color);

    // OPCION 2: JUGADORES
    color = (m->opcionSeleccionada == 2) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
    sprintf(buffer, "Jugadores: %d",config->cantJugadores);
    dibujarTextoCentrados(renderer,m->fuenteOpciones,buffer, CONF_Y_JUGADORES, color);

    // OPCION 3: GUARDAR
    color = (m->opcionSeleccionada == 3) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
    dibujarTextoCentrados(renderer,m->fuenteOpciones,"GUARDAR Y VOLVER", CONF_Y_GUARDAR, color);
    dibujarTextoCentrados(renderer,m->nombre.fuenteChica, "Clic o Enter para cambiar valores",650,oro);
}

void cargarConfiguracion(Configuracion* config)
{
    FILE* arch = fopen(ARCHIVOCONFIG, "rb");
    if(!arch)
    {
        printf("No existe un config.dat. Usando los valores por defecto...\n");
        config->filas = FILAS;
        config->columnas = COLUMNAS;
        config->idSetImagenes = SETA;
        config->cantJugadores = CANTJUGADORES;
    }
    else
    {
        fread(config, sizeof(Configuracion),1,arch);
        fclose(arch);
        printf("Configuracion cargada con exito...\n");
    }
}

void guardarConfiguracion(Configuracion* config)
{
    FILE* arch = fopen(ARCHIVOCONFIG, "wb");
    if(!arch)
    {
        printf("No se pudo guardar el archivo de configuracion...\n");
    }
    else
    {
        fwrite(config,sizeof(Configuracion),1,arch);
        fclose(arch);
        printf("Configuracion guardada en %s...\n", ARCHIVOCONFIG);
    }
}

//ingreso del nombre en el juego

void ingresoNombreIniciar(IngresoNombre* ing, const char* titulo)
{
    ing->cursor = 0;
    strcpy(ing->nombre,"AAA");
    strcpy(ing->titulo, titulo);
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

//funncion auxiliar para conectar enum con texto
const char* menuObtenerTexto(int opcion)
{
    switch(opcion)
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
        return "???"; //opcion desconocida, no es valida
    }
}

int ingresoNombreOpciones(IngresoNombre* ing, SDL_Event* e, ContextoJuego* juego, int indiceJugador)
{
    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
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
                ing->nombre[ing->cursor] = siguienteCaracter(ing->nombre[ing->cursor]);
            break;

        case SDLK_DOWN:
            if(ing->cursor<3)
                ing->nombre[ing->cursor] = anteriorCaracter(ing->nombre[ing->cursor]);
            break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            if(ing->cursor == 3)
            {
                strcpy(juego->nombreJugador[indiceJugador], ing->nombre);
                return 1;
            }
            if(ing->cursor == 4)
                return 2; // Volver
            break;

        case SDLK_ESCAPE:
            return 2;
        }
    }
    else if(e->type == SDL_MOUSEWHEEL)
    {
        if(ing->cursor < 3)
        {
            if(e->wheel.y > 0)
                ing->nombre[ing->cursor] = siguienteCaracter(ing->nombre[ing->cursor]);
            else if(e->wheel.y < 0)
                ing->nombre[ing->cursor] = anteriorCaracter(ing->nombre[ing->cursor]);
        }
    }
    else if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
        int my = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
        SDL_Point p = {mx, my};

        int inicio = (ANCHOVENTANA / 2) - 100;

        for(int i=0; i<3; i++)
        {
            SDL_Rect r = {inicio + (i*SEPARACION), INICIOY, 50, 80};
            if(SDL_PointInRect(&p, &r))
            {
                ing->cursor = i;
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                    ing->nombre[i] = siguienteCaracter(ing->nombre[i]);
            }
        }

        int wC, hC;
        TTF_SizeText(ing->fuenteMedia, "CONFIRMAR", &wC, &hC);
        SDL_Rect rC = {(ANCHOVENTANA-wC)/2, 500, wC, hC};

        if(SDL_PointInRect(&p, &rC))
        {
            ing->cursor = 3;
            if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
            {
                strcpy(juego->nombreJugador[indiceJugador], ing->nombre);
                return 1;
            }
        }

        int wV, hV;
        TTF_SizeText(ing->fuenteMedia, "VOLVER", &wV, &hV);
        SDL_Rect rV = {(ANCHOVENTANA-wV)/2, 580, wV, hV};

        if(SDL_PointInRect(&p, &rV))
        {
            ing->cursor = 4;
            if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) return 2;
        }
    }
    return 0;
}

void ingresoNombreDibujar(IngresoNombre* ing, SDL_Renderer* renderer)
{
    SDL_Color oro = {255,215,0};
    dibujarTextoCentrados(renderer,ing->fuenteGrande,ing->titulo,100,oro);

    int inicio = (ANCHOVENTANA / 2) - 100;

    for(int i=0; i<3; i++)
    {
        char letra[2] = {ing->nombre[i], '\0'};
        SDL_Color color = (i==ing->cursor) ? (SDL_Color){0,255,0} : (SDL_Color){255,255,255};
        dibujarTexto(renderer,ing->fuenteGrande,letra,inicio+(i*SEPARACION),INICIOY,color);
    }

    SDL_Color colConf = (ing->cursor == 3) ? (SDL_Color){0,255,0} : (SDL_Color){100,100,100};
    dibujarTextoCentrados(renderer, ing->fuenteMedia, "CONFIRMAR", 500, colConf);

    SDL_Color colVol = (ing->cursor == 4) ? (SDL_Color){255,0,0} : (SDL_Color){100,100,100};
    dibujarTextoCentrados(renderer, ing->fuenteMedia, "VOLVER", 580, colVol);

    dibujarTextoCentrados(renderer,ing->fuenteChica,"Flechas/Rueda: Cambiar. Enter: Seleccionar.",650,oro);
}
