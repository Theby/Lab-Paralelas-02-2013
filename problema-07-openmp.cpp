/************************************************
Ingenieria  Civil en Informatica
Esteban Gaete Flores
Mosheh-efra Landaeta Sanchez

Grupo cp7
Problema 7

Laboratorio n°4 OPENMP - Computacion Paralela
Segundo Semestre del 2013
*****************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
extern "C" {
	#include <omp.h>
	#include <stdlib.h>
	#include <math.h>
}

#include "Objeto.h"
#include "Mouse.h"

using namespace std;

/** Seccion de variables Globales */
	//Buffer del archivo a leer
	ifstream local_file;
/** Fin Seccion de variables globales */

void problema_7(int num_threads, vector<Mouse> mouse_click, bool debugg, int* tam_pack){
	//Guardara el id de cada hebra
	int tid;

	/** Debugg **/
		if(debugg){ 
			cout << endl << "Ejecutando con " << num_threads << " hebra(s)." << endl;
		}

	//Para guardar el objeto que se esta leyendo
	Objeto o_aux;

	//Para guardar los objetos leidos
	vector<Objeto> l_obj;

	//Para guardar la linea que se esta leyendo y las posiciones de lectura
	string linea;
	string aux;
	int var[4];
	int pos1;
	int pos2;

	//Para mantener el orden de aparicion de los objetos
	int orden_1=1;
	int orden_2=1;

	//Para guardar los resultados totales
	vector<Objeto> resultados;

	//creando threads solicitados
	omp_set_num_threads(num_threads);

	#pragma omp parallel default(none) private(num_threads, tid, o_aux, l_obj, linea, aux, var, pos1, pos2) shared(orden_1, orden_2, resultados, mouse_click, debugg, tam_pack, cout, local_file)
	{
		//Obtiene el ID
		tid = omp_get_thread_num();		

		/** Debugg **/
			if(debugg){ 
				cout << "Soy la " << tid << " de " << omp_get_num_threads() << "." << endl; 
			}

		//Se inicializan variables necesarias para el correcto funcionamiento
		pos1=2;
		pos2=0;

		//Cierra la seccion critica para que pase solo una hebra
		#pragma omp critical(lectura)
			/** Debugg **/
				if(debugg){ 
					cout << endl << "Soy " << tid << " y he cerrado la seccion critica" << endl;
				    cout << "--Comenzando lectura de iconos y regiones" << endl; 
				}
			//Comienza a Leer los Iconos y Regiones
			for(int j=0;j<tam_pack[tid];j++){
				getline(local_file,linea);

				/** Debugg **/
					if(debugg){ 
						cout << "--Linea: " << linea << " leida" << endl; 
					}

				if(linea[0] == 'I'){				
					pos2 = linea.find(' ',pos1);
					aux = linea.substr(pos1,pos2-pos1);
	
					var[0] = atoi(aux.c_str());
	
					pos1=pos2+1;
					pos2 = linea.find(' ',pos1);
					aux = linea.substr(pos1,pos2-pos1);
	
					var[1] = atoi(aux.c_str());
	
					pos1=2;
	
					/** Debugg **/
						if(debugg){ 
							cout << "Comenzando a asignar las variables al objeto o_aux en Icono" << endl;
						}

					o_aux.setTipo(linea[0]);
					o_aux.setCord1((double)var[0],(double)var[1]);
					o_aux.setCord2(0,0);
					o_aux.setOrdenObjeto(orden_2);
					o_aux.setOrdenMouse(0);
					o_aux.setDistancia(0);
					l_obj.push_back(o_aux);

					/** Debugg **/
						if(debugg){ 
							cout << "--El valor guardado es: " << o_aux.getTipo();
							cout << " , " << o_aux.getCord1(0) << " , " << o_aux.getCord1(1);
							cout << " , " << o_aux.getCord2(0) << " , " << o_aux.getCord2(1);
							cout << " , " << o_aux.getOrdenObjeto() << " , " << o_aux.getDistancia();
							cout << " , " << o_aux.getOrdenMouse() << endl; 
						}

					orden_2++;
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
	
					/** Debugg **/
						if(debugg){ 
							cout << "Comenzando a asignar las variables al objeto o_aux en Region" << endl;
						}

					o_aux.setTipo(linea[0]);
					o_aux.setCord1((double)var[0],(double)var[1]);
					o_aux.setCord2((double)var[2],(double)var[3]);
					o_aux.setOrdenObjeto(orden_1);
					o_aux.setOrdenMouse(0);
					o_aux.setDistancia(0);
					l_obj.push_back(o_aux);

					/** Debugg **/
						if(debugg){ 
							cout << "--El valor guardado es: " << o_aux.getTipo();
							cout << " , " << o_aux.getCord1(0) << " , " << o_aux.getCord1(1);
							cout << " , " << o_aux.getCord2(0) << " , " << o_aux.getCord2(1);
							cout << " , " << o_aux.getOrdenObjeto() << " , " << o_aux.getDistancia();
							cout << " , " << o_aux.getOrdenMouse() << endl;
						}

					orden_1++;
				}
			}
			
			/** Debugg **/
				if(debugg){ 
					cout << "--Lei " << l_obj.size() << " objetos" << endl;
					cout << "Soy " << tid << " y abrire la seccion critica" << endl; 
				}
		
		//Abre la seccion critica
		#pragma omp end critical(lectura)
		
		/** Debugg **/
			if(debugg){ 
				cout << "Soy " << tid << " y estoy esperando a las otras hebras para cerrar el Archivo" << endl << endl; 
			}
		
		//Barrera que espera a TODAS las hebras antes de continuar
		#pragma omp barrier		
		
		if(tid==0){
			//Cierra el fichero abierto
			local_file.close();	

			/** Debugg **/
				if(debugg){ 
					cout << "Archivo cerrado" << endl << endl; 
				}
		
		}
		
		//Se realiza el calculo de soluciones locales
		vector<Objeto> soluciones_locales;
		Objeto solucion_local;
		solucion_local.setDistancia(1000000.0);
		solucion_local.setTipo('n');
		solucion_local.setOrdenObjeto(0);

		
		/** Debugg **/
			if(debugg){ 
				cout << "Soy " << tid << " y Comenzare a calcular mis soluciones locales" << endl; 
			}

		//Se calculan los clicks
		//Por cada mouse click
		for(int i=0;i<mouse_click.size();i++){
			//Por cada objeto leido
			for(int j=0;j<l_obj.size();j++){
				if(l_obj[j].getTipo() == 'R'){

					/** Debugg **/
						if(debugg){
							cout << endl << "Comparando Regiones" << endl;
							cout << "Comparando " << i << ": (";
							cout << mouse_click[i].getCord(0) << "," << mouse_click[i].getCord(1) << ") con (";
							cout << l_obj[j].getCord1(0) << "," << l_obj[j].getCord1(1) << ")(";
							cout << l_obj[j].getCord2(0) << "," << l_obj[j].getCord2(1) << ")";
							cout << " Orden:" << l_obj[j].getOrdenObjeto() << endl;
						}

					if( mouse_click[i].getCord(0) >= l_obj[j].getCord1(0) &&
						mouse_click[i].getCord(1) >= l_obj[j].getCord1(1) &&
						mouse_click[i].getCord(0) <= l_obj[j].getCord2(0) &&
						mouse_click[i].getCord(1) <= l_obj[j].getCord2(1) ){
							
							/** Debugg **/
								if(debugg){ 
									cout << "Comparando Orden: " << l_obj[j].getOrdenObjeto();
									cout << " -> " << solucion_local.getTipo() << ":";
									cout << solucion_local.getOrdenObjeto() << endl;
								}
							
							if(l_obj[j].getOrdenObjeto() >= solucion_local.getOrdenObjeto() || solucion_local.getTipo() == 'I'){
								solucion_local = l_obj[j];
								solucion_local.setOrdenMouse(mouse_click[i].getOrden());
								soluciones_locales.push_back(solucion_local);
							}
					}
				} else if(solucion_local.getTipo() != 'R'){
					int distancia_ideal = l_obj[j].calcularDistancia(mouse_click[i].getCord(0),mouse_click[i].getCord(1));
					if(distancia_ideal<=solucion_local.getDistancia()){
						solucion_local = l_obj[j];
						solucion_local.setDistancia(distancia_ideal);
						solucion_local.setOrdenMouse(mouse_click[i].getOrden());
						soluciones_locales.push_back(solucion_local);
					}
				}
			}
			
			solucion_local.setDistancia(1000000.0);
			solucion_local.setTipo('n');
		}

		/** Debugg **/
			if(debugg){ 
				cout << "Soy " << tid << " y termine de calcular mis soluciones locales" << endl;
				cout << "Soy " << tid << " y encontre ";
				cout << soluciones_locales.size();
				cout << " soluciones locales" << endl << endl;

				for(int k=0; k<soluciones_locales.size(); k++){
					cout << soluciones_locales[k].getTipo() << " , " << soluciones_locales[k].getCord1(0);
					cout << " , " << soluciones_locales[k].getCord1(1) << " , " << soluciones_locales[k].getCord2(0);
					cout << " , " << soluciones_locales[k].getCord2(1) << " , " << soluciones_locales[k].getOrdenObjeto();
					cout << " , " << soluciones_locales[k].getDistancia() << " , ";
					cout << soluciones_locales[k].getOrdenMouse() << endl; 
				} 
			}

	
		

		/** Debugg **/	
			if(debugg){ 
				cout << "Soy " << tid << " y estoy esperando a las otras hebras" << endl << endl; 
			}

		//Barrera que espera a TODAS las hebras antes de continuar ya que necesita todas las soluciones locales
		#pragma omp barrier

		for(int i=0;i<mouse_click.size();i++){
			//Cierra la seccion critica para que pase solo una hebra a guardar sus soluciones locales
			#pragma omp critical(soluciones)

				/** Debugg **/
					if(debugg){ 
						cout << "Soy " << tid << " y cerre la seccion critica" << endl; 
					}

				//Ingresa todos los posibles resultados de un unico mouse click
				for(int k=0;k<soluciones_locales.size();k++){
					if(soluciones_locales[k].getOrdenMouse() == mouse_click[i].getOrden()){
						resultados.push_back(soluciones_locales[k]);

						/** Debugg **/
							if(debugg){ 
								cout << "--He ingresado la solucion " << k << " que corresponde a: ";
								cout << soluciones_locales[k].getTipo() << " , " << soluciones_locales[k].getCord1(0);
								cout << " , " << soluciones_locales[k].getCord1(1) << " , " << soluciones_locales[k].getCord2(0);
								cout << " , " << soluciones_locales[k].getCord2(1) << " , " << soluciones_locales[k].getOrdenObjeto();
								cout << " , " << soluciones_locales[k].getDistancia() << " , " << soluciones_locales[k].getOrdenMouse() << endl; 
							}
					
					}
				}
				

				/** Debugg **/
					if(debugg){ 
						cout << "Soy " << tid << " y abrire la seccion critica" << endl << endl; 
					}
			
			//Abre la seccion critica
			#pragma omp end critical(soluciones)
			
			/** Debugg **/
				if(debugg){ 
					cout << "Soy " << tid << " y estoy esperando a las otras hebras" << endl << endl; 
				}

			//Barrera que espera a TODAS las hebras antes de continuar ya que neceita todas las soluciones en un solo vector
			#pragma omp barrier

			if(tid == 0){
				//Obtiene la solucion final para el Mouse Click
				vector<Objeto> solucion;
				solucion.push_back(resultados[0]);
				for( int k = 1; k < resultados.size(); k++ ){
					if(solucion[0].getTipo() == 'n'){
						solucion[0] = resultados[k];
					}else{
						if(resultados[k].getTipo() == 'R' && solucion[0].getTipo() == 'I'){	
							solucion[0] = resultados[k];
						}else if(resultados[k].getTipo() == 'R' && solucion[0].getTipo() == 'R') {
							if(resultados[k].getOrdenObjeto() >solucion[0].getOrdenObjeto()){
								solucion[0] = resultados[k];
							}
						}else if(resultados[k].getTipo() == 'I' && solucion[0].getTipo() == 'I') {
							if(resultados[k].getDistancia() < solucion[0].getDistancia()){
								solucion[0] = resultados[k];
							}else if(resultados[k].getDistancia() == solucion[0].getDistancia()){
								solucion.push_back(resultados[k]);
							}
						}
					}
				}
		
				if(solucion[0].getTipo() == 'R'){
					char letra = solucion[0].getOrdenObjeto() + 64;
					cout << letra << endl;
				}else{
					vector<int> iconos;
					for(int j=0;j<solucion.size();j++){
						iconos.push_back(solucion[j].getOrdenObjeto());
					}
					sort(iconos.begin(),iconos.end());
					for(int k=0;k<iconos.size();k++){
						cout << "   " << iconos[k];
					}
			
					cout << endl;
				}
				resultados.clear();
			}		

			/** Debugg **/
				if(debugg){ 
					cout << "Soy " << tid << " y estoy esperando que 0 termine de encontrar la solucion al mouse click: " << i << endl << endl; 
				}
			
			//Barrera que espera a TODAS las hebras antes de continuar
			#pragma omp barrier

		}
	}
}

void develop(int num_threads, string input, bool debugg){
	//Arreglo con los mouse clicks
	vector<Mouse> mouse_click;

	//Arreglo con la cantidad de objetos para cada hebra
	int* tam_pack;

	//Para mantener el orden de aparicion de los objetos
	int orden;

	//Se abre el archivo del input
	local_file.open(input.c_str());

	if(local_file.is_open()){

		/** Debugg **/
			if(debugg){ 
				cout << endl << "Fichero abierto" << endl; 
			}

		//Para guardar el mouse que se esta leyendo
		Mouse m_aux;

		//Para guardar la linea que se esta leyendo y las posiciones de lectura
		string linea;
		string aux;
		int var[4];
		int pos1=2;
		int pos2;

		//Para saber cuantos objetos hay
		int n_objetos=0;
		
		//Lee hasta llegar a los Mouses
		getline(local_file,linea);
		while(linea[0] != 'M'){
			n_objetos++;
			getline(local_file,linea);
		}

		/** Debugg **/
			if(debugg){ 
				cout << "Leido " << n_objetos << " objetos" << endl << endl; 
			}

		//Calculando el tamaño del paquete que le toca a cada hebra
		tam_pack = (int*)malloc(sizeof(int)*num_threads);
		for(int i=0;i<num_threads;i++){
			tam_pack[i] = 0;
		}
		
		//Asignando la cantidad de elementos que tendra cada hebra 
		int posicion;
		for(int i=0;i<n_objetos;i++){
			posicion = i%num_threads;
			tam_pack[posicion]++;
		}		

		/** Debugg **/
			if(debugg){ 
				for(int i=0;i<num_threads;i++){ 
					cout << "Hebra " << i << " tendra  " << tam_pack[i] << " objetos" << endl; 
				}

				cout << endl << "Comenzando a Leer los mouse clicks" << endl; 
			}

		//Comienza a leer mouse tras mouse
		orden = 1;
		while(linea[0] != '#'){

			/** Debugg **/
				if(debugg){ 
					cout << "Leida la linea : {" << linea.c_str() << "}" << endl; 
				}

			pos2 = linea.find(' ',pos1);
			aux = linea.substr(pos1,pos2-pos1);
			
			var[0] = atoi(aux.c_str());
			
			pos1=pos2+1;
			pos2 = linea.find(' ',pos1);
			aux = linea.substr(pos1,pos2-pos1);
			
			var[1] = atoi(aux.c_str());
			
			pos1=2;			

			/** Debugg **/
				if(debugg){ 
					cout << "Asignando valores al objeto m_aux" << endl; 
				}

			//Crea el mouse y lo guarda en el arreglo global
			m_aux.setCord((double)var[0],(double)var[1]);
			m_aux.setOrden(orden);

			/** Debugg **/
				if(debugg){ 
					cout << "Agregando m_aux al vector mouse_click" << endl; 
				}

			mouse_click.push_back(m_aux);
			orden++;

			/** Debugg **/
				if(debugg){ 
					cout << "Leyendo siguiente linea" << endl; 
				}

			getline(local_file,linea);				
		}

		/** Debugg **/
			if(debugg){ 
				cout << endl << "Mouse leidos" << endl; 
				for(int k=0;k<mouse_click.size();k++){ 
					cout << k << ") " << mouse_click[k].getCord(0) << " , ";
					cout << mouse_click[k].getCord(1) << " , " << mouse_click[k].getOrden() << endl; 
				}
			}

		//Se reinician los parametros de lectura del archivo
		local_file.clear();
		local_file.seekg(0);
	}

	problema_7(num_threads, mouse_click, debugg, tam_pack);
}

int main( int argc, char *argv[] ){
	//Para el Debugg
	bool debugg = false;
	bool err = false;

	if(argc >= 3 && argc <= 5){
		string s_arg = argv[1];
		int num_threads = atoi(argv[2]);
		string s_arg_2 = "AM";
		string nom_archivo = "input.txt";
		string ext_archivo;

		if(argc == 5){
			s_arg_2 = argv[3];
			nom_archivo = argv[4];
		}else if(argc == 4) {
			err = true;
		}

		ext_archivo = nom_archivo.substr(nom_archivo.length()-3,3);
		
		if("-npd" == s_arg){
			debugg = true;
			s_arg = "-np";
		}

		if("-np" == s_arg && num_threads >= 1 && (("-f" == s_arg_2 || "AM" == s_arg_2)&& "txt" == ext_archivo)){
			develop(num_threads,nom_archivo,debugg);
		}else{
			err = true;	
		}
	}else{
		err = true;
	}

	/* Si existe algun error en el input */	
	if(err){
		cout << "Error en la Linea de comandos, debe escribir: nombre.out -np N" << endl;

		cout << "Donde:" << endl;
		cout << "\"nombre.out\" es el nombre del archivo compilado." << endl;
		cout << "\"-np\" es el comando para definir la cantidad de threads." << endl;
		cout << "\"N\" es el numero de threads que se crearan con pthread," << endl;
		cout << "el cual debe ser un numero mayor a 0, con un maximo dado por la capacidad de hebras soportadas por el procesador." << endl << endl;

		cout << "Tambien es posible asignar -f archivo.txt despues de -np N" << endl;
		cout << "Donde:" << endl;
		cout << "\"archivo.txt\" es el nombre del archivo de texto con el input." << endl;
		cout << "Este archivo DEBE ser extencion txt" << endl;
		cout << "Si este comando no es activado por defecto se intentara leer input.txt." << endl;

		cout << "Teniendo esto en cuenta intentelo nuevamente."<< endl << endl;
	}
	
	return 0;
}
