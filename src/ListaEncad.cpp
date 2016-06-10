/*
    Name:        ListaEncad.cpp
    Copyright:   Version 0.1
    Author:      Lucas Carvalho Ribeiro
                 Warley Almeida Silva
    Date:        06/05/2016
    Description: Lista com os pontos de controle
*/

#include <iostream>
#include <stdlib.h>
#include "ListaEncad.h"

using namespace std;

ListaEncad::ListaEncad()
{
    primeiro = NULL;
    ultimo = NULL;
    n = 0;
}

ListaEncad::~ListaEncad()
{
    Ponto* p = primeiro;
    while(p != NULL)
    {
        Ponto* t = p->getProx();
        delete p;
        p = t;
    }
}

bool ListaEncad::busca(float val)
{
    Ponto* p;
    for(p = primeiro; p != NULL; p = p->getProx())
    {
        if(p->getNumero() == val)
            return true;
    }
    return false;
}

void ListaEncad::elimina(int ponto){
    if(primeiro->getNumero()==ponto){
        if(primeiro==ultimo){
            delete primeiro;
            primeiro = NULL;
            ultimo = NULL;
            n = n-1;
        }else{
            Ponto *p = primeiro;
            primeiro = p->getProx();
            delete p;
            for(p=primeiro;p!=ultimo;p=p->getProx())
            {
                p->setNumero(p->getNumero()-1);
            }
            n = n-1;
        }
    }else{
        Ponto * aux = primeiro;
        while(aux->getProx()!=NULL){
            if(aux->getProx()->getNumero() == ponto){
                Ponto *p = aux->getProx()->getProx();
                delete aux->getProx();
                aux->setProx(p);
                Ponto *correcao = aux->getProx();
                if(correcao==NULL){
                    ultimo = aux;
                }else{
                    while(correcao!=NULL){  //Correção dos próximos índices
                        int f = correcao->getNumero();
                        correcao->setNumero(f-1);
                        correcao = correcao->getProx();
                    }
                }
            }
            aux = aux->getProx();
            if(aux==NULL){
                break;
            }
        }
        n = n-1;
    }
}

void ListaEncad::insere(float x, float y){
    Ponto* p = new Ponto(n,x,y);
    n++;
    if(primeiro==NULL){
        primeiro = p;
        ultimo = p;
    }else{
        ultimo->setProx(p);
        p->setProx(NULL);
        ultimo = p;
    }
}

void ListaEncad::imprime(){
    Ponto *p;
    p = primeiro;
    while(p!=NULL){
        cout << "Ponto: " << p->getNumero() << " - X: " << p->getX() << " - Y: " << p->getY() << endl;
        p = p->getProx();
    }
}

int ListaEncad::consultaTamanho(){
    return n;
}

void ListaEncad::retornaPontos(int num, float x[], float y[])
{
    Ponto *p = primeiro;
    for(int i=num; i<n; i++)
    {
        if(p!=NULL){
        x[i] = p->getX();
        y[i] = p->getY();
        p = p->getProx();
        }
    }
}

void ListaEncad::atualizaPonto(int num, float x, float y){
    Ponto *p = primeiro;
    while(p!=NULL){
        if(p->getNumero()==num){
            p->setX(p->getX()+x);
            p->setY(p->getY()+y);
        }
        p = p->getProx();
    }
}
