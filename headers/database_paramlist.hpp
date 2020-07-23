//
// database_paramlist.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DATABASE_PARAMLIST_HPP
#define DATABASE_PARAMLIST_HPP

#include <postgresql/libpq-fe.h>
#include <string>
#include <vector>

#include "dewey_namespace.hpp"

class dewey::DatabaseParamList
{
public:
    DatabaseParamList();
    void addParam(const std::string&, int type, bool binary = false);
    void addParam(const std::string&, bool binary = false);
    int *getFormats();
    int *getLengths();
    int getNum();
    Oid *getTypes();
    const char * const *getValues();
    ~DatabaseParamList();
protected:
private:
    ////////////////////////////////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // Private Properties
    ////////////////////////////////////////////////////////////////////////////////
    int numParams;
    std::vector<int> paramFormats;
    std::vector<int> paramLengths;
    std::vector<Oid> paramTypes;
    std::vector<const char*> paramValues;
};

#endif