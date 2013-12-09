/*
** DbWorker.h for  in /home/vaga/Projects/Skypy/src/shared/Database
** 
** Made by 
** Login   <@epitech.eu>
** 
** Started on  Mon Dec 09 16:30:33 2013 
** Last update Mon Dec 09 17:22:54 2013 
*/

#ifndef DBWORKER_H_
# define DBWORKER_H_

class DbWorker
{
public:
    DbWorker(std::queue<std::string> queue, DbConnection *_conn);
    void run();
};

#endif
