#ifndef CX_ERROR_H_NDB
#define CX_ERROR_H_NDB

namespace ndb
{
    template<class T, template<class...> class Container>
    struct cx_err_type_not_found;

    template<template<class...> class Source, class Err>
    struct cx_error_occured;

    template<class Source, class Err>
    struct cx_error;

    template<template<class...> class Source, class Err, class... Ts>
    struct cx_error<Source<Ts...>, Err>
    {
        static constexpr auto value = cx_error_occured<Source, Err>{};
    };
} // ndb

#endif // CX_ERROR_H_NDB
