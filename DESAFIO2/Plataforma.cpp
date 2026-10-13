
#include "Plataforma.h"
#include "Usuario.h"
#include "Album.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;


static int leerNumero() {
    int numero = 0;

    if (!(cin >> numero)) {
        cin.clear();
        numero = 0;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return numero;
}


static Cadena leerTexto(const char* titulo, int maximo = 512) {
    char texto[2048];

    cout << titulo;
    cin.getline(texto, sizeof(texto));

    int longitud = 0;

    while (texto[longitud]) {
        longitud++;
    }

    if (longitud > maximo) {
        return Cadena();
    }

    return Cadena(texto);
}


Plataforma::Plataforma()
    : usuarioActual(nullptr),
    archivos(),
    medidor(),
    fotosActualizadas(false),
    fotosInicialesMostradas(false),
    fotosAnteriores() {
}


Plataforma::~Plataforma() {
    for (int i = 0; i < solicitudes.getCantidad(); i++) {
        delete solicitudes[i];
    }

    for (int i = 0; i < mensajes.getCantidad(); i++) {
        delete mensajes[i];
    }

    for (int i = 0; i < usuarios.getCantidad(); i++) {
        delete usuarios[i];
    }
}


Usuario* Plataforma::buscarUsuario(const Cadena& nombre) {
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        medidor.contarIteracion();

        if (usuarios[i]->getNombreUsuario() == nombre) {
            return usuarios[i];
        }
    }

    return nullptr;
}


Usuario* Plataforma::usuarioEn(int indice) const {
    return indice >= 0 && indice < usuarios.getCantidad()
    ? usuarios[indice]
    : nullptr;
}


int Plataforma::getCantidadUsuarios() const {
    return usuarios.getCantidad();
}


const ListaDinamica<Usuario*>& Plataforma::getUsuarios() const {
    return usuarios;
}


const ListaDinamica<Mensaje*>& Plataforma::getMensajes() const {
    return mensajes;
}


const ListaDinamica<SolicitudAmistad*>&
Plataforma::getSolicitudes() const {
    return solicitudes;
}


void Plataforma::agregarUsuario(Usuario* usuario) {
    if (usuario) {
        usuarios.agregar(usuario);
    }
}


void Plataforma::agregarAmistad(Usuario* a, Usuario* b) {
    if (a && b && a != b) {
        a->agregarAmigo(b);
        b->agregarAmigo(a);
    }
}


void Plataforma::agregarMensaje(Mensaje* mensaje) {
    if (!mensaje ||
        !mensaje->getEmisor() ||
        !mensaje->getReceptor()) {
        return;
    }

    mensajes.agregar(mensaje);

    mensaje->getEmisor()->agregarEnviado(mensaje);
    mensaje->getReceptor()->agregarRecibido(mensaje);
}


void Plataforma::agregarSolicitud(SolicitudAmistad* solicitud) {
    if (!solicitud ||
        !solicitud->getEmisor() ||
        !solicitud->getReceptor()) {
        return;
    }

    solicitudes.agregar(solicitud);
    solicitud->getReceptor()->agregarSolicitud(solicitud);
}


bool Plataforma::cargarDatos() {
    return archivos.cargar(*this);
}


bool Plataforma::guardarDatos() {
    return archivos.guardar(*this);
}


bool Plataforma::iniciarSesion(
    const Cadena& nombre,
    const Cadena& clave
    ) {
    medidor.reiniciar();

    Usuario* encontrado = buscarUsuario(nombre);

    if (!encontrado ||
        !(encontrado->getClave() == clave)) {
        return false;
    }

    usuarioActual = encontrado;
    fotosActualizadas = false;
    fotosInicialesMostradas = false;

    fotosAnteriores.limpiar();

    mostrarFotosAmigos();
    mostrarMetricas();

    return true;
}


bool Plataforma::haySolicitudPendiente(
    Usuario* a,
    Usuario* b
    ) const {
    for (int i = 0; i < solicitudes.getCantidad(); i++) {
        const SolicitudAmistad* s = solicitudes[i];

        if (s->getEstado() == 0 &&
            (
                (s->getEmisor() == a && s->getReceptor() == b) ||
                (s->getEmisor() == b && s->getReceptor() == a)
                )) {
            return true;
        }
    }

    return false;
}


bool Plataforma::enviarSolicitud(Usuario* destino) {
    if (!usuarioActual ||
        !usuarioActual->puedeEnviarSolicitud(destino) ||
        haySolicitudPendiente(usuarioActual, destino)) {
        return false;
    }

    agregarSolicitud(
        new SolicitudAmistad(usuarioActual, destino)
        );

    return true;
}


bool Plataforma::enviarMensaje(
    Usuario* destino,
    const Cadena& texto
    ) {
    if (!usuarioActual ||
        !usuarioActual->puedeEnviarMensaje(destino, texto)) {
        return false;
    }

    agregarMensaje(
        new Mensaje(
            usuarioActual,
            destino,
            FechaHora::actual(),
            texto
            )
        );

    return true;
}


void Plataforma::mostrarFotosAmigos() {
    if (!usuarioActual) {
        return;
    }

    const ListaDinamica<Usuario*>& amigos =
        usuarioActual->getAmigos();

    ListaDinamica<Usuario*> elegidos;

    if (fotosActualizadas) {
        cout << "La actualizacion ya se utilizo en esta sesion.\n";
        return;
    }

    int disponibles = amigos.getCantidad();

    if (disponibles == 0) {
        cout << "No tienes amigos para mostrar.\n";
        return;
    }

    int cantidad = disponibles < 3 ? disponibles : 3;

    for (int turno = 0; turno < cantidad; turno++) {
        int pesoTotal = 0;

        for (int i = 0; i < amigos.getCantidad(); i++) {
            medidor.contarIteracion();

            Usuario* candidato = amigos[i];
            bool usado = false;

            for (int j = 0; j < elegidos.getCantidad(); j++) {
                medidor.contarIteracion();

                if (elegidos[j] == candidato) {
                    usado = true;
                }
            }

            if (fotosInicialesMostradas) {
                for (int j = 0; j < fotosAnteriores.getCantidad(); j++) {
                    medidor.contarIteracion();

                    if (fotosAnteriores[j] == candidato) {
                        usado = true;
                    }
                }
            }

            if (!usado) {
                int comunes =
                    usuarioActual->contarAmigosComunes(
                        *candidato,
                        &medidor
                        );

                pesoTotal += comunes <= 10
                                 ? 10
                                 : (comunes <= 20 ? 25 : 35);
            }
        }

        if (pesoTotal == 0) {
            break;
        }

        int resultado = rand() % pesoTotal;
        Usuario* seleccionado = nullptr;

        for (int i = 0; i < amigos.getCantidad(); i++) {
            medidor.contarIteracion();

            Usuario* candidato = amigos[i];
            bool usado = false;

            for (int j = 0; j < elegidos.getCantidad(); j++) {
                medidor.contarIteracion();

                if (elegidos[j] == candidato) {
                    usado = true;
                }
            }

            if (fotosInicialesMostradas) {
                for (int j = 0; j < fotosAnteriores.getCantidad(); j++) {
                    medidor.contarIteracion();

                    if (fotosAnteriores[j] == candidato) {
                        usado = true;
                    }
                }
            }

            if (usado) {
                continue;
            }

            int comunes =
                usuarioActual->contarAmigosComunes(
                    *candidato,
                    &medidor
                    );

            int peso = comunes <= 10
                           ? 10
                           : (comunes <= 20 ? 25 : 35);

            if (resultado < peso) {
                seleccionado = candidato;
                break;
            }

            resultado -= peso;
        }

        if (seleccionado) {
            elegidos.agregar(seleccionado);
        }
    }

    if (elegidos.getCantidad() == 0) {
        cout << "No quedan fotos diferentes para actualizar.\n";
        return;
    }

    cout << "\nFotos de perfil de amigos:\n";

    for (int i = 0; i < elegidos.getCantidad(); i++) {
        medidor.contarIteracion();

        cout << elegidos[i]->getNombreUsuario().c_str()
             << ": "
             << elegidos[i]->getFotoPerfil().c_str()
             << '\n';
    }

    if (!fotosInicialesMostradas) {
        for (int i = 0; i < elegidos.getCantidad(); i++) {
            fotosAnteriores.agregar(elegidos[i]);
        }

        fotosInicialesMostradas = true;
    } else {
        fotosActualizadas = true;
    }
}


void Plataforma::mostrarSugerencias(int k) {
    if (k < 1 || k > 20 || !usuarioActual) {
        cout << "K debe estar entre 1 y 20.\n";
        return;
    }

    ListaDinamica<Usuario*> candidatos;
    ListaDinamica<int> cantidades;

    const ListaDinamica<Usuario*>& amigos =
        usuarioActual->getAmigos();

    for (int i = 0; i < amigos.getCantidad(); i++) {
        medidor.contarIteracion();

        const ListaDinamica<Usuario*>& conocidos =
            amigos[i]->getAmigos();

        for (int j = 0; j < conocidos.getCantidad(); j++) {
            medidor.contarIteracion();

            Usuario* candidato = conocidos[j];

            if (candidato == usuarioActual ||
                usuarioActual->tieneAmigo(candidato) ||
                haySolicitudPendiente(usuarioActual, candidato)) {
                continue;
            }

            bool repetido = false;

            for (int m = 0; m < candidatos.getCantidad(); m++) {
                medidor.contarIteracion();

                if (candidatos[m] == candidato) {
                    repetido = true;
                }
            }

            if (repetido) {
                continue;
            }

            int comunes =
                usuarioActual->contarAmigosComunes(
                    *candidato,
                    &medidor
                    );

            int pos = 0;

            while (pos < candidatos.getCantidad()) {
                medidor.contarIteracion();

                if (comunes > cantidades[pos] ||
                    (
                        comunes == cantidades[pos] &&
                        candidato->getNombreUsuario() <
                            candidatos[pos]->getNombreUsuario()
                        )) {
                    break;
                }

                pos++;
            }

            candidatos.insertar(pos, candidato);
            cantidades.insertar(pos, comunes);
        }
    }

    int limite = k < candidatos.getCantidad()
                     ? k
                     : candidatos.getCantidad();

    if (limite == 0) {
        cout << "No hay sugerencias disponibles.\n";
        return;
    }

    for (int i = 0; i < limite; i++) {
        medidor.contarIteracion();

        cout << i + 1
             << ". "
             << candidatos[i]->getNombreUsuario().c_str()
             << " ("
             << cantidades[i]
             << " amigos en comun)\n";
    }

    cout << "Enviar solicitud a cual numero (0 para volver): ";

    int eleccion = leerNumero();

    if (eleccion >= 1 && eleccion <= limite) {
        cout << (
            enviarSolicitud(candidatos[eleccion - 1])
                ? "Solicitud enviada.\n"
                : "No se pudo enviar.\n"
            );

        guardarDatos();
    }
}


void Plataforma::menuAmistades() {
    cout << "1. Enviar solicitud\n"
         << "2. Ver solicitudes recibidas\n"
         << "Opcion: ";

    int opcion = leerNumero();

    if (opcion == 1) {
        Cadena nombre = leerTexto("Nombre del usuario: ", 100);

        Usuario* destino = buscarUsuario(nombre);

        cout << (
            enviarSolicitud(destino)
                ? "Solicitud enviada.\n"
                : "No se puede enviar la solicitud.\n"
            );

        guardarDatos();

    } else if (opcion == 2) {
        const ListaDinamica<SolicitudAmistad*>& lista =
            usuarioActual->getSolicitudes();

        ListaDinamica<SolicitudAmistad*> pendientes;

        for (int i = 0; i < lista.getCantidad(); i++) {
            medidor.contarIteracion();

            if (lista[i]->getEstado() == 0) {
                pendientes.agregar(lista[i]);

                cout << pendientes.getCantidad()
                     << ". "
                     << lista[i]->getEmisor()
                            ->getNombreUsuario().c_str()
                     << '\n';
            }
        }

        if (pendientes.getCantidad() == 0) {
            cout << "No hay solicitudes pendientes.\n";
            return;
        }

        cout << "Numero (0 volver): ";

        int numero = leerNumero();

        if (numero < 1 || numero > pendientes.getCantidad()) {
            return;
        }

        cout << "1. Aceptar  2. Rechazar: ";

        int accion = leerNumero();

        bool ok = accion == 1
                      ? usuarioActual->aceptarSolicitud(
                            pendientes[numero - 1]
                            )
                      : (
                            accion == 2 &&
                            pendientes[numero - 1]->rechazar()
                            );

        cout << (
            ok
                ? "Solicitud procesada.\n"
                : "No se proceso.\n"
            );

        if (ok) {
            guardarDatos();
        }
    }
}


void Plataforma::menuMensajes() {
    cout << "1. Enviar mensaje\n"
         << "2. Leer nuevos\n"
         << "3. Buscar por fechas\n"
         << "Opcion: ";

    int opcion = leerNumero();

    if (opcion == 1) {
        Cadena nombre =
            leerTexto("Amigo destinatario: ", 100);

        Cadena texto =
            leerTexto("Mensaje (maximo 512 caracteres): ", 512);

        bool ok = enviarMensaje(
            buscarUsuario(nombre),
            texto
            );

        cout << (
            ok
                ? "Mensaje enviado.\n"
                : "Mensaje invalido o destinatario no es amigo.\n"
            );

        if (ok) {
            guardarDatos();
        }

    } else if (opcion == 2) {
        int mostrados = 0;

        const ListaDinamica<Mensaje*>& recibidos =
            usuarioActual->getRecibidos();

        for (int i = 0; i < recibidos.getCantidad(); i++) {
            medidor.contarIteracion();

            Mensaje* m = recibidos[i];

            if (!m->estaLeido()) {
                m->getFecha().mostrar();

                cout << " | "
                     << m->getEmisor()->getNombreUsuario().c_str()
                     << ": "
                     << m->getTexto().c_str()
                     << '\n';

                m->marcarLeido();
                mostrados++;
            }
        }

        if (!mostrados) {
            cout << "No hay mensajes nuevos.\n";
        } else {
            guardarDatos();
        }

    } else if (opcion == 3) {
        FechaHora inicio, fin;

        Cadena a =
            leerTexto("Fecha inicial AAAA-MM-DD: ", 10);

        Cadena b =
            leerTexto("Fecha final AAAA-MM-DD: ", 10);

        if (!inicio.desdeTexto(a.c_str()) ||
            !fin.desdeTexto(b.c_str()) ||
            fin < inicio) {
            cout << "Rango invalido.\n";
            return;
        }

        const ListaDinamica<Mensaje*>& recibidos =
            usuarioActual->getRecibidos();

        int mostrados = 0;

        for (int i = 0; i < recibidos.getCantidad(); i++) {
            medidor.contarIteracion();

            const Mensaje* m = recibidos[i];

            if (m->estaEnRango(inicio, fin)) {
                m->getFecha().mostrar();

                cout << " | "
                     << m->getEmisor()->getNombreUsuario().c_str()
                     << ": "
                     << m->getTexto().c_str()
                     << '\n';

                mostrados++;
            }
        }

        cout << "Mensajes encontrados: "
             << mostrados
             << '\n';
    }
}


long Plataforma::memoriaTotal() const {
    long total =
        sizeof(Plataforma)
        + usuarios.memoria()
        + mensajes.memoria()
        + solicitudes.memoria()
        + archivos.memoria()
        + fotosAnteriores.memoria();

    for (int i = 0; i < usuarios.getCantidad(); i++) {
        total += usuarios[i]->memoria();
    }

    for (int i = 0; i < mensajes.getCantidad(); i++) {
        total += mensajes[i]->memoria();
    }

    for (int i = 0; i < solicitudes.getCantidad(); i++) {
        total += sizeof(SolicitudAmistad);
    }

    return total;
}


void Plataforma::mostrarMetricas() {
    medidor.establecerMemoria(memoriaTotal());
    medidor.mostrarResultados();
}


void Plataforma::mostrarMenu() {
    while (usuarioActual) {
        cout << "\nUde@Book - "
             << usuarioActual->getNombreUsuario().c_str()
             << "\n";

        cout << "1. Administrar amistades\n"
             << "2. Visitar perfil de un amigo\n"
             << "3. Sugerencias de amistad\n"
             << "4. Mensajes\n"
             << "5. Actualizar fotos de amigos (una vez)\n"
             << "6. Cerrar sesion\n"
             << "Opcion: ";

        int opcion = leerNumero();

        if (opcion == 6) {
            usuarioActual = nullptr;
            break;
        }

        medidor.reiniciar();

        if (opcion == 1) {
            menuAmistades();

        } else if (opcion == 2) {
            Cadena nombre =
                leerTexto("Nombre de amigo: ", 100);

            Usuario* amigo = buscarUsuario(nombre);

            usuarioActual->verAlbumes(amigo, medidor);
            guardarDatos();

        } else if (opcion == 3) {
            cout << "Cantidad K (1 a 20): ";

            mostrarSugerencias(leerNumero());

        } else if (opcion == 4) {
            menuMensajes();

        } else if (opcion == 5) {
            mostrarFotosAmigos();

        } else {
            cout << "Opcion no valida.\n";
            continue;
        }

        mostrarMetricas();
    }
}
