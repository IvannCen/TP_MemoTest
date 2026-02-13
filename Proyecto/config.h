#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

typedef struct
{
    int filas;       // 3 o 4
    int cols;        // 4 o 5
    int setFiguras;  // 0 = A, 1 = B
    int jugadores;   // 1 o 2
} Config;

void config_por_defecto(Config* c);
int  config_cargar(Config* c, const char* path);   // 1 ok, 0 fallo/no existia
int  config_guardar(const Config* c, const char* path); // 1 ok, 0 fallo

#endif // CONFIG_H_INCLUDED
