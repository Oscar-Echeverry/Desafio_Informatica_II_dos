#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H
class MedidorRecursos {
private:
    long long iteraciones;
    long long memoriaActual;
public:
    MedidorRecursos();
    void reiniciar();
    void contarIteracion(long long cantidad = 1);
    void sumarMemoria(long long bytes);
    void restarMemoria(long long bytes);
    void establecerMemoria(long long bytes);
    void mostrarResultados() const;
};
#endif
