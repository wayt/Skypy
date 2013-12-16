#ifndef CONDITIONVARIABLE_H_
# define CONDITIONVARIABLE_H_

#include <boost/thread/condition_variable.hpp>

class ConditionVariable : public boost::condition_variable
{

};

#endif

