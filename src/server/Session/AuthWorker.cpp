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

    // Check email validity before the SQL request to prevent SQL injections

    DbResultPtr result = sSkypyDb->query("SELECT id FROM account WHERE email = '%s' LIMIT 1", _email.c_str());
    if (!result->fetch())
    {
        _result = AUTHRESULT_BAD_USER;
        return *this;
    }

    uint32 id = (*result)["id"]->getValue<uint32>();

    result = sSkypyDb->query("SELECT 1 FROM account WHERE id = '%u' AND pass_hash = SHA1('%s')", id, _pass.c_str());

    if (!result->fetch())
    {
        _result = AUTHRESULT_BAD_PASS;
        return *this;
    }


    _result = AUTHRESULT_OK;
    return *this;
}

