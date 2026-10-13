#ifndef ARCHIVO_H
#define ARCHIVO_H
#include "Cadena.h"

class Plataforma;

class Archivos {
private:
    Cadena rutaBase;
public:
    Archivos();
    explicit Archivos(const Cadena& ruta);
    bool cargar(Plataforma& sistema);
    bool guardar(const Plataforma& sistema);
    const Cadena& getRutaBase() const;
    long memoria() const;
};
#endif
