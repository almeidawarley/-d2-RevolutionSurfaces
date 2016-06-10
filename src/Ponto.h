/*
    Name:        Ponto.h
    Copyright:   Version 0.1
    Author:      Lucas Carvalho Ribeiro
                 Warley Almeida Silva
    Date:        06/05/2016
    Description: Estrutura dos pontos contidos na lista
*/

#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED

class Ponto
{
  private:
    int numero;
    float x,y;
    Ponto* prox;
    public:
    Ponto(int numeroponto, float xponto, float yponto) {
        numero = numeroponto;
        x = xponto;
        y = yponto;
    };
    ~Ponto() {};

    float getNumero(){
        return numero;
    };
    Ponto* getProx(){
        return prox;
    };
    void setNumero(int val) {
        numero = val;
    };
    void setProx(Ponto* p){
        prox = p;
    };
    float getX(){
        return x;
    }
     float getY(){
        return y;
    }
    void setY(float newy){
        y = newy;
    }
    void setX(float newx){
        x = newx;
    }
};
#endif // NO_H_INCLUDED
