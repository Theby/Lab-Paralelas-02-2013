#ifndef _OBJETO_H_
#define _OBJETO_H_ 

class Objeto{
	protected:
		/** Atributos **/
			//Puede ser I o R
			char tipo; 

			//Para guardar las coordenadas (x1,y1) y (x2,y2)
			double cord1[2];
			double cord2[2];

			//Orden de aparicion para [R,I] correspondientemente
			int orden[2]; 

			//Distancia entre dos Iconos
			double distancia; 

			//Solucion a la que pertenece
			int orden_mouse; 

	public:
		/** Constructor **/
			Objeto();
		
		/** Metodos Set **/
			//Para asignar el tipo
			void setTipo(char tipo);

			//Para asignar el valor de cord1[2]
			void setCord1(double X1, double Y1);
			
			//Para asignar el valor de cord2[2]
			void setCord2(double X2, double Y2);
			
			//Para asignar el valor del Orden de las Regiones e Iconos segun si correspondiente tipo
			void setOrdenObjeto(int orden);
			
			//Para asignar el Objeto a un click en particular
			void setOrdenMouse(int orden);
		
			//Distancia entre dos Iconos
			void setDistancia(double distancia);

		/** Metodos Get **/
			char getTipo();
			double* getCord1();
			double getCord1(int XY);
			double* getCord2();
			double getCord2(int XY);
			int getOrdenObjeto();
			double getDistancia();
			int getOrdenMouse();

		/** Otros metodos **/
			//Calcula la distancia entre este objeto(Icono) y otro Icono
			double calcularDistancia(double I_X, double I_Y);	
};

#endif /* _OBJETO_H_ */