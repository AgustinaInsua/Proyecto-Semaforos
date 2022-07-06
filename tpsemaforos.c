#include  <stdio.h>       // libreria estandar
#include  <stdlib.h>      // para usar exit y funciones de la libreria standard
#include  <string.h>
#include  <pthread.h>     // para usar hilos
#include  <semaphore.h>   // para usar semaforos
#include  <unistd.h>

# define  LIMITE  50

pthread_mutex_t mutex_salar ;
pthread_mutex_t mutex_cocinarEnSarten ;
sem_t sem_hornear;

// creo estructura de semaforos
struct semaforos {
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_agregarCarne;
    sem_t sem_empanarMilanesas;
    sem_t sem_cocinarEnSarten;
    sem_t sem_cortarExtras;
    sem_t sem_armarSanguche;
    sem_t sem_armarSanguche1;
    sem_t sem_armarSanguche2;
    sem_t sem_armarSanguche3;

};

// creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[ 4 ] [LIMITE];

};

// creo los parametros de los hilos
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param [9];
};

// funcion para imprimir las acciones y los ingredientes de la accion
void * imprimirAccion ( void * data, char * accionIn) {
	//se escriben los pasos e ingredientes en un archivo
        FILE *log;
        log = fopen("log.txt", "a");
	struct parametro * mydata = data;
	// calculo la longitud del array de pasos
	int sizeArray = ( int ) ( sizeof (mydata-> pasos_param ) / sizeof (mydata-> pasos_param [ 0 ]));
	// indice para recorrer array de pasos
	int i;
	for (i = 0 ; i <sizeArray; i ++) {
		// pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if ( strcmp (mydata-> pasos_param [i]. accion , accionIn) == 0 ) {
		printf ("\t Equipo %d - accion %s  \n  " , mydata-> equipo_param , mydata-> pasos_param [i]. accion );
		// calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = ( int ) ( sizeof (mydata-> pasos_param [i]. ingredientes ) / sizeof (mydata-> pasos_param [i]. ingredientes [ 0 ]));
		// indice para recorrer array de ingredientes
		int h;
		printf ( "\t Equipo %d ----------- ingredientes: ---------- \n " , mydata-> equipo_param );
			for (h = 0 ; h <sizeArrayIngredientes; h ++) {
				// consulto si la posicion tiene valor porque no hay cuantos ingredientes tengo por accion
				if ( strlen (mydata-> pasos_param [i].ingredientes[h]) != 0 ) {
					fprintf(log,"\t%s \n",mydata->pasos_param[i].ingredientes[h]);
							printf ("\t Equipo %d ingrediente   %d : %s  \n " , mydata-> equipo_param , h, mydata-> pasos_param [i]. ingredientes [h]);
				}
			}
		}
	}
	fclose(log);
}

void * cortar ( void *data) {
	// creo el nombre de la accion de la funcion
	char * accion = "cortar";
	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion (mydata, accion);
	// uso sleep para simular que que pasa tiempo
	sleep ( 3 );
	// doy la seÃƒÂ±al a la siguiente accion
        sem_post (& mydata-> semaforos_param . sem_mezclar );

    pthread_exit ( NULL );
}

void * mezclar ( void *data) {
	// creo el nombre de la accion de la funcion
	char * accion = "mezclar";
	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
        sem_wait(& mydata->semaforos_param.sem_mezclar);
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion (mydata, accion);
	// uso sleep para simular que que pasa tiempo
	sleep ( 3 );
	// doy la seÃƒÂ±al a la siguiente accion
        sem_post (& mydata-> semaforos_param . sem_salar );

    pthread_exit ( NULL );
}
void * salar ( void *data) {
       //se bloquea el mutex de la sal para que solo un equipo pueda utilizarlo a la vez
        pthread_mutex_lock(&mutex_salar);
	// creo el nombre de la accion de la funcion
	char * accion = "salar";
	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_salar);
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion (mydata, accion);
	// uso sleep para simular que que pasa tiempo
	sleep ( 3 );
	// doy la seÃƒÂ±al a la siguiente accion
        sem_post (& mydata-> semaforos_param . sem_agregarCarne );
        sem_post (& mydata-> semaforos_param . sem_armarSanguche1);
        //desbloqueo el salero para que otro equipo pueda utilizarlo
	pthread_mutex_unlock(&mutex_salar);
    pthread_exit ( NULL );
}
void * agregarCarne ( void *data) {
	// creo el nombre de la accion de la funcion
	char * accion = "agregarCarne";
	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_agregarCarne);
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion (mydata, accion);
	// uso sleep para simular que que pasa tiempo
	sleep ( 3 );
	// doy la seÃƒÂ±al a la siguiente accion
        sem_post (& mydata-> semaforos_param . sem_empanarMilanesas );

    pthread_exit ( NULL );
}
void * empanarMilanesas ( void *data) {
	// creo el nombre de la accion de la funcion
	char * accion = "empanarMilanesas";
	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_empanarMilanesas);
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion (mydata, accion);
	// uso sleep para simular que que pasa tiempo
	sleep ( 6);
	// doy la seÃƒÂ±al a la siguiente accion
        sem_post (& mydata-> semaforos_param . sem_cocinarEnSarten );

    pthread_exit ( NULL );
}

void * cocinarEnSarten ( void *data) {
        //se bloquea el mutex de cocinarEnSarten para que solo un equipo pueda utilizarlo a la vez
        pthread_mutex_lock(&mutex_cocinarEnSarten);
	// creo el nombre de la accion de la funcion
	char * accion = "cocinarEnSarten";
	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
       sem_wait(&mydata->semaforos_param.sem_cocinarEnSarten);
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion (mydata, accion);
	// uso sleep para simular que que pasa tiempo
	sleep ( 8 );
	// doy la seÃƒÂ±al a la siguiente accion
        sem_post (& mydata-> semaforos_param . sem_cortarExtras );
        //desbloqueo la sarten para que otro equipo pueda utilizarlo
        pthread_mutex_unlock(&mutex_cocinarEnSarten);

    pthread_exit ( NULL );
}

void * hornear(void *data) {

       sem_wait(&sem_hornear);
       //creo el nombre de la accion de la funcion
       char *accion = "hornear";
       //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
       struct parametro *mydata = data;
       //llamo a la funcion imprimir le paso el struct y la accion de la funcion
       imprimirAccion(mydata,accion);
       //uso sleep para simular que que pasa tiempo
       sleep(16);
       //liberamos el horno para que otros 2 equipos puedan hornear
       sem_post(&sem_hornear);
       //doy la señal a la siguiente accion
       sem_post(&mydata->semaforos_param.sem_armarSanguche2);

    pthread_exit(NULL);
}

void * cortarExtras ( void *data) {
       // creo el nombre de la accion de la funcion
       char * accion = "cortarExtras";
       // creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
       struct parametro *mydata = data;
       sem_wait(&mydata->semaforos_param.sem_cortarExtras);
       // llamo a la funcion imprimir le paso el struct y la accion de la funcion
       imprimirAccion (mydata, accion);
       // uso sleep para simular que que pasa tiempo
       sleep ( 3 );
       // doy la seÃƒÂ±al a la siguiente accion (cortar me habilita mezclar)
       sem_post (& mydata-> semaforos_param . sem_armarSanguche3);
    pthread_exit ( NULL );
}

void* armarSanguche(void *data) {
       //al archivo log le escribimos la parte final de la competencia
       FILE *log;
       log = fopen("log.txt", "a");
       //creo el nombre de la accion de la funcion
       char *accion = "- - - -";
       //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
       struct parametro *mydata = data;
       //se da la orden para verificar si se puede armar el sanguche
       sem_wait(&mydata->semaforos_param.sem_armarSanguche1);
       sem_wait(&mydata->semaforos_param.sem_armarSanguche2);
       sem_wait(&mydata->semaforos_param.sem_armarSanguche3);

       //llamo a la funcion imprimir le paso el struct y la accion de la funcion
       imprimirAccion(mydata,accion);
       //uso sleep para simular que que pasa tiempo
       sleep(1);
       //se muestra por pantalla y se escribe en el log el final
       printf("\tEl equipo %d gano!!! \n",mydata->equipo_param);
       fprintf(log,"\tEl equipo %d gano!!! \n",mydata->equipo_param);
       //se cierra el archivo
       fclose(log);
       //se termina el programa al haber un ganador
       exit(-1);

 pthread_exit(NULL);
}

void * ejecutarReceta ( void * i) {
        pthread_mutex_t mutex_salar ;
        pthread_mutex_t mutex_cocinarEnSarten ;
	// variables semaforos
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_agregarCarne;
        sem_t sem_empanarMilanesas;
        sem_t sem_cocinarEnSarten;
        sem_t sem_cortarExtras;
        sem_t sem_armarSanguche1;
	sem_t sem_armarSanguche2;
	sem_t sem_armarSanguche3;
	// crear variables semaforos aqui

	// variables hilos
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;
	pthread_t p9;
	// crear variables hilos aqui

	// numero del equipo (casteo el puntero a un int)
	int p = * (( int *) i);

	printf ( " Ejecutando equipo %d  \n" , p);

        usleep(5000);
	// reservo memoria para el struct
	struct parametro *pthread_data = malloc ( sizeof ( struct parametro));

	// seteo los valores al estructura

	// seteo numero de grupo
	pthread_data-> equipo_param = p;

	// seteo semaforos

	pthread_data-> semaforos_param . sem_mezclar = sem_mezclar;
	pthread_data-> semaforos_param . sem_salar = sem_salar;
	pthread_data-> semaforos_param . sem_agregarCarne = sem_agregarCarne;
	pthread_data-> semaforos_param . sem_empanarMilanesas = sem_empanarMilanesas;
	pthread_data-> semaforos_param . sem_cocinarEnSarten = sem_cocinarEnSarten;
	pthread_data-> semaforos_param . sem_cortarExtras = sem_cortarExtras;
	pthread_data-> semaforos_param . sem_armarSanguche1 = sem_armarSanguche1;
	pthread_data-> semaforos_param . sem_armarSanguche2= sem_armarSanguche2;
	pthread_data-> semaforos_param . sem_armarSanguche3 = sem_armarSanguche3;
        mutex_salar = mutex_salar;
        mutex_cocinarEnSarten = mutex_cocinarEnSarten;


        //se carga la receta desde un txt y van leyendo las acciones e ingredientes
        FILE *receta;
        receta = fopen("receta.txt","r");
        //se utiliza | como delimitador de acciones e ingredientes
        char delimitador[] = "|";
        char cadena[50];
        //utilizo contadores para guiar dentro del txt y seguir ya que strtok es destructiva y si por error volvemos hacia atras no tendremos forma de seguir la receta
        int cont = 0;
        int accion = 0;
        int alimentos = 0;
        while(fgets(cadena,100,receta) != NULL) {
        //Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
        char*token = strtok(cadena,delimitador);
        strcpy(pthread_data->pasos_param[accion].accion, token);
        //mientras no sea NULL el token podremos seguir con la receta
        while(token != NULL) {
        if(cont != 0) {
        strcpy(pthread_data->pasos_param[accion].ingredientes[alimentos], token);
        token = strtok(NULL, delimitador);
        alimentos++;}
        else {
        token = strtok(NULL, delimitador);
        cont++;}
        }
        cont = 0;
        alimentos = 0;
        accion++;
        }
        //cerramos el archivo de la receta
        fclose(receta);

	// inicializo los semaforos

	sem_init (& (pthread_data-> semaforos_param . sem_mezclar ), 0 , 0);
	sem_init (& (pthread_data-> semaforos_param . sem_salar ), 0 , 0);
        pthread_mutex_init(&(mutex_salar),NULL);
	sem_init (& (pthread_data-> semaforos_param . sem_agregarCarne ), 0 , 0);
	sem_init (& (pthread_data-> semaforos_param . sem_empanarMilanesas ), 0 , 0);
	sem_init (& (pthread_data-> semaforos_param . sem_cocinarEnSarten ), 0 , 0);
        pthread_mutex_init(&(mutex_cocinarEnSarten),NULL);
	sem_init (& (pthread_data-> semaforos_param . sem_cortarExtras), 0 , 0);
	sem_init (& (pthread_data-> semaforos_param . sem_armarSanguche1 ), 0 , 0);
	sem_init (& (pthread_data-> semaforos_param . sem_armarSanguche2 ), 0 , 0);
	sem_init (& (pthread_data-> semaforos_param . sem_armarSanguche3 ), 0 , 0);



	// creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos
    int rc;
        rc = pthread_create (& p1,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                cortar,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia

        rc = pthread_create (& p2,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                mezclar,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia
        rc = pthread_create (& p3,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                salar,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia
        rc = pthread_create (& p4,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                agregarCarne,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia
        rc = pthread_create (& p5,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                empanarMilanesas,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create (& p6,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                cocinarEnSarten,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create (& p7,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                cortarExtras,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create (& p8,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                hornear,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create (& p9,                            // identificador unico
                            NULL ,                           // atributos del hilo
                                armarSanguche,              // funcion a ejecutar
                                pthread_data);                     // parametros de la funcion a ejecutar, pasado por referencia



	// join de todos los hilos
	pthread_join (p1, NULL );
	pthread_join (p2, NULL );
	pthread_join (p3, NULL );
	pthread_join (p4, NULL );
	pthread_join (p5, NULL );
	pthread_join (p6, NULL );
	pthread_join (p7, NULL );
	pthread_join (p8, NULL );
	pthread_join (p9, NULL );



	// valido que el hilo se alla creado bien
    if(rc) {
       printf ( "Error: no se puede crear el hilo, %d  \n " , rc);
       exit(-1);
     }


	// destruccion de los semaforos
	sem_destroy (& sem_mezclar);
	sem_destroy (& sem_salar);
	sem_destroy (& sem_agregarCarne);
	sem_destroy (& sem_empanarMilanesas);
	sem_destroy (& sem_cocinarEnSarten);
        pthread_mutex_destroy(&mutex_salar);
        pthread_mutex_destroy(&mutex_cocinarEnSarten);
	sem_destroy (& sem_cortarExtras);
	sem_destroy (& sem_armarSanguche1);
	sem_destroy (& sem_armarSanguche2);
	sem_destroy (& sem_armarSanguche3);
	// destruir demas semaforos

	// salida del hilo
	 pthread_exit ( NULL );
}


int  main ()
{
        //inicio el horno y lo seteo para que lo usen 2 equipos
        sem_init(&(sem_hornear), 0, 2);
        //elimino el log previo al iniciar el programa
        remove ("log.txt");

	// creo los nombres de los equipos
	int rc;
	int * equipoNombre1 = malloc ( sizeof (* equipoNombre1));
	int * equipoNombre2 = malloc ( sizeof (* equipoNombre2));
	int * equipoNombre3 = malloc ( sizeof (* equipoNombre3));
	int * equipoNombre4 = malloc ( sizeof (* equipoNombre4));

	* equipoNombre1 = 1 ;
	* equipoNombre2 = 2 ;
	* equipoNombre3 = 3 ;
	* equipoNombre4 = 4 ;
	// creo las variables los hilos de los equipos
	pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;


	// inicializo los hilos de los equipos
        rc = pthread_create (& equipo1,                      // identificador unico
                            NULL ,                           // atributos del hilo
                                ejecutarReceta,              // funcion a ejecutar
                                equipoNombre1);

        rc = pthread_create (& equipo2,                      // identificador unico
                            NULL ,                           // atributos del hilo
                                ejecutarReceta,              // funcion a ejecutar
                                equipoNombre2);
        rc = pthread_create (& equipo3,                      // identificador unico
                            NULL ,                           // atributos del hilo
                                ejecutarReceta,              // funcion a ejecutar
                                equipoNombre3);
        rc = pthread_create (& equipo4,                      // identificador unico
                            NULL ,                           // atributos del hilo
                                ejecutarReceta,              // funcion a ejecutar
                                equipoNombre4);



   if(rc) {
       printf("Error: no se puede crear el hilo, %d  \n " , rc);
       exit(-1);
     }

	// join de todos los hilos
	pthread_join (equipo1, NULL );
	pthread_join (equipo2, NULL );
	pthread_join (equipo3, NULL );
	pthread_join (equipo4, NULL );

 //destruyo el semaforo del horno
 sem_destroy(&sem_hornear);

    pthread_exit ( NULL );
}

