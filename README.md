# Neuroshok Database Interface

| Master | Unix ![](https://travis-ci.org/ads00/ndb.svg?branch=master)|
|--------|-------------------------------------------------------|
| Dev    | Unix ![](https://travis-ci.org/ads00/ndb.svg?branch=dev)   | 

**ndb** is a generic interface to connect to any databases. 
Querys are written in pure C++ and can be executed on different database engines just by changing one parameter.
Database model is defined and accessible compile time. 

# Features
- **Header only** interface
- Full **C++** and **generic** querys
- Support **any** type of **databases**
- **Easy** to add **new engines**
- **Compile-time** generation for **SQL expressions**
- **Customizable** **types** conversion 
- **Field access by C++** structure

## Current support
- Expressions based on **SQL** and **BSON**
- **Sqlite**, **MySQL** and **MongoDB**.

# Example
## Database (using macros)
```
ndb_table(movie,
          ndb_field(id, int, 8),
          ndb_field(name, std::string, 255)
)
ndb_table(music,
          ndb_field(id, int, 8),
          ndb_field(image, std::string, 255)
)
```
## Querys
```
ndb::query<dbs::zeta>() << ( movie.id, movie.image ); // get
ndb::query<dbs::zeta>() << ( movie.id == a && movie.name == b ); // get by condition
ndb::query<dbs::zeta>() + ( movie.id = 3, movie.name = "test" ); // add
ndb::query<dbs::zeta>() - ( movie.id == 3 ); // del
```
## Minimal
An example with a *libray* database using a *collection* model and a *movie* table
```
#include "my_database.h"
static constexpr const models::collection collection;

int main()
{
    const auto& movie = collection.movie;
    
    ndb::initializer<ndb::sqlite> init;
    ndb::connect<dbs::library>();

    ndb::query<dbs::library>() + (movie.name = "Interstellar", movie.duration = 2.49_h) );

    return 0;
}
  
```
