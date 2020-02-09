# La-cena-de-los-filosofos-mediante-paso-de-mensajes-MPI
Para ejecutar: mpirun -np "numero_de_procesos" ./cena_de_los_filosofos

ejemplo: mpirun -np 11 ./cena_de_los_filosofos

Alberto Robles Hernandez

sistemas concurrentes y Distribuidos.
Práctica 3. Implementación de algoritmos distribuidos con MPI

Archivo: cena_de_los_filosofos.cpp
Implementación del problema de los filósofos (con camarero).

Enunciado del problema:
  Cinco filósofos se sientan alrededor de una mesa y pasan su vida cenando y pensando. Cada filósofo tiene un plato de fideos y
  un tenedor a la izquierda de su plato. Para comer los fideos son necesarios dos tenedores y cada filósofo sólo puede tomar 
  los que están a su izquierda y derecha. Si cualquier filósofo toma un tenedor y el otro está ocupado, se quedará esperando, 
  con el tenedor en la mano, hasta que pueda tomar el otro tenedor, para luego empezar a comer.

  Si dos filósofos adyacentes intentan tomar el mismo tenedor a una vez, se produce una condición de carrera: ambos compiten
  por tomar el mismo tenedor, y uno de ellos se queda sin comer.

  Si todos los filósofos toman el tenedor que está a su derecha al mismo tiempo, entonces todos se quedarán esperando 
  eternamente, porque alguien debe liberar el tenedor que les falta. Nadie lo hará porque todos se encuentran en la misma 
  situación (esperando que alguno deje sus tenedores). Entonces los filósofos se morirán de hambre. Este bloqueo mutuo se 
  denomina interbloqueo o deadlock.
  
