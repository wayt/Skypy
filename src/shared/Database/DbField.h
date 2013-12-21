#ifndef DBFIELD_H_
# define DBFIELD_H_

#include <string>
#include "Utils.hpp"

class DbField
{
private:
    std::string _value;
public:
    DbField();
    void setValue(std::string const &value);
    std::string const &getValue() const;
    template<class T>
    T getValue() const { return Utils::to<T>(_value); }
};

#endif

