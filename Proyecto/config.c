#include "config.h"
#include <stdio.h>

void config_por_defecto(Config* c)
{
    c->filas = 4;
    c->cols = 4;
    c->setFiguras = 0;
    c->jugadores = 1;
}

int config_cargar(Config* c, const char* path)
{
    FILE* f = fopen(path, "rb");
    if(!f)
    {
        config_por_defecto(c);
        return 0;
    }

    size_t n = fread(c, sizeof(Config), 1, f);
    fclose(f);

    if(n != 1)
    {
        config_por_defecto(c);
        return 0;
    }

    // validacion minima
    if(!((c->filas == 3 && c->cols == 4) ||
         (c->filas == 4 && c->cols == 4) ||
         (c->filas == 4 && c->cols == 5)))
    {
        config_por_defecto(c);
        return 0;
    }

    if(c->setFiguras < 0 || c->setFiguras > 1) c->setFiguras = 0;
    if(c->jugadores < 1 || c->jugadores > 2) c->jugadores = 1;

    return 1;
}

int config_guardar(const Config* c, const char* path)
{
    FILE* f = fopen(path, "wb");
    if(!f) return 0;

    size_t n = fwrite(c, sizeof(Config), 1, f);
    fclose(f);

    return (n == 1);
}
