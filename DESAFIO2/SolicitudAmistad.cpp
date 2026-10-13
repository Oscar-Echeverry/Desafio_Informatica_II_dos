#include "SolicitudAmistad.h"
#include "Usuario.h"

SolicitudAmistad::SolicitudAmistad(Usuario* e, Usuario* r, int s)
    : emisor(e), receptor(r), estado(s) {
}

bool SolicitudAmistad::aceptar() {
    if (estado != 0 || emisor == nullptr || receptor == nullptr) {
        return false;
    }

    if (emisor == receptor) {
        return false;
    }

    if (emisor->getNombreUsuario() == receptor->getNombreUsuario()) {
        return false;
    }

    if (!emisor->agregarAmigo(receptor)) {
        return false;
    }

    if (!receptor->agregarAmigo(emisor)) {
        return false;
    }

    estado = 1;
    return true;
}

bool SolicitudAmistad::rechazar() {
    if (estado != 0) {
        return false;
    }

    estado = 2;
    return true;
}

Usuario* SolicitudAmistad::getEmisor() const {
    return emisor;
}

Usuario* SolicitudAmistad::getReceptor() const {
    return receptor;
}

int SolicitudAmistad::getEstado() const {
    return estado;
}