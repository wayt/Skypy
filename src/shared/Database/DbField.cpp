#include "DbField.h"

DbField::DbField()
{

}

void DbField::setValue(std::string const &value)
{
    _value = value;
}

std::string const &DbField::getValue() const
{
    return _value;
}
