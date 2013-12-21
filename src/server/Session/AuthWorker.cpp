#include "AuthWorker.h"
#include "SkypyDatabase.h"
#include <boost/regex.hpp>

AuthWorker::AuthWorker(std::string const& email, std::string const& pass) :
    _email(email), _pass(pass), _result(AUTHRESULT_INTERAL_ERR)
{
}

AuthWorker const& AuthWorker::digest()
{
    boost::regex re("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+.[A-Za-z]{2,6}$");
    if (!boost::regex_match(_email, re))
    {
        _result = AUTHRESULT_BAD_USER;
        return *this;
    }

    _result = AUTHRESULT_OK;
    return *this;
}

