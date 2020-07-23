//
// database_result.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DATABASE_RESULT_HPP
#define DATABASE_RESULT_HPP

#include <postgresql/libpq-fe.h>
#include <string>

#include "dewey_namespace.hpp"
#include "database_connection.hpp"

class dewey::DatabaseResult
{
public:
    std::string getField(const int);
    std::string getField(const std::string&);
    bool hasError();
    bool hasNext();
    bool next();
    ~DatabaseResult();
protected:
private:
    ////////////////////////////////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////////////////////////////////
    friend class dewey::DatabaseConnection;
    explicit DatabaseResult(PGresult*);
    void setError();

    ////////////////////////////////////////////////////////////////////////////////
    // Private Properties
    ////////////////////////////////////////////////////////////////////////////////
    bool onRow;
    int columnCount;
    int index;
    int rowCount;
    PGresult *resultSet;
};

#endif