#ifndef ERROR_H_NDB
#define ERROR_H_NDB

#include <exception>
#include <stdexcept>

#define ndb_error(M) throw std::logic_error(M);

#endif // ERROR_H_NDB
