#ifndef CADENA_H
#define CADENA_H
#include <cstddef>

class Cadena {
private:
    char* texto;
    int longitud;
public:
    Cadena();
    Cadena(const char* origen);
    Cadena(const Cadena& otra);
    ~Cadena();
    Cadena& operator=(const Cadena& otra);
    bool operator==(const Cadena& otra) const;
    bool operator!=(const Cadena& otra) const;
    bool operator<(const Cadena& otra) const;
    const char* c_str() const;
    int getLongitud() const;
    long memoria() const;
};
#endif
