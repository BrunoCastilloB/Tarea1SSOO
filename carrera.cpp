#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>

std::mutex mtx;  // Uso de mutex

struct Auto {
    int id;
    int distancia;
    int total_avance;
    bool terminado;
    Auto(int id, int distancia) : id(id), distancia(distancia), total_avance(0), terminado(false) {}
};

void correr(Auto &auto_obj, int M, int &lugar_actual, std::vector<int> &resultados) {
    while (auto_obj.total_avance < M) {
        // Simular avance entre 1 a 10 metros
        int avance = rand() % 10 + 1;
        auto_obj.total_avance += avance;

        // Verificación de no pasar los 100 metros
        if (auto_obj.total_avance > M) {
            auto_obj.total_avance = M;
        }

        // Pausa entre 100 a 500 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 400));

        std::lock_guard<std::mutex> guard(mtx);
        std::cout << "Auto" << auto_obj.id << " avanza " << avance << " metros (total: " 
                  << auto_obj.total_avance << " metros)" << std::endl;

        if (auto_obj.total_avance == M) {
            auto_obj.terminado = true;
            lugar_actual++;
            resultados[auto_obj.id] = lugar_actual;
            std::cout << "Auto" << auto_obj.id << " termina la carrera en el lugar " << lugar_actual << "!" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    // Validar parámetros de entrada
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total> <numero_autos>" << std::endl;
        return 1;
    }

    int M = std::stoi(argv[1]);  // Distancia total de la carrera
    int N = std::stoi(argv[2]);  // Número de autos

    // Crear los autos
    std::vector<Auto> autos;
    for (int i = 0; i < N; ++i) {
        autos.emplace_back(i, M);
    }

    srand(time(0));  // Generador números aleatorios
    int lugar_actual = 0;  // Lugar de llegada
    std::vector<int> resultados(N, 0);  // Guarda orden de llegada

    // Crear hebras para cada auto
    std::vector<std::thread> threads;
    for (int i = 0; i < N; ++i) {
        threads.push_back(std::thread(correr, std::ref(autos[i]), M, std::ref(lugar_actual), std::ref(resultados)));
    }

    // Esperar a que todas las hebras terminen
    for (auto &t : threads) {
        t.join();
    }

    // Mostrar los resultados finales
    std::cout << "\nLugar\tAuto" << std::endl;
    for (int i = 1; i <= N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (resultados[j] == i) {
                std::cout << i << "\tAuto" << j << std::endl;
            }
        }
    }

    return 0;
}
