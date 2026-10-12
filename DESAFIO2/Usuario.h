#ifndef USUARIO_H
#define USUARIO_H
#include "Cadena.h"
#include "FechaHora.h"
#include "ListaDinamica.h"

class Album;
class Mensaje;
class SolicitudAmistad;
class MedidorRecursos;

class Usuario {
    friend bool mismoNombre(const Usuario& a, const Usuario& b);
private:
    Cadena nombreUsuario, correoElectronico, clave;
    Cadena ciudad, pais, fotoPerfil;
    FechaHora nacimiento, inscripcion;
    ListaDinamica<Usuario*> amigos;
    ListaDinamica<Album*> albumes;
    ListaDinamica<Mensaje*> enviados;
    ListaDinamica<Mensaje*> recibidos;
    ListaDinamica<SolicitudAmistad*> solicitudes;
public:
    Usuario(const Cadena& nombre, const Cadena& correo, const Cadena& clave,
            const Cadena& ciudad, const Cadena& pais, const Cadena& fotoPerfil,
            const FechaHora& nacimiento, const FechaHora& inscripcion);
    ~Usuario();
    bool tieneAmigo(const Usuario* otro) const;
    bool agregarAmigo(Usuario* otro);
    void agregarAlbum(Album* album);
    void agregarEnviado(Mensaje* mensaje);
    void agregarRecibido(Mensaje* mensaje);
    void agregarSolicitud(SolicitudAmistad* solicitud);
    bool puedeEnviarSolicitud(Usuario* destino) const;
    bool aceptarSolicitud(SolicitudAmistad* solicitud);
    int contarAmigosComunes(const Usuario& otro, MedidorRecursos* medidor = nullptr) const;
    bool puedeEnviarMensaje(Usuario* destino, const Cadena& texto) const;
    void verAlbumes(Usuario* amigo, MedidorRecursos& medidor);
    const Cadena& getNombreUsuario() const;
    const Cadena& getCorreo() const;
    const Cadena& getClave() const;
    const Cadena& getCiudad() const;
    const Cadena& getPais() const;
    const Cadena& getFotoPerfil() const;
    const FechaHora& getNacimiento() const;
    const FechaHora& getInscripcion() const;
    void setCorreo(const Cadena& correo);
    const ListaDinamica<Usuario*>& getAmigos() const;
    const ListaDinamica<Album*>& getAlbumes() const;
    const ListaDinamica<Mensaje*>& getEnviados() const;
    const ListaDinamica<Mensaje*>& getRecibidos() const;
    const ListaDinamica<SolicitudAmistad*>& getSolicitudes() const;
    long memoria() const;
};
#endif
