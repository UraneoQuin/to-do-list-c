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

//Enumerador de importancia de tarea
enum{ALTA,MEDIA,BAJA};

//Estructura tarea
struct TAREA
{
    char nombre[1000];
    char descripcion[1000];
    int importancia;
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
void obtenerFecha(char* destino)
{
    time_t actual = time(NULL);
    struct tm* fechaLocal = localtime(&actual);
    strftime(destino,buffer,"%Y/%m/%d Hora:%H:%M:%S",fechaLocal);
}

//Funcion crear tarea
struct TAREA* crearTarea(char* nombre,char* descripcion,char* fechaVence,
                         int importancia)
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
    obtenerFecha(tarea->fechaInicio);
    tarea->importancia = importancia;
    tarea->estado = INCOMPLETA;
    return tarea;
}

//Interfaz crear tarea
struct TAREA* menuTarea()
{
    //Asignacion de memoria
    struct TAREA* aux;
    //Var aux entradas
    char nombre[buffer];
    char descripcion[buffer];
    char fechaVence[buffer];
    char importancia[buffer];
    //Entrada de usuario
    printf("===============CREAR TAREA=================");
    SALTARLINEA(2);
    printf("Digitar nombre:");
    fgets(nombre,buffer,stdin);
    nombre[strcspn(nombre,"\n")] = '\0';
    printf("Digitar descripcion:");
    fgets(descripcion,buffer,stdin);
    descripcion[strcspn(descripcion,"\n")] = '\0';
    printf("Digitar caducidad:");
    fgets(fechaVence,buffer,stdin);
    fechaVence[strcspn(fechaVence,"\n")] = '\0';
    printf("Digitar importancia (0-ALTA,1-MEDIA,2-BAJA):");
    fgets(importancia,buffer,stdin);
    importancia[strcspn(importancia,"\n")] = '\0';
    //Excepcion entrada importancia
    if(strcmp(importancia,"0") != 0 && atoi(importancia) == 0)
    {
        printf("Entrada invalida: por defecto importancia es ALTA\n");
    }
    aux = crearTarea(nombre,descripcion,fechaVence,atoi(importancia));
    return aux;
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
    //Eiterar y encontrar el valor mayor a la derecha
    while(nodo->derecha != NULL)
    {
        nodo = nodo->derecha;
    }
    return nodo;
}

//Retornar el nodo predecesor del arbol
struct NODOAVL* minValorNodo(struct NODOAVL* nodo)
{
    //Eiterar y econtrar el valor menor en la izquierda
    while(nodo->izquierda != NULL)
    {
        nodo = nodo->izquierda;
    }
    return nodo;
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
    return z;
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
    return z;
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

void imprimirTarea(struct TAREA* tarea)
{
    printf("===============TAREA=================");
    SALTARLINEA(2);
    printf("Nombre:%s\n",tarea->nombre);
    printf("Descripcion:%s\n",tarea->descripcion);
    printf("Fecha ingreso:%s\n",tarea->fechaInicio);
    printf("Fecha caducidad:%s\n",tarea->fechaVence);
    //Condicionales importancia de tarea
    switch(tarea->importancia)
    {
    case ALTA: printf("Importancia:ALTA\n"); break;
    case MEDIA: printf("Importancia:MEDIA\n"); break;
    case BAJA: printf("Importancia:BAJA\n"); break;
    }
    //Cond
    switch(tarea->estado)
    {
    case COMPLETA: printf("Estado:COMPLETA\n"); break;
    case INCOMPLETA: printf("Estado:INCOMPLETA\n"); break;
    case FALLIDA: printf("Estado:FALLIDA\n"); break;
    }
}

//Funcion liberar arbol AVL
void liberarAVL(struct NODOAVL* nodo)
{
    //Excepcion nodo es igual a null
    if(nodo == NULL)return;
    liberarAVL(nodo->izquierda);
    liberarAVL(nodo->derecha);
    //Liberar memoria de tarea y nodo
    free(nodo->tarea);
    free(nodo);
}
int main()
{
    return 0;
}

