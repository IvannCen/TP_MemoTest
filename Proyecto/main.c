#include "Tablero.h"
#include "Juego.h"
#include "Menu.h"
#include "Carta.h"

int main(int argc, char *argv[])
{
//    srand(time(NULL)); //para generar la aleatoriedad en el programa
//
//    if(SDL_Init(SDL_INIT_VIDEO) < 0)
//        return 1;
//
//    if(IMG_Init(IMG_INIT_PNG)==0)
//        return 1;
//
//    if(TTF_Init()==-1) //inicio las fuentes del juego para las letras
//        return 1;
//
//
//    //Inicializo el subsistema de sonido
//    tFormatosSnd formatos = sonidos_inicializar();
//
//    if(formatos == SONIDO_ERR)
//    {
//        return 1;
//    }
//
//    // Usamos tus constantes de Comun.h
//    //NOTA: agrego "| SDL_WINDOW_RESIZABLE" para habilitar los botones de maximizar y los
//    //que modifican el tamanio de la ventana.
//    SDL_Window *window = SDL_CreateWindow("TP Memotest",
//                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                                          ANCHOVENTANA, ALTOVENTANA,
//                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
//
//    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//
//    //funcion inteligente que sirve para el manejo de la pantalla
//    SDL_RenderSetLogicalSize(renderer,ANCHOVENTANA,ALTOVENTANA);
//
//    //inicio las fuentes para que se vean en la pantalla
//    TTF_Font* fuenteGrande = TTF_OpenFont(FUENTE, FUENTEGRANDE);
//    TTF_Font* fuenteChica = TTF_OpenFont(FUENTE, FUENTECHICA);
//    TTF_Font* fuenteMedia= TTF_OpenFont(FUENTE, FUENTEMEDIA);
//
//    if(!fuenteGrande || !fuenteChica || !fuenteMedia)
//    {
//        printf("Error en la fuente ingresada: %s\n", TTF_GetError());
//    }
//
//    //sistema del menu
//    Menu menuPrincipal;
//    menuIniciar(&menuPrincipal, renderer);
//
//    //logica inicial para arrancar en el menu
//    Tablero miTablero;
//
//    //estructura de datos de configuracion
//    Configuracion config;
//    ContextoJuego juego;
//    juego.confirmandoSalida = 0;
//
//    //cargo las configuraciones
//    cargarConfiguracion(&config);
//    EstadoJuego estadoActual = ESTADO_MENU;
//
//    //variable para ver si hay un tablero en memoria
//    int tableroCargado = 0;
//    // loop del juego
//    int corriendo = 1;
//    SDL_Event e;
//
//    // Usamos sonidos_cargar.
//    // IMPORTANTE: Asegurate de tener la carpeta /snd con estos archivos
//    juego.sndSeleccion = sonidos_cargar(SELECCION);
//    juego.sndAcierto = sonidos_cargar(ACIERTO);
//    juego.sndFallo = sonidos_cargar(FALLO);
//    Mix_Volume(-1, 40); // Reducion del volumen al 40% de forma fija a efectos de no molestar a quien inicie esta demo
//
//    while (corriendo)
//    {
//        // proceso los ingresos segun el estado en el que se encuentre el juego
//        while (SDL_PollEvent(&e))
//        {
//            if (e.type == SDL_QUIT)
//            {
//                corriendo = 0;
//            }
//
//            switch(estadoActual)
//            {
//                case ESTADO_MENU:
//                {
//                    int seleccion = menuManejarOpciones(&menuPrincipal, &e);
//                    if(seleccion != -1)
//                    {
//                        if(seleccion == OPCION_AVENTURA)
//                        {
//                            ingresoNombreIniciar(&menuPrincipal.nombre);
//                            estadoActual = ESTADO_NOMBRE;
//                        }
//                        else if(seleccion == OPCION_SALIR)
//                        {
//                            corriendo = 0;
//                        }
//                        else if(seleccion == OPCION_COMPETITIVO)
//                        {
//                            printf("Proximamente Modo Competitivo\n");
//                        }
//                        else if(seleccion == OPCION_ESTADISTICAS)
//                        {
//                            printf("Proximamente Ranking\n");
//                        }
//                        else if(seleccion == OPCION_CONFIGURACION)
//                        {
//                            menuPrincipal.opcionSeleccionada = 0;
//                            estadoActual = ESTADO_CONFIGURACION;
//                        }
//                    }
//                    break;
//                }
//                case ESTADO_CONFIGURACION:
//                {
//                    int salir = menuConfiguracionOpciones(&menuPrincipal, &e, &config);
//                    if(salir)
//                    {
//                        guardarConfiguracion(&config);
//                        menuPrincipal.opcionSeleccionada = 0;
//                        estadoActual = ESTADO_MENU;
//                    }
//                    break;
//                }
//                case ESTADO_NOMBRE:
//                {
//                    int confirmado = ingresoNombreOpciones(&menuPrincipal.nombre, &e, &juego);
//                    if(confirmado)
//                    {
//                        juego.turnoJugador = 1;
//                        juego.puntos = 0;
//                        juego.tiempoInicio = SDL_GetTicks();//esta funcion resetea el reloj
//                        juego.confirmandoSalida = 0;
//
//                        tableroIniciar(&miTablero, &config);
//                        tableroCargarImagenes(&miTablero,renderer, config.idSetImagenes);
//                        tableroRellenar(&miTablero);
//                        tableroMezclar(&miTablero);
//                        tableroCargado = 1;
//
//                        estadoActual = ESTADO_JUGANDO;
//                    }
//                    else if(confirmado==2)
//                    {
//                        menuPrincipal.opcionSeleccionada = 0;
//                        estadoActual = ESTADO_MENU;
//                    }
//                    break;
//                }
//                case ESTADO_JUGANDO:
//                {
//                    if(juego.confirmandoSalida)
//                    {
//                        // Si el popup está abierto, solo escuchamos Enter o Escape
//                        if(e.type == SDL_KEYDOWN)
//                        {
//                            if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
//                            {
//                                // CONFIRMAR SALIDA
//                                if(tableroCargado)
//                                {
//                                    tableroDestruir(&miTablero);
//                                    tableroCargado = 0;
//                                }
//                                juego.confirmandoSalida = 0; // Limpiamos la bandera
//                                menuPrincipal.confirmaSalida = 0; // Aseguramos que el menu no arranque con popup
//                                estadoActual = ESTADO_MENU;
//                            }
//                            else if(e.key.keysym.sym == SDLK_ESCAPE)
//                            {
//                                // CANCELAR SALIDA
//                                juego.confirmandoSalida = 0;
//                            }
//                        }
//                    }
//                    else // JUEGO NORMAL
//                    {
//                        if(e.type == SDL_KEYDOWN)
//                        {
//                            if(e.key.keysym.sym == SDLK_ESCAPE)
//                            {
//                                juego.confirmandoSalida = 1; // ACTIVAR POPUP
//                            }
//                            else
//                            {
//                                // Jugar con teclado
//                                tableroManejarTeclado(&miTablero, &e, renderer, &juego);
//                                if(tableroCompleto(&miTablero)) { SDL_Delay(DELAYCHICO); estadoActual=ESTADO_GANO; }
//                            }
//                        }
//                        else if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
//                        {
//                            int puntos = tableroClic(&miTablero,e.button.x,e.button.y, renderer, &juego);
//                            juego.puntos += puntos;
//                            if(juego.puntos < 0) juego.puntos = 0;
//
//                            if(tableroCompleto(&miTablero)) { SDL_Delay(DELAYCHICO); estadoActual=ESTADO_GANO; }
//                        }
//                        else if(e.type == SDL_MOUSEMOTION)
//                        {
//                            tableroManejarHover(&miTablero, e.motion.x, e.motion.y);
//                        }
//                    }
//                    break;
//                }
//                case ESTADO_GANO:
//                {
//                    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
//                    {
//                        //elimino el tablero del nivel anterior
//                        if(tableroCargado)
//                        {
//                            tableroDestruir(&miTablero);
//                            tableroCargado = 0;
//                        }
//
//                        estadoActual=ESTADO_MENU;
//
//                    }
//                    break;
//                }
//            }
//        }
//
//        //Renderizado segun el estado en el que este el juego(Dibujar)
//        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Fondo Gris oscuro
//        SDL_RenderClear(renderer);
//
//        if(estadoActual == ESTADO_MENU)
//        {
//            menuDibujar(&menuPrincipal, renderer);
//
//        }
//        else if(estadoActual == ESTADO_CONFIGURACION)
//        {
//            menuConfiguracionDibujar(&menuPrincipal, renderer, &config);
//        }
//        else if(estadoActual == ESTADO_NOMBRE)
//        {
//            ingresoNombreDibujar(&menuPrincipal.nombre, renderer);
//        }
//        else if(estadoActual == ESTADO_JUGANDO)
//        {
//            //dibujo el juego en si mismo
//            int mX, mY;
//            SDL_GetMouseState(&mX,&mY);
//            dibujarEstadisticas(renderer,fuenteChica,&juego);
//            if(juego.confirmandoSalida)
//            {
//                dibujarPopupSalida(renderer,fuenteMedia);
//            }
//
//        }
//        else if(estadoActual == ESTADO_GANO)
//        {
//            if(tableroCargado)
//            {
//                //dibujo la pantalla de victoria, con el tablero de fondo
//                tableroDibujar(&miTablero, renderer, 0, 0);
//            }
//
//            //dibujo el fondo transparente de fondo
//            //activo un blending para que se acepten transparencias
//            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//            //agrego el color negro para el cuadrado, poniendo 200 de alpha, que le da solides al color
//            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // Negro con opacidad al 80
//            //hago el rectangulo donde va a ir el texto de Ganaste
//            SDL_Rect rect = {0, 0, ANCHOVENTANA, ALTOVENTANA};
//            SDL_RenderFillRect(renderer, &rect);
//            //desactivo el blending para que lo demas se dibuje
//            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
//
//            //color de los textos
//            SDL_Color colorVerde = {50, 255, 50};
//            SDL_Color colorBlanco = {255, 255, 255};
//            SDL_Color colorAmarillo = {255, 255, 0};
//
//            //dibujo el texto encima del tablero
//            dibujarTextoCentrados(renderer, fuenteGrande, "PARTIDA COMPLETADA", 250, colorVerde);
//            char puntajeBuffer[64];
//            sprintf(puntajeBuffer, "Jugador: %s - Puntos Totales: %d", juego.nombreJugador, juego.puntos);
//            dibujarTextoCentrados(renderer,fuenteMedia,puntajeBuffer,350,colorAmarillo);
//            dibujarTextoCentrados(renderer,fuenteMedia,"Presiona SPACE para volver al Menu",500,colorBlanco);
//        }
//        SDL_RenderPresent(renderer);
//    }
//
//    //limpio todas las variables
//    menuDestruir(&menuPrincipal);
//
//    sonidos_destruir(juego.sndSeleccion);
//    sonidos_destruir(juego.sndAcierto);
//    sonidos_destruir(juego.sndFallo);
//    sonidos_finalizar();
//
//    TTF_CloseFont(fuenteGrande);
//    TTF_CloseFont(fuenteMedia);
//    TTF_CloseFont(fuenteChica);
//    if(tableroCargado)
//    {
//        tableroDestruir(&miTablero); //libero el vector dinamico
//    }
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    TTF_Quit(); // cierro el TTF
//    IMG_Quit();
//    SDL_Quit();
//
//    return 0;

    srand(time(NULL));
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    if(IMG_Init(IMG_INIT_PNG)==0) return 1;
    if(TTF_Init()==-1) return 1;

    tFormatosSnd formatos = sonidos_inicializar();
    if(formatos == SONIDO_ERR) return 1;

    SDL_Window *window = SDL_CreateWindow("TP Memotest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHOVENTANA, ALTOVENTANA, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer,ANCHOVENTANA,ALTOVENTANA);

    TTF_Font* fuenteGrande = TTF_OpenFont(FUENTE, FUENTEGRANDE);
    TTF_Font* fuenteChica = TTF_OpenFont(FUENTE, FUENTECHICA);
    TTF_Font* fuenteMedia= TTF_OpenFont(FUENTE, FUENTEMEDIA);

    Menu menuPrincipal;
    menuIniciar(&menuPrincipal, renderer);

    Tablero miTablero;
    Configuracion config;
    ContextoJuego juego;
    juego.confirmandoSalida = 0;
    juego.opcionSalidaPopup = 1;

    cargarConfiguracion(&config);
    EstadoJuego estadoActual = ESTADO_MENU;

    int tableroCargado = 0;
    int corriendo = 1;
    SDL_Event e;

    juego.sndSeleccion = sonidos_cargar(SELECCION);
    juego.sndAcierto = sonidos_cargar(ACIERTO);
    juego.sndFallo = sonidos_cargar(FALLO);
    Mix_Volume(-1, 40);

    while (corriendo)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) corriendo = 0;

            switch(estadoActual)
            {
                case ESTADO_MENU:
                {
                    int seleccion = menuManejarOpciones(&menuPrincipal, &e);
                    if(seleccion != -1)
                    {
                        if(seleccion == OPCION_AVENTURA) {
                            ingresoNombreIniciar(&menuPrincipal.nombre);
                            estadoActual = ESTADO_NOMBRE;
                        }
                        else if(seleccion == OPCION_SALIR) corriendo = 0;
                        else if(seleccion == OPCION_CONFIGURACION) {
                            menuPrincipal.opcionSeleccionada = 0;
                            estadoActual = ESTADO_CONFIGURACION;
                        }
                    }
                    break;
                }
                case ESTADO_CONFIGURACION:
                {
                    int salir = menuConfiguracionOpciones(&menuPrincipal, &e, &config);
                    if(salir) {
                        guardarConfiguracion(&config);
                        menuPrincipal.opcionSeleccionada = 0;
                        estadoActual = ESTADO_MENU;
                    }
                    break;
                }
                case ESTADO_NOMBRE:
                {
                    int res = ingresoNombreOpciones(&menuPrincipal.nombre, &e, &juego);
                    if(res == 1)
                    {
                        juego.turnoJugador = 1;
                        juego.puntos = 0;
                        juego.tiempoInicio = SDL_GetTicks();
                        juego.confirmandoSalida = 0;
                        juego.opcionSalidaPopup = 1;

                        tableroIniciar(&miTablero, &config);
                        tableroCargarImagenes(&miTablero,renderer, config.idSetImagenes);
                        tableroRellenar(&miTablero);
                        tableroMezclar(&miTablero);
                        tableroCargado = 1;
                        estadoActual = ESTADO_JUGANDO;

                        // Limpiar cola de eventos para evitar inputs fantasmas al iniciar
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                    }
                    else if(res == 2) {
                        menuPrincipal.opcionSeleccionada = 0;
                        estadoActual = ESTADO_MENU;
                    }
                    break;
                }
                case ESTADO_JUGANDO:
                {
                    if(juego.confirmandoSalida)
                    {
                        if(e.type == SDL_KEYDOWN)
                        {
                            if(e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)
                                juego.opcionSalidaPopup = !juego.opcionSalidaPopup;
                            else if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
                            {
                                if(juego.opcionSalidaPopup == 0) // SI
                                {
                                    if(tableroCargado) { tableroDestruir(&miTablero); tableroCargado = 0; }
                                    juego.confirmandoSalida = 0;
                                    menuPrincipal.confirmaSalida = 0;
                                    estadoActual = ESTADO_MENU;
                                    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                                }
                                else juego.confirmandoSalida = 0; // NO
                            }
                            else if(e.key.keysym.sym == SDLK_ESCAPE) juego.confirmandoSalida = 0;
                        }
                        else if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                        {
                            int mx = e.button.x, my = e.button.y;
                            SDL_Point p = {mx, my};
                            int cY = (ALTOVENTANA-200)/2;
                            SDL_Rect rSi = { (ANCHOVENTANA/2)-80, cY+120, 50, 40 };
                            SDL_Rect rNo = { (ANCHOVENTANA/2)+40, cY+120, 50, 40 };

                            if(SDL_PointInRect(&p, &rSi)) {
                                if(tableroCargado) { tableroDestruir(&miTablero); tableroCargado = 0; }
                                juego.confirmandoSalida = 0;
                                menuPrincipal.confirmaSalida = 0;
                                estadoActual = ESTADO_MENU;
                                SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            }
                            else if(SDL_PointInRect(&p, &rNo)) juego.confirmandoSalida = 0;
                        }
                        else if(e.type == SDL_MOUSEMOTION) {
                            int mx = e.motion.x, my = e.motion.y;
                            SDL_Point p = {mx, my};
                            int cY = (ALTOVENTANA-200)/2;
                            SDL_Rect rSi = { (ANCHOVENTANA/2)-80, cY+120, 50, 40 };
                            SDL_Rect rNo = { (ANCHOVENTANA/2)+40, cY+120, 50, 40 };
                            if(SDL_PointInRect(&p, &rSi)) juego.opcionSalidaPopup = 0;
                            if(SDL_PointInRect(&p, &rNo)) juego.opcionSalidaPopup = 1;
                        }
                    }
                    else
                    {
                        if(e.type == SDL_KEYDOWN) {
                            if(e.key.keysym.sym == SDLK_ESCAPE) { juego.confirmandoSalida = 1; juego.opcionSalidaPopup = 1; }
                            else {
                                tableroManejarTeclado(&miTablero, &e, renderer, &juego);
                                if(tableroCompleto(&miTablero)) { SDL_Delay(DELAYCHICO); estadoActual=ESTADO_GANO; }
                            }
                        }
                        else if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                            int puntos = tableroClic(&miTablero,e.button.x,e.button.y, renderer, &juego);
                            juego.puntos += puntos;
                            if(juego.puntos<0) juego.puntos = 0;
                            if(tableroCompleto(&miTablero)) { SDL_Delay(DELAYCHICO); estadoActual=ESTADO_GANO; }
                        }
                        else if(e.type == SDL_MOUSEMOTION) {
                            tableroManejarHover(&miTablero, e.motion.x, e.motion.y);
                        }
                    }
                    break;
                }
                case ESTADO_GANO:
                {
                    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
                    {
                        if(tableroCargado) { tableroDestruir(&miTablero); tableroCargado = 0; }
                        estadoActual=ESTADO_MENU;
                        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        if(estadoActual == ESTADO_MENU) menuDibujar(&menuPrincipal, renderer);
        else if(estadoActual == ESTADO_CONFIGURACION) menuConfiguracionDibujar(&menuPrincipal, renderer, &config);
        else if(estadoActual == ESTADO_NOMBRE) ingresoNombreDibujar(&menuPrincipal.nombre, renderer);
        else if(estadoActual == ESTADO_JUGANDO)
        {
            int mX, mY; SDL_GetMouseState(&mX,&mY);
            tableroDibujar(&miTablero, renderer, mX, mY);
            dibujarEstadisticas(renderer, fuenteChica, &juego);
            if(juego.confirmandoSalida) dibujarPopupSalidaJuego(renderer, fuenteMedia, juego.opcionSalidaPopup);
        }
        else if(estadoActual == ESTADO_GANO)
        {
            if(tableroCargado) tableroDibujar(&miTablero, renderer, 0, 0);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_Rect rect = {0, 0, ANCHOVENTANA, ALTOVENTANA};
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            dibujarTextoCentrados(renderer, fuenteGrande, "PARTIDA COMPLETADA", 250, (SDL_Color){50, 255, 50});
            char puntajeBuffer[64];
            sprintf(puntajeBuffer, "Jugador: %s - Puntos Totales: %d", juego.nombreJugador, juego.puntos);
            dibujarTextoCentrados(renderer,fuenteMedia,puntajeBuffer,350, (SDL_Color){255, 255, 0});
            dibujarTextoCentrados(renderer,fuenteMedia,"Presiona SPACE para volver",500, (SDL_Color){255, 255, 255});
        }
        SDL_RenderPresent(renderer);
    }

    menuDestruir(&menuPrincipal);
    sonidos_destruir(juego.sndSeleccion);
    sonidos_destruir(juego.sndAcierto);
    sonidos_destruir(juego.sndFallo);
    sonidos_finalizar();
    TTF_CloseFont(fuenteGrande);
    TTF_CloseFont(fuenteMedia);
    TTF_CloseFont(fuenteChica);
    if(tableroCargado) tableroDestruir(&miTablero);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); IMG_Quit(); SDL_Quit();

    return 0;
}
