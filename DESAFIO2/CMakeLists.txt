cmake_minimum_required(VERSION 3.16)

project(DESAFIO2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DESAFIO2
    main.cpp
    Cadena.h
    Cadena.cpp
    FechaHora.h
    FechaHora.cpp
    ListaDinamica.h
    Foto.h
    Foto.cpp
    Album.h
    Album.cpp
    MedidorRecursos.h
    MedidorRecursos.cpp
    Usuario.h
    Mensaje.h
    SolicitudAmistad.h
    Usuario.cpp
    Mensaje.cpp
    SolicitudAmistad.cpp
    Archivo.h
    Plataforma.h
    Plataforma.cpp

)

include(GNUInstallDirs)

install(TARGETS DESAFIO2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)