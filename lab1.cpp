/****************************************
Esteban Gaete Flores
Mosheh-efra Landaeta Sanchez

Grupo cp7
Problema 7

Laboratorio n°1 BSP - Computación Paralela
Segundo Semestre del 2013
*****************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
extern "C" {
	#include <bsp.h>
	#include <stdlib.h>
	#include <math.h>
}

using namespace std;

class objeto{
	public:
		char tipo;
		double cord1[2];
		double cord2[2];
		int orden[2];
		double distancia;
	objeto(){
		tipo = ' ';
		cord1[0] = 0.0;
		cord1[1] = 0.0;
		cord2[0] = 0.0;
		cord2[1] = 0.0;
		orden[0] = 0;
		orden[1] = 0;
		distancia = 0.0;
	}
	
	void setfc(char t,double c10, double c11){
		tipo = t;
		cord1[0] = c10;
		cord1[1] = c11;
		cord2[0] = 0.0;
		cord2[1] = 0.0;
		distancia = 0.0;
	}
	
	void setsc(double c20, double c21){
		cord2[0] = c20;
		cord2[1] = c21;
	}
	
	void setord(int ord){
		if(tipo == 'R'){
			orden[0] = ord; //orden para la region
			orden[1] = 0;
		}else{
			orden[0] = 0;
			orden[1] = ord; //orden para el item
		}
	}
	
	void set_distancia(double d){
		distancia = d;
	}
	
	double distance(double dX1, double dY1)
	{
		return sqrt((dX1 - cord1[0])*(dX1 - cord1[0]) + (dY1 - cord1[1])*(dY1 - cord1[1]));
	}
};


class mouse{
	public:
		double cord[2];
		int orden;
	mouse(){
		cord[0] = 0.0;
		cord[1] = 0.0;
		orden = 0;
	}
	
	void setc(double c1, double c2, int o){
		cord[0] = c1;
		cord[1] = c2;
		orden = o;
	}
};

void bsp_main(){
	// ?
	int TAG = 0;

	// Da inicio al codigo paralelo 
	bsp_begin( bsp_nprocs( ) );
	
	// Obtiene la cantidad de procesos totales 
	int nprocs = bsp_nprocs( );
	
	// Asigna el pid para cada procesador 
	int pid = bsp_pid( );
	
/** Super Step 1 **/	
	vector<mouse> M;
	
	if(pid == 0){
		ifstream local_file;
		string linea;		
		objeto item;
		mouse mitem;
		
		local_file.open("input.txt");
		
		if(local_file.is_open()){
			getline(local_file,linea);
			string aux;
			int var[4];
			int pos1=2;
			int pos2;
			int i=0;
			int j1=1;
			int j2=1;
			
			//Lectura de los Iconos y Regiones
			while(linea[0] != 'M'){
				
				//Paso del string al objeto
				if(linea[0] == 'I'){				
					pos2 = linea.find(' ',pos1);
					aux = linea.substr(pos1,pos2-pos1);
					
					var[0] = atoi(aux.c_str());
					
					pos1=pos2+1;
					pos2 = linea.find(' ',pos1);
					aux = linea.substr(pos1,pos2-pos1);
					
					var[1] = atoi(aux.c_str());
					
					pos1=2;
					
					item.setfc(linea[0],(double)var[0],(double)var[1]);
					item.setord(j2);
					j2++;
				} else {
					pos2 = linea.find(' ',pos1);
					aux = linea.substr(pos1,pos2-pos1);
					
					var[0] = atoi(aux.c_str());
					
					pos1=pos2+1;
					pos2 = linea.find(' ',pos1);
					aux = linea.substr(pos1,pos2-pos1);
					
					var[1] = atoi(aux.c_str());
					
					pos1=pos2+1;
					pos2 = linea.find(' ',pos1);
					aux = linea.substr(pos1,pos2-pos1);
					
					var[2] = atoi(aux.c_str());
					
					pos1=pos2+1;
					pos2 = linea.find(' ',pos1);
					aux = linea.substr(pos1,pos2-pos1);
					
					var[3] = atoi(aux.c_str());
					
					pos1=2;
					
					item.setfc(linea[0],(double)var[0],(double)var[1]);
					item.setsc((double)var[2],(double)var[3]);
					item.setord(j1);
					j1++;
				}

				//envio de un item
				bsp_send( i,   //Procesador al que es enviado el dato
					  &TAG,
					  &item, //Dato enviado
					  sizeof( item ) * ( 1 ) //OJO: TIENEN que ser bytes, OJO: + 1
					);
						
				i++; //para los procesadores
	
				if(i==nprocs){
					i=0;
				}
				getline(local_file,linea);			
			}
			j1 = 1;
			//Lectura de los Mouse Click			
			while(linea[0] != '#'){
				pos2 = linea.find(' ',pos1);
				aux = linea.substr(pos1,pos2-pos1);
				
				var[0] = atoi(aux.c_str());
				
				
				pos1=pos2+1;
				pos2 = linea.find(' ',pos1);
				aux = linea.substr(pos1,pos2-pos1);
				
				var[1] = atoi(aux.c_str());
				
				pos1=2;
			
				mitem.setc((double)var[0],(double)var[1],j1);
				M.push_back(mitem);
				j1++;
				
				getline(local_file,linea);				
			}
			
		}else{
			cout << "No se puede abrir el Archivo" << endl;
		}
		
		local_file.close();
	}
	
/** Fin SS 1 **/
	bsp_sync( );

/** Super Step 2 **/
	
	int nm, nb, size;
	vector<objeto> lista_item;
	objeto item_recibido;
	
	//Obtiene el tamaño de elementos en la cola (nm) y el tamaño acomulado (nb)
	bsp_qsize( &nm, &nb );
	
	//obtener todos los mensajes que llegaron al procesador
	for( int i = 0; i < nm; i++ ){		
		//Obtiene el tag enviado
		bsp_get_tag( &size,
					 &TAG );

		bsp_move ( &item_recibido, size );
		
		lista_item.push_back(item_recibido);

	}
	
	//Procesador 0 hace broadcast del vector de mouse clicks
	if( pid == 0){
		for(int i=0;i<nprocs;i++){
			for(int j=0;j<M.size();j++){
				bsp_send( i,   //Procesador al que es enviado el dato
					  &TAG,
					  &M[j], //Dato enviado
					  sizeof( M[j] ) * ( 1 )
					);
			}
		}
	}

/** Fin SS 2 **/
	bsp_sync( );
	
/** Super Step 3 **/
	vector<mouse> mouse_clicks;
	mouse MC;
	
	//Obtiene el tamaño de elementos en la cola (nm) y el tamaño acomulado (nb)
	bsp_qsize( &nm, &nb );
	
	//obtener todos los mensajes que llegaron al procesador
	for( int i = 0; i < nm; i++ ){		
		//Obtiene el tag enviado
		bsp_get_tag( &size,
					 &TAG );

		bsp_move ( &MC, size );
		
		mouse_clicks.push_back(MC);
	}

	//Se tiene a vector mouse_clicks y vector lista_item
	vector<objeto> solucion_local;
	objeto aux;
	aux.set_distancia(1000000.0);
	aux.tipo = 'n';

	//Se calculan los clicks
	for(int i=0;i<mouse_clicks.size();i++){
		for(int j=0;j<lista_item.size();j++){
			if(lista_item[j].tipo == 'R'){
				if( mouse_clicks[i].cord[0] >= lista_item[j].cord1[0] &&
					mouse_clicks[i].cord[1] >= lista_item[j].cord1[1] &&
					mouse_clicks[i].cord[0] <= lista_item[j].cord2[0] &&
					mouse_clicks[i].cord[1] <= lista_item[j].cord2[1] ){
						if(lista_item[j].orden[0]>=aux.orden[0]){
							aux = lista_item[j];
						}
				}
			} else if(aux.tipo != 'R'){
				if(lista_item[j].distance(mouse_clicks[i].cord[0],mouse_clicks[i].cord[1])<aux.distancia){
					aux = lista_item[j];
					aux.set_distancia(lista_item[j].distance(mouse_clicks[i].cord[0],mouse_clicks[i].cord[1]));
				}
			}
		}
		solucion_local.push_back(aux);
		aux.set_distancia(1000000.0);
		aux.tipo = 'n';
	}
	
/** Super Steps Recursivos **/
	//Se envian los datos de la solucion local
	for(int i=0;i<solucion_local.size();i++){
		bsp_sync( );

		//Se envian todas las soluciones de un Mouse Click al P0		
		bsp_send( 0,   //Procesador al que es enviado el dato
			  &TAG,
			  &solucion_local[i], //Dato enviado
			  sizeof( solucion_local[i] ) * ( 1 )
			);
				
		bsp_sync( );
		
		if(pid == 0){
			vector<objeto> resultados;
			objeto objt;
			
			//Obtiene el tamaño de elementos en la cola (nm) y el tamaño acomulado (nb)
			bsp_qsize( &nm, &nb );
			
			//obtener todos los mensajes que llegaron al procesador
			for( int i = 0; i < nm; i++ ){		
				//Obtiene el tag enviado
				bsp_get_tag( &size,
	 				     &TAG );

				bsp_move ( &objt, size );
				
				resultados.push_back(objt);
			}

			//Obtiene la solucion final para el Mouse Click
			vector<objeto> solucion;
			solucion.push_back(resultados[0]);
			for( int i = 1; i < resultados.size(); i++ ){
				if(resultados[i].tipo == 'R' && solucion[0].tipo == 'I'){	
					solucion[0] = resultados[i];
				}else if(resultados[i].tipo == 'R' && solucion[0].tipo == 'R') {
					if(resultados[i].orden[0]>solucion[0].orden[0]){
						solucion[0] = resultados[i];
					}
				}else if(resultados[i].tipo == 'I' && solucion[0].tipo == 'I') {
					if(resultados[i].distancia<solucion[0].distancia){
						solucion[0] = resultados[i];
					}else if(resultados[i].distancia == solucion[0].distancia){
						solucion.push_back(resultados[i]);
					}
				}
			}
			
			if(solucion[0].tipo == 'R'){
				char letra = solucion[0].orden[0] + 64;
				cout << letra << endl;
			}else{
				vector<int> iconos;
				for(int j=0;j<solucion.size();j++){
					iconos.push_back(solucion[j].orden[1]);
				}
				sort(iconos.begin(),iconos.end());
				for(int i=0;i<iconos.size();i++){
					cout << "   " << iconos[i];
				}
				
				cout << endl;
			}			
		}
	}
/** Fin SS Recursivos **/

/** Fin SS 3 **/
	bsp_sync( );
	
	/* Finaliza el codigo paralelo */
	bsp_end();	
}

int main( int argc, char *argv[] ){

	/* bsp_main se replicará tantas veces como el comando -np diga
		la cantidad de procesadores para el paralelismo */
	bsp_init( bsp_main, argc, argv );

	bsp_main( );

	return 0;
}