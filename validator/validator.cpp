#include "validator.h"
#include <vector>
#include <string>
#include <iostream>

string ValidatorException::getMessage() const {

    string full_msg = "";
    for (const auto& m : msg) {
        full_msg += m + " ";
    }
    return full_msg;
}

void Validator::valideaza(const Activitate& a) {

    vector<string> erori;

    if (a.getDurata() <= 0) {
        erori.push_back("Durata nu poate fi negativa sau zero!");

    }

    if (a.getTitlu().empty()) {
        erori.push_back("Titlul nu poate fi vid!");

    }
    else {
        for (char c : a.getTitlu()) {
            if (!isalpha(c) && !isspace(c)) {
                erori.push_back("Titlul trebuie sa contina doar litere!");
                break;
            }

        }

    }
    if (a.getDescriere().empty()) {
        erori.push_back("Descrierea nu poate fi vida!");

    }
    else {
        for (char c : a.getDescriere()) {
            if (!isalpha(c) && !isspace(c)) {
                erori.push_back("Descriere trebuie sa contina doar litere!");
                break;
            }
        }
    }

    if (a.getTip().empty()) {
        erori.push_back("Tipul nu poate fi vid!");

    }
    else {
        for (char c : a.getTip()) {
            if (!isalpha(c) && !isspace(c)) {
                erori.push_back("Tipul trebuie sa contina doar litere!");
                break;
            }
        }
    }
    if (!erori.empty()) {
        throw ValidatorException(erori);
    }
}