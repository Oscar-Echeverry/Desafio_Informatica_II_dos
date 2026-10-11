#ifndef LISTADINAMICA_H
#define LISTADINAMICA_H
#include <cstddef>

template <typename T>
class ListaDinamica {
private:
    T* datos;
    int cantidad;
    int capacidad;
public:
    ListaDinamica() : datos(nullptr), cantidad(0), capacidad(0) {}
    ListaDinamica(const ListaDinamica& otra) : datos(nullptr), cantidad(0), capacidad(0) {
        for (int i = 0; i < otra.cantidad; i++) agregar(otra.datos[i]);
    }
    ListaDinamica& operator=(const ListaDinamica& otra) {
        if (this != &otra) {
            limpiar();
            for (int i = 0; i < otra.cantidad; i++)
                agregar(otra.datos[i]);
        }
        return *this;
    }
    ~ListaDinamica() { delete[] datos; }
    void agregar(const T& elemento) { insertar(cantidad, elemento); }
    void insertar(int posicion, const T& elemento) {
        if (posicion < 0 || posicion > cantidad) return;
        if (cantidad == capacidad) {
            int nueva = capacidad == 0 ? 4 : capacidad * 2;
            T* reemplazo = new T[nueva];
            for (int i = 0; i < cantidad; i++) reemplazo[i] = datos[i];
            delete[] datos;
            datos = reemplazo;
            capacidad = nueva;
        }
        for (int i = cantidad; i > posicion; i--) datos[i] = datos[i - 1];
        datos[posicion] = elemento;
        cantidad++;
    }
    void quitar(int posicion) {
        if (posicion < 0 || posicion >= cantidad) return;
        for (int i = posicion; i < cantidad - 1; i++) datos[i] = datos[i + 1];
        cantidad--;
    }
    void limpiar() { delete[] datos; datos = nullptr; cantidad = 0; capacidad = 0; }
    T& operator[](int indice) { return datos[indice]; }
    const T& operator[](int indice) const { return datos[indice]; }
    int getCantidad() const { return cantidad; }
    long memoria() const { return static_cast<long>(capacidad) * sizeof(T); }
};

#endif
