#pragma once

#include "../domain/activitate.h"

class Repo;
class ActiuneUndo {

public:
    virtual~ActiuneUndo() = default;//destructorul
    virtual void doUndo() = 0;
};



class UndoAdauga : public ActiuneUndo {
    Activitate activitateAdaugata;
    Repo & repo;// referinta la repo pt a putea sterge

public:
    UndoAdauga(Repo & r,const Activitate& a) :  activitateAdaugata{a} ,repo{r} {};

    void doUndo() override {
        repo.sterge(activitateAdaugata.getTitlu());
    }
};

class UndoSterge : public ActiuneUndo {

        Activitate activitateStearsa;
        Repo & repo;

public:
    UndoSterge(Repo & r,const Activitate& a):  activitateStearsa{a} ,repo{r} {};
    void doUndo() override {
        repo.adauga(activitateStearsa);
    }
};

class UndoModifica : public ActiuneUndo {

    Activitate activitateVeche;
    Repo & repo;

public:
    UndoModifica(Repo & r,const Activitate& a) : activitateVeche {a},repo{r} {};

    void doUndo() override {
        repo.modifica(activitateVeche);
    }

};
