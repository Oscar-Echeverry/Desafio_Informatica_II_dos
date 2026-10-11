#include "Foto.h"

Foto::Foto(const FechaHora& c, long t, const Cadena& r, int l)
    : captura(c), tamaño(t), ruta(r), likes(l < 0 ? 0 : l) {
}

void Foto::darLike() {
    likes++;
}

void Foto::quitarLike() {
    if (likes > 0) {
        likes--;
    }
}

int Foto::getLikes() const {
    return likes;
}

const Cadena& Foto::getRuta() const {
    return ruta;
}

const FechaHora& Foto::getCaptura() const {
    return captura;
}

long Foto::gettamaño() const {
    return tamaño;
}

long Foto::memoria() const {
    return sizeof(Foto) + ruta.memoria();
}