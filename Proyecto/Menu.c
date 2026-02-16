#include "Menu.h"
#include "Tablero.h"

// --- MACROS DE CONFIGURACION VISUAL ---
// Definimos las posiciones aqui para usarlas tanto en Dibujar como en ManejarOpciones
#define MENU_Y_INI      300 //anteriormente int inicioY = 300 en menuDibujar (altura donde esta la primer opcion para tener referencia)
#define MENU_SEP        70  //anteriormente int separacion = 70 en menuDibujar (espacio entre opciones)

// Macros para el Popup de Salida
#define POPUP_W         400
#define POPUP_H         200
#define POPUP_X         ((ANCHOVENTANA - POPUP_W) / 2)
#define POPUP_Y         ((ALTOVENTANA - POPUP_H) / 2)

void menuIniciar(Menu* m, SDL_Renderer* renderer)
{
    m->opcionSeleccionada = 0; //arranca desde la primer opcion del menu
    m->confirmaSalida = 0; //arranca desactivado
    m->opcionSalida = 1; //por defecto, arranca en no

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

    if(m->nombre.fuenteMedia)
        TTF_CloseFont(m->nombre.fuenteMedia);
}

int menuManejarOpciones(Menu* m, SDL_Event* e)
{
    // MANEJO POR TECLADO
    if(e->type == SDL_KEYDOWN)
    {
        // CASO 1: ESTAMOS DENTRO DEL POPUP DE CONFIRMACION
        if (m->confirmaSalida)
        {
            switch (e->key.keysym.sym)
            {
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    //cambia entre 0 (SI) y 1 (NO)
                    //usamos ! para invertir el valor booleano (0 pasa a 1, 1 pasa a 0)
                    m->opcionSalida = !m->opcionSalida;
                    break;

                case SDLK_RETURN: //enter normal
                case SDLK_KP_ENTER: //enter del pad numerico
                    if (m->opcionSalida == 0) //si eligio "SI" (asumimos 0 = SI)
                    {
                        //aca si devolvemos la senial para que el main cierre el juego
                        return OPCION_SALIR;
                    }
                    else //si eligio "NO"
                    {
                        m->confirmaSalida = 0; //apagamos el popup
                        //no devolvemos nada, seguimos en el menu
                    }
                    break;

                case SDLK_ESCAPE:
                    //si aprieta escape, cancelamos la salida tambien
                    m->confirmaSalida = 0;
                    break;
            }
            return -1; //retornamos -1 para que el juego siga corriendo
        }

        // CASO 2: ESTAMOS EN EL MENU NORMAL (Navegacion Vertical)
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
            //revisamos que opcion se eligio
            if (m->opcionSeleccionada == OPCION_SALIR)
            {
                // no salimos todavia! activamos el modo confirmacion
                m->confirmaSalida = 1;
                m->opcionSalida = 1; //ponemos el cursor en "NO" por seguridad
                return -1; //devolvemos -1 para que el main espere
            }

            //si es cualquier otra opcion (Aventura, Config, etc), la devolvemos normal
            return m->opcionSeleccionada;
        }
    }

    // MANEJO POR MOUSE
    else if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        // 1. Obtenemos coordenadas del mouse unificadas
        int mouseX = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
        int mouseY = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
        SDL_Point p = {mouseX, mouseY};

        // CASO 1: POPUP ACTIVO (Detectar mouse sobre SI o NO)
        if (m->confirmaSalida)
        {
            int wSi, hSi, wNo, hNo;
            // Medimos los textos. Nota: Asegurate que 'fuenteOpciones' sea la misma que usas en menuDibujar
            TTF_SizeText(m->fuenteOpciones, "SI", &wSi, &hSi);
            TTF_SizeText(m->fuenteOpciones, "NO", &wNo, &hNo);

            // Definimos las cajas (mismas coordenadas que en menuDibujar)
            SDL_Rect rectSi = { (ANCHOVENTANA/2) - 80, (ALTOVENTANA/2) + 20, wSi, hSi };
            SDL_Rect rectNo = { (ANCHOVENTANA/2) + 40, (ALTOVENTANA/2) + 20, wNo, hNo };

            // Chequeamos colision con "SI"
            if(SDL_PointInRect(&p, &rectSi))
            {
                m->opcionSalida = 0; // 0 = SI
                // Si fue clic, confirmamos salida
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                {
                    return OPCION_SALIR;
                }
            }
            // Chequeamos colision con "NO"
            else if(SDL_PointInRect(&p, &rectNo))
            {
                m->opcionSalida = 1; // 1 = NO
                // Si fue clic, cancelamos popup
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                {
                    m->confirmaSalida = 0;
                }
            }
        }
        // CASO 2: MENU NORMAL (Detectar mouse sobre opciones)
        else
        {
            for(int i = 0; i < CANTIDADOPCIONES; i++)
            {
                int w, h;
                const char* texto = menuObtenerTexto(i);

                TTF_SizeText(m->fuenteOpciones, texto, &w, &h);

                SDL_Rect rect;
                rect.x = (ANCHOVENTANA - w) / 2;
                rect.y = MENU_Y_INI + (i * MENU_SEP);
                rect.w = w;
                rect.h = h;

                // Si el mouse esta encima del texto
                if(SDL_PointInRect(&p, &rect))
                {
                    m->opcionSeleccionada = i; // Efecto visual (cambia de color)

                    // Si hizo clic
                    if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                    {
                        if (i == OPCION_SALIR)
                        {
                            // Mismo comportamiento que el teclado: Activar Popup
                            m->confirmaSalida = 1;
                            m->opcionSalida = 1;
                            return -1;
                        }
                        // Cualquier otra opcion, la retornamos
                        return i;
                    }
                }
            }
        }
    }

    return -1; // Si no se apretó nada relevante
}

void menuDibujar(Menu* m, SDL_Renderer* renderer)
{
    //lo dibujo al menu centrado en la pantalla
    int w,h;
    SDL_QueryTexture(m->texturaTitulo, NULL, NULL, &w, &h);
    SDL_Rect recTitulo = {(ANCHOVENTANA - w)/2, 100, w, h};
    SDL_RenderCopy(renderer, m->texturaTitulo, NULL, &recTitulo);

    //dibujo las opciones en pantalla

    for(int i=0;i<CANTIDADOPCIONES;i++)
    {
        SDL_Color color = (i == m->opcionSeleccionada) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
        //obtengo el texto directamente del id que devuelve la funcion auxiliar
        const char* texto = menuObtenerTexto(i);

        dibujarTextoCentrados(renderer, m->fuenteOpciones, texto, MENU_Y_INI + (i * MENU_SEP), color);
    }

    // === DIBUJADO DEL POPUP DE SALIDA ===
    if (m->confirmaSalida)
    {
        // 1. Fondo semitransparente (Overlay)
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // Negro con opacidad
        SDL_Rect overlay = {0, 0, ANCHOVENTANA, ALTOVENTANA};
        SDL_RenderFillRect(renderer, &overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        // 2. Cuadro del diálogo (Borde blanco opcional)
        SDL_Rect caja = {POPUP_X, POPUP_Y, POPUP_W, POPUP_H}; //SDL_Rect caja = { (ANCHOVENTANA/2)-200, (ALTOVENTANA/2)-100, 400, 200 };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Gris oscuro fondo caja
        SDL_RenderFillRect(renderer, &caja);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Borde blanco
        SDL_RenderDrawRect(renderer, &caja);

        // 3. Texto de pregunta
        SDL_Color blanco = {255, 255, 255};
        dibujarTextoCentrados(renderer, m->fuenteOpciones, "Seguro desea salir?", (ALTOVENTANA/2)-40, blanco);

        // 4. Opciones SI / NO
        // Opcion SI (0)
        SDL_Color colorSi = (m->opcionSalida == 0) ? (SDL_Color){0, 255, 0} : (SDL_Color){150, 150, 150};
        // Opcion NO (1)
        SDL_Color colorNo = (m->opcionSalida == 1) ? (SDL_Color){255, 0, 0} : (SDL_Color){150, 150, 150};

        // Dibujamos "SI" un poco a la izquierda y "NO" a la derecha
        // (Nota: puedes ajustar las coordenadas X e Y a tu gusto)
        dibujarTexto(renderer, m->fuenteOpciones, "SI", (ANCHOVENTANA/2) - 80, (ALTOVENTANA/2) + 20, colorSi);
        dibujarTexto(renderer, m->fuenteOpciones, "NO", (ANCHOVENTANA/2) + 40, (ALTOVENTANA/2) + 20, colorNo);
    }
}

int menuDificultadOpciones(Menu* m, SDL_Event* e)
{
    //LOGICA DE TECLADO
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

    //LOGICA DE MOUSE
    else if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = (e->type == SDL_MOUSEMOTION) ? e->motion.x : e->button.x;
        int mouseY = (e->type == SDL_MOUSEMOTION) ? e->motion.y : e->button.y;
        SDL_Point p = {mouseX, mouseY};

        for(int i=0; i<CANTIDADIFICULTAD; i++)
        {
            int w, h;
            const char* texto = menuDificultadObtenerTexto(i);

            TTF_SizeText(m->fuenteOpciones, texto, &w, &h);

            SDL_Rect rect;
            rect.x = (ANCHOVENTANA - w) / 2;
            rect.y = MENU_Y_INI + (i * MENU_SEP);
            rect.w = w;
            rect.h = h;

            if(SDL_PointInRect(&p, &rect))
            {
                m->opcionSeleccionada = i;
                if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                    return i;
            }
        }
    }
    return -1;
}

void menuDificultadDibujar(Menu* m, SDL_Renderer* renderer)
{
    SDL_Color colorTitulo = {255,215,0};
    dibujarTextoCentrados(renderer,m->fuenteTitulo,"SELECCIONA DIFICULTAD",100,colorTitulo);

    for(int i=0;i<CANTIDADIFICULTAD;i++)
    {
        SDL_Color color = (i == m->opcionSeleccionada) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
        const char* texto = menuDificultadObtenerTexto(i);

        dibujarTextoCentrados(renderer, m->fuenteOpciones, texto, MENU_Y_INI + (i * MENU_SEP), color);
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
        case OPCION_AVENTURA:       return "Aventura";
        case OPCION_COMPETITIVO:    return "Competitivo";
        case OPCION_ESTADISTICAS:   return "Ranking";
        case OPCION_CONFIGURACION:  return "Configuracion";
        case OPCION_SALIR:          return "Salir";
        default:                    return "???"; //opcion desconocida, no es valida
    }
}

//Ya no hay dificultades, ver eso
const char* menuDificultadObtenerTexto(int opcion)
{
    switch(opcion)
    {
        case FACIL:     return "FACIL";
        case NORMAL:    return "NORMAL";
        case DIFICIL:   return "DIFICIL";
        case VOLVER:    return "VOLVER";
        default:        return "???";
    }
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
