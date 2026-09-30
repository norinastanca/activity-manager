#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <vector>

using std::string;
#include "../domain/activitate.h"

using std::vector;


class RepoException :public std::exception {
    string msg;
public:
    RepoException(string m) :msg { m } {

    }
    string getMessage()const {
        return msg;
    }
};

class Repo {
private:
    vector<Activitate> activitati;

public:
    virtual void adauga(const Activitate& a);

    virtual void sterge(string titlu);

    virtual void modifica(const Activitate& a_noua);

    virtual void cauta(string titlu);

    virtual const vector<Activitate>& getAll() const;

    virtual ~Repo() = default;
};




class FileRepo : public Repo {
private:
    std::string fileName;
    void loadFromFile();
    void saveToFile();

public:
    FileRepo(std::string fName) : Repo(), fileName{ fName } {
        loadFromFile();
    }

    // Suprascriem metodele care modifica datele
    void adauga(const Activitate& a) override {
        Repo::adauga(a);
        saveToFile();
    }

    void sterge(string titlu) override {
        Repo::sterge(titlu);
        saveToFile();
    }

    void modifica(const Activitate& a) override {
        Repo::modifica(a);
        saveToFile();
    }
};
#endif