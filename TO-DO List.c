#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//MACROS
#define SALTARLINEA(N){for(int i=0; i<N; i++){printf("\n");}}

//Tamaño de entrada
const int buffer = 1000;

//Enumerador de estados tarea
enum{COMPLETA,INCOMPLETA,FALLIDA};

//Estructura tarea
struct TAREA
{
    char nombre[1000];
    char descripcion[1000];
    int estado;
    char fechaInicio[1000];
    char fechaVence[10000];
};

//Estructura NODOAVL
struct NODOAVL
{
    struct TAREA* tarea;
    struct NODOAVL* derecha;
    struct NODOAVL* izquierda;
    int altura;
};

//Obtener fecha actual
char* obtenerFecha()
{
    time_t actual = time(NULL);
    struct tm* fechaLocal = localtime(&actual);
    //Asignacion de memoria en cadena
    char* fecha = malloc(sizeof(fecha)*buffer);
    //Excepcion de memoria
    if(fecha == NULL)
    {
        printf("Error: Asignacion de memoria invalida");
        exit(1);
    }
    strftime(fecha,buffer,"%Y/%m/%d Hora:%H:%M:%S",fechaLocal);
    return fecha;
}

//Funcion crear tarea
struct TAREA* crearTarea(char* nombre,char* descripcion,char* fechaVence)
{
    //Asignacion de memoria
    struct TAREA* tarea = (struct TAREA*)malloc(sizeof(struct TAREA)*1);
    //Excepcion de memoria
    if(!tarea)
    {
        printf("Error: Asignacion de memoria invalida");
        exit(1);
    }
    strcpy(tarea->nombre,nombre);
    strcpy(tarea->descripcion,descripcion);
    strcpy(tarea->fechaVence,fechaVence);
    strcpy(tarea->fechaInicio,obtenerFecha());
    tarea->estado = INCOMPLETA;
}

//Funcion crear NODO AVL
struct NODOAVL* crearNodo(struct TAREA* tarea)
{
    //Excepcion tarea null
    if(tarea == NULL)
    {
        printf("Error: tarea esta vacia\n");
        return NULL;
    }
    //Asignacion de memoria
    struct NODOAVL* nodo = (struct NODOAVL*)malloc(sizeof(struct NODOAVL)*1);
    //Excepcion de memoria
    if(nodo == NULL)
    {
        printf("Error: Asignacion de memoria invalida");
        exit(1);
    }
    nodo->tarea = tarea;
    nodo->altura = 1;
    nodo->derecha = nodo->izquierda = NULL;
    return nodo;
}

//Funcion obtener altura del nodo
int alturaNodo(struct NODOAVL* nodo)
{
    //Excepcion nodo es igual a null
    if(nodo == NULL)
    {
        return 0;
    }
    else
    {
        return nodo->altura;
    }
}

//Funcion balance nodo
int balance(struct NODOAVL* nodo)
{
    //Excepcion nodo es igual a null
    if(nodo == NULL)
    {
        return 0;
    }
    return alturaNodo(nodo->izquierda) - alturaNodo(nodo->derecha);
}

//Funcion retornar el numero mayor
int max(int a,int b)
{
    return (a > b) ? a : b;
}

//Retornar el nodo sucesor del arbol
struct NODOAVL* maxValorNodo(struct NODOAVL* nodo)
{
    //Var auxiliar nodo sucesor
    struct NODOAVL* nodoAux = nodo->derecha;
    //Excepcion nodo es null
    if(nodoAux == NULL)
    {
        return nodo;
    }
    nodo->derecha = maxValorNodo(nodo->derecha);
}

//Retornar el nodo predecesor del arbol
struct NODOAVL* minValorNodo(struct NODOAVL* nodo)
{
    //Var auxiliar nodo predecesor
    struct NODOAVL* nodoAux = nodo->izquierda;
    //Excepcion nodo es null
    if(nodoAux == NULL)
    {
        return nodo;
    }
    nodo->izquierda = minValorNodo(nodo->izquierda);
}

//Funcion rotacion derecha arbol AVL
struct NODOAVL* rotacionDerecha(struct NODOAVL* nodo)
{
    struct NODOAVL* z = nodo->izquierda;
    struct NODOAVL* t2 = z->derecha;
    z->derecha = nodo;
    nodo->izquierda = t2;
    z->altura = max(alturaNodo(z->izquierda),alturaNodo(z->derecha))+ 1;
    nodo->altura = max(alturaNodo(nodo->izquierda),alturaNodo(nodo->derecha))+ 1;
    return nodo;
}

//Funcion rotacion izquierda arbol AVL
struct NODOAVL* rotacionIzquierda(struct NODOAVL* nodo)
{
    struct NODOAVL* z = nodo->derecha;
    struct NODOAVL* t2 = z->izquierda;
    z->izquierda = nodo;
    nodo->derecha = t2;
    z->altura = max(alturaNodo(z->izquierda),alturaNodo(z->derecha))+ 1;
    nodo->altura = max(alturaNodo(nodo->izquierda),alturaNodo(nodo->derecha))+ 1;
    return nodo;
}

//Insertar tarea en arbol
struct NODOAVL* insertarNodo(struct NODOAVL* nodo,struct TAREA* tarea)
{
    //Excepcion nodo es igual null
    if(nodo == NULL)
    {
        nodo = crearNodo(tarea);
        return nodo;
    }
    //Excepcion nombre de tarea es mayor nodo
    else if(strcmp(tarea->nombre,nodo->tarea->nombre) > 0)
    {
       nodo->derecha = insertarNodo(nodo->derecha,tarea);
    }
    //Excepcion nombre de tarea es menor nodo
    else if(strcmp(tarea->nombre,nodo->tarea->nombre) < 0)
    {
        nodo->izquierda = insertarNodo(nodo->izquierda,tarea);
    }
    else
    {
        return nodo;
    }
    //Asignar altura general
    nodo->altura = max(alturaNodo(nodo->izquierda),alturaNodo(nodo->derecha)) + 1;
    int balanceNodo = balance(nodo);
    //Excepcion rotacion derecha
    if (balanceNodo > 1 && strcmp(tarea->nombre,nodo->izquierda->tarea->nombre) < 0)
    {
        return rotacionDerecha(nodo);
    }
    //Excepcion rotacion izquierda
    if (balanceNodo < -1 && strcmp(tarea->nombre,nodo->derecha->tarea->nombre) > 0)
    {
        return rotacionIzquierda(nodo);
    }
    //Excepcion rotacion derecha
    if (balanceNodo > 1 && strcmp(tarea->nombre,nodo->izquierda->tarea->nombre) > 0)
    {
        nodo->izquierda =  rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }
    //Excepcion rotacion izquierda
    if (balanceNodo < -1 && strcmp(tarea->nombre,nodo->derecha->tarea->nombre) < 0)
    {
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }
    return nodo;
}

//Funcion impresion tarea
void imprimirTarea(struct TAREA* tarea)
{
    printf("================TAREA================");
    SALTARLINEA(2);
    printf("Nombre:%s\n",tarea->nombre);
    printf("Descripcion:%s\n",tarea->descripcion);
    printf("Fecha vencimiento:%s\n",tarea->fechaVence);
    printf("Fecha:%s\n",tarea->fechaInicio);
    //Excepcion estado completa
    if(tarea->estado == COMPLETA)
    {
        printf("Estado tarea:COMPLETA");
    }
    else if(tarea->estado == INCOMPLETA)
    {
        printf("Estado tarea:INCOMPLETA");
    }
    else
    {
        printf("Estado tarea:FALLIDA");
    }
    getchar();
}

int main()
{
    struct NODOAVL* nodo = NULL;
    return 0;
}

