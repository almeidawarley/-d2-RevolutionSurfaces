/*
    Name:        ListaEncad.h
    Copyright:   Version 0.1
    Author:      Lucas Carvalho Ribeiro
                 Warley Almeida Silva
    Date:        06/05/2016
    Description: Lista com os pontos de controle
*/

#ifndef LISTASENCAD_H_INCLUDED
#define LISTASENCAD_H_INCLUDED
#include "Ponto.h"

class ListaEncad
{
  public:
    ListaEncad();
    ~ListaEncad();
    bool busca(float val);
    void elimina(int ponto);
    void insere(float x, float y);
    void imprime();
    int consultaTamanho();
    void retornaPontos(int num, float x[], float y[]);
    void atualizaPonto(int num, float x, float y);

    /*
        Explica��o de cada uma das fun��es de nossa lista encadeada:


        ListaEncad() -> construtor da lista, inicia os par�metros
        ~ListaEncad() -> destrutor da lista
        bool busca(float val) -> busca um determinado valor na lista
        void elimina(int ponto -> elimina um determinado ponto atrav� de seu �ndice;
        void insere(float x, float y) -> insere um novo ponto com base nas suas coordenadas;
        void imprime() -> imprime a lista, �til para confer�ncia;
        int consultaTamanho() -> informa quantos pontos est�o cadastrados na lista;
        void retornaPontos(int num, float x[], float y[]) -> retorna os valores de X e de Y de todos os valores da lista em vetores;
        void atualizaPonto(int num, float x, float y) -> atualiza os valores de X e Y de um determinado ponto, sendo x e y a varia��o desses valores;

    */

  private:
    Ponto* primeiro;
    Ponto* ultimo;
    int n;
};
#endif
