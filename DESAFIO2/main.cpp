#include "Plataforma.h"
#include "Cadena.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    Plataforma sistema;
    cout << "\n"
         << "  +--------------------------------------------------+\n"
         << "  |                    Ude@Book                      |\n"
         << "  |             Tu comunidad universitaria           |\n"
         << "  +--------------------------------------------------+\n"
         << "  Conecta con tus amigos, comparte recuerdos y\n"
         << "  mantente al dia con tus mensajes.\n\n";
    if (!sistema.cargarDatos()) {

        cout << "\n"
             << "  [AVISO] No fue posible iniciar la plataforma.\n"
             << "  Verifica que la carpeta 'datos' exista y\n"
             << "  contenga el archivo usuarios.txt.\n";

        return 1;
    }
    char nombre[128];
    char clave[128];
    while (true) {

        cout << "\n"
             << "  +-------------------- INICIAR SESION --------------------+\n"
             << "  Usuario (salir para terminar): ";

        cin.getline(nombre, sizeof(nombre));
        if (Cadena(nombre) == Cadena("salir")) {
            break;
        }
        cout << "  contraseña: ";
        cin.getline(clave, sizeof(clave));
        if (sistema.iniciarSesion(
                Cadena(nombre),
                Cadena(clave))) {
            cout << "\n"
                 << "  Bienvenido/a a Ude@Book.\n"
                 << "  Que disfrutes tu comunidad.\n";
            sistema.mostrarMenu();
        } else {
            cout << "\n"
                 << "  [ACCESO DENEGADO]\n"
                 << "  Usuario o contraseña incorrectos.\n";
        }
    }
    sistema.guardarDatos();
    cout << "\n"
         << "  +--------------------------------------------------+\n"
         << "  |              Gracias por usar Ude@Book           |\n"
         << "  |                      Oscar y Luna                |\n"
         << "  +--------------------------------------------------+\n";

    return 0;
}