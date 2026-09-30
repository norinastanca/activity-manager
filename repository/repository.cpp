#include "repository.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

void Repo::adauga(const Activitate& a) {
    auto it = std::find_if(activitati.begin(), activitati.end(), [&](const Activitate& act) {
        return act.getTitlu() == a.getTitlu();
    });

    if (it != activitati.end()) {
        throw RepoException("Exista deja activitate cu acest titlu.");
    }

    activitati.push_back(a);
}

const vector<Activitate>& Repo::getAll() const {
    return activitati;
}

void Repo::sterge(string titlu) {

    auto it = std::find_if(activitati.begin(), activitati.end(), [&](const Activitate& act) {
        return act.getTitlu() == titlu;
    });

    if (it == activitati.end()) {
        throw RepoException("Nu exista activitatea.");
    }


    activitati.erase(it);
}

void Repo::modifica(const Activitate& a_noua) {
    auto it = std::find_if(activitati.begin(), activitati.end(), [&](const Activitate& act) {
        return act.getTitlu() == a_noua.getTitlu();
    });

    if (it == activitati.end()) {
        throw RepoException("Nu exista activitatea.");
    }

    *it = a_noua;
}

void Repo::cauta(string titlu) {
    auto it = std::find_if(activitati.begin(), activitati.end(), [&](const Activitate& act) {
        return act.getTitlu() == titlu;
    });

    if (it == activitati.end()) {
        throw RepoException("Nu exista activitatea.");
    }

}


void FileRepo::saveToFile() {
    std::ofstream out(fileName);
    if (!out.is_open()) {
        throw RepoException("Nu s-a putut deschide fisierul pentru scriere: " + fileName);
    }
    for (const auto& a : getAll()) {
        out << a.getTitlu() << "," << a.getDescriere() << ","
            << a.getTip() << "," << a.getDurata() << "\n";
    }
    out.close();
}


void FileRepo::loadFromFile() {
    std::ifstream in(fileName);
    if (!in.is_open()) {
        return; // daca fisierul nu exista pornim cu lista goala
    }
    std::string linie;
    while (std::getline(in, linie)) {
        if (linie.empty()) continue;
        std::stringstream ss(linie);
        std::string titlu, desc, tip, durataStr;

        // Extragem atributele separate prin virgula
        std::getline(ss, titlu, ',');
        std::getline(ss, desc, ',');
        std::getline(ss, tip, ',');
        std::getline(ss, durataStr, ',');

        if (!titlu.empty()) {
            Activitate a{ titlu, desc, tip,
                std::stoi(durataStr) };
            Repo::adauga(a);
        }
    }
    in.close();
}