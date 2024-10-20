# Variables
CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall
TARGET = carrera
SRC = carrera.cpp

# Compilar el programa
all:
	g++ -std=c++11 -pthread carrera.cpp -o carrera

# Para compilar el programa
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Para limpieza de archivos
clean:
	rm -f $(TARGET)

# Para ejecutar el programa con parametros de 100 metros y 3 hebras
run:
	./$(TARGET) 100 3

