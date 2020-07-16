//
// database_manager.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Author: Joseph Adomatis
// Copyright (c) 2020 Joseph R Adomatis (joseph dot adomatis at gmail dot com)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <map>
#include <mutex>
#include <libpq-fe.h>
#include <string>
#include <vector>

#include "dewey_namespace.hpp"

class dewey::DatabaseManager
{
public:    
    bool connect();
    dewey::DatabaseConnection* getConnection();
    static dewey::DatabaseManager* getInstance();
    static void killInstance();
    void returnConnection(dewey::DatabaseConnection*);
    bool setAddress(const std::string&);
    bool setConnections(int);
    bool setDatabase(const std::string&);
    bool setPassword(const std::string&);
    bool setPort(int);
    bool setUsername(const std::string&);
    
protected:
private:
    ////////////////////////////////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////////////////////////////////
    DatabaseManager();
    ~DatabaseManager();
    dewey::DatabaseManager& operator= (const dewey::DatabaseManager&) = delete;
    std::string getConnectionString();

    ////////////////////////////////////////////////////////////////////////////////
    // Private Properties
    ////////////////////////////////////////////////////////////////////////////////
    static inline dewey::DatabaseManager *instance = nullptr;
    static inline bool hasInstance = false;
    
    bool connected;
    int connections;
    int port;
    
    std::map<double, dewey::DatabaseConnection*> issued;
    std::mutex availableMutex;
    std::mutex connectedMutex;
    std::mutex issuedMutex;
    std::string address;
    std::string database;
    std::string password;
    std::string username;
    std::vector<PGconn*> available;
};

#endif