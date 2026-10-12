#ifndef SOLICITUDAMISTAD_H
#define SOLICITUDAMISTAD_H
class Usuario;

class SolicitudAmistad {
private:
    Usuario* emisor;
    Usuario* receptor;
    int estado;
public:
    SolicitudAmistad(Usuario* emisor, Usuario* receptor, int estado = 0);
    bool aceptar();
    bool rechazar();
    Usuario* getEmisor() const;
    Usuario* getReceptor() const;
    int getEstado() const;
};
#endif