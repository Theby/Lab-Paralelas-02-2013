#Target del Makefile
TARGET = openmp

#Nombre del ejecutable final
EXEC = cp7sollab04

#Compilador a usar
CC = g++

#Librerias a usar
LIBS = -fopenmp

#Compiler Comands
CFLAGS = -c

#Headers
HEADERS = $(wildcard *.h)

#Objects
OBJECTS =  problema-07-openmp.o Objeto.o Mouse.o

#Carpeta de Objetos
OUT = out/

OWR = $(OUT)problema-07-openmp.o $(OUT)Objeto.o $(OUT)Mouse.o

default: $(TARGET)
all: default

$(TARGET): $(OBJECTS)
		$(CC) $(OWR) -o $(EXEC) $(LIBS)

%.o: %.cpp $(HEADERS)
		$(CC) $(CFLAGS) $< -o $(OUT)$@ $(LIBS)

clean:
		rm -rf $(OUT)*o ($TARGET)
