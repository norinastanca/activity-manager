#include "test/test.h"


#include "repository/repository.h"
#include "service/service.h"
#include "ui/ui.h"
#include "validator/validator.h"
#include <iostream>

int main() {
    ruleazaToateTestele();

     FileRepo repo{"activitati.txt"};
     Validator val;
     Service serv{repo,val};
     Ui ui{serv};
     ui.run();

    return 0;
}