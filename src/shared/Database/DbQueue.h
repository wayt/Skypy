/*
** DbQueue.h for  in /home/vaga/Projects/Skypy/src/shared/Database
** 
** Made by 
** Login   <@epitech.eu>
** 
** Started on  Mon Dec 09 17:03:02 2013 
** Last update Mon Dec 09 17:42:40 2013 
*/

#ifndef DBQUEUE_H_
# define DBQUEUE_H_

#include <string>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

class DbQueue
{
public:
    DbQueue();
    void enqueue(std::string const &sql);
    std::string const &dequeue();
    unsigned int size();

private:
    std::queue<std::string> _queue;
    boost::mutex _mutex;
    boost::condition_variable _cond;
};

#endif
