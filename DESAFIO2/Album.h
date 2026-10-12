#ifndef ALBUM_H
#define ALBUM_H
#include "Cadena.h"
#include "ListaDinamica.h"

class Foto;
class MedidorRecursos;

class Album {
private:
    Cadena nombre;
    Cadena ruta;
    int privacidad;
    ListaDinamica<Foto*> fotos;
public:
    Album(const Cadena& nombre, const Cadena& ruta, int privacidad);
    ~Album();
    void agregarFoto(Foto* foto);
    bool puedeVer(bool esAmigo) const;
    void mostrarCarrusel(MedidorRecursos& medidor);
    int getCantidadFotos() const;
    Foto* getFoto(int posicion) const;
    const Cadena& getNombre() const;
    const Cadena& getRuta() const;
    int getPrivacidad() const;
    long memoria() const;
};
#endif
