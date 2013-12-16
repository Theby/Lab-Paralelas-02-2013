/************************************************
Ingeniería  Civil en Informatica
Esteban Gaete Flores
Mosheh-efra Landaeta Sanchez

Grupo cp7
Problema 7

Laboratorio n°3 PTHREAD - Computación Paralela
Segundo Semestre del 2013
*****************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
extern "C" {
	#include <pthread.h>
	#include <stdlib.h>
	#include <math.h>
}

using namespace std;

class objeto{
	public:
		char tipo; //Puede ser I o R
		double cord1[2];
		double cord2[2];
		int orden[2]; //Orden de aparicion para [R,I] correspondientemente
		double distancia; //Distancia entre un I y otro I
		int solucion_mouse; //Solucion a la que pertenece

	objeto(){
		tipo = ' ';
		cord1[0] = 0.0;
		cord1[1] = 0.0;
		cord2[0] = 0.0;
		cord2[1] = 0.0;
		orden[0] = 0;
		orden[1] = 0;
		distancia = 0.0;
		solucion_mouse = 0;
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
		solucion_mouse = 0;
	}
	
	void set_morden(int ord){
		solucion_mouse = ord;
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

/** Seccion de variables Globales */

//Arreglo con los mouse clicks
vector<mouse> mouse_click;

//Arreglo con la cantidad de objetos para cada hebra
int* tam_pack;

//Buffer del archivo a leer
ifstream local_file;

//Mutex para la lectura del archivo
pthread_mutex_t mutex_read;

//Mutex para algunos test
pthread_mutex_t mutex_test;

//Barrera de espera tras obtener las soluciones locales
pthread_barrier_t barrier_sol;

//Para mantener el orden de aparicion de los objetos
int orden_1;
int orden_2;

//Para guardar los resultados totales
vector<objeto> resultados;

//Mutex para los resultados
pthread_mutex_t mutex_res;

//Barreras para el control de los resultados
pthread_barrier_t barrier_pre_res;
pthread_barrier_t barrier_pos_res;

//Para el Debugg
bool debugg;
/** Fin Seccion de variables globales */

void* problema_7(void* arg){
	long pid;

	pid = (long)arg;

	//Para guardar el objeto que se esta leyendo
	objeto o_aux;

	//Para guardar los objetos leidos
	vector<objeto> l_obj;

	//Para guardar la linea que se esta leyendo y las posiciones de lectura
	string linea;
	string aux;
	int var[4];
	int pos1=2;
	int pos2;

	//Cierra el Mutex
	pthread_mutex_lock( &mutex_read );
	if(debugg){ cout << "Soy " << pid << " y he cerrado el mutex_read" << endl; }
	if(debugg){ cout << "--Comenzando lectura de iconos y regiones" << endl; }
		//Comienza a Leer los Iconos y Regiones
		for(int j=0;j<tam_pack[pid];j++){
			getline(local_file,linea);
			if(debugg){ cout << "--Linea: " << linea.c_str() << " leida" << endl; }
			if(linea[0] == 'I'){				
				pos2 = linea.find(' ',pos1);
				aux = linea.substr(pos1,pos2-pos1);
		
				var[0] = atoi(aux.c_str());
		
				pos1=pos2+1;
				pos2 = linea.find(' ',pos1);
				aux = linea.substr(pos1,pos2-pos1);
		
				var[1] = atoi(aux.c_str());
		
				pos1=2;
		
				o_aux.setfc(linea[0],(double)var[0],(double)var[1]);
				o_aux.setord(orden_2);
				l_obj.push_back(o_aux);
				if(debugg){ cout << "--El valor guardado es: " << l_obj[j].tipo << " , " << l_obj[j].cord1[0] << " , " << l_obj[j].cord1[1] << " , " << l_obj[j].cord2[0] << " , " << l_obj[j].cord2[1] << " , " << l_obj[j].orden[0] << " , " << l_obj[j].orden[1] << " , " << l_obj[j].distancia << endl; }
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
		
				o_aux.setfc(linea[0],(double)var[0],(double)var[1]);
				o_aux.setsc((double)var[2],(double)var[3]);
				o_aux.setord(orden_1);
				l_obj.push_back(o_aux);
				if(debugg){ cout << "--El valor guardado es: " << l_obj[j].tipo << " , " << l_obj[j].cord1[0] << " , " << l_obj[j].cord1[1] << " , " << l_obj[j].cord2[0] << " , " << l_obj[j].cord2[1] << " , " << l_obj[j].orden[0] << " , " << l_obj[j].orden[1] << " , " << l_obj[j].distancia << endl; }
				orden_1++;
			}
		}
	//Abre el mutex
	if(debugg){ cout << "--Lei " << l_obj.size() << " objetos" << endl; }
	if(debugg){ cout << "Soy " << pid << " y abriré mutex_read" << endl; }
	pthread_mutex_unlock( &mutex_read ); 

	
	//Se realiza el calculo de soluciones locales
	vector<objeto> soluciones_locales;
	objeto solucion_local;
	solucion_local.set_distancia(1000000.0);
	solucion_local.tipo = 'n';

	//Se calculan los clicks
	if(debugg){ cout << "Soy " << pid << " y Comenzare a calcular mis soluciones locales" << endl; }
	for(int i=0;i<mouse_click.size();i++){
		for(int j=0;j<l_obj.size();j++){
			if(l_obj[j].tipo == 'R'){
				if( mouse_click[i].cord[0] >= l_obj[j].cord1[0] &&
					mouse_click[i].cord[1] >= l_obj[j].cord1[1] &&
					mouse_click[i].cord[0] <= l_obj[j].cord2[0] &&
					mouse_click[i].cord[1] <= l_obj[j].cord2[1] ){
						if(l_obj[j].orden[0]>=solucion_local.orden[0]){
							solucion_local = l_obj[j];
							solucion_local.set_morden(mouse_click[i].orden);
							soluciones_locales.push_back(solucion_local);
						}
				}
			} else if(solucion_local.tipo != 'R'){
				if(l_obj[j].distance(mouse_click[i].cord[0],mouse_click[i].cord[1])<=solucion_local.distancia){
					solucion_local = l_obj[j];
					solucion_local.set_distancia(l_obj[j].distance(mouse_click[i].cord[0],mouse_click[i].cord[1]));
					solucion_local.set_morden(mouse_click[i].orden);
					soluciones_locales.push_back(solucion_local);
				}
			}
		}
		//soluciones_locales.push_back(solucion_local);
		solucion_local.set_distancia(1000000.0);
		solucion_local.tipo = 'n';
	}
	if(debugg){ cout << "Soy " << pid << " y termine de calcular mis soluciones locales" << endl; }
	if(debugg){ cout << "Soy " << pid << " y encontre " << soluciones_locales.size() << " soluciones locales" << endl << endl; }

	//Barrera que espera a TODAS las hebras antes de continuar
	if(debugg){ cout << "Soy " << pid << " y estoy esperando a las otras hebras" << endl << endl; }
	pthread_barrier_wait( &barrier_sol );

	for(int i=0;i<mouse_click.size();i++){
		//Cierra el mutex
		pthread_mutex_lock( &mutex_res ); 
			if(debugg){ cout << "Soy " << pid << " y cerre el mutex_res" << endl; }
			//Ingresa todos los posibles resultados de un unico mouse click
			for(int k=0;k<soluciones_locales.size();k++){
				if(soluciones_locales[k].solucion_mouse == mouse_click[i].orden){
					resultados.push_back(soluciones_locales[k]);
				}
			}
			if(debugg){ cout << "--He ingresado la solucion " << i << " que corresponde a: " << soluciones_locales[i].tipo << " , " << soluciones_locales[i].cord1[0] << " , " << soluciones_locales[i].cord1[1] << " , " << soluciones_locales[i].cord2[0] << " , " << soluciones_locales[i].cord2[1] << " , " << soluciones_locales[i].orden[0] << " , " << soluciones_locales[i].orden[1] << " , " << soluciones_locales[i].distancia << endl; }
		//Abre el mutex
			if(debugg){ cout << "Soy " << pid << " y abrire el mutex_res" << endl << endl; }
		pthread_mutex_unlock( &mutex_res );
		
		//Barrera que espera a TODAS las hebras antes de continuar
		if(debugg){ cout << "Soy " << pid << " y estoy esperando a las otras hebras" << endl << endl; }
		pthread_barrier_wait( &barrier_pre_res );

		if(pid == 0){
			//Obtiene la solucion final para el Mouse Click
			vector<objeto> solucion;
			solucion.push_back(resultados[0]);
			for( int k = 1; k < resultados.size(); k++ ){
				if(solucion[0].tipo == 'n'){
					solucion[0] = resultados[k];
				}else{
					if(resultados[k].tipo == 'R' && solucion[0].tipo == 'I'){	
						solucion[0] = resultados[k];
					}else if(resultados[k].tipo == 'R' && solucion[0].tipo == 'R') {
						if(resultados[k].orden[0]>solucion[0].orden[0]){
							solucion[0] = resultados[k];
						}
					}else if(resultados[k].tipo == 'I' && solucion[0].tipo == 'I') {
						if(resultados[k].distancia<solucion[0].distancia){
							solucion[0] = resultados[k];
						}else if(resultados[k].distancia == solucion[0].distancia){
							solucion.push_back(resultados[k]);
						}
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
				for(int k=0;k<iconos.size();k++){
					cout << "   " << iconos[k];
				}
			
				cout << endl;
			}
			resultados.clear();
		}		

		//Barrera que espera a TODAS las hebras antes de continuar
		if(debugg){ cout << "Soy " << pid << " y estoy esperando que 0 termine de encontrar la solucion al mouse click: " << i << endl << endl; }
		pthread_barrier_wait( &barrier_pos_res );

	}


	pthread_exit(NULL);
}

void develop(int num_threads, string input){
	//Para saber si el hilo ha sido creado correctamente
	int err_thread;

	//Arreglo de pthreads solicitados
	pthread_t* threads;
	threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);

	if(debugg){ cout << "Ejecutando con " << num_threads << " hebra desde " << input << endl; }

	//Se abre el archivo del input
	local_file.open(input.c_str());

	if(local_file.is_open()){
		if(debugg){ cout << "Fichero abierto" << endl; }
		//Para guardar el mouse que se esta leyendo
		mouse m_aux;

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

		if(debugg){ cout << "Leido " << n_objetos << " objetos" << endl; }

		//Calculando el tamaño del paquete que le toca a cada hebra
		tam_pack = (int*)malloc(sizeof(int)*num_threads);
		for(int i=0;i<num_threads;i++){
			tam_pack[i] = 0;
		}
		
		int posicion;
		for(int i=0;i<n_objetos;i++){
			posicion = i%num_threads;
			tam_pack[posicion]++;
			if(debugg){ cout << "Hebra " << posicion << " tendra  " << tam_pack[posicion] << " objetos" << endl; }			
		}			

		//Comienza a leer mouse tras mouse
		if(debugg){ cout << endl << "Comenzando a Leer los mouse clicks" << endl; }
		orden_1 = 1;
		while(linea[0] != '#'){
			if(debugg){ cout << "Leida la linea : {" << linea.c_str() << "}" << endl; }			
			pos2 = linea.find(' ',pos1);
			aux = linea.substr(pos1,pos2-pos1);
			
			var[0] = atoi(aux.c_str());
			
			pos1=pos2+1;
			pos2 = linea.find(' ',pos1);
			aux = linea.substr(pos1,pos2-pos1);
			
			var[1] = atoi(aux.c_str());
			
			pos1=2;
			
			//Crea el mouse y lo guarda en el arreglo global
			if(debugg){ cout << "Asignando valores al objeto m_aux" << endl; }
			m_aux.setc((double)var[0],(double)var[1],orden_1);
			if(debugg){ cout << "Agregando m_aux al vector mouse_click" << endl; }
			mouse_click.push_back(m_aux);
			orden_1++;
			if(debugg){ cout << "Leyendo siguiente linea" << endl; }
			getline(local_file,linea);				
		}
		if(debugg){ cout << endl << "Mouse leidos" << endl; for(int k=0;k<mouse_click.size();k++){ cout << k << ") " << mouse_click[k].cord[0] << " , " << mouse_click[k].cord[1] << " , " << mouse_click[k].orden << endl; }}
		//Se reinician los parametros de lectura del archivo
		local_file.clear();
		local_file.seekg(0);
		orden_1 = 1;
		orden_2 = 1;

		//Se inicializan las barreras
		if(debugg){ cout << "Creando barreras" << endl; }
		pthread_barrier_init( &barrier_sol, NULL, num_threads);
		pthread_barrier_init( &barrier_pre_res, NULL, num_threads);
		pthread_barrier_init( &barrier_pos_res, NULL, num_threads);
		if(debugg){ cout << "Barreras Creadas" << endl << endl; }
		//Se crean los threads
		if(debugg){ cout << "Creando Threads:" << endl; }		
		for(int i=0;i<num_threads;i++){
			err_thread = pthread_create(&threads[i],NULL,&problema_7,(void*)i);

			if(err_thread){
				cout << "Hebra: " << i << " No ha podido ser creada" << endl;
			}else if(debugg){
				cout << "Hebra: " << i << " Creada satisfactoriamente" << endl;
			}
			err_thread = false;
		}
		if(debugg){ cout << "Terminada la creacion de hebras" << endl << endl; }
	}

	//Espera a que las hebras terminen su ejecucion
	int hebra_return;
	if(debugg){ cout << "Proceso inicial esperando en el Join" << endl; }
	for(int i = 0; i < num_threads; i++ ){
	      pthread_join( threads[ i ], (void**)&hebra_return); 
	}
	if(debugg){ cout << "Proceso inicial ya no esta esperando en el join" << endl << endl; }

	//Cierra el fichero abierto
	local_file.close();		
	if(debugg){ cout << "Archivo " << input << " cerrado" << endl; }
	//Termina la ejecucion de las hebras
	if(debugg){ cout << "Proceso inicial ha ejecutado pthread_exit" << endl; }
	pthread_exit(0);
}

int main( int argc, char *argv[] ){
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

		if("-np" == s_arg && num_threads >= 1 && num_threads <= 50 && (("-f" == s_arg_2 || "AM" == s_arg_2)&& "txt" == ext_archivo)){
			develop(num_threads,nom_archivo);
		}else{
			err = true;	
		}
	}else{
		err = true;
	}

	/* Si existe algún error en el input */	
	if(err){
		cout << "Error en la Linea de comandos, debe escribir: nombre.out -np N" << endl;

		cout << "Donde:" << endl;
		cout << "\"nombre.out\" es el nombre del archivo compilado." << endl;
		cout << "\"-np\" es el comando para definir la cantidad de threads." << endl;
		cout << "\"N\" es el numero de threads que se crearan con pthread," << endl;
		cout << " el cual debe ser un numero mayor a 0 e inferior a 51." << endl << endl;

		cout << "También es posible asignar -f archivo.txt despues de -np N" << endl;
		cout << "Donde:" << endl;
		cout << "\"archivo.txt\" es el nombre del archivo de texto con el input." << endl;
		cout << "Este archivo DEBE ser extencion txt" << endl;
		cout << "Si este comando no es activado por defecto se intentará leer input.txt." << endl;

		cout << "Teniendo esto en cuenta intentelo nuevamente."<< endl << endl;
	}
	
	return 0;
}
