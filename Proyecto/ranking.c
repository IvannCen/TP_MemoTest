#include "ranking.h"
#include <stdio.h>
#include <string.h>
#include "tablero.h"

void ranking_Guardar(const char* nombre, int puntos, int tiempo)
{
    RegistroRanking registros[MAX_REGISTROS_RANKING + 1];
    int cantidad = 0;
    FILE* file = fopen(ARCHIVO_RANKING, "rb");
    if(file)
    {
        cantidad = fread(registros, sizeof(RegistroRanking), MAX_REGISTROS_RANKING, file);
        fclose(file);
    }

    strcpy(registros[cantidad].nombre, nombre);
    registros[cantidad].puntos = puntos;
    registros[cantidad].tiempo = tiempo;
    cantidad++;

    for(int i=0; i < cantidad - 1; i++)
    {
        for(int j=0; j < cantidad - i - 1; j++)
        {
            int cambiar = 0;
            if(registros[j].puntos < registros[j+1].puntos)
                cambiar = 1;
            else if(registros[j].puntos == registros[j+1].puntos && registros[j].tiempo > registros[j+1].tiempo)
                cambiar = 1;
            if(cambiar)
            {
                RegistroRanking aux = registros[j];
                registros[j] = registros[j+1];
                registros[j+1] = aux;
            }
        }
    }
    if(cantidad > MAX_REGISTROS_RANKING)
        cantidad = MAX_REGISTROS_RANKING;
    file = fopen(ARCHIVO_RANKING, "wb");
    if(file)
    {
        fwrite(registros, sizeof(RegistroRanking), cantidad, file);
        fclose(file);
    }
}

void ranking_Dibujar(SDL_Renderer* render, TTF_Font* fuenteTitulo, TTF_Font* fuenteLista, SDL_Texture* fondo)
{
    if(fondo)
        SDL_RenderCopy(render, fondo, NULL, NULL);
    else
        SDL_SetRenderDrawColor(render, 20, 20, 20, 255);

    int centroX = ANCHO_VENTANA / 2;
    int offset = 250;

    int colPos = centroX - offset;
    int colNom = centroX - 60;
    int colPts = centroX + offset - 50;

    int yCabecera = 150;

    dibujarTextoCentrados(render, fuenteTitulo, "TOP 10 MEJORES PUNTAJES", 50, COLOR_TITULO);

    dibujarTexto(render, fuenteLista, "POS", colPos, yCabecera, COLOR_NORMAL);
    dibujarTexto(render, fuenteLista, "NOMBRE", colNom, yCabecera, COLOR_NORMAL);
    dibujarTexto(render, fuenteLista, "PUNTOS", colPts, yCabecera, COLOR_NORMAL);

    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderDrawLine(render, colPos - 20, yCabecera + 40, colPts + 100, yCabecera + 40);

    FILE* file = fopen(ARCHIVO_RANKING, "rb");
    if(!file)
    {
        dibujarTextoCentrados(render, fuenteLista, "(No hay registros aun)", 300, COLOR_GRIS);
    }
    else
    {
        RegistroRanking reg;
        int i = 0;
        int y = 220;
        char buffer[64];

        while(fread(&reg, sizeof(RegistroRanking), 1, file))
        {
            sprintf(buffer, "#%d", i+1);
            SDL_Color colorFila = (i < 3) ? COLOR_TITULO : COLOR_BLANCO;

            dibujarTexto(render, fuenteLista, buffer, colPos, y, colorFila);

            dibujarTexto(render, fuenteLista, reg.nombre, colNom, y, colorFila);

            sprintf(buffer, "%d", reg.puntos);
            dibujarTexto(render, fuenteLista, buffer, colPts, y, COLOR_BLANCO);

            y += 45;
            i++;
        }
        fclose(file);
    }
    dibujarTextoCentrados(render, fuenteLista, "Presiona ESCAPE para volver", ALTO_VENTANA - 80, COLOR_BLANCO);
}
