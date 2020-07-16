# dewey
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/c8e416ba58f7493d851d902f3fc08396)](https://www.codacy.com/manual/joseph.adomatis/dewey?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DynasticSponge/dewey&amp;utm_campaign=Badge_Grade)

frederick2 is a C++ Library for accessing PostgreSQL utilizing its libpq C libraries.

## Stage/Release

dewey is still in an alpha / pre-release stage of development

## Build

Modify the makefile to address your own folder structure or ensure that "object" and "output" folders exist as peers of the "source" and "headers" folders.

```bash
make static
```
The above will compile the source and generate the libdewey.a static library in the "output" folder

## Installation

Copy the 'headers' folder and libdewey.a library file to the appropriate locations within your project's folder structure and update your compiler/linker appropriately.

## Basic Example

```c++
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "dewey/dewey_namespace.hpp"
#include "dewey/database_manager.hpp"
#include "dewey/database_connection.hpp"
#include "dewey/database_paramlist.hpp"
#include "dewey/database_result.hpp"

int main(int argc, char *argv[])
{
    int returnValue{EXIT_FAILURE};
    dewey::DatabaseManager* dbManager = dewey::DatabaseManager::getInstance();
    dbManager->setAddress("127.0.0.1");
    dbManager->setDatabase("dbtest");
    dbManager->setUsername("dbtestowner");
    dbManager->setPassword("dbtestpassword");
    dbManager->setConnections(10); // connection pool size
    if(dbManager->connect())
    {
        std::unique_ptr<dewey::DatabaseConnection> dbConnection(dbManager->getConnection());
        std::unique_ptr<dewey::DatabaseParamList> params(new dewey::DatabaseParamList());
        
        std::string query{"SELECT id FROM employee WHERE name = $1::varchar"};
        std::string employee_name{"testemp3"};
        std::string employee_id;
        
        params->addParam(employee_name);       
        std::unique_ptr<dewey::DatabaseResult> resultSet(dbConnection->executeQuery(query, params.get()));
        while(resultSet->next())
        {
            employee_id = resultSet->getField(0);
            std::cout << "Updating employee #" << employee_id << std::endl;
        }
        
        query = "UPDATE employee SET salary = salary + 1 WHERE id = $1::uuid";
        params.reset(new dewey::DatabaseParamList());
        params->addParam(employee_id);
        resultSet.reset(dbConnection->executeQuery(query, params.get()));
        
        dbManager->returnConnection(dbConnection.release());
        returnValue = EXIT_SUCCESS;
    }

    dewey::DatabaseManager::killInstance();
    return(returnValue);
}
```

## Contributing

This is a hobby project for me to have fun learning new things and tweaking the results.  If someone is interested, I'm more than happy to share the work/results with whomever wants to make use of it.

If you are interested in contributing however, please open an issue and we can discuss what it is you would like to tackle.  

Thanks!

## License
[MIT](https://choosealicense.com/licenses/mit/)

## Project Namesake(s)
-   Learn about: [Melvil Dewey](https://en.wikipedia.org/wiki/Melvil_Dewey)