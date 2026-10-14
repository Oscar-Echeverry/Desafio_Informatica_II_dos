#ifndef PLATAFORMA_H
#define PLATAFORMA_H
#include "Cadena.h"
#include "ListaDinamica.h"
#include "Archivo.h"
#include "MedidorRecursos.h"

class Usuario;
class Mensaje;
class SolicitudAmistad;

class Plataforma {
private:
    ListaDinamica<Usuario*> usuarios;
    ListaDinamica<Mensaje*> mensajes;
    ListaDinamica<SolicitudAmistad*> solicitudes;
    Usuario* usuarioActual;
    Archivos archivos;
    MedidorRecursos medidor;
    bool fotosActualizadas;
    bool fotosInicialesMostradas;
    ListaDinamica<Usuario*> fotosAnteriores;
    void mostrarMetricas();
    void menuAmistades();
    void menuMensajes();
    bool haySolicitudPendiente(Usuario* origen, Usuario* destino) const;
public:
    Plataforma();
    ~Plataforma();
    bool iniciarSesion(const Cadena& nombre, const Cadena& clave);
    Usuario* buscarUsuario(const Cadena& nombre);
    Usuario* usuarioEn(int indice) const;
    int getCantidadUsuarios() const;
    const ListaDinamica<Usuario*>& getUsuarios() const;
    const ListaDinamica<Mensaje*>& getMensajes() const;
    const ListaDinamica<SolicitudAmistad*>& getSolicitudes() const;
    void agregarUsuario(Usuario* usuario);
    void agregarAmistad(Usuario* a, Usuario* b);
    void agregarMensaje(Mensaje* mensaje);
    void agregarSolicitud(SolicitudAmistad* solicitud);
    bool cargarDatos();
    bool guardarDatos();
    void mostrarFotosAmigos();
    void mostrarSugerencias(int k);
    bool enviarSolicitud(Usuario* destino);
    bool enviarMensaje(Usuario* destino, const Cadena& texto);
    void mostrarMenu();
    long memoriaTotal() const;
};
#endif
