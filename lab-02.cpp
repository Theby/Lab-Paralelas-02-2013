/************************************************
Ingeniería  Civil en Informatica
Esteban Gaete Flores
Mosheh-efra Landaeta Sanchez

Grupo cp7
Problema 7

Laboratorio n°2 MPI - Computación Paralela
Segundo Semestre del 2013
*****************************************/

#include <mpi.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
extern "C" {
	#include <stdlib.h>
	#include <math.h>
}

using namespace std;

int distance2(int dX1, int dY1, int dX2, int dY2){
		return sqrt((dX1 - dX2)*(dX1 - dX2) + (dY1 - dY2)*(dY1 - dY2));
}

int main( int argc, char *argv[] ){	

	MPI_Init(&argc,&argv);
	/* Items se estructura de la siguiente forma:
			( tipo,cord1_x,cord1_y,cord2_x,cord2_y,orden_r,orden_i, distancia ) 
			por lo que cada linea contiene 8 ints (0->7)
		*/
	int* items;
	int cant_items;
	
	//usado para poder mandar y recibir con el mismo tamaño
	int tam_array;	
	
	// Guardará id del proceso (pid) y total de procesos
	int rank,size;
	
	/* mouse es (cord_x,cord_y,orden) */
	int* mouse;
	int cant_soluciones;
	
	//Se tiene a array mouse y array items
	int* solucion_local;
	int* aux;
	
	//Para el status del mpi
	MPI_Status status;
	
	// Obtiene la cantidad de procesos totales, el resultado es guardado en size
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// Asigna el pid para cada procesador, el resultado es guardado en rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	

/** Super Step 1 **/	
	//Para la lectura de I y R
	if(rank == 0){
		ifstream local_file;
		string linea;
		int n_lineas=0;
		int offset = 0;
		int tam_pack;	
		
		local_file.open("input.txt");
		linea[0] = 'x';
		
		if(local_file.is_open()){
			/* Contando el numero de regiones e iconos */
			while(linea[0] != 'M'){
				getline(local_file,linea);
				n_lineas++;
			}
			
			tam_pack = n_lineas/size;
			if(n_lineas%size!=0){
				offset = 1;
			}
			
			/* Memoria necesaria para el arreglo que se enviará a cada proceso*/
			tam_array = 8*(tam_pack+offset);
			items = (int*)malloc(sizeof(int)*tam_array);		
			
			local_file.seekg(0);		
		
			getline(local_file,linea);
			string aux;
			
			int pos1=2;
			int pos2;
			int j1=1;
			int j2=1;
			int procesos;
			int leida;
			
			//Por cada proceso(size) se leen las lineas correspondientes(tam_pack)
			for(procesos=0;procesos<size;procesos++){
			
				//Si es que hay que leer una linea extra para el ultimo proceso
				if(offset==1 && procesos+1==size){
					tam_pack++;
					offset=0;
				}
			
				//Lectura de los Iconos y Regiones
				for(leida=0;leida<tam_pack;leida++){
					
					//Paso del string al arreglo
					if(linea[0] == 'I'){
						//guarda el nombre
						items[0+leida*8] = linea[0];
					
						pos2 = linea.find(' ',pos1);
						aux = linea.substr(pos1,pos2-pos1);
						
						//guarda la primera cordenada
						items[1+leida*8] = atoi(aux.c_str());
						
						pos1=pos2+1;
						pos2 = linea.find(' ',pos1);
						aux = linea.substr(pos1,pos2-pos1);
						
						//guarda la segunda cordenada
						items[2+leida*8] = atoi(aux.c_str());
					
						pos1=2;

						//tercera y cuarta cordenada
						items[3+leida*8] = 0;
						items[4+leida*8] = 0;

						//orden region
						items[5+leida*8] = 0;
		
						//orden item
						items[6+leida*8] = j2;

						j2++;
						
						//distacia
						items[7+leida*8] = 0;

					} else {
						//guarda el nombre
						items[0+leida*8] = linea[0];
					
						pos2 = linea.find(' ',pos1);
						aux = linea.substr(pos1,pos2-pos1);
						
						//guarda la primera cordenada
						items[1+leida*8] = atoi(aux.c_str());
						
						pos1=pos2+1;
						pos2 = linea.find(' ',pos1);
						aux = linea.substr(pos1,pos2-pos1);
						
						//guarda la segunda cordenada
						items[2+leida*8] = atoi(aux.c_str());
						
						pos1=pos2+1;
						pos2 = linea.find(' ',pos1);
						aux = linea.substr(pos1,pos2-pos1);
						
						//guarda la tercera cordenada
						items[3+leida*8] = atoi(aux.c_str());
						
						pos1=pos2+1;
						pos2 = linea.find(' ',pos1);
						aux = linea.substr(pos1,pos2-pos1);
						
						//guarda la cuarta cordenada
						items[4+leida*8] = atoi(aux.c_str());
						
						pos1=2;
						
						//orden region
						items[5+leida*8] = j1;
						j1++;
						
						//orden item
						items[6+leida*8] = 0;
						
						//distacia
						items[7+leida*8] = 0;
					}
					
					getline(local_file,linea);			
				}
				
				//Calculo de última linea
				if(offset==1){
					//guarda el nombre
					items[0+leida*8] = 'x';
					
					//guarda la primera cordenada
					items[1+leida*8] = -1;
					
					//guarda la segunda cordenada
					items[2+leida*8] = -1;
					
					//guarda la tercera cordenada
					items[3+leida*8] = -1;
									
					//guarda la cuarta cordenada
					items[4+leida*8] = -1;
					
					//orden region
					items[5+leida*8] = -1;
					
					//orden item
					items[6+leida*8] = -1;
					
					//distacia
					items[7+leida*8] = -1;
				}

				//envio de un item
				MPI_Send(&items //referencia al vector de elementos a enviar
						 ,tam_array // tamaño del vector a enviar
						 ,MPI_INT // Tipo de dato que envias
						 ,procesos // pid del proceso destino
						 ,0 //etiqueta
						 ,MPI_COMM_WORLD); //Comunicador por el que se manda
			}
		}else{
			cout << "No se puede abrir el Archivo" << endl;
			exit(0);
		}
		
		free(items);
		local_file.close();
	}
	
	//Barrera
	MPI_Barrier(MPI_COMM_WORLD);
	tam_array = 0;
	
	MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &tam_array);
	
	items = (int*)malloc(sizeof(int)*tam_array);
	
	MPI_Recv(&items // Referencia al vector donde se almacenara lo recibido
 		,tam_array // numero de elementos maximo a recibir
		,MPI_INT // Tipo de dato que recibe
		,MPI_ANY_SOURCE // pid del proceso origen de la que se recibe
		,0 // etiqueta
		,MPI_COMM_WORLD // Comunicador por el que se recibe
		,&status); // estructura informativa del status
	
	cant_items = tam_array;

	//Para las lecturas de los mouse
	if(rank == 0){
		ifstream local_file;
		string linea;
		int n_lineas=0;
		
		local_file.open("input.txt");
		linea[0] = 'x';
		
		if(local_file.is_open()){
			/* Contando el numero de mouse clicks */
			while(linea[0] != 'M'){
				getline(local_file,linea);			
			}
			
			while(linea[0] != '#'){
				n_lineas++;
				getline(local_file,linea);			
			}
			
			/* Memoria necesaria para el arreglo que se enviará a cada proceso*/
			tam_array = 3*(n_lineas);
			mouse = (int*)malloc(sizeof(int)*tam_array);		

			local_file.clear();
			local_file.seekg(0);						
			getline(local_file,linea);
			while(linea[0] != 'M'){
				getline(local_file,linea);			
			}			
			string aux;	
			
			int pos1=2;
			int pos2;
			int j1=1;
			int leida;			
			
			//Lectura de los Mouse Click			
			for(leida=0;leida<n_lineas;leida++){
				pos2 = linea.find(' ',pos1);
				aux = linea.substr(pos1,pos2-pos1);
				
				mouse[0+3*leida] = atoi(aux.c_str());				
				
				pos1=pos2+1;
				pos2 = linea.find(' ',pos1);
				aux = linea.substr(pos1,pos2-pos1);
				
				mouse[1+3*leida] = atoi(aux.c_str());
				
				pos1=2;
			
				mouse[2+3*leida] = j1;
				j1++;
				
				getline(local_file,linea);				
			}
			
			int procesos;		
			for(procesos=0;procesos<size;procesos++){
				//envio del arreglo
				MPI_Send(&mouse //referencia al vector de elementos a enviar
						 ,tam_array // tamaño del vector a enviar
						 ,MPI_INT // Tipo de dato que envias
						 ,procesos // pid del proceso destino
						 ,10 //etiqueta
						 ,MPI_COMM_WORLD); //Comunicador por el que se manda
			}			
		}else{
			cout << "No se puede abrir el Archivo" << endl;
			exit(0);
		}			
		
		free(mouse);
		local_file.close();
	}
	
	//Barrera
	MPI_Barrier(MPI_COMM_WORLD);

	tam_array = 0;

	MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &tam_array);

	mouse = (int*)malloc(sizeof(int)*tam_array);	
	
	MPI_Recv(&mouse // Referencia al vector donde se almacenara lo recibido
			,tam_array // numero de elementos máximo a recibir
			,MPI_INT // Tipo de dato que recibe
			,0 // pid del proceso origen de la que se recibe
			,10 // etiqueta
			,MPI_COMM_WORLD // Comunicador por el que se recibe
			,&status); // estructura informativa del status
	
	cant_soluciones = tam_array;
/** Fin SS 1 **/	
	
	//Barrera
	MPI_Barrier(MPI_COMM_WORLD);
	
/** Super Step 2 **/
		
	cant_items = cant_items/8;
	cant_soluciones = cant_soluciones/3;
	
	/* Una solucion por mouse click */
	solucion_local = (int*)malloc(sizeof(int)*8*cant_soluciones);
	
	aux = (int*)malloc(sizeof(int)*8);
	
	if(aux == NULL){
		cout << "NULL CSM D:!" << endl;
	}
	//Se inicializa aux
	for(int k=0;k<8;k++){
		aux[k] = 0;
	}
	aux[0] = 64;
	aux[5] = 0;
	aux[6] = 0;
	aux[7] = 100000;
	
	//Se calculan los clicks localmente
	for(int i=0;i<cant_soluciones;i++){
		for(int j=0;j<cant_items;j++){
			//Si es region
			if(items[0+j*8] == 82){ //82 = R
				if( mouse[0+i*3] >= items[1+j*8] &&
					mouse[1+i*3] >= items[2+j*8] &&
					mouse[0+i*3] <= items[3+j*8] &&
					mouse[1+i*3] <= items[4+j*8] ){
						if(items[5+j*8]>=aux[5]){
							//Se igualan los arreglos
							for(int k=0;k<8;k++){
								aux[k] = items[k+j*8];
							}
						}
				}
			//Si no es region
			} else if(aux[0] != 82){
				int dist;
				dist = distance2(mouse[0+i*3],mouse[1+i*3],items[1+j*8],items[2+j*8]);
				if(dist<aux[7]){
					//Se igualan los arreglos
					for(int k=0;k<7;k++){
						aux[k] = items[k+j*8];
					}
					aux[7] = dist;
				}
			}
		}
		
		//se agrega a la solucion local
		for(int k=0;k<8;k++){
			solucion_local[k+i*8] = aux[k];
		}		
		
		//Se inicializa aux
		for(int k=0;k<8;k++){
			aux[k] = 0;
		}
		aux[0] = 64;
		aux[5] = 0;
		aux[6] = 0;
		aux[7] = 100000;
	}
/** Fin SS 2 **/	
	
cout << "Antes-B4 " << rank << endl; MPI_Finalize(); exit(10);
	
/** Super Steps Recursivos **/
	//Se envian los datos de la solucion local
	for(int i=0;i<cant_soluciones;i++){
		//Barrera
		MPI_Barrier(MPI_COMM_WORLD);
	
		//Se envian todas las soluciones de un Mouse Click al P0, se envian de a uno	
		MPI_Send(&solucion_local[i] //referencia al item a enviar
				 ,1 // tamaño del vector a enviar
				 ,MPI_INT // Tipo de dato que envias
				 ,0 // pid del proceso destino
				 ,100 //etiqueta
				 ,MPI_COMM_WORLD); //Comunicador por el que se manda
		
		//Barrera
		MPI_Barrier(MPI_COMM_WORLD);
		
		if(rank == 0){
			//Se reciben las soluciones locales
			int* resultados;
			int cant_resultados;
			tam_array = 0;
			
			MPI_Probe(0, 100, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &tam_array);
			
			resultados = (int*)malloc(sizeof(int)*tam_array);	
			
			MPI_Recv(&resultados // Referencia al vector donde se almacenara lo recibido
					,tam_array // numero de elementos máximo a recibir
					,MPI_INT // Tipo de dato que recibe
					,MPI_ANY_SOURCE // pid del proceso origen de la que se recibe
					,100 // etiqueta
					,MPI_COMM_WORLD // Comunicador por el que se recibe
					,&status); // estructura informativa del status
					
			cant_resultados = tam_array;

			//Obtiene la solucion final para el Mouse Click
			int* solucion_aux;
			solucion_aux = (int*)malloc(sizeof(int)*8);	
			
			//Se toma el primer resultado como solución inicial
			for(int k=0;k<8;k++){
				solucion_aux[k] = resultados[k];
			}			
			
			//Por si hay multiples soluciones
			vector<int> iconos;
			
			for( int i = 1; i < cant_resultados; i++ ){
				if(resultados[0+i*8] == 82 && solucion_aux[0] == 73){	// R e I, 82 y 73
					//Se igualan los arreglos
					for(int k=0;k<8;k++){
						solucion_aux[k] = resultados[k+i*8];
					}
				}else if(resultados[0+i*8] == 82 && solucion_aux[0] == 82) { // R y R
					//Revisa el orden
					if(resultados[5+i*8]>solucion_aux[5]){
						//Se igualan los arreglos
						for(int k=0;k<8;k++){
							solucion_aux[k] = resultados[k+i*8];
						}
					}
				}else if(resultados[0+i*8] == 73 && solucion_aux[0] == 73) { // I e I
					//Revisa distancias
					if(resultados[7+i*8]<solucion_aux[7]){
						//Se igualan los arreglos
						for(int k=0;k<8;k++){
							solucion_aux[k] = resultados[k+i*8];
						}
					}else if(resultados[7+i*8] == solucion_aux[7]){
						//print del icono en resultado
						iconos.push_back(resultados[6+i*8]);
					}
				}
			}
			
			if(solucion_aux[0] == 82){
				char letra = solucion_aux[5] + 64;
				cout << letra << endl;
			}else{
				iconos.push_back(solucion_aux[6]);
				sort(iconos.begin(),iconos.end());
				for(int i=0;i<iconos.size();i++){
					cout << "   " << iconos[i];
				}
				
				cout << endl;
			}			
		}
	}
/** Fin SS Recursivos **/

	//Barrera
	MPI_Barrier(MPI_COMM_WORLD);

	/* Finaliza el codigo paralelo */
	MPI_Finalize(); 

	return 0;
}
