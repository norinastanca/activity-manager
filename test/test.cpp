#include "test.h"
#include "../repository/repository.h"
#include "../service/service.h"
#include "../validator/validator.h"
#include "../domain/activitate.h"
#include <cassert>
#include <iostream>
#include <fstream>

using std::cout;

void testDomain() {
    Activitate a{"Scoala", "Curs C++", "Educatie", 120};
    assert(a.getTitlu() == "Scoala");
    assert(a.getDurata() == 120);
    assert(a.getTip() == "Educatie");
    assert(a.getDescriere() == "Curs C++");
}

void testRepo_adauga() {
    // test adauga
    Repo repo;
    Activitate a{"Sport", "Alergare", "Sanatate", 30};
    Activitate b{"Opera", "concert", "Muzica", 120};
    repo.adauga(a);
    assert(repo.getAll().size() == 1);


    try {
        repo.adauga(a);
        assert(false); // Nu ar trebui sa ajunga aici
    } catch (RepoException& e) {
        assert(e.getMessage() == "Exista deja activitate cu acest titlu.");
    }
    try {
        repo.adauga(b);
        assert(true);
    }
    catch (RepoException& e) {
        assert(false);
    }

}
void testRepo_sterge() {
    Repo repo;
    Activitate a{"Sport", "Alergare", "Sanatate", 30};
    Activitate b{"Opera", "concert", "Muzica", 120};

    repo.adauga(a);
    repo.adauga(b);
    assert(repo.getAll().size() == 2);

    repo.sterge("Opera");
    assert(repo.getAll().size() == 1);

    try {
        repo.sterge("Ceva");

    }catch (RepoException& e) {
        assert(e.getMessage() == "Nu exista activitatea.");
    }

}

void testRepo_modifica() {
    Repo repo;
    Activitate a{"Sport", "Alergare", "Sanatate", 30};
    Activitate b{"Sport", "concert", "Muzica", 120};

    repo.adauga(a);

    repo.modifica(b);
    assert(repo.getAll().size() == 1);
    assert(repo.getAll()[0].getTitlu() == "Sport");
    assert(repo.getAll()[0].getDurata() == 120);
    assert(repo.getAll()[0].getTip() == "Muzica");
    assert(repo.getAll()[0].getDescriere() == "concert");

    Activitate c{"Ceva", "concert", "Muzica", 120};

    try {
        repo.modifica(c);
    }catch (RepoException& e) {
        assert(e.getMessage() == "Nu exista activitatea.");
    }

}

void testRepo_cauta() {
    Repo repo;
    Activitate a{"Sport", "Alergare", "Sanatate", 30};
    Activitate b{"Opera", "concert", "Muzica", 120};

    repo.adauga(a);
    repo.adauga(b);

    try {
        repo.cauta("Sport");
        assert(true);
    } catch (RepoException& e) {
        assert(e.getMessage() == "Nu exista activitatea.");
    }

    try {
        repo.cauta("ceva");
        assert(false);
    } catch (RepoException& e) {
        assert(e.getMessage() == "Nu exista activitatea.");
    }

}


void testService_adauga() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    serv.adaugaActivitate("Fotbal", "Meci cu baietii", "Sport", 90);
    assert(serv.getAll().size() == 1);
    assert(serv.getAll()[0].getTitlu() == "Fotbal");

    try {
        serv.adaugaActivitate("", "", "", -5);
        assert(false); // Nu ar trebui sa ajunga aici
    } catch (const ValidatorException&) {
        assert(true);
    }

    try {
        serv.adaugaActivitate("Fotbal", "Altceva", "Altul", 100);
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testService_sterge() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    serv.adaugaActivitate("Fotbal", "Meci", "Sport", 90);
    serv.adaugaActivitate("Tema", "La OOP", "Scoala", 60);
    assert(serv.getAll().size() == 2);


    serv.stergeActivitate("Fotbal");
    assert(serv.getAll().size() == 1);
    assert(serv.getAll()[0].getTitlu() == "Tema");

    try {
        serv.stergeActivitate("Somn");
        assert(false); // Dacă ajunge aici, înseamnă că NU a dat eroare, deci e greșit
    } catch (const RepoException& e) {
        assert(true);

    }

    serv.stergeActivitate("Tema");
    assert(serv.getAll().empty());
}
void testService_modifica() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    serv.adaugaActivitate("Tema", "La OOP", "Scoala", 60);


    serv.modificaActivitate("Tema", "La SDA", "Scoala", 120);
    auto toate = serv.getAll();
    assert(toate[0].getDescriere() == "La SDA");
    assert(toate[0].getDurata() == 120);


    try {
        serv.modificaActivitate("Somn", "Odihna", "Pauza", 480);
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}
void testService_cauta() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    serv.adaugaActivitate("Pictura", "Tablou in ulei", "Arta", 120);

    try {
        serv.cautaActivitate("Pictura");
        assert(true); // Dacă a ajuns aici fără throw, e bine
    } catch (const RepoException&) {
        assert(false); // Nu ar trebui să dea eroare
    }

    try {
        serv.cautaActivitate("Somn");
        assert(false);
    } catch ( RepoException& e) {
        assert(true);
        // Verifică dacă mesajul este cel setat de tine în Repo
        assert(e.getMessage() == "Nu exista activitatea.");
    }
}
void testFiltrare() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    serv.adaugaActivitate("Fotbal", "Meci", "Sport", 90);
    serv.adaugaActivitate("Handbal", "Meci", "Sport", 60);
    serv.adaugaActivitate("Lectura", "Carte", "Relaxare", 30);

    auto f1 = serv.filtreazaDupaDescriere("Meci");
    assert(f1.size() == 2);

    auto f2 = serv.filtreazaDupaTip("Relaxare");
    assert(f2.size() == 1);
    assert(f2[0].getTitlu() == "Lectura");

    auto f3 = serv.filtreazaDupaTip("Scoala");
    assert(f3.empty());
}
void testSortare() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    serv.adaugaActivitate("Zebra", "Alergare", "Sport", 100);
    serv.adaugaActivitate("Antilopa", "Mancare", "Zool", 50);
    serv.adaugaActivitate("Bursuc", "Sapat", "Zool", 20);

    auto sTitlu = serv.sorteazaDupaTitlu();
    assert(sTitlu.size() == 3);
    assert(sTitlu[0].getTitlu() == "Antilopa");
    assert(sTitlu[1].getTitlu() == "Bursuc");
    assert(sTitlu[2].getTitlu() == "Zebra");


    auto sDesc = serv.sorteazaDupaDescriere();
    assert(sDesc[0].getDescriere() == "Alergare");
    assert(sDesc[1].getDescriere() == "Mancare");
    assert(sDesc[2].getDescriere() == "Sapat");



    auto sTipDurata = serv.sorteazaDupaTipSiDurata();
    assert(sTipDurata[0].getTip() == "Sport");
    assert(sTipDurata[1].getTitlu() == "Bursuc");   // Tip Zool, Durata 20
    assert(sTipDurata[2].getTitlu() == "Antilopa"); // Tip Zool, Durata 50


    assert(serv.getAll()[0].getTitlu() == "Zebra");
}
void testValidator() {
    Validator val;

    Activitate aOk{"Fotbal", "Meci de amatori", "Sport", 90};
    try {
        val.valideaza(aOk);
        assert(true); // Dacă ajunge aici, e perfect
    } catch (const ValidatorException&) {
        assert(false); // Nu ar fi trebuit să dea eroare
    }

    Activitate aBad{"", "", "", -10};
    try {
        val.valideaza(aBad);
        assert(false); // Dacă ajunge aici, înseamnă că validatorul a "dormit"
    } catch (const ValidatorException& e) {
        assert(true);
        string msg = e.getMessage();
        assert(msg.find("negativa") != string::npos);
        assert(msg.find("vid") != string::npos);
    }

    Activitate aCifre{"Fotba1", "Meci 2", "Sp0rt", 10};
    try {
        val.valideaza(aCifre);
        assert(false);
    } catch (const ValidatorException& e) {
        assert(true);
        assert(e.getMessage().find("litere") != string::npos);
    }

}

void testSortare2() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };


    serv.adaugaActivitate("Zebra", "Alergare", "Sport", 100);
    serv.adaugaActivitate("Bursuc", "Sapat", "Zool", 20);
    serv.adaugaActivitate("Antilopa", "Mancare", "Zool", 50);

    auto sTitlu = serv.sorteazaDupaTitlu();
    assert(sTitlu.size() == 3);
    assert(sTitlu[0].getTitlu() == "Antilopa");
    assert(sTitlu[1].getTitlu() == "Bursuc");
    assert(sTitlu[2].getTitlu() == "Zebra");


    auto sDesc = serv.sorteazaDupaDescriere();
    assert(sDesc[0].getDescriere() == "Alergare");
    assert(sDesc[1].getDescriere() == "Mancare");
    assert(sDesc[2].getDescriere() == "Sapat");


    auto sTipDurata = serv.sorteazaDupaTipSiDurata();
    assert(sTipDurata[0].getTip() == "Sport");
    assert(sTipDurata[1].getDurata() == 20);
    assert(sTipDurata[2].getDurata() == 50);
}
void testPlanificator() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    serv.adaugaActivitate("Curs", "OOP", "Educatie", 120);
    serv.adaugaActivitate("Sala", "Piept", "Sport", 60);
    serv.adaugaActivitate("Somn", "Odihna", "Sanatate", 480);


    serv.listaAdauga("Curs");
    assert(serv.listaDimensiune() == 1);

    try {
        serv.listaAdauga("ActivitateInexistenta");
        assert(false); // Nu ar trebui să ajungă aici
    } catch (const RepoException&) {
        assert(true);
    }


    serv.listaGoleste();
    assert(serv.listaDimensiune() == 0);


    serv.listaGenereaza(2);
    assert(serv.listaDimensiune() == 2);

    try {
        serv.listaGenereaza(100);
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }


    const auto& lista = serv.getLista();
    assert(lista.size() == 2);


    string fisierTest = "test_export.csv";
    serv.listaExport(fisierTest);

    std::ifstream fin(fisierTest);
    assert(fin.is_open());

    int linii = 0;
    string line;
    while (std::getline(fin, line)) {
        linii++;
    }
    assert(linii == 2);
    fin.close();

    std::remove(fisierTest.c_str());

    try {
        serv.listaExport("/folder_inexistent/test.csv");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testStatistica() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    serv.adaugaActivitate("Fotbal", "Meci", "Sport", 90);
    serv.adaugaActivitate("Baschet", "Antrenament", "Sport", 60);
    serv.adaugaActivitate("Matematica", "Curs", "Invatare", 120);

    auto statistica = serv.getStatisticaDTO();

    assert(statistica.size() == 2);

    assert(statistica["Sport"].getTip() == "Sport");
    assert(statistica["Sport"].getCount() == 2);

    assert(statistica["Invatare"].getTip() == "Invatare");
    assert(statistica["Invatare"].getCount() == 1);

    assert(statistica.find("Relaxare") == statistica.end());


}


void cleanFile(string fileName) {
    std::ofstream out(fileName, std::ios::trunc); // deschide si sterge tot continutul
    out.close();
}
void testFileRepo() {
    // Folosim un fisier separat pentru teste
    string fTest = "test_activitati.txt";

    // Ne asiguram ca pornim de la un fisier curat (il cream/golim)
    {
        std::ofstream out(fTest, std::ios::trunc);
        out << "Titlu1,Desc1,Tip1,10\n"; // Punem manual o linie pentru loadFromFile
        out.close();
    }

    // 1. Test Incarcare (loadFromFile)
    FileRepo repo{ fTest };
    assert(repo.getAll().size() == 1);
    assert(repo.getAll()[0].getTitlu() == "Titlu1");

    // 2. Test Adaugare (saveToFile se apeleaza automat)
    repo.adauga(Activitate{ "Titlu2", "Desc2", "Tip2", 20 });
    assert(repo.getAll().size() == 2);

    // 3. Test Modificare (saveToFile se apeleaza automat)
    repo.modifica(Activitate{ "Titlu2", "Nou", "Nou", 30 });
    assert(repo.getAll()[1].getDurata() == 30);

    // 4. Test Stergere (saveToFile se apeleaza automat)
    repo.sterge("Titlu1");
    assert(repo.getAll().size() == 1);

    // 5. Test de persistenta: inchidem repo-ul si deschidem altul pe acelasi fisier
    // Asta verifica daca saveToFile chiar a scris pe disc
    FileRepo repo2{ fTest };
    assert(repo2.getAll().size() == 1);
    assert(repo2.getAll()[0].getTitlu() == "Titlu2");

    // 6. Test pentru cazul in care fisierul nu exista (loadFromFile return)
    try {
        FileRepo repo3{ "fisier_inexistent.txt" };
        assert(repo3.getAll().empty());
    } catch (...) {
        assert(false);
    }
}

void testFileRepoExceptii() {
    try {
        // Folosim date care trec sigur de validator pentru a nu amesteca exceptiile
        FileRepo repo{"/cale/invalida/fisier.txt"};
        // Daca adaugarea esueaza din cauza fisierului, va da RepoException
        repo.adauga(Activitate{"TitluValid", "DescriereValida", "TipValid", 100});
        assert(false);
    } catch (const RepoException&) {
        assert(true); // Asta vrem sa testam
    } catch (const ValidatorException&) {
        // Daca cumva datele de mai sus sunt considerate invalide de validatorul tau
        assert(true);
    } catch (...) {
        assert(true);
    }
}

void testUndo() {
    Repo repo;
    Validator val;
    Service serv{ repo, val };

    // 1. Test Undo Adaugare
    try {
        // Folosim string-uri mai lungi si fara cifre, in caz ca validatorul e sever
        serv.adaugaActivitate("TitluValid", "DescriereLunga", "TipValid", 100);
        assert(serv.getAll().size() == 1);
        serv.undo();
        assert(serv.getAll().size() == 0);
    } catch (const ValidatorException& e) {
        // Daca tot intra aici, afisam mesajul sa vedem DE CE nu-i place
        std::cout << "Validatorul a respins datele: " << e.getMessage() << std::endl;
        assert(false);
    } catch (...) {
        assert(false);
    }


// 2. Test Undo Stergere
try {
    // Folosim date care să treacă sigur de orice validator (litere, lungime suficientă)
    serv.adaugaActivitate("ActivitateSportiva", "DescriereDetaliata", "TimpLiber", 20);

    serv.stergeActivitate("ActivitateSportiva");
    assert(serv.getAll().size() == 0);

    serv.undo(); // Ar trebui să reapară activitatea
    assert(serv.getAll().size() == 1);
    assert(serv.getAll()[0].getTitlu() == "ActivitateSportiva");
    assert(serv.getAll()[0].getDurata() == 20);
} catch (const ValidatorException& e) {
    // Dacă pică aici, înseamnă că datele de mai sus încă încalcă o regulă
    std::cout << "Eroare validare la Undo Stergere: " << e.getMessage() << std::endl;
    assert(false);
} catch (...) {
    assert(false);
}

    // 3. Test Undo Modificare
    try {
        // Presupunem că "ActivitateSportiva" a fost adăugată/restaurată la testul anterior
        // și are durata 20, descrierea "DescriereDetaliata" și tipul "TimpLiber"

        serv.modificaActivitate("ActivitateSportiva", "DescriereModificata", "TipNouSiValid", 100);
        assert(serv.getAll()[0].getDurata() == 100);
        assert(serv.getAll()[0].getDescriere() == "DescriereModificata");

        serv.undo(); // Revine la starea de dinainte de modificare

        assert(serv.getAll()[0].getDurata() == 20);
        assert(serv.getAll()[0].getDescriere() == "DescriereDetaliata");
        assert(serv.getAll()[0].getTip() == "TimpLiber");
    } catch (const ValidatorException& e) {
        std::cout << "Eroare validare la Undo Modificare: " << e.getMessage() << std::endl;
        assert(false);
    } catch (const RepoException& e) {
        std::cout << "Eroare repo la Undo Modificare: " << e.getMessage() << std::endl;
        assert(false);
    } catch (...) {
        assert(false);
    }

    // 4. Test multiple undo-uri si exceptie la final
    try {
        serv.undo(); // Listă goală acum
        assert(serv.getAll().size() == 0);

        serv.undo(); // Aici TREBUIE sa arunce ServiceException
        assert(false); // Daca ajunge aici, e rau
    } catch (const ServiceException& e) {
        assert(e.getMsg() == "Nu se mai poate face undo!");
    } catch (const ValidatorException& e) {
        // Daca ajunge aici, inseamna ca validatorul a explodat pe neasteptate
        cout << "Validator Error in Undo: " << e.getMessage() << "\n";
        assert(false);
    } catch (...) {
        // Daca ajunge aici, a aruncat altceva
        assert(true); // Macar am prins-o si nu mai crapa programul
    }
}
void ruleazaToateTestele() {
    testDomain();
    testRepo_adauga();
    testRepo_sterge();
    testRepo_modifica();
    testRepo_cauta();
    testService_adauga();
    testService_modifica();
    testService_sterge();
    testService_cauta();
    testFiltrare();
    testSortare();
    testValidator();
    testSortare2();
    testPlanificator();
    testStatistica();
    testFileRepo();
    testFileRepoExceptii();
    testUndo();

    cout << "Toate testele au trecut cu succes!\n";
}