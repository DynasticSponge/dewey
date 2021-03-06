//
// database_connection.cpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <dewey/dewey.hpp>
#include <iostream>
#include <memory>
#include <postgresql/libpq-fe.h>
#include <string>
#include <vector>

using namespace dewey;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global variable definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global function definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseConnection member definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

DatabaseConnection::DatabaseConnection(PGconn *newConnection)
{
    this->id = DatabaseConnection::instanceCount;
    DatabaseConnection::instanceCount += 0.0001;
    this->connection = newConnection;
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseConnection::executeQuery
///////////////////////////////////////////////////////////////////////////////

DatabaseResult *DatabaseConnection::executeQuery(const std::string& strQuery, DatabaseParamList *pList)
{
    DatabaseResult *returnResult{nullptr};
    PGresult *result{nullptr};
    
    const char* query = strQuery.c_str();
    int nParams = pList->getNum();
    const Oid *pTypes = pList->getTypes();
    const char * const *pVaues = pList->getValues();
    const int *pLengths = pList->getLengths();
    const int *pFormats = pList->getFormats();

    result = PQexecParams(this->connection, query, nParams, pTypes, pVaues, pLengths, pFormats, 0);
    returnResult = new DatabaseResult(result);
    return(returnResult);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseConnection::executeQuery
///////////////////////////////////////////////////////////////////////////////

DatabaseResult *DatabaseConnection::executeQuery(const std::string& strQuery)
{
    DatabaseResult *returnResult{nullptr};
    PGresult *result{nullptr};

    const char* query = strQuery.c_str();

    result = PQexecParams(this->connection, query, 0, NULL, NULL, NULL, NULL, 0);
    returnResult = new DatabaseResult(result);
    return(returnResult);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseConnection::getID
///////////////////////////////////////////////////////////////////////////////

double DatabaseConnection::getID()
{
    return(this->id);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseConnection::getConnection
///////////////////////////////////////////////////////////////////////////////

PGconn *DatabaseConnection::getConnection()
{
    return(this->connection);
}

///////////////////////////////////////////////////////////////////////////////
// Deconstructor
///////////////////////////////////////////////////////////////////////////////

DatabaseConnection::~DatabaseConnection()
{
    this->connection = nullptr;
}