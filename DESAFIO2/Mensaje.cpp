#include "Mensaje.h"

Mensaje::Mensaje(Usuario* e, Usuario* r, const FechaHora& f, const Cadena& t, bool l)
    : emisor(e), receptor(r), fechaEnvio(f), texto(t), leido(l) {
}

void Mensaje::marcarLeido() {
    leido = true;
}

bool Mensaje::estaEnRango(const FechaHora& i, const FechaHora& f) const {
    return fechaEnvio.enRango(i, f);
}

Usuario* Mensaje::getEmisor() const {
    return emisor;
}

Usuario* Mensaje::getReceptor() const {
    return receptor;
}

const FechaHora& Mensaje::getFecha() const {
    return fechaEnvio;
}

const Cadena& Mensaje::getTexto() const {
    return texto;
}

bool Mensaje::estaLeido() const {
    return leido;
}

long Mensaje::memoria() const {
    return sizeof(Mensaje) + texto.memoria();
}