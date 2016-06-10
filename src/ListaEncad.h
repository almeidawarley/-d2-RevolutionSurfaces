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
        Explicação de cada uma das funções de nossa lista encadeada:


        ListaEncad() -> construtor da lista, inicia os parâmetros
        ~ListaEncad() -> destrutor da lista
        bool busca(float val) -> busca um determinado valor na lista
        void elimina(int ponto -> elimina um determinado ponto atravé de seu índice;
        void insere(float x, float y) -> insere um novo ponto com base nas suas coordenadas;
        void imprime() -> imprime a lista, útil para conferência;
        int consultaTamanho() -> informa quantos pontos estão cadastrados na lista;
        void retornaPontos(int num, float x[], float y[]) -> retorna os valores de X e de Y de todos os valores da lista em vetores;
        void atualizaPonto(int num, float x, float y) -> atualiza os valores de X e Y de um determinado ponto, sendo x e y a variação desses valores;

    */

  private:
    Ponto* primeiro;
    Ponto* ultimo;
    int n;
};
#endif
