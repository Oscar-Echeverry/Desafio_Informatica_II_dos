#include "Archivo.h"
#include "Plataforma.h"
#include "Usuario.h"
#include "Album.h"
#include "Foto.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"
#include "FechaHora.h"
#include <fstream>
#include <iostream>

using namespace std;


static bool campo(
    const char* origen,
    int columna,
    char* destino,
    int tamano,
    bool hastaFinal = false
    ) {
    int actual = 0;
    int j = 0;
    int i = 0;

    while (origen[i] && actual < columna) {
        if (origen[i] == '|') {
            actual++;
        }

        i++;
    }

    if (actual != columna) {
        destino[0] = '\0';
        return false;
    }

    while (origen[i] && (hastaFinal || origen[i] != '|')) {
        if (j + 1 < tamano) {
            destino[j++] = origen[i];
        }

        i++;
    }

    destino[j] = '\0';

    return true;
}


static long numero(const char* texto) {
    int i = 0;
    long n = 0;

    bool negativo = texto[0] == '-';

    if (negativo) {
        i++;
    }

    while (texto[i] >= '0' && texto[i] <= '9') {
        n = n * 10 + texto[i] - '0';
        i++;
    }

    return negativo ? -n : n;
}


static int privacidad(const char* texto) {
    if (Cadena(texto) == Cadena("publico")) {
        return 0;
    }

    if (Cadena(texto) == Cadena("amigos")) {
        return 1;
    }

    return 2;
}


Archivos::Archivos() : rutaBase("datos/") {
}


Archivos::Archivos(const Cadena& ruta) : rutaBase(ruta) {
}


const Cadena& Archivos::getRutaBase() const {
    return rutaBase;
}


long Archivos::memoria() const {
    return rutaBase.memoria();
}


bool Archivos::cargar(Plataforma& sistema) {
    char linea[4096];

    char a[2048], b[2048], c[2048], d[2048];
    char e[2048], f[2048], g[2048], h[2048];

    ifstream usuarios("datos/usuarios.txt");

    if (!usuarios) {
        return false;
    }

    while (usuarios.getline(linea, sizeof(linea))) {
        if (!campo(linea, 0, a, sizeof(a)) ||
            !campo(linea, 1, b, sizeof(b)) ||
            !campo(linea, 2, c, sizeof(c)) ||
            !campo(linea, 3, d, sizeof(d)) ||
            !campo(linea, 4, e, sizeof(e)) ||
            !campo(linea, 5, f, sizeof(f)) ||
            !campo(linea, 6, g, sizeof(g)) ||
            !campo(linea, 7, h, sizeof(h))) {
            continue;
        }

        FechaHora nacimiento, inscripcion;

        if (!nacimiento.desdeTexto(b) ||
            !inscripcion.desdeTexto(h)) {
            continue;
        }

        if (!sistema.buscarUsuario(Cadena(a))) {
            sistema.agregarUsuario(
                new Usuario(
                    Cadena(a),
                    Cadena(c),
                    Cadena(e),
                    Cadena(f),
                    Cadena(g),
                    Cadena(d),
                    nacimiento,
                    inscripcion
                    )
                );
        }
    }

    usuarios.close();


    ifstream amistades("datos/amistades.txt");

    while (amistades.getline(linea, sizeof(linea))) {
        if (!campo(linea, 0, a, sizeof(a)) ||
            !campo(linea, 1, b, sizeof(b))) {
            continue;
        }

        Usuario* primero = sistema.buscarUsuario(Cadena(a));
        Usuario* segundo = sistema.buscarUsuario(Cadena(b));

        sistema.agregarAmistad(primero, segundo);
    }

    amistades.close();


    ifstream albumes("datos/albumes.txt");

    while (albumes.getline(linea, sizeof(linea))) {
        if (!campo(linea, 0, a, sizeof(a)) ||
            !campo(linea, 1, b, sizeof(b)) ||
            !campo(linea, 2, c, sizeof(c)) ||
            !campo(linea, 3, d, sizeof(d))) {
            continue;
        }

        Usuario* duenio = sistema.buscarUsuario(Cadena(a));

        if (duenio) {
            duenio->agregarAlbum(
                new Album(
                    Cadena(b),
                    Cadena(c),
                    privacidad(d)
                    )
                );
        }
    }

    albumes.close();


    ifstream fotos("datos/fotos.txt");

    while (fotos.getline(linea, sizeof(linea))) {
        if (!campo(linea, 0, a, sizeof(a)) ||
            !campo(linea, 1, b, sizeof(b)) ||
            !campo(linea, 2, c, sizeof(c)) ||
            !campo(linea, 3, d, sizeof(d)) ||
            !campo(linea, 4, e, sizeof(e)) ||
            !campo(linea, 5, f, sizeof(f))) {
            continue;
        }

        Usuario* duenio = sistema.buscarUsuario(Cadena(a));

        FechaHora captura;

        if (!duenio || !captura.desdeTexto(c)) {
            continue;
        }

        const ListaDinamica<Album*>& lista =
            duenio->getAlbumes();

        for (int i = 0; i < lista.getCantidad(); i++) {
            if (lista[i]->getNombre() == Cadena(b)) {
                lista[i]->agregarFoto(
                    new Foto(
                        captura,
                        numero(d),
                        Cadena(e),
                        static_cast<int>(numero(f))
                        )
                    );

                break;
            }
        }
    }

    fotos.close();


    ifstream mensajes("datos/mensajes.txt");

    while (mensajes.getline(linea, sizeof(linea))) {
        if (!campo(linea, 0, a, sizeof(a)) ||
            !campo(linea, 1, b, sizeof(b)) ||
            !campo(linea, 2, c, sizeof(c)) ||
            !campo(linea, 3, d, sizeof(d)) ||
            !campo(linea, 4, e, sizeof(e), true)) {
            continue;
        }

        Usuario* emisor = sistema.buscarUsuario(Cadena(a));
        Usuario* receptor = sistema.buscarUsuario(Cadena(b));

        FechaHora fecha;

        if (!emisor ||
            !receptor ||
            !fecha.desdeTexto(c)) {
            continue;
        }

        sistema.agregarMensaje(
            new Mensaje(
                emisor,
                receptor,
                fecha,
                Cadena(e),
                numero(d) != 0
                )
            );
    }

    mensajes.close();


    ifstream solicitudes("datos/solicitudes.txt");

    while (solicitudes.getline(linea, sizeof(linea))) {
        if (!campo(linea, 0, a, sizeof(a)) ||
            !campo(linea, 1, b, sizeof(b)) ||
            !campo(linea, 2, c, sizeof(c))) {
            continue;
        }

        Usuario* emisor = sistema.buscarUsuario(Cadena(a));
        Usuario* receptor = sistema.buscarUsuario(Cadena(b));

        int estado = static_cast<int>(numero(c));

        if (emisor &&
            receptor &&
            emisor != receptor &&
            estado >= 0 &&
            estado <= 2) {
            sistema.agregarSolicitud(
                new SolicitudAmistad(
                    emisor,
                    receptor,
                    estado
                    )
                );
        }
    }

    solicitudes.close();

    return sistema.getCantidadUsuarios() > 0;
}


bool Archivos::guardar(const Plataforma& sistema) {
    ofstream usuarios("datos/usuarios.txt");
    ofstream amistades("datos/amistades.txt");
    ofstream albumes("datos/albumes.txt");
    ofstream fotos("datos/fotos.txt");
    ofstream mensajes("datos/mensajes.txt");
    ofstream solicitudes("datos/solicitudes.txt");

    if (!usuarios ||
        !amistades ||
        !albumes ||
        !fotos ||
        !mensajes ||
        !solicitudes) {
        return false;
    }

    for (int i = 0; i < sistema.getCantidadUsuarios(); i++) {
        const Usuario* u = sistema.usuarioEn(i);

        usuarios << u->getNombreUsuario().c_str() << '|';

        u->getNacimiento().escribir(usuarios);

        usuarios << '|'
                 << u->getCorreo().c_str() << '|'
                 << u->getFotoPerfil().c_str() << '|'
                 << u->getClave().c_str() << '|'
                 << u->getCiudad().c_str() << '|'
                 << u->getPais().c_str() << '|';

        u->getInscripcion().escribir(usuarios);

        usuarios << '\n';


        const ListaDinamica<Usuario*>& amigos =
            u->getAmigos();

        for (int j = 0; j < amigos.getCantidad(); j++) {
            if (u->getNombreUsuario() <
                amigos[j]->getNombreUsuario()) {
                amistades
                    << u->getNombreUsuario().c_str()
                    << '|'
                    << amigos[j]->getNombreUsuario().c_str()
                    << '\n';
            }
        }


        const ListaDinamica<Album*>& lista =
            u->getAlbumes();

        for (int j = 0; j < lista.getCantidad(); j++) {
            const Album* al = lista[j];

            const char* visibilidad =
                al->getPrivacidad() == 0
                    ? "publico"
                    : (
                          al->getPrivacidad() == 1
                              ? "amigos"
                              : "privado"
                          );

            albumes
                << u->getNombreUsuario().c_str()
                << '|'
                << al->getNombre().c_str()
                << '|'
                << al->getRuta().c_str()
                << '|'
                << visibilidad
                << '\n';


            for (int k = 0; k < al->getCantidadFotos(); k++) {
                const Foto* foto = al->getFoto(k);

                fotos
                    << u->getNombreUsuario().c_str()
                    << '|'
                    << al->getNombre().c_str()
                    << '|';

                foto->getCaptura().escribir(fotos);

                fotos
                    << '|'
                    << foto->gettamaño()
                    << '|'
                    << foto->getRuta().c_str()
                    << '|'
                    << foto->getLikes()
                    << '\n';
            }
        }
    }


    for (int i = 0;
         i < sistema.getMensajes().getCantidad();
         i++) {

        const Mensaje* m = sistema.getMensajes()[i];

        mensajes
            << m->getEmisor()->getNombreUsuario().c_str()
            << '|'
            << m->getReceptor()->getNombreUsuario().c_str()
            << '|';

        m->getFecha().escribir(mensajes);

        mensajes
            << '|'
            << (m->estaLeido() ? 1 : 0)
            << '|'
            << m->getTexto().c_str()
            << '\n';
    }


    for (int i = 0;
         i < sistema.getSolicitudes().getCantidad();
         i++) {

        const SolicitudAmistad* s =
            sistema.getSolicitudes()[i];

        solicitudes
            << s->getEmisor()->getNombreUsuario().c_str()
            << '|'
            << s->getReceptor()->getNombreUsuario().c_str()
            << '|'
            << s->getEstado()
            << '\n';
    }

    return usuarios.good() &&
           amistades.good() &&
           albumes.good() &&
           fotos.good() &&
           mensajes.good() &&
           solicitudes.good();
}