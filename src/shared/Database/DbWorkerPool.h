/*
** DbWorkerPool.h for  in /home/vaga/Projects/Skypy/src/shared/Database
** 
** Made by 
** Login   <@epitech.eu>
** 
** Started on  Mon Dec 09 16:34:04 2013 
** Last update Mon Dec 09 17:42:43 2013 
*/

#ifndef DBWORKERPOOL_H_
# define DBWORKERPOOL_H_

#include "DbConnection.h"

class DbWorkerPool
{
public:
    DbWorkerPool(DbInfo const &info, unisgned int nbWorker);
private:
    DbQueue _queue;
};

#endif
