#include "MedidorRecursos.h"
#include <iostream>

using namespace std;

MedidorRecursos::MedidorRecursos() : iteraciones(0), memoriaActual(0) {}

void MedidorRecursos::reiniciar() {
    iteraciones = 0;
    memoriaActual = 0;
}

void MedidorRecursos::contarIteracion(long long n) {
    iteraciones += n;
}

void MedidorRecursos::sumarMemoria(long long n) {
    memoriaActual += n;
}

void MedidorRecursos::restarMemoria(long long n) {
    memoriaActual -= n;
}

void MedidorRecursos::establecerMemoria(long long n) {
    memoriaActual = n;
}

void MedidorRecursos::mostrarResultados() const {
    cout << "Iteraciones contabilizadas: " << iteraciones << '\n';
    cout << "Memoria de objetos y variables contabilizadas: "
         << memoriaActual << " bytes\n";
}