#include "Objeto.h"
extern "C" {
	#include <math.h>
}

using namespace std;

/** Constructor **/
	Objeto::Objeto(){
		this->tipo = ' ';
		this->cord1[0] = 0.0;
		this->cord1[1] = 0.0;
		this->cord2[0] = 0.0;
		this->cord2[1] = 0.0;
		this->orden[0] = 0;
		this->orden[1] = 0;
		this->distancia = 0.0;
		this->orden_mouse = 0;
	}

/** Seccion SET **/	
	void Objeto::setTipo(char tipo){
		this->tipo = tipo;
	}

	void Objeto::setCord1(double X1, double Y1){
		this->cord1[0] = X1;
		this->cord1[1] = Y1;
	}
		
	void Objeto::setCord2(double X2, double Y2){
		this->cord2[0] = X2;
		this->cord2[1] = Y2;
	}
		
	void Objeto::setOrdenObjeto(int orden){
		if(this->tipo == 'R'){
			this->orden[0] = orden; //orden para la region
			this->orden[1] = 0;
		}else{
			this->orden[0] = 0;
			this->orden[1] = orden; //orden para el item
		}
	}
		
	void Objeto::setOrdenMouse(int orden){
		this->orden_mouse = orden;
	}
		
	void Objeto::setDistancia(double distancia){
		this->distancia = distancia;
	}

/** Seccion GET **/	
	char Objeto::getTipo(){
		return this->tipo;
	}
				
	double* Objeto::getCord1(){
		return this->cord1;
	}

	double Objeto::getCord1(int XY){
		return this->cord1[XY];
	}

	double* Objeto::getCord2(){
		return this->cord2;
	}

	double Objeto::getCord2(int XY){
		return this->cord2[XY];
	}

	int Objeto::getOrdenObjeto(){
		if(this->tipo == 'R'){
			return this->orden[0];//orden para la region
		}else{
			return this->orden[1];//orden para el item
		}
	}

	double Objeto::getDistancia(){
		return this->distancia;
	}

	int Objeto::getOrdenMouse(){
		return orden_mouse;
	}

/** Otros Metodos **/
	double Objeto::calcularDistancia(double I_X, double I_Y){
		double X = I_X - this->cord1[0];
		double Y = I_Y - this->cord1[1];

		return sqrt((pow(X,2) + pow(Y,2)));
	}