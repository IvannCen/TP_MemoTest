#include <stdlib.h>
#include <string.h>
#include "vector.h"

bool vectorCrear(Vector* v, size_t tamElem)
{
    v->ce = 0;

    if(tamElem == 0)
    {
        v->vec = NULL;
        v->cap = 0;
        v->tamElem = 0;
        return false;
    }

    v->vec = malloc(CAP_INI * tamElem);

    if(v->vec == NULL)
    {
        v->cap = 0;
        v->tamElem = 0;
        return false;
    }

    v->cap = CAP_INI;
    v->tamElem = tamElem;

    return true;
}

void vectorDestruir(Vector* v)
{
    free(v->vec);
    v->vec = NULL;
    v->ce = 0;
    v->cap = 0;
}

bool redimensionarVector(Vector* v, float factor)
{
    size_t nuevaCap = v->cap * factor;
    void* nuevoVec = NULL;

    if(nuevaCap < CAP_INI)
    {
        return true;
    }

    nuevoVec = realloc(v->vec, nuevaCap * v->tamElem);

    if(nuevoVec == NULL)
    {
        return false;
    }

    /* Comentado para evitar el spam en consola cada vez que se agreguen cartas */
    /* printf("Redimensión de %lld a %lld\n", v->cap, nuevaCap); */

    v->vec = nuevoVec;
    v->cap = nuevaCap;

    return true;
}

int vectorInsertarAlFinal(Vector* v, const void* elem)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, FACTOR_INCR))
        {
            return SIN_MEM;
        }
    }

    memcpy(v->vec + v->ce * v->tamElem, elem, v->tamElem);
    v->ce++;

    return TODO_OK;
}

size_t vectorCantidadDeElementos(const Vector* v)
{
    return v->ce;
}

void intercambiar(void* a, void* b, size_t tam)
{
    void* aTemp = malloc(tam);
    if(!aTemp)
        return;

    memcpy(aTemp, a, tam);
    memcpy(a, b, tam);
    memcpy(b, aTemp, tam);

    free(aTemp);
}

void* vectorObtenerPuntero(const Vector* v, int pos)
{
    if(pos < 0 || pos >= (int)v->ce)
        return NULL;

    return (char*)v->vec + (pos * v->tamElem);
}

void vectorMezclar(Vector* v)
{
    int i, j;
    void *elemI, *elemJ;

    if(v->ce < 2)
        return;

    for(i = v->ce - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        elemI = (char*)v->vec + (i * v->tamElem);
        elemJ = (char*)v->vec + (j * v->tamElem);

        //Reutilizamos su propia funcion de intercambiar
        intercambiar(elemI, elemJ, v->tamElem);
    }
}
