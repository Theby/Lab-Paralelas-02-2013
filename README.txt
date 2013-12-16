------------------
Universidad de Santiago de Chile - USACH
Ingeniería  Civil en Informatica
Esteban Gaete Flores
Mosheh-efra Landaeta Sanchez

Grupo cp7
Problema 7

Laboratorio n°4 OPENMP - Computación Paralela
Segundo Semestre del 2013
--------------------

Probado en Ubuntu 10.04
Para compilar basta con ejecutar el makefile con make en la consola.

-------------------

El programa ha sido probado en el cluster usando la cuenta cp7
(existe una copia del programa en /home/cp7/lab-04).
Sin embargo no funciona apropiadamente, ya que en el cluster existe la posibilidad de que
dos threads entren a una sección critica, tras varios análisis de la situación he llegado
a la conclusión de que el cluster maneja cada thread en procesadores diferentes, permitiendo
que estos entren a la sección critica de forma "independiente" a nivel local, sin embargo
la sincronicación de las variables compartidas provocan que el resultado final sea
inconsistente al esperado.

También existe una copia del programa en GitHub: OMP-v2.1 Laboratorio 4 - openMP (Modular)
	https://github.com/Theby/Lab-Paralelas-02-2013/releases

Donde también es posible encontrar el enunciado del problema.

Para compilar basta con hacer make, luego puede ejecutar con: 

cp7sollab04.out -np 3.

Si necesita probar diferentes parametros puede ingresar cualquier número positivo
tras el comando -np.
Tambien es posible usar un "nombre.txt" especifico con el comando -f de la siguiente manera

cp7sollab04.out -np 2 -f nombre.txt

El archivo debe ser de extensión txt.
Si no se especifica el comando -f, se usará input.txt por defecto.

Atte. Esteban Gaete Flores
