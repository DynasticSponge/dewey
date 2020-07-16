//
// database_paramlist.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <libpq-fe.h>
#include <string>
#include <vector>

#include "../headers/dewey_namespace.hpp"
#include "../headers/database_paramlist.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global variable definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global function definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseParamList member definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

dewey::DatabaseParamList::DatabaseParamList()
{
    this->numParams = 0;
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseParamList::addParam
///////////////////////////////////////////////////////////////////////////////

void dewey::DatabaseParamList::addParam(const std::string& value, int type, bool binary)
{
    this->numParams++;
    this->paramTypes.push_back(static_cast<Oid>(type));
    this->paramLengths.push_back(value.size());
    if(binary){
        this->paramFormats.push_back(1);
        this->paramValues.push_back(&(value[0]));
    }
    else
    {
        this->paramFormats.push_back(0);
        this->paramValues.push_back(value.c_str());
    }   
    return;
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseParamList::addParam
///////////////////////////////////////////////////////////////////////////////

void dewey::DatabaseParamList::addParam(const std::string& value, bool binary)
{
    this->numParams++;
    this->paramTypes.push_back(static_cast<Oid>(0));
    this->paramLengths.push_back(value.size());
    if(binary){
        this->paramFormats.push_back(1);
        this->paramValues.push_back(&(value[0]));
    }
    else
    {
        this->paramFormats.push_back(0);
        this->paramValues.push_back(value.c_str());
    }   
    return;
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseParamList::getFormats
///////////////////////////////////////////////////////////////////////////////

int *dewey::DatabaseParamList::getFormats()
{
    return(&(this->paramFormats[0]));
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseParamList::getLengths
///////////////////////////////////////////////////////////////////////////////

int *dewey::DatabaseParamList::getLengths()
{
    return(&(this->paramLengths[0]));
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseParamList::getNum
///////////////////////////////////////////////////////////////////////////////

int dewey::DatabaseParamList::getNum()
{
    return(this->numParams);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseParamList::getTypes
///////////////////////////////////////////////////////////////////////////////

Oid *dewey::DatabaseParamList::getTypes()
{
    return(&(this->paramTypes[0]));
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseParamList::getValues
///////////////////////////////////////////////////////////////////////////////

const char * const *dewey::DatabaseParamList::getValues()
{
    return(&(this->paramValues[0]));
}

///////////////////////////////////////////////////////////////////////////////
// Deconstructor
///////////////////////////////////////////////////////////////////////////////

dewey::DatabaseParamList::~DatabaseParamList()
{
    
}