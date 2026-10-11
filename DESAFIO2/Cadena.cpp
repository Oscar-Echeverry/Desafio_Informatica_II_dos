#include "Cadena.h"

Cadena::Cadena() : texto(new char[1]), longitud(0) { texto[0] = '\0'; }

Cadena::Cadena(const char* origen) : texto(nullptr), longitud(0) {
    if (origen) while (origen[longitud] != '\0') longitud++;
    texto = new char[longitud + 1];
    for (int i = 0; i < longitud; i++) texto[i] = origen[i];
    texto[longitud] = '\0';
}

Cadena::Cadena(const Cadena& otra) : texto(new char[otra.longitud + 1]), longitud(otra.longitud) {
    for (int i = 0; i <= longitud; i++) texto[i] = otra.texto[i];
}

Cadena::~Cadena() { delete[] texto; }

Cadena& Cadena::operator=(const Cadena& otra) {
    if (this != &otra) {
        char* nuevo = new char[otra.longitud + 1];
        for (int i = 0; i <= otra.longitud; i++) nuevo[i] = otra.texto[i];
        delete[] texto;
        texto = nuevo;
        longitud = otra.longitud;
    }
    return *this;
}

bool Cadena::operator==(const Cadena& otra) const {
    if (longitud != otra.longitud) return false;
    for (int i = 0; i < longitud; i++) if (texto[i] != otra.texto[i]) return false;
    return true;
}

bool Cadena::operator!=(const Cadena& otra) const { return !(*this == otra); }

bool Cadena::operator<(const Cadena& otra) const {
    int limite = longitud < otra.longitud ? longitud : otra.longitud;
    for (int i = 0; i < limite; i++) {
        if (texto[i] != otra.texto[i]) return texto[i] < otra.texto[i];
    }
    return longitud < otra.longitud;
}

const char* Cadena::c_str() const { return texto; }
int Cadena::getLongitud() const { return longitud; }
long Cadena::memoria() const { return longitud + 1; }