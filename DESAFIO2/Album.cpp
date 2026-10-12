#include "Album.h"
#include "Foto.h"
#include "MedidorRecursos.h"
#include <iostream>
#include <limits>

using namespace std;

Album::Album(const Cadena& n, const Cadena& r, int p)
    : nombre(n), ruta(r), privacidad(p), fotos() {
}

Album::~Album() {
    for (int i = 0; i < fotos.getCantidad(); i++) {
        delete fotos[i];
    }
}

void Album::agregarFoto(Foto* foto) {
    if (foto) fotos.agregar(foto);
}

bool Album::puedeVer(bool esAmigo) const {
    return privacidad == 0 || (privacidad == 1 && esAmigo);
}

int Album::getCantidadFotos() const {
    return fotos.getCantidad();
}

Foto* Album::getFoto(int p) const {
    return p >= 0 && p < fotos.getCantidad() ? fotos[p] : nullptr;
}

const Cadena& Album::getNombre() const {
    return nombre;
}

const Cadena& Album::getRuta() const {
    return ruta;
}

int Album::getPrivacidad() const {
    return privacidad;
}

void Album::mostrarCarrusel(MedidorRecursos& medidor) {
    if (fotos.getCantidad() == 0) {
        cout << "Este album no tiene fotos.\n";
        return;
    }

    int posicion = 0;
    char opcion = ' ';

    do {
        medidor.contarIteracion();
        Foto* foto = fotos[posicion];

        cout << "\nFoto " << posicion + 1 << '/'
             << fotos.getCantidad() << ": "
             << foto->getRuta().c_str();

        cout << " | Likes: " << foto->getLikes() << '\n';
        cout << "[s]iguiente [p]revia [l]ike [q]uitar like [x]salir: ";

        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 's' || opcion == 'S')
            posicion = (posicion + 1) % fotos.getCantidad();

        if (opcion == 'p' || opcion == 'P')
            posicion = (posicion + fotos.getCantidad() - 1) % fotos.getCantidad();

        if (opcion == 'l' || opcion == 'L')
            foto->darLike();

        if (opcion == 'q' || opcion == 'Q')
            foto->quitarLike();

    } while (opcion != 'x' && opcion != 'X');
}

long Album::memoria() const {
    long total = sizeof(Album) + nombre.memoria()
    + ruta.memoria() + fotos.memoria();

    for (int i = 0; i < fotos.getCantidad(); i++) {
        total += fotos[i]->memoria();
    }

    return total;
}