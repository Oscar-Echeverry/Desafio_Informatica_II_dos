#ifndef MENSAJE_H
#define MENSAJE_H
#include "Cadena.h"
#include "FechaHora.h"

class Usuario;

class Mensaje {
private:
    Usuario* emisor;
    Usuario* receptor;
    FechaHora fechaEnvio;
    Cadena texto;
    bool leido;
public:
    Mensaje(Usuario* emisor, Usuario* receptor,
            const FechaHora& fecha, const Cadena& texto, bool leido = false);
    void marcarLeido();
    bool estaEnRango(const FechaHora& inicio, const FechaHora& fin) const;
    Usuario* getEmisor() const;
    Usuario* getReceptor() const;
    const FechaHora& getFecha() const;
    const Cadena& getTexto() const;
    bool estaLeido() const;
    long memoria() const;
};
#endif
