#ifndef SERVICE_H
#define SERVICE_H

#include <vector>
#include <memory>
#include <string>
#include "../repository/repository.h"
#include "../validator/validator.h"
#include <map>
#include "../Undo/Undo.h"

using std::vector;
using std::string;

class ServiceException {
    std::string msg;
public:
    ServiceException(std::string m) : msg{ m } {}
    std::string getMsg() const { return msg; }
};

class TipStatistica {
private:
    string tip;
    int count;
public:
    TipStatistica() : tip{ "" }, count{ 0 } {};
    TipStatistica(string t, int c) : tip{ t }, count{ c } {};

    string getTip() const { return tip; }
    int getCount() const { return count; }
};
class Service {
private:
    Repo& repo;
    Validator& val;

    vector<Activitate> listaPlanificate;

    vector <std::unique_ptr<ActiuneUndo>> istoricUndo;

public:
    Service(Repo& repo, Validator& val) : repo{ repo }, val{ val } {};


    const vector<Activitate>& getAll() const;

    void adaugaActivitate(const string& titlu, const string& descriere, const string& tip, int durata);
    void stergeActivitate(const string& titlu);
    void modificaActivitate(const string& titlu, const string& descriereNoua, const string& tipNou, int durataNoua);
    void cautaActivitate(const string& titlu);

    vector<Activitate> filtreazaDupaDescriere(const string& descriere);
    vector<Activitate> filtreazaDupaTip(const string& tip);

    vector<Activitate> sorteazaDupaTitlu();
    vector<Activitate> sorteazaDupaDescriere();
    vector<Activitate> sorteazaDupaTipSiDurata();


    void listaGoleste();
    void listaAdauga(string titlu);
    void listaGenereaza(int nr);
    void listaExport(string numeFisier);
    int listaDimensiune () const;
    const vector<Activitate>& getLista() const;

    std::map<std::string, TipStatistica> getStatisticaDTO() const;

    void undo();
};

#endif