#include "Menu.h"
#include "Tablero.h"

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
}

int menuManejarOpciones(Menu* m, SDL_Event* e)
{
    /*
    if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
            m->opcionSeleccionada--;
            if(m->opcionSeleccionada < 0)
                m->opcionSeleccionada = CANTIDADOPCIONES - 1; //vuelve a la opcion de abajo del todo

            break;
        case SDLK_DOWN:
            m->opcionSeleccionada++;
            if(m->opcionSeleccionada >= CANTIDADOPCIONES)
                m->opcionSeleccionada = 0; //vuelve a la primera opcion

            break;
        case SDLK_RETURN: //este es el enter normal
        case SDLK_KP_ENTER: //enter del pad numerico
            return m->opcionSeleccionada; //devuelvo la opcion si se selecciona el enter
        }
    }

    return -1; //devuelvo -1 si sigue en el menu y no eligio nada
    */

    if(e->type == SDL_KEYDOWN)
    {
        // ---------------------------------------------------------
        // CASO 1: ESTAMOS DENTRO DEL POPUP DE CONFIRMACION
        // ---------------------------------------------------------
        if (m->confirmaSalida)
        {
            switch (e->key.keysym.sym)
            {
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    // Cambia entre 0 (SI) y 1 (NO)
                    // Usamos ! para invertir el valor booleano (0 pasa a 1, 1 pasa a 0)
                    m->opcionSalida = !m->opcionSalida;
                    break;

                case SDLK_RETURN: // Enter normal
                case SDLK_KP_ENTER: // Enter numerico
                    if (m->opcionSalida == 0) // Si eligio "SI" (asumimos 0 = SI)
                    {
                        // Aca SI devolvemos la senial para que el main cierre el juego
                        return OPCION_SALIR;
                    }
                    else // Si eligio "NO"
                    {
                        m->confirmaSalida = 0; // Apagamos el popup
                        // No devolvemos nada, seguimos en el menu
                    }
                    break;

                case SDLK_ESCAPE:
                    // Si aprieta Escape, cancelamos la salida tambien
                    m->confirmaSalida = 0;
                    break;
            }
            return -1; // Retornamos -1 para que el juego siga corriendo
        }

        // ---------------------------------------------------------
        // CASO 2: ESTAMOS EN EL MENU NORMAL (Navegacion Vertical)
        // ---------------------------------------------------------
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
            // Revisamos qué opción eligió
            if (m->opcionSeleccionada == OPCION_SALIR)
            {
                // ¡NO salimos todavía! Activamos el modo confirmación
                m->confirmaSalida = 1;
                m->opcionSalida = 1; // Ponemos el cursor en "NO" por seguridad
                return -1; // Devolvemos -1 para que el main espere
            }

            // Si es cualquier otra opción (Aventura, Config, etc), la devolvemos normal
            return m->opcionSeleccionada;
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
    const char* opciones[] = {"Aventura","Competitivo","Ranking","Configuracion","Salir"};

    //estos los pongo estaticos pero podrian ser macros, vamos viendo
    int inicioY = 300; //altura donde esta la primer opcion para tener referencia
    int separacion = 70; //espacion entre opciones

    for(int i=0;i<CANTIDADOPCIONES;i++)
    {
        SDL_Color color = (i==m->opcionSeleccionada) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
        dibujarTextoCentrados(renderer, m->fuenteOpciones, opciones[i], inicioY + (i * separacion), color);
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
        SDL_Rect caja = { (ANCHOVENTANA/2)-200, (ALTOVENTANA/2)-100, 400, 200 };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Gris oscuro fondo caja
        SDL_RenderFillRect(renderer, &caja);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Borde blanco
        SDL_RenderDrawRect(renderer, &caja);

        // 3. Texto de pregunta
        SDL_Color colorPregunta = {255, 255, 255};
        dibujarTextoCentrados(renderer, m->fuenteOpciones, "Seguro desea salir?", (ALTOVENTANA/2)-60, colorPregunta);

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
    return -1;
}

void menuDificultadDibujar(Menu* m, SDL_Renderer* renderer)
{
    SDL_Color colorTitulo = {255,215,0};
    dibujarTextoCentrados(renderer,m->fuenteTitulo,"SELECCIONA DIFICULTAD",100,colorTitulo);

    const char* opciones[]={"FACIL","NORMAL","DIFICIL","VOLVER"};

    //estos los pongo estaticos pero podrian ser macros, vamos viendo
    int inicioY = 300; //altura donde esta la primer opcion para tener referencia
    int separacion = 70; //espacion entre opciones

    for(int i=0;i<CANTIDADIFICULTAD;i++)
    {
        SDL_Color color = (i==m->opcionSeleccionada) ? (SDL_Color){50,255,50} : (SDL_Color){200,200,200};
        dibujarTextoCentrados(renderer,m->fuenteOpciones,opciones[i],inicioY + (i * separacion),color);
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
