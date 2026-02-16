#include "cartanormal.h"

CartaNormal::CartaNormal(LadoCarta frontal): frente(frontal) {}

const LadoCarta& CartaNormal::getLadoActivo()const{
    return frente;
}

void CartaNormal::voltear(){
    //* No hay efecto para este modo de juego
}

bool CartaNormal::esFlip()const{
    return false;
}
