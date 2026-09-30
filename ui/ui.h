#ifndef UI_H
#define UI_H
#include "../service/service.h"

class Ui {
private:
    Service& serv;
public:
    Ui(Service& serv) : serv{ serv } {}
    void run();
    void afiseazaStatistica();
};

#endif