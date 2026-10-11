#include "FechaHora.h"
#include <iostream>
#include <iomanip>
#include <ctime>

FechaHora::FechaHora() : dia(1), mes(1), año(2000), hora(0), minuto(0), segundo(0) {}
FechaHora::FechaHora(int a, int m, int d, int h, int min, int s) : dia(d), mes(m), año(a), hora(h), minuto(min), segundo(s) {}

bool FechaHora::desdeTexto(const char* t) {
    int largo = 0;
    while (t[largo]) largo++;
    if (largo != 10 && largo != 19) return false;
    if (t[4] != '-' || t[7] != '-') return false;
    if (largo == 19 && (t[10] != 'T' || t[13] != ':' || t[16] != ':')) return false;
    for (int i = 0; i < largo; i++) {
        if (i == 4 || i == 7 || (largo == 19 && (i == 10 || i == 13 || i == 16))) continue;
        if (t[i] < '0' || t[i] > '9') return false;
    }
    año = (t[0]-'0')*1000+(t[1]-'0')*100+(t[2]-'0')*10+t[3]-'0';
    mes = (t[5]-'0')*10+t[6]-'0';
    dia = (t[8]-'0')*10+t[9]-'0';
    hora = minuto = segundo = 0;
    if (largo == 19) {
        hora = (t[11]-'0')*10+t[12]-'0';
        minuto = (t[14]-'0')*10+t[15]-'0';
        segundo = (t[17]-'0')*10+t[18]-'0';
    }
    return valida();
}

bool FechaHora::valida() const {
    if (año < 1900 || mes < 1 || mes > 12 || hora < 0 || hora > 23 || minuto < 0 || minuto > 59 || segundo < 0 || segundo > 59) return false;
    int dias = 31;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) dias = 30;
    if (mes == 2) dias = 28 + ((año % 4 == 0 && (año % 100 != 0 || año % 400 == 0)) ? 1 : 0);
    return dia >= 1 && dia <= dias;
}

bool FechaHora::operator<(const FechaHora& o) const {
    if (año != o.año) return año < o.año;
    if (mes != o.mes) return mes < o.mes;
    if (dia != o.dia) return dia < o.dia;
    if (hora != o.hora) return hora < o.hora;
    if (minuto != o.minuto) return minuto < o.minuto;
    return segundo < o.segundo;
}

bool FechaHora::operator==(const FechaHora& o) const {
    return año == o.año && mes == o.mes && dia == o.dia && hora == o.hora && minuto == o.minuto && segundo == o.segundo;
}

bool FechaHora::enRango(const FechaHora& a, const FechaHora& b) const {
    FechaHora primera(a.año, a.mes, a.dia);
    FechaHora ultima(b.año, b.mes, b.dia, 23, 59, 59);
    return !(*this < primera) && !(ultima < *this);
}

void FechaHora::escribir(std::ostream& salida) const {
    salida << std::setfill('0') << std::setw(4) << año << '-' << std::setw(2) << mes << '-' << std::setw(2) << dia;
    salida << 'T' << std::setw(2) << hora << ':' << std::setw(2) << minuto << ':' << std::setw(2) << segundo << std::setfill(' ');
}

void FechaHora::mostrar() const { escribir(std::cout); }

FechaHora FechaHora::actual() {
    std::time_t t = std::time(nullptr);
    std::tm* fecha = std::localtime(&t);
    return FechaHora(1900 + fecha->tm_year, 1 + fecha->tm_mon, fecha->tm_mday, fecha->tm_hour, fecha->tm_min, fecha->tm_sec);
}