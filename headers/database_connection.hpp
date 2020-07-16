//
// database_connection.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <libpq-fe.h>
#include <memory>
#include "dewey_namespace.hpp"
#include "database_result.hpp"

class dewey::DatabaseConnection 
{
public:
    dewey::DatabaseResult *executeQuery(const std::string&, dewey::DatabaseParamList*);
    dewey::DatabaseResult *executeQuery(const std::string&);
    double getID();    
protected:
private:
    ////////////////////////////////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////////////////////////////////
    friend class dewey::DatabaseManager;
    friend std::unique_ptr<dewey::DatabaseConnection>::deleter_type;
    explicit DatabaseConnection(PGconn*);
    PGconn *getConnection();
    ~DatabaseConnection();
    
    ////////////////////////////////////////////////////////////////////////////////
    // Private Properties
    ////////////////////////////////////////////////////////////////////////////////
    static inline double instanceCount = 0;
    double id;
    PGconn *connection;
};

#endif