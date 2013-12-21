#ifndef AUTHWORKER_H_
# define AUTHWORKER_H_

#include <string>

enum AuthResults
{
    AUTHRESULT_OK           = 0,
    AUTHRESULT_BAD_USER     = 1,
    AUTHRESULT_BAD_PASS     = 2,
    AUTHRESULT_INTERAL_ERR  = 3,
};

class AuthWorker
{
public:
    AuthWorker(std::string const& email, std::string const& pass);

    AuthWorker const& digest();
    AuthResults result() const { return _result; }

private:
    std::string const _email;
    std::string const _pass;
    AuthResults _result;
};

#endif /* !AUTHWORKER_H_ */
