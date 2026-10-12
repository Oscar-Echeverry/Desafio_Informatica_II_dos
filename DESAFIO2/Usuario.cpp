#include "Usuario.h"
#include "Album.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"
#include "MedidorRecursos.h"
#include <iostream>
#include <limits>

using namespace std;

bool mismoNombre(const Usuario& a, const Usuario& b) {
    return a.nombreUsuario == b.nombreUsuario;
}

Usuario::Usuario(const Cadena& n, const Cadena& c, const Cadena& cl,
                 const Cadena& ci, const Cadena& p, const Cadena& f,
                 const FechaHora& na, const FechaHora& in)
    : nombreUsuario(n), correoElectronico(c), clave(cl),
    ciudad(ci), pais(p), fotoPerfil(f),
    nacimiento(na), inscripcion(in) {}

Usuario::~Usuario() {
    for (int i = 0; i < albumes.getCantidad(); i++)
        delete albumes[i];
}

bool Usuario::tieneAmigo(const Usuario* otro) const {
    for (int i = 0; i < amigos.getCantidad(); i++)
        if (amigos[i] == otro) return true;
    return false;
}

bool Usuario::agregarAmigo(Usuario* otro) {
    if (!otro || mismoNombre(*this, *otro) || tieneAmigo(otro))
        return false;
    amigos.agregar(otro);
    return true;
}

void Usuario::agregarAlbum(Album* a) {
    if (a) albumes.agregar(a);
}

void Usuario::agregarEnviado(Mensaje* m) {
    if (!m) return;
    int i = 0;
    while (i < enviados.getCantidad() &&
           !(m->getFecha() < enviados[i]->getFecha()))
        i++;
    enviados.insertar(i, m);
}

void Usuario::agregarRecibido(Mensaje* m) {
    if (!m) return;
    int i = 0;
    while (i < recibidos.getCantidad() &&
           !(m->getFecha() < recibidos[i]->getFecha()))
        i++;
    recibidos.insertar(i, m);
}

void Usuario::agregarSolicitud(SolicitudAmistad* s) {
    if (s) solicitudes.agregar(s);
}

bool Usuario::puedeEnviarSolicitud(Usuario* d) const {
    return d != nullptr && d != this && !tieneAmigo(d);
}

bool Usuario::aceptarSolicitud(SolicitudAmistad* s) {
    return s != nullptr && s->getReceptor() == this && s->aceptar();
}

int Usuario::contarAmigosComunes(const Usuario& otro,
                                 MedidorRecursos* medidor) const {
    int comunes = 0;
    for (int i = 0; i < amigos.getCantidad(); i++) {
        if (medidor) medidor->contarIteracion();
        for (int j = 0; j < otro.amigos.getCantidad(); j++) {
            if (medidor) medidor->contarIteracion();
            if (amigos[i] == otro.amigos[j]) {
                comunes++;
                break;
            }
        }
    }
    return comunes;
}

bool Usuario::puedeEnviarMensaje(Usuario* d, const Cadena& t) const {
    return d != nullptr && tieneAmigo(d) &&
           t.getLongitud() > 0 && t.getLongitud() <= 512;
}

void Usuario::verAlbumes(Usuario* amigo, MedidorRecursos& medidor) {
    if (!amigo || !tieneAmigo(amigo)) {
        cout << "No es un amigo registrado.\n";
        return;
    }

    ListaDinamica<Album*> visibles;

    for (int i = 0; i < amigo->albumes.getCantidad(); i++) {
        medidor.contarIteracion();
        if (amigo->albumes[i]->puedeVer(true))
            visibles.agregar(amigo->albumes[i]);
    }

    if (visibles.getCantidad() == 0) {
        cout << "No hay albumes visibles.\n";
        return;
    }

    for (int i = 0; i < visibles.getCantidad(); i++) {
        medidor.contarIteracion();
        cout << i + 1 << ". "
             << visibles[i]->getNombre().c_str()
             << " (" << visibles[i]->getCantidadFotos()
             << " fotos)\n";
    }

    int opcion = 0;
    cout << "Seleccione album (0 para volver): ";

    if (!(cin >> opcion)) {
        cin.clear();
        opcion = 0;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (opcion >= 1 && opcion <= visibles.getCantidad())
        visibles[opcion - 1]->mostrarCarrusel(medidor);
}

const Cadena& Usuario::getNombreUsuario() const {
    return nombreUsuario;
}

const Cadena& Usuario::getCorreo() const {
    return correoElectronico;
}

const Cadena& Usuario::getClave() const {
    return clave;
}

const Cadena& Usuario::getCiudad() const {
    return ciudad;
}

const Cadena& Usuario::getPais() const {
    return pais;
}

const Cadena& Usuario::getFotoPerfil() const {
    return fotoPerfil;
}

const FechaHora& Usuario::getNacimiento() const {
    return nacimiento;
}

const FechaHora& Usuario::getInscripcion() const {
    return inscripcion;
}

void Usuario::setCorreo(const Cadena& correo) {
    correoElectronico = correo;
}

const ListaDinamica<Usuario*>& Usuario::getAmigos() const {
    return amigos;
}

const ListaDinamica<Album*>& Usuario::getAlbumes() const {
    return albumes;
}

const ListaDinamica<Mensaje*>& Usuario::getEnviados() const {
    return enviados;
}

const ListaDinamica<Mensaje*>& Usuario::getRecibidos() const {
    return recibidos;
}

const ListaDinamica<SolicitudAmistad*>& Usuario::getSolicitudes() const {
    return solicitudes;
}

long Usuario::memoria() const {
    long total = sizeof(Usuario)
    + nombreUsuario.memoria()
        + correoElectronico.memoria()
        + clave.memoria()
        + ciudad.memoria()
        + pais.memoria()
        + fotoPerfil.memoria();

    total += amigos.memoria()
             + albumes.memoria()
             + enviados.memoria()
             + recibidos.memoria()
             + solicitudes.memoria();

    for (int i = 0; i < albumes.getCantidad(); i++)
        total += albumes[i]->memoria();

    return total;
}
