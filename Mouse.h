#ifndef _MOUSE_H_
#define _MOUSE_H_ 

class Mouse{
	protected:
		/** Atributos **/
			//Coordenadas X,Y
			double cord[2];

			//Para saber en que orden han sido hecho los clicks
			int orden;

	public:
		/** Constructor **/
			Mouse();
	
		/** Metodos Set **/
			//Para asignar las coordenadas
			void setCord(double X, double Y);

			//Para asignar el orden
			void setOrden(int orden);

		/** Metodos Get **/
			double* getCord();
			double getCord(int XY);
			int getOrden();

};

#endif /* _MOUSE_H_ */