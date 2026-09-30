#include "ui.h"
#include <iomanip>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;


void Ui::afiseazaStatistica() {
    auto statistica = serv.getStatisticaDTO();

    if (statistica.empty()) {
        cout << "Nu exista activitati in lista.\n";
        return;
    }

    cout << "--- STATISTICA ACTIVITATI ---\n";


    for (const auto& x : statistica) {
        // Luăm DTO-ul din map
        const TipStatistica& dto = x.second;

        cout << "Tipul: " << dto.getTip()
             << " | Numar activitati: " << dto.getCount() << "\n";
    }
    cout << "\n";
}
void afiseazaLista(const std::vector<Activitate>& lista) {
    if (lista.empty()) {
        cout << "Lista este goala!" << endl;
        return;
    }
    for (const auto& i : lista) {
        cout << "Titlu: " << i.getTitlu() << " | Descriere: " << i.getDescriere()
             << " | Tip: " << i.getTip() << " | Durata: " << i.getDurata() << endl;
    }
}

void Ui::run() {
    while (true) {
        cout << "\n MENIU ACTIVITATI \n";
        cout << "0. Exit\n1. Adauga\n2. Sterge\n3. Modifica\n4. Cauta\n5. Afisare\n";
        cout << "6. Filtrare (0-descriere, 1-tip)\n7. Sortare (0-titlu, 1-descriere, 2-tip+durata)\n";
        cout << "\n13.Undo.\n";
        cout << "\n MENIU PLANIFICARE \n";
        cout << "8. Goleste cos\n9. Adauga in cos (dupa titlu)\n10. Genereaza cos (aleator)\n11. Export cos (CSV)\n12.Statistica tipuri.";
        cout << "\n";
        cout << "Activitati in cos: " << serv.listaDimensiune() << endl;
        cout << "Comanda: ";



        int cmd;
        if (!(cin >> cmd)) {
            cout << "Comanda invalida! Introduceti un numar.\n";
            cin.clear(); cin.ignore(1000, '\n');
            continue;
        }

        switch (cmd) {
            case 0: return;

            case 1: {
                string titlu, desc, tip; int durata;
                cout << "Titlu: "; getline(cin >> std::ws, titlu);
                cout << "Descriere: "; getline(cin >> std::ws, desc);
                cout << "Tip: "; getline(cin >> std::ws, tip);
                cout << "Durata: ";
                if (!(cin >> durata)) { cin.clear(); cin.ignore(1000, '\n'); break; }
                try {
                    serv.adaugaActivitate(titlu, desc, tip, durata);
                    cout << "Succes!\n";
                }
                catch (const RepoException& e) { cout << "Eroare Repo: " << e.getMessage() << endl; }
                catch (const ValidatorException& e) { cout << "Eroare Validare: " << e.getMessage() << endl; }
                break;
            }

            case 2: {
                string titlu;
                cout << "Titlu pentru stergere: "; cin >> titlu;
                try {
                    serv.stergeActivitate(titlu);
                    cout << "Succes!\n";
                } catch (const RepoException& e) { cout << "Eroare: " << e.getMessage() << endl; }
                catch (const ValidatorException& e) { cout << "Eroare Validare: " << e.getMessage() << endl; } // SI ASTA
                break;
            }

            case 3: {
                string titlu, desc, tip; int durata;
                cout << "Titlu existent: "; getline(cin >> std::ws, titlu);
                cout << "Descriere noua: "; getline(cin >> std::ws, desc);
                cout << "Tip nou: "; getline(cin >> std::ws, tip);
                cout << "Durata noua: ";
                if (!(cin >> durata)) { cin.clear(); cin.ignore(1000, '\n'); break; }
                try {
                    serv.modificaActivitate(titlu, desc, tip, durata);
                    cout << "Modificare realizata!\n";
                }
                catch (const RepoException& e) { cout << "Eroare Repo: " << e.getMessage() << endl; }
                catch (const ValidatorException& e) { cout << "Eroare Validare: " << e.getMessage() << endl; }
                break;
            }

            case 4: {
                string titlu;
                cout << "Titlu pentru cautare: "; cin >> titlu;
                try {
                    serv.cautaActivitate(titlu);
                    cout << "Activitatea exista.\n";
                } catch (const RepoException& e) { cout << "Eroare: " << e.getMessage() << endl; }
                catch (const ValidatorException& e) { cout << "Eroare Validare: " << e.getMessage() << endl; } // SI ASTA
                break;
            }

            case 5: afiseazaLista(serv.getAll()); break;

            case 6: {
                int mod;
                cout << "0-descriere, 1-tip: "; cin >> mod;
                if (mod == 0) {
                    string desc; cout << "Descriere: "; cin >> desc;
                    afiseazaLista(serv.filtreazaDupaDescriere(desc));
                } else {
                    string tip; cout << "Tip: "; cin >> tip;
                    afiseazaLista(serv.filtreazaDupaTip(tip));
                }
                break;
            }

            case 7: {
                int mod;
                cout << "0-titlu, 1-descriere, 2-tip+durata: "; cin >> mod;
                std::vector<Activitate> rez;
                if (mod == 0) rez = serv.sorteazaDupaTitlu();
                else if (mod == 1) rez = serv.sorteazaDupaDescriere();
                else if (mod == 2) rez = serv.sorteazaDupaTipSiDurata();
                afiseazaLista(rez);
                break;
            }

            case 8: serv.listaGoleste(); cout << "Cos golit.\n"; break;

            case 9: {
                string titlu;
                cout << "Titlu pentru cos: "; getline(cin >> std::ws, titlu);
                try {
                    serv.listaAdauga(titlu);
                    cout << "Adaugat in cos!\n";
                } catch (const RepoException& e) { cout << "Eroare: " << e.getMessage() << endl; }
                catch (const ValidatorException& e) { cout << "Eroare Validare: " << e.getMessage() << endl; } // ADAUGA ASTA
                break;
            }

            case 10: {
                int nr;
                cout << "Cate activitati? ";
                if (!(cin >> nr)) { cin.clear(); cin.ignore(1000, '\n'); break; }
                try {
                    serv.listaGenereaza(nr);
                    cout << "S-au generat " << nr << " activitati.\n";
                } catch (const RepoException& e) { cout << "Eroare: " << e.getMessage() << endl; }
                catch (const ValidatorException& e) { cout << "Eroare Validare: " << e.getMessage() << endl; } // SI ASTA
                break;
            }

            case 11: {
                string nume;
                cout << "Nume fisier (CSV): "; cin >> nume;
                try {
                    serv.listaExport(nume);
                    cout << "Exportat!\n";
                } catch (const RepoException& e) { cout << "Eroare: " << e.getMessage() << endl; }
                break;
            }
            case 12: {
                afiseazaStatistica();
            }
            default: cout << "Comanda invalida!\n";
            case 13: {
                try {
                    serv.undo();
                    cout << "Undo realizat cu succes!\n";
                } catch (const ServiceException& e) {
                    // Aici folosim clasa de exceptii pe care am creat-o pentru Service
                    cout << e.getMsg() << "\n";
                } catch (const RepoException& e) {
                    // Uneori repo-ul poate arunca erori daca datele restaurate sunt invalide
                    cout << "Eroare la refacerea datelor: " << e.getMessage() << "\n";
                }
                break;
            }
        }
    }
}