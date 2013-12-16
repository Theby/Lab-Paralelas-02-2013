#include "Mouse.h"

using namespace std;

	/** Constructor **/
		Mouse::Mouse(){
			this->cord[0] = 0.0;
			this->cord[1] = 0.0;
			this->orden = 0;
		}
	
	/** Seccion Set **/
		void Mouse::setCord(double X, double Y){
			this->cord[0] = X;
			this->cord[1] = Y;
		}

		void Mouse::setOrden(int orden){
			this->orden = orden;
		}

	/** Seccion Get **/	
		double* Mouse::getCord(){
			return this->cord;
		}

		double Mouse::getCord(int XY){
			return this->cord[XY];
		}

		int Mouse::getOrden(){
			return this->orden;
		}