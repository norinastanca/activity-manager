#include "service.h"
#include <algorithm>
#include <iterator>
#include <fstream>
#include <chrono>
#include <random>
const vector<Activitate>& Service::getAll() const {
    return repo.getAll();
}

void Service::adaugaActivitate(const string& titlu, const string& descriere, const string& tip, int durata) {
    Activitate a{ titlu, descriere, tip, durata };
    val.valideaza(a);
    repo.adauga(a);

    istoricUndo.push_back(std::make_unique<UndoAdauga>(repo,a));
}

void Service::stergeActivitate(const string& titlu) {
    // Luam toate activitatile din repo (presupunand ca ai o metoda getAll)
    const auto& toate = repo.getAll();

    // Cautam activitatea cu titlul respectiv
    auto it = std::find_if(toate.begin(), toate.end(), [&](const Activitate& a) {
        return a.getTitlu() == titlu;
    });

    if (it!=toate.end()){
    // Salvam o copie a obiectului gasit
    Activitate a = *it;
    repo.sterge(titlu);
    istoricUndo.push_back(std::make_unique<UndoSterge>(repo,a));
    }
    else {
        repo.sterge(titlu);
    }
}

void Service::modificaActivitate(const string& titlu, const string& descriereNoua, const string& tipNou, int durataNoua) {
    Activitate a_noua{ titlu, descriereNoua, tipNou, durataNoua };
    val.valideaza(a_noua);

    const auto& toate = repo.getAll();
    auto it = std::find_if(toate.begin(), toate.end(), [&](const Activitate& a) {
        return a.getTitlu() == titlu;
    });

    if (it == toate.end()) {
        // Daca nu am gasit-o, lasam Repo-ul sa arunce exceptia specifica
        // (Sau arunci tu una daca Repo-ul nu o face)
        repo.modifica(a_noua);
        return; // Linia de mai sus va arunca exceptie si va opri functia
    }

    // Daca am ajuns aici, inseamna ca exista
    Activitate a_veche = *it;
    repo.modifica(a_noua); // Facem modificarea
    istoricUndo.push_back(std::make_unique<UndoModifica>(repo, a_veche));
}

void Service::cautaActivitate(const string& titlu) {
    repo.cauta(titlu);
}

vector<Activitate> Service::filtreazaDupaDescriere(const string& descriere) {
    const vector<Activitate>& toate = repo.getAll();
    vector<Activitate> rez;

    std::copy_if(toate.begin(), toate.end(), std::back_inserter(rez),[&descriere](const Activitate& a) {
            return a.getDescriere() == descriere;
        });

    return rez;
}

vector<Activitate> Service::filtreazaDupaTip(const string& tip) {
    const vector<Activitate>& toate = repo.getAll();
    vector<Activitate> rez;

    std::copy_if(toate.begin(), toate.end(), std::back_inserter(rez),
        [&tip](const Activitate& a) {
            return a.getTip() == tip;
        });

    return rez;
}

vector<Activitate> Service::sorteazaDupaTitlu() {
    vector<Activitate> v = repo.getAll();
    std::sort(v.begin(), v.end(), [](const Activitate& a1, const Activitate& a2) {
        return a1.getTitlu() < a2.getTitlu();
    });
    return v;
}

vector<Activitate> Service::sorteazaDupaDescriere() {
    vector<Activitate> v = repo.getAll();
    std::sort(v.begin(), v.end(), [](const Activitate& a1, const Activitate& a2) {
        return a1.getDescriere() < a2.getDescriere();
    });
    return v;
}

vector<Activitate> Service::sorteazaDupaTipSiDurata() {
    vector<Activitate> v = repo.getAll();
    std::sort(v.begin(), v.end(), [](const Activitate& a1, const Activitate& a2) {
        if (a1.getTip() == a2.getTip()) {
            return a1.getDurata() < a2.getDurata();
        }
        return a1.getTip() < a2.getTip();
    });
    return v;
}

void Service::listaGoleste() {
    listaPlanificate.clear();
}

void Service::listaAdauga(string titlu) {
    const auto& toate=repo.getAll();
    auto it = std::find_if(toate.begin(), toate.end(),[&] (const Activitate& a){
            return a.getTitlu() == titlu;
    });

    if (it == toate.end()) {
        throw RepoException("Activitatea nu a fost gasita!");
    }

    listaPlanificate.push_back(*it);
}


void Service::listaGenereaza(int nr) {
    auto toate=repo.getAll();

    if ((unsigned long) nr >toate.size()) {
        throw RepoException("Nu sunt destule activitati!");
    }

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(toate.begin(), toate.end(), std::default_random_engine((unsigned int)seed));

    listaPlanificate.clear();
    for (int i=0;i<nr;i++)
        listaPlanificate.push_back(toate[i]);
}

const vector<Activitate>& Service::getLista() const {
    return listaPlanificate;
}

void Service::listaExport(string numeFisier) {

    std::ofstream fout(numeFisier);
    if (!fout.is_open()) {
        throw RepoException("Nu s-a putut deschide fisierul!");
    }

    for (const auto& a : listaPlanificate) {
        fout <<a.getTitlu() << ","
            <<a.getDescriere() << ","
            <<a.getTip() << ","
            <<a.getDurata() << '\n';
    }
    fout.close();
}

int Service::listaDimensiune() const {
    return (int)listaPlanificate.size();
}

std::map<std::string, TipStatistica> Service::getStatisticaDTO() const {
    std::map<std::string, TipStatistica> statistica;
    const auto& toate = repo.getAll();

    for (const auto& a : toate) {
        string tip = a.getTip();

        if (statistica.find(tip) == statistica.end()) {
            statistica[tip] = TipStatistica(tip, 1);
        } else {
            int countActual = statistica[tip].getCount();
            statistica[tip] = TipStatistica(tip, countActual + 1);
        }
    }

    return statistica;
}

void Service::undo() {
    if (istoricUndo.empty()) {
        throw ServiceException("Nu se mai poate face undo!");
    }

    istoricUndo.back()->doUndo();

    istoricUndo.pop_back();
}