#ifndef ACTIVITATE_H  // Dacă "ACTIVITATE_H" nu este definit...
#define ACTIVITATE_H
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;


class Activitate {

private:
    string titlu;
    string descriere;
    string tip;
    int durata;

public:

    int getDurata() const;
    const string& getTitlu () const;
    const string& getDescriere() const;
    const string& getTip() const;

    Activitate(const string& titlu,const string& descriere,const string& tip, int durata) :titlu{ titlu }, descriere { descriere } ,tip { tip }, durata { durata }{};

    Activitate( const Activitate& act) :titlu{ act.getTitlu() }, descriere{ act.getDescriere() }, tip{ act.getTip() }, durata{ act.getDurata() } {
        cout << "S-a copiat" <<endl;
        cout << act.getTitlu() << endl;
    };
    Activitate& operator=( const Activitate& act);
};
#endif