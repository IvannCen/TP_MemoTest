#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>

#define CAP_INI 10
#define FACTOR_INCR 1.5
#define FACTOR_DECR 0.5
#define FACTOR_OCUP 0.25

#define TODO_OK 0
#define LLENO 1
#define DUPLICADO 2
#define SIN_MEM 3

typedef struct
{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
} Vector;

bool vectorCrear(Vector* v, size_t tamElem);
void vectorDestruir(Vector* v);
bool redimensionarVector(Vector* v, float factor);
int vectorInsertarAlFinal(Vector* v, const void* elem);
size_t vectorCantidadDeElementos(const Vector* v);
void intercambiar(void* a, void* b, size_t tam);

/* --- Primitivas agregadas para el Memotest --- */
void* vectorObtenerPuntero(const Vector* v, int pos);
void vectorMezclar(Vector* v);

#endif // VECTOR_H_INCLUDED
