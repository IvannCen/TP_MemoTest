#include "Menu.h"
#include "Tablero.h"

// --- MACROS DE CONFIGURACION VISUAL ---
// Definimos las posiciones aqui para usarlas tanto en Dibujar como en ManejarOpciones
#define INICIOY      300 //anteriormente int inicioY = 300 en menuDibujar (altura donde esta la primer opcion para tener referencia)
#define SEPARACION        70  //anteriormente int separacion = 70 en menuDibujar (espacio entre opciones)

// Macros para el Popup de Salida
#define POPUP_W         400
#define POPUP_H         200
#define POPUP_X         ((ANCHOVENTANA - POPUP_W) / 2)
#define POPUP_Y         ((ALTOVENTANA - POPUP_H) / 2)

#define CONF_Y_TABLERO   200
#define CONF_Y_SET       280
#define CONF_Y_JUGADORES 360
#define CONF_Y_GUARDAR   480

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

    if(m->nombre.fuenteMedia)
        TTF_CloseFont(m->nombre.fuenteMedia);
}

int menuManejarOpciones(Menu* m, SDL_Event* e)
{
//    // MANEJO POR TECLADO
//    if(e->type == SDL_KEYDOWN)
//    {
//        // CASO 1: ESTAMOS DENTRO DEL POPUP DE CONFIRMACION
//        if (m->confirmaSalida)
//        {
//            switch (e->key.keysym.sym)
//            {
//                case SDLK_LEFT:
//                case SDLK_RIGHT:
//                    //uso ! para invertir el valor (0 pasa a 1, 1 pasa a 0)
//                    m->opcionSalida = !m->opcionSalida;
//                    break;
//
//                case SDLK_RETURN: //enter normal
//                case SDLK_KP_ENTER: //enter del pad numerico
//                    if (m->opcionSalida == 0) //si eligio "SI" (asumimos 0 = SI)
//                    {
//                        //aca si devolvemos la senial para que el main cierre el juego
//                        return OPCION_SALIR;
//                    }
//                    else //si eligio "NO"
//                    {
//                        m->confirmaSalida = 0; //apagamos el popup
//                        //no devolvemos nada, seguimos en el menu
//                    }
//                    break;
//
//                case SDLK_ESCAPE:
//                    //si aprieta escape, cancelamos la salida tambien
//                    m->confirmaSalida = 0;
//                    break;
//            }
//            return -1; //retornamos -1 para que el juego siga corriendo
//        }
//
//        // CASO 2: ESTAMOS EN EL MENU NORMAL (Navegacion Vertical)
//        switch (e->key.keysym.sym)
//        {
//        case SDLK_UP:
//            m->opcionSeleccionada--;
//            if(m->opcionSeleccionada < 0)
//                m->opcionSeleccionada = CANTIDADOPCIONES - 1;
//            break;
//
//        case SDLK_DOWN:
//            m->opcionSeleccionada++;
//            if(m->opcionSeleccionada >= CANTIDADOPCIONES)
//                m->opcionSeleccionada = 0;
//            break;
//
//        case SDLK_RETURN:
//        case SDLK_KP_ENTER:
//            //revisamos que opcion se eligio
//            if (m->opcionSeleccionada == OPCION_SALIR)
//            {
//                // no salimos todavia! activamos el modo confirmacion
//                m->confirmaSalida = 1;
//                m->opcionSalida = 1; //ponemos el cursor en "NO" por seguridad
//                return -1; //devolvemos -1 para que el main espere
//            }
//
//            //si es cualquier otra opcion (Aventura, Config, etc), la devolvemos normal
//            return m->opcionSeleccionada;
//        }
//    }
//
//    // MANEJO POR MOUSE
//    else if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
//    {
//        // 1. Obtenemos coordenadas del mouse unificadas
//        int mouseX = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
//        int mouseY = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
//        SDL_Point p = {mouseX, mouseY};
//
//        // CASO 1: POPUP ACTIVO (Detectar mouse sobre SI o NO)
//        if (m->confirmaSalida)
//        {
//            int wSi, hSi, wNo, hNo;
//            // Medimos los textos. Nota: Asegurate que 'fuenteOpciones' sea la misma que usas en menuDibujar
//            TTF_SizeText(m->fuenteOpciones, "SI", &wSi, &hSi);
//            TTF_SizeText(m->fuenteOpciones, "NO", &wNo, &hNo);
//
//            // Definimos las cajas (mismas coordenadas que en menuDibujar)
//            SDL_Rect rectSi = { (ANCHOVENTANA/2) - 80, (ALTOVENTANA/2) + 20, wSi, hSi };
//            SDL_Rect rectNo = { (ANCHOVENTANA/2) + 40, (ALTOVENTANA/2) + 20, wNo, hNo };
//
//            // Chequeamos colision con "SI"
//            if(SDL_PointInRect(&p, &rectSi))
//            {
//                m->opcionSalida = 0; // 0 = SI
//                // Si fue clic, confirmamos salida
//                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
//                {
//                    return OPCION_SALIR;
//                }
//            }
//            // Chequeamos colision con "NO"
//            else if(SDL_PointInRect(&p, &rectNo))
//            {
//                m->opcionSalida = 1; // 1 = NO
//                // Si fue clic, cancelamos popup
//                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
//                {
//                    m->confirmaSalida = 0;
//                }
//            }
//        }
//        // CASO 2: MENU NORMAL (Detectar mouse sobre opciones)
//        else
//        {
//            for(int i = 0; i < CANTIDADOPCIONES; i++)
//            {
//                int w, h;
//                const char* texto = menuObtenerTexto(i);
//
//                TTF_SizeText(m->fuenteOpciones, texto, &w, &h);
//
//                SDL_Rect rect;
//                rect.x = (ANCHOVENTANA - w) / 2;
//                rect.y = INICIOY + (i * SEPARACION);
//                rect.w = w;
//                rect.h = h;
//
//                // Si el mouse esta encima del texto
//                if(SDL_PointInRect(&p, &rect))
//                {
//                    m->opcionSeleccionada = i; // Efecto visual (cambia de color)
//
//                    // Si hizo clic
//                    if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
//                    {
//                        if (i == OPCION_SALIR)
//                        {
//                            // Mismo comportamiento que el teclado: Activar Popup
//                            m->confirmaSalida = 1;
//                            m->opcionSalida = 1;
//                            return -1;
//                        }
//                        // Cualquier otra opcion, la retornamos
//                        return i;
//                    }
//                }
//            }
//        }
//    }
//
//    return -1; // Si no se apret� nada relevante

    ///////////////////////////////////////////////////////////////////////////////////////////////////////

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

            if(SDL_PointInRect(&p, &rectSi)) return OPCION_SALIR;
            if(SDL_PointInRect(&p, &rectNo)) m->confirmaSalida = 0;
        }
        return -1;
    }

    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
            case SDLK_UP: m->opcionSeleccionada--; if(m->opcionSeleccionada < 0) m->opcionSeleccionada = CANTIDADOPCIONES - 1; break;
            case SDLK_DOWN: m->opcionSeleccionada++; if(m->opcionSeleccionada >= CANTIDADOPCIONES) m->opcionSeleccionada = 0; break;
            case SDLK_RETURN: case SDLK_KP_ENTER:
                if (m->opcionSeleccionada == OPCION_SALIR) { m->confirmaSalida = 1; m->opcionSalida = 1; return -1; }
                return m->opcionSeleccionada;
            case SDLK_ESCAPE: m->confirmaSalida = 1; m->opcionSalida = 1; return -1;
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
                    if (i == OPCION_SALIR) { m->confirmaSalida = 1; m->opcionSalida = 1; return -1; }
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

    for(int i=0;i<CANTIDADOPCIONES;i++)
    {
        SDL_Color color = (i == m->opcionSeleccionada) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
        dibujarTextoCentrados(renderer, m->fuenteOpciones, menuObtenerTexto(i), INICIOY + (i * SEPARACION), color);
    }

    if (m->confirmaSalida)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect overlay = {0, 0, ANCHOVENTANA, ALTOVENTANA};
        SDL_RenderFillRect(renderer, &overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        SDL_Rect caja = {POPUP_X, POPUP_Y, POPUP_W, POPUP_H};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &caja);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &caja);

        dibujarTextoCentrados(renderer, m->fuenteOpciones, "Desea salir?", (ALTOVENTANA/2)-40, (SDL_Color){255,255,255});
        SDL_Color colorSi = (m->opcionSalida == 0) ? (SDL_Color){0, 255, 0} : (SDL_Color){150, 150, 150};
        SDL_Color colorNo = (m->opcionSalida == 1) ? (SDL_Color){255, 0, 0} : (SDL_Color){150, 150, 150};
        dibujarTexto(renderer, m->fuenteOpciones, "SI", (ANCHOVENTANA/2) - 80, (ALTOVENTANA/2) + 20, colorSi);
        dibujarTexto(renderer, m->fuenteOpciones, "NO", (ANCHOVENTANA/2) + 40, (ALTOVENTANA/2) + 20, colorNo);
    }
}

int obtenerYConfig(int opcion)
{
    int inicioY = 200;
    int sep = 80;
    if(opcion == 2)
        return inicioY + (int)(sep*3.5);
    return inicioY + (opcion * sep);
}

int menuConfiguracionOpciones(Menu* m, SDL_Event* e, Configuracion* config)
{
    const int maxOpciones = 4; // 0:Tablero, 1:Set, 2:Jugadores, 3:Guardar

    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym) {
            case SDLK_UP: m->opcionSeleccionada--; if(m->opcionSeleccionada < 0) m->opcionSeleccionada = maxOpciones - 1; break;
            case SDLK_DOWN: m->opcionSeleccionada++; if(m->opcionSeleccionada >= maxOpciones) m->opcionSeleccionada = 0; break;
            case SDLK_RETURN: case SDLK_KP_ENTER: case SDLK_RIGHT:
                if(m->opcionSeleccionada == 0) {
                    if(config->filas == 3 && config->columnas == 4) { config->filas=4; config->columnas=4; }
                    else if(config->filas == 4 && config->columnas == 4) { config->filas=4; config->columnas=5; }
                    else { config->filas=3; config->columnas=4; }
                }
                else if(m->opcionSeleccionada == 1) config->idSetImagenes = 1 - config->idSetImagenes;
                else if(m->opcionSeleccionada == 2) config->cantJugadores = (config->cantJugadores == 1) ? 2 : 1;
                else if(m->opcionSeleccionada == 3) return 1;
                break;
            case SDLK_ESCAPE: return 1;
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
            if(i==0) { sprintf(buffer, "Tablero: %d x %d (%d cartas)",config->filas, config->columnas, config->filas*config->columnas); posY = CONF_Y_TABLERO; }
            else if(i==1) { sprintf(buffer, "Set Grafico: %s",(config->idSetImagenes == 0) ? "SET A (Clasico)" : "SET B (Alternativo)"); posY = CONF_Y_SET; }
            else if(i==2) { sprintf(buffer, "Jugadores: %d",config->cantJugadores); posY = CONF_Y_JUGADORES; }
            else if(i==3) { sprintf(buffer, "GUARDAR Y VOLVER"); posY = CONF_Y_GUARDAR; }

            int w, h;
            TTF_SizeText(m->fuenteOpciones, buffer, &w, &h);
            SDL_Rect rect = {(ANCHOVENTANA - w) / 2, posY, w, h};

            if(SDL_PointInRect(&p, &rect))
            {
                m->opcionSeleccionada = i;
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                {
                    if(i == 0) {
                        if(config->filas == 3 && config->columnas == 4) { config->filas=4; config->columnas=4; }
                        else if(config->filas == 4 && config->columnas == 4) { config->filas=4; config->columnas=5; }
                        else { config->filas=3; config->columnas=4; }
                    }
                    else if(i == 1) config->idSetImagenes = 1 - config->idSetImagenes;
                    else if(i == 2) config->cantJugadores = (config->cantJugadores == 1) ? 2 : 1;
                    else if(i == 3) return 1;
                }
            }
        }
    }
    return 0;
}

void menuConfiguracionDibujar(Menu* m, SDL_Renderer* renderer, Configuracion* config)
{
    SDL_Color oro = {255,215,0};
    dibujarTextoCentrados(renderer,m->fuenteTitulo,"CONFIGURACION",50,oro);

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

//funncion auxiliar para conectar enum con texto
const char* menuObtenerTexto(int opcion)
{
    switch(opcion)
    {
        case OPCION_AVENTURA:
            return "Aventura";
        case OPCION_COMPETITIVO:
            return "Competitivo";
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

////Ya no hay dificultades, ver eso
//const char* menuDificultadObtenerTexto(int opcion)
//{
//    switch(opcion)
//    {
//        case FACIL:     return "FACIL";
//        case NORMAL:    return "NORMAL";
//        case DIFICIL:   return "DIFICIL";
//        case VOLVER:    return "VOLVER";
//        default:        return "???";
//    }
//}

int ingresoNombreOpciones(IngresoNombre* ing, SDL_Event* e, ContextoJuego* juego)
{
//    if(e->type == SDL_KEYDOWN)
//    {
//        switch (e->key.keysym.sym)
//        {
//            case SDLK_LEFT:
//                ing->cursor--;
//                if(ing->cursor<0)
//                    ing->cursor=4; //es el boton de confirmar
//                break;
//
//            case SDLK_RIGHT:
//                ing->cursor++;
//                if(ing->cursor>4)
//                    ing->cursor=0;
//                break;
//
//            case SDLK_UP:
//                if(ing->cursor<3)
//                    ing->nombre[ing->cursor] = siguienteCaracter((ing->nombre[ing->cursor]));
//                break;
//
//            case SDLK_DOWN:
//                if(ing->cursor<3)
//                    ing->nombre[ing->cursor] = anteriorCaracter((ing->nombre[ing->cursor]));
//                break;
//
//            case SDLK_RETURN:
//            case SDLK_KP_ENTER:
//                if(ing->cursor == 3)
//                {
//                    strcpy(juego->nombreJugador, ing->nombre);
//                    return 1; //1=confirmado
//                }
//                if(ing->cursor == 4)
//                    return 2; //volver
//                break;
//
//            case SDLK_ESCAPE:
//                return 2;
//                break;
//        }
//    }
//    else if(e->type == SDL_MOUSEWHEEL)
//    {
//        if(ing->cursor<3)
//        {
//            if(e->wheel.y>0)
//                ing->nombre[ing->cursor] = siguienteCaracter(ing->nombre[ing->cursor]);
//            else if(e->wheel.y<0)
//                ing->nombre[ing->cursor] = anteriorCaracter(ing->nombre[ing->cursor]);
//        }
//    }
//    return 0; //0 = seguir adelante

    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT: ing->cursor--; if(ing->cursor<0) ing->cursor=4; break;
            case SDLK_RIGHT: ing->cursor++; if(ing->cursor>4) ing->cursor=0; break;
            case SDLK_UP: if(ing->cursor<3) ing->nombre[ing->cursor] = siguienteCaracter(ing->nombre[ing->cursor]); break;
            case SDLK_DOWN: if(ing->cursor<3) ing->nombre[ing->cursor] = anteriorCaracter(ing->nombre[ing->cursor]); break;
            case SDLK_RETURN: case SDLK_KP_ENTER:
                if(ing->cursor == 3) { strcpy(juego->nombreJugador, ing->nombre); return 1; }
                if(ing->cursor == 4) return 2; // Volver
                break;
            case SDLK_ESCAPE: return 2;
        }
    }
    else if(e->type == SDL_MOUSEWHEEL)
    {
        if(ing->cursor < 3) {
            if(e->wheel.y > 0) ing->nombre[ing->cursor] = siguienteCaracter(ing->nombre[ing->cursor]);
            else if(e->wheel.y < 0) ing->nombre[ing->cursor] = anteriorCaracter(ing->nombre[ing->cursor]);
        }
    }
    else if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
        int my = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
        SDL_Point p = {mx, my};

        int inicio = (ANCHOVENTANA / 2) - 100;
        int inicioY = 300;
        int separacion = 70;

        for(int i=0; i<3; i++) {
            SDL_Rect r = {inicio + (i*separacion), inicioY, 50, 80};
            if(SDL_PointInRect(&p, &r)) {
                ing->cursor = i;
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                    ing->nombre[i] = siguienteCaracter(ing->nombre[i]);
            }
        }
        int wC, hC; TTF_SizeText(ing->fuenteMedia, "CONFIRMAR", &wC, &hC);
        SDL_Rect rC = {(ANCHOVENTANA-wC)/2, 500, wC, hC};
        if(SDL_PointInRect(&p, &rC)) {
            ing->cursor = 3;
            if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
                strcpy(juego->nombreJugador, ing->nombre);
                return 1;
            }
        }
        int wV, hV; TTF_SizeText(ing->fuenteMedia, "VOLVER", &wV, &hV);
        SDL_Rect rV = {(ANCHOVENTANA-wV)/2, 580, wV, hV};
        if(SDL_PointInRect(&p, &rV)) {
            ing->cursor = 4;
            if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) return 2;
        }
    }
    return 0;
}

void ingresoNombreDibujar(IngresoNombre* ing, SDL_Renderer* renderer)
{
    SDL_Color oro = {255,215,0};
    dibujarTextoCentrados(renderer,ing->fuenteGrande,"INGRESE NOMBRE",100,oro);

    int inicio = (ANCHOVENTANA / 2) - 100;
    int inicioY = 300;
    int separacion = 70;

    for(int i=0;i<3;i++)
    {
        char letra[2] = {ing->nombre[i], '\0'};
        SDL_Color color = (i==ing->cursor) ? (SDL_Color){0,255,0} : (SDL_Color){255,255,255};
        dibujarTexto(renderer,ing->fuenteGrande,letra,inicio+(i*separacion),inicioY,color);
    }

    SDL_Color colConf = (ing->cursor == 3) ? (SDL_Color){0,255,0} : (SDL_Color){100, 100, 100};
    dibujarTextoCentrados(renderer,ing->fuenteMedia,"CONFIRMAR",500,colConf);

    SDL_Color colVol = (ing->cursor == 4) ? (SDL_Color){255,0,0} : (SDL_Color){100, 100, 100};
    dibujarTextoCentrados(renderer,ing->fuenteMedia,"VOLVER",580,colVol);

    dibujarTextoCentrados(renderer,ing->fuenteChica,"Flechas/Rueda: Cambiar. Enter: Seleccionar.",650,oro);
}
