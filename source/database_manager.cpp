//
// database_manager.cpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <libpq-fe.h>
#include <mutex>

#include "../headers/dewey_namespace.hpp"
#include "../headers/database_manager.hpp"
#include "../headers/database_connection.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global variable definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global function definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager member definitions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

dewey::DatabaseManager::DatabaseManager()
{
    this->connected = false;
    this->connections = -1;
    this->port = 5432;
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::connect
///////////////////////////////////////////////////////////////////////////////

bool dewey::DatabaseManager::connect()
{
    std::lock_guard<std::mutex> connectedLock(this->connectedMutex);
    if(!this->connected)
    {
        std::string connectStr{this->getConnectionString()};
        if(this->connections < 0)
        {
            PGconn *connPtr = PQconnectdb(connectStr.c_str());
            if(PQstatus(connPtr) == CONNECTION_OK)
            {
                this->available.push_back(connPtr);
                this->connected = true;
            }
            else
            {
                PQfinish(connPtr);
            }                    
        }
        else
        {
            bool connError{false};
            for(int count = 0; count < this->connections && !connError; count++){
                PGconn *connPtr = PQconnectdb(connectStr.c_str());
                if(PQstatus(connPtr) == CONNECTION_OK)
                {
                    this->available.push_back(connPtr);
                }
                else
                {
                    connError = true;
                    PQfinish(connPtr);
                }
            }
            this->connected = !connError;
        }
    }
    
    return(this->connected);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::getConnectionString
///////////////////////////////////////////////////////////////////////////////

std::string dewey::DatabaseManager::getConnectionString()
{
    std::string connectStr{"postgresql://"};
    connectStr.append(this->username);
    connectStr.append(":");
    connectStr.append(this->password);
    connectStr.append("@");
    connectStr.append(this->address);
    connectStr.append(":");
    connectStr.append(std::to_string(this->port));
    connectStr.append("/");
    connectStr.append(this->database);
    return(std::move(connectStr));
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::getInstance
///////////////////////////////////////////////////////////////////////////////

dewey::DatabaseConnection* dewey::DatabaseManager::getConnection()
{
    dewey::DatabaseConnection* returnPtr{nullptr};    
    
    this->connectedMutex.lock();
    if(this->connected){
        this->connectedMutex.unlock();

        PGconn *connPtr{nullptr};
        if(this->connections < 0)
        {   
            while(connPtr == nullptr)
            {
                connPtr = PQconnectdb(this->getConnectionString().c_str());
                if(PQstatus(connPtr) == CONNECTION_BAD)
                {
                    PQfinish(connPtr);
                    connPtr = nullptr;
                }
            }            
        }
        else 
        {
            while(connPtr == nullptr)
            {
                std::lock_guard<std::mutex> availableLock(this->availableMutex);
                if (this->available.size() > 0)
                {
                    connPtr = this->available.back();
                    this->available.pop_back();
                }
            }                        
        }
        
        if(connPtr != nullptr)
        {
            std::lock_guard<std::mutex> issuedLock(this->issuedMutex);
            returnPtr = new dewey::DatabaseConnection(connPtr);
            this->issued.insert({returnPtr->getID(), returnPtr});
        }   
    }
    else {
        this->connectedMutex.unlock();
    }
    
    return(returnPtr);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::getInstance
///////////////////////////////////////////////////////////////////////////////

dewey::DatabaseManager* dewey::DatabaseManager::getInstance()
{
    if(!dewey::DatabaseManager::hasInstance){
        dewey::DatabaseManager::instance = new dewey::DatabaseManager();
        dewey::DatabaseManager::hasInstance = true;
    }
    
    return(dewey::DatabaseManager::instance);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::killInstance
///////////////////////////////////////////////////////////////////////////////

void dewey::DatabaseManager::killInstance()
{
    if(dewey::DatabaseManager::hasInstance){
        delete dewey::DatabaseManager::instance;
        dewey::DatabaseManager::instance = nullptr;
        dewey::DatabaseManager::hasInstance = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::returnConnection
///////////////////////////////////////////////////////////////////////////////

void dewey::DatabaseManager::returnConnection(dewey::DatabaseConnection* oldConnection)
{
    {
        std::lock_guard<std::mutex> availableLock(this->availableMutex);
        this->available.push_back(oldConnection->getConnection());
    }
    {
        std::lock_guard<std::mutex> issuedLock(this->issuedMutex);
        this->issued.erase(oldConnection->getID());
    }
    delete oldConnection;
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::setAddress
///////////////////////////////////////////////////////////////////////////////

bool dewey::DatabaseManager::setAddress(const std::string& newAddress)
{
    bool returnValue = false;
    if(!this->connected){
        this->address = newAddress;
        returnValue = true;
    }
    return(returnValue);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::setConnections
///////////////////////////////////////////////////////////////////////////////

bool dewey::DatabaseManager::setConnections(int newConnections)
{
    bool returnValue = false;
    if(!this->connected){
        this->connections = newConnections;
        returnValue = true;
    }
    return(returnValue);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::setDatabase
///////////////////////////////////////////////////////////////////////////////

bool dewey::DatabaseManager::setDatabase(const std::string& newDatabase)
{
    bool returnValue = false;
    if(!this->connected){
        this->database = newDatabase;
        returnValue = true;
    }
    return(returnValue);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::setPassword
///////////////////////////////////////////////////////////////////////////////

bool dewey::DatabaseManager::setPassword(const std::string& newPassword)
{
    bool returnValue = false;
    if(!this->connected){
        this->password = newPassword;
        returnValue = true;
    }
    return(returnValue);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::setPort
///////////////////////////////////////////////////////////////////////////////

bool dewey::DatabaseManager::setPort(int newPort)
{
    bool returnValue = false;
    if(!this->connected){
        this->port = newPort;
        returnValue = true;
    }
    return(returnValue);
}

///////////////////////////////////////////////////////////////////////////////
// dewey::DatabaseManager::setUsername
///////////////////////////////////////////////////////////////////////////////

bool dewey::DatabaseManager::setUsername(const std::string& newUsername)
{
    bool returnValue = false;
    if(!this->connected){
        this->username = newUsername;
        returnValue = true;
    }
    return(returnValue);
}

///////////////////////////////////////////////////////////////////////////////
// Deconstructor
///////////////////////////////////////////////////////////////////////////////

dewey::DatabaseManager::~DatabaseManager()
{
    while(this->available.size() > 0)
    {
        PGconn *connPtr{this->available.back()};
        this->available.pop_back();
        PQfinish(connPtr);
    }
    
    while(this->issued.size() > 0)
    {
        auto iter{this->issued.begin()};
        PGconn *connPtr{iter->second->getConnection()};
        this->issued.erase(iter);
        PQfinish(connPtr);
    }
}