// -----------------------------------------------------------------------------
//
// Alberto Robles Hernandez
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: cena_de_los_filosofos.cpp
// Implementación del problema de los filósofos (con camarero).
//
// -----------------------------------------------------------------------------

/**********************************************************************************/
/* COMPILAR: mpicxx -std=c++11 -o cena_de_los_filosofos cena_de_los_filosofos.cpp */
/* EJECUTAR: mpirun -np 11 ./cena_de_los_filosofos ********************************/
/**********************************************************************************/

#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
   num_filosofos = 5,
   n_tenedores_y_filo = 2*num_filosofos,
   id_camarero   =  n_tenedores_y_filo,
   num_procesos  = id_camarero+1,
   elementos_a_enviar = 1,
   tag = 0,
   tag_sentarse = 1,
   tag_levantarse = 2;
// T: tenedor
// F: filosofo
// F , T , F , T , F , T , F , T , F , T

//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

// ---------------------------------------------------------------------

void funcion_filosofos( int id )
{
  int id_ten_izq = (id+1)              % n_tenedores_y_filo, //id. tenedor izq.
      id_ten_der = (id+n_tenedores_y_filo-1) % n_tenedores_y_filo; //id. tenedor der.
  int vacio;


  while ( true )
  {
  
    MPI_Ssend(&vacio, elementos_a_enviar, MPI_INT, id_camarero, tag_sentarse, MPI_COMM_WORLD);
    cout <<"Filósofo " <<id << " va a sentarse. " << endl << flush;

    cout <<"Filósofo " <<id << " solicita ten. " <<id_ten_izq <<endl << flush;
    MPI_Ssend(&vacio, elementos_a_enviar, MPI_INT, id_ten_izq, tag, MPI_COMM_WORLD);

    cout <<"Filósofo " <<id <<" solicita ten. " <<id_ten_der <<endl << flush;
    MPI_Ssend(&vacio, elementos_a_enviar, MPI_INT, id_ten_der, tag, MPI_COMM_WORLD);

    cout <<"Filósofo " <<id <<" comienza a comer" <<endl << flush;
    sleep_for( milliseconds( aleatorio<10,100>() ) );

    cout <<"Filósofo " <<id <<" suelta ten. " <<id_ten_izq <<endl << flush;
    MPI_Ssend(&vacio, elementos_a_enviar, MPI_INT, id_ten_izq, tag, MPI_COMM_WORLD);

    cout<< "Filósofo " <<id <<" suelta ten. " <<id_ten_der << endl << flush;
    MPI_Ssend(&vacio, elementos_a_enviar, MPI_INT, id_ten_der, tag, MPI_COMM_WORLD);

    MPI_Ssend(&vacio, elementos_a_enviar, MPI_INT, id_camarero, tag_levantarse, MPI_COMM_WORLD);
    cout <<"Filósofo " <<id << " va a levantarse. " << endl << flush;

    cout << "Filósofo " << id << " comienza a pensar" << endl << flush;
    sleep_for( milliseconds( aleatorio<10,100>() ) );
 }
}
// ---------------------------------------------------------------------

void funcion_tenedores( int id )
{
  int valor, id_filosofo;  // valor recibido, identificador del filósofo
  MPI_Status estado ;       // metadatos de las dos recepciones
 
  while ( true )
  {
     MPI_Recv ( &valor, elementos_a_enviar, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,&estado );
     id_filosofo = estado.MPI_SOURCE;
     cout <<"Ten. " <<id <<" ha sido cogido por filo. " <<id_filosofo << endl << flush;

     MPI_Recv ( &valor, elementos_a_enviar, MPI_INT, id_filosofo, tag, MPI_COMM_WORLD,&estado );
     cout <<"Ten. "<< id<< " ha sido liberado por filo. " <<id_filosofo << endl << flush;
  }
}
// ---------------------------------------------------------------------

void funcion_camarero()
{
	int sentados = 0,
		vacio,
	    etiqueta;
	MPI_Status estado ;
	while ( true )
  	{
		if(sentados < num_filosofos-1)
			etiqueta = MPI_ANY_TAG;
		else
			etiqueta = tag_levantarse;

		MPI_Recv( &vacio, elementos_a_enviar, MPI_INT, MPI_ANY_SOURCE, etiqueta, MPI_COMM_WORLD,&estado );

		if(estado.MPI_TAG == tag_sentarse)
			sentados++;
		else
			sentados--;
		
	}
}

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual ;

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if ( num_procesos == num_procesos_actual )
   {
      // ejecutar la función correspondiente a 'id_propio'
	  if( id_propio < id_camarero){
		  if ( id_propio % 2 == 0 )          // si es par
		     funcion_filosofos( id_propio ); //   es un filósofo
		  else                               // si es impar
		     funcion_tenedores( id_propio ); //   es un tenedor
	  }
	  else
		 funcion_camarero();
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   MPI_Finalize( );
   return 0;
}

// ---------------------------------------------------------------------
