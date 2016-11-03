#include <stdlib.h>
#include <stdio.h>
#include "random.h"
#include <ctype.h>
#include <time.h>
#include "buttonsAndScissorsFront.h"
#include "buttonsAndScissorsBack.h"

//aclaro que algunas cosas de final de archivo las tube que validar con \n y EOF porque con el editor de texto que tenia
//parece que automaticamente me agregaba un \n, no se si es por el editor o por linux, pero por las dudas valide las 2

static int existeArchivo( char * );
static int sobrescribir();
static int buscarMatriz(FILE*, int, int);
static int escribirMatriz(FILE*, char **, int);
static void liberarMatrizCuadrada(char **, int);
static char ** creaMatrizCuadrada(int);
static int leerArchivo(char *, tipoJuego * );


int cargarJuego(tipoJuego * juego){
    char c=0;
    int i=0;
    int error=SIN_ERROR;
    char * nombreArchivo=NULL;
    char * aux=NULL;
    printf("Ingrese el nombre del archivo a cargar: ");
    while((c=getchar())!='\n' && !error){
        if (i%BLOQUE==0){
            aux = realloc(aux, sizeof(char)*(i+BLOQUE));
            if (aux==NULL)
                error=E_MEM_DIN;
            else
                nombreArchivo=aux;
        }
        nombreArchivo[i]=c;
        i++;
    }
    if(!error){
        nombreArchivo[i]=0;
        nombreArchivo=realloc(nombreArchivo, sizeof(char)*i);
        error = leerArchivo(nombreArchivo, juego);
    }
    if(error)
        printError(error);
    return error;
}

static int leerArchivo(char * nombreArchivo, tipoJuego * juego){
    char c;
    int error=SIN_ERROR, flag=0,i,j;
    FILE * archivo;
    archivo=fopen(nombreArchivo, "rb");
    if (archivo!=NULL) {
        juego->tablero.v = creaMatrizCuadrada(juego->tablero.n);
        if (juego->tablero.v != NULL) {
            c = fgetc(archivo);
            if (c == '0' || c == '1') {
                juego->modoJuego = c;
                c = fgetc(archivo);
                if (c == '1' || c == '2') {
                    juego->turno = c;
                    for (i = 0; i < juego->tablero.n && !error; i++) {
                        for (j = 0; j < juego->tablero.n && !error; j++) {
                            c = fgetc(archivo);
                            if (isalpha(c))
                                juego->tablero.v[i][j] = c;
                            else{
                                error=E_ARCHIVO_MAL;
                            }
                        }
                    }

                }
                else
                    error=E_ARCHIVO_MAL;
            }
            else
                error=E_ARCHIVO_MAL;
        } else
            error = E_MEM_DIN;
    }
    else
        error=E_ABRIR_ARCHIVO;
    if(error && error!=E_MEM_DIN)
        liberarMatrizCuadrada(juego->tablero.v, juego->tablero.n);
    return error;
}

/*int guardarJuego(tipoJuego * juego){
    int error=SIN_ERROR;
    char whitespace=0;
    char nombreArchivo[MAXNOMBREARCHIVO];
    do{
        printf("Nombre del archivo?: ");
        scanf("%10s%c", nombreArchivo, &whitespace);
        if(whitespace=='\n'){
            if (existeArchivo(nombreArchivo)) {
                if (sobrescribir())
                    error = escribirArchivo(nombreArchivo, juego);
                else
                    error=NO_SOBRESCRIBIR;
            }
            else
                error=escribirArchivo(nombreArchivo, juego);
        }
        else{
            error = E_NOMBRE_ARCHIVO;
            limpiaBuffer
        }
        if(error)
            printError(error);
    }while (error);
    return error;
}
*/

int guardarJuego(char * nombreArchivo, tipoJuego * juego){
    int i,j;
    FILE * archivo;
    int error=SIN_ERROR;
    archivo = fopen(nombreArchivo, "wb");
    if (archivo!=NULL){
        fprintf(archivo,"%d%d", juego->modoJuego, juego->turno);
        for(i=0;i<juego->tablero.n;i++){
            for(j=0;j<juego->tablero.n;j++)
                fputc(juego->tablero.v[i][j], archivo);
        }
    }
    else
        error=E_CREAR_ARCHIVO;
    if (!error)
        fclose(archivo);
    return error;
}

/*static int sobrescribir(){
    int respuesta;
    do{
        printf("Hay un archivo con el mismo nombre, desea sobrescribirlo?(y/n) \n");
        respuesta=getchar();
        if(getchar()!='\n')
            limpiaBuffer
    }
    while( (respuesta!='y' && (respuesta!='n')));
    return 'n'-respuesta;
}
*/
//FUNCIONES QUE ERAN PARA VALIDAR Y PREGUNTAR SI SE SOBRESCRIBE UN ARCHIVO, COMO NO LO PIDE EN EL TP NO LAS AGREGAMOS.
/*
static int existeArchivo(char * nombreArchivo){
    int error = VERDADERO;
    FILE * aux;
    aux = fopen(nombreArchivo, "r");
    if (aux == NULL)
        error=FALSO;
    return error;
}
*/

void printError(int error){
    char * s_errores[]={"No hay error\n", "Error\n", "Error en la memoria dinamica\n", "Error, el archivo de matrices esta mal hecho\n",
                        "Error, el archivo esta corrupto o no existe\n", "Error en el nombre de archivo \n",
                        "Error al crear/sobrescribir el archivo \n", "El archivo esta corrupto o mal escrito\n",
                        "Elija otro nombre \n"};
    printf("%s\n", s_errores[error]);
    return;
}


char ** matrizDsdArchivo(int n){
    int error=SIN_ERROR;
    int i, c;
    char nombreArchivo[10] = {0}, whitespace=0;
    sprintf(nombreArchivo, "/home/ubuntu/ClionProjects/TPE/%dx%d", n,n );
    char ** aux=NULL;
    FILE * archivo;
    archivo = fopen(nombreArchivo, "r");
    if (archivo!=NULL) {
        fscanf(archivo, "%d%c", &c, &whitespace);
        if (whitespace != '\n') {
            error = E_ARCHIVO_MATRICES;
        }
        else {
           // error = buscarMatriz(archivo,randInt(1,c), n);
            if (!error) {
                aux = creaMatrizCuadrada(n);
                if (aux == NULL)
                    error = E_MEM_DIN;
                else {
                    error = escribirMatriz(archivo, aux, n);
                    if (error) {
                        error = E_ARCHIVO_MATRICES;
                        liberarMatrizCuadrada(aux, n);
                    }
                }
            }

        }
    }
    else
        error=E_ABRIR_ARCHIVO;
    if(error){
        printError(error);
        aux=NULL;
    }
    else
        fclose(archivo);
    return aux;
}

static int escribirMatriz(FILE * archivo, char ** matriz, int n){
    int i,j;
    char c;
    int error=SIN_ERROR;
    for(i=0; i<n && !error; i++){
        for(j=0; j<n && !error; j++){
            c=fgetc(archivo);
            if(isalpha(c))
                matriz[i][j]=c;
            else
                error=E_ARCHIVO_MATRICES;
        }
        c=fgetc(archivo);
        if(c!='\n' && c!=EOF)
            error=E_ARCHIVO_MATRICES;
    }
    return error;
}

static int buscarMatriz(FILE * archivo, int random, int n){
    int error=SIN_ERROR, flag=0, lineas=0;
    char c;
    while(random!=1 && !error){
        c=fgetc(archivo);
        if (isalpha(c)) {
            flag++;
        }
        else if(c=='\n' && flag==n) {
            flag = 0;
            lineas++;
        }
        else if(lineas==n && c=='-' && (c=fgetc(archivo))=='\n'){
            random--;
        }
        else
            error=E_ARCHIVO_MATRICES;
    }
    return error;
}


static char ** creaMatrizCuadrada(int n){
    int i,flag,j;
    char ** aux=NULL;
    aux=malloc(sizeof(*aux)*n);
    if(aux!=NULL){
        for (i=0, flag=1; i<n && flag; i++){
            aux[i]=malloc(sizeof(**aux)*n);
            if(aux[i]==NULL){
                flag=0;
                for(j=i-1;j>=0; j--)
                    free(aux[j]);
                free(aux);
            }
        }
    }
    return aux;
}


static void liberarMatrizCuadrada(char ** matriz, int n){
    int i;
    for(i=0; i<n; i++)
        free(matriz[i]);
    free(matriz);
    return;
}
