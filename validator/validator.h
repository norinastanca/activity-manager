#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <exception>
#include <vector>
#include <string>
using std::vector;
#include "../domain/activitate.h"

class ValidatorException {
    std::vector<string> msg;
public:
    ValidatorException(const vector<string>& erori) : msg{erori} {}
    string getMessage() const ;
};
class Validator {
public:
    void valideaza(const Activitate& a);
};


#endif