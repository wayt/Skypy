#ifndef DBFIELD_H_
# define DBFIELD_H_

#include <string>

class DbField
{
private:
    std::string _value;
public:
    DbField();
    void setValue(std::string const &value);
    std::string const &getValue() const;
};

#endif

