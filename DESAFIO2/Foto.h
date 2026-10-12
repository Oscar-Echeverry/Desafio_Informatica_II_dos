#ifndef FOTO_H
#define FOTO_H
#include "Cadena.h"
#include "FechaHora.h"

class Foto {
private:
    FechaHora captura;
    long tamaño;
    Cadena ruta;
    int likes;
public:
    Foto(const FechaHora& captura, long tamaño, const Cadena& ruta, int likes);
    void darLike();
    void quitarLike();
    int getLikes() const;
    const Cadena& getRuta() const;
    const FechaHora& getCaptura() const;
    long gettamaño() const;
    long memoria() const;
};
#endif
