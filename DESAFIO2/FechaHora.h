#ifndef FECHAHORA_H
#define FECHAHORA_H
#include <iosfwd>
class FechaHora {
private:
    int dia, mes, año;
    int hora, minuto, segundo;
public:
    FechaHora();
    FechaHora(int año, int mes, int dia, int hora = 0, int minuto = 0, int segundo = 0);
    bool desdeTexto(const char* texto);
    bool valida() const;
    bool operator<(const FechaHora& otra) const;
    bool operator==(const FechaHora& otra) const;
    bool enRango(const FechaHora& inicio, const FechaHora& fin) const;
    void escribir(std::ostream& salida) const;
    void mostrar() const;
    static FechaHora actual();
};
#endif
