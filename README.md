# Minitp Semaforos 1S 2021
##### Introduccion:
En la cocina de Subway Argento compiten cuatro equipos de cocineros
para ganarse el puesto de trabajo en el local. ¿Quién será el primero en
entregar 1 sándwich de milanesa terminado, de 15 cm, al dueño del
local?

En este trabajo se demuestra la concurrencia y organizacion entre los equipos para con las acciones que deben realizar, para lograr su objetivo en simulacion a lo real.


##### pseucodigo:
los semaforos a utilizar:

mutex_salar

mutex_cocinarEnSarten

S_mezclar

S_salar

S_agregarCarne

S_empanarMilanesas

S_cocinarEnSarten

S_hornear

S_cortarExtras

S_armarSanguche

S_aux1;

S_aux2;

S_aux3;

•	El semaforo hornear se inicializa en 2 para que puedan entrar 2 panes.

•	los que son 1 solo recurso, voy a usar mutex.

•	El resto de los semaforos se inicializan en 0.

entonces la funcion cortar dara señal a mezclar.

P(S_mezclar)

 S_mezclar

V(S_salar)

la funcion salar dara señal a salar y a aux1,  con un mutex se bloquea al salero para que una persona pueda utilizarlo a la vez.

lock (mutex_salar)

P(S_salar)

S_salar

V(S_agregarCarne)

V(aux1)

unlock(mutex_salar)

la funcion agregarCarne dara señal a empanarMilanesas.

P(S_agregarCarne)

S_agregarCarne

V(S_empanarMilanesas)

la funcion empanarMilanesas dara señal a cocinarEnSarten.

P(S_empanarMilanesas)

S_empanarMilansesas

V(cocinarEnSarten)

la funcion cocinarEnSarten dara señal a cortarExtras, con un mutex se bloquea a cocinarEnSarten para que una persona pueda utilizarlo a la vez.

lock(mutex_cocinarEnSarten)

P(cocinarEnSarten)

S_cocinarEnSarten

V(cortarExtras)

unlock(mutex_cocinarEnSarten)

la funcion hornear dara señal a si mismo y a aux2.

P(S_hornear)

S_hornear

V(hornear)

V(aux2)

la funcion cortarExtras dara señal a aux3.

P(cortarExtras)

S_cortarExtras

V(aux3)

los semaforos aux1, aux2, aux3 deben recibir una señal para que la funcion armarSanguche se ejecute.

P(aux1)

P(aux2)

P(aux3)

S_armarSanguche

##### prueba de escritorio:

S_Mezclar	1	0	0	0	0	0	0	0  	0    
  
S_Salar	0	1	0	0	0	0	0	0	0 

S_AgregarCarne	0	0	1	0	0	0	0	0	0

S_EmpanarMilanesas	0	0	0	1	0	0	0	0	0

S_CocinarEnSarten	0	0	0	0	1	0	0	0	0

S_UtilizarHorno	2	2	2	2	2	2	0	0	0

CortarExtras	0	0	0	0	0	0	1	0	0

S_ArmarSanguche	0	0	1	1	1	"1+1"	"1+1"	"1+1+1"	3 0




