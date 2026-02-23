#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <exception>

class IndiceFueraDeRangoException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Indice fuera de rango en ListaGenerica";
    }
};

#endif // EXCEPCIONES_H
