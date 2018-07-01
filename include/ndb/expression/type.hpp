#ifndef EXPRESSION_TYPE_NDB
#define EXPRESSION_TYPE_NDB

#include <ndb/expression/code.hpp>
#include <iostream>

namespace ndb
{
    enum class expression_forms { op_args, a_op_b, op_a_b };
    
    namespace expression_types
    {
        struct basic_type
        {
            static constexpr auto form = expression_forms::op_args;
        };
        
        struct statement : basic_type{};
        struct list : basic_type{ static constexpr auto form = expression_forms::a_op_b; };
        struct function : basic_type{};

        // filter
        struct range : basic_type{};
        struct assign : basic_type{};
        struct equal : basic_type{};

        // clause
        struct get : basic_type{};
        struct set : basic_type{};
        struct source : basic_type{};
        struct filter : basic_type{};

        // scalar
        struct field : basic_type{};
        struct table : basic_type{};
        struct value : basic_type{};
    }


    

    template<class Expression_type>
    struct nexpr_code
    {
        static constexpr const char* value = " _nexpr_code_undefined_ ";
    };

    template<> struct nexpr_code<ndb::expression_types::get> { static constexpr const char* value = "SELECT "; };
    template<> struct nexpr_code<ndb::expression_types::assign> { static constexpr const char* value = "="; };
    template<> struct nexpr_code<ndb::expression_types::list> { static constexpr const char* value = ","; };
    template<> struct nexpr_code<ndb::expression_types::source> { static constexpr const char* value = " FROM "; };
    template<> struct nexpr_code<ndb::expression_types::filter> { static constexpr const char* value = " WHERE "; };
    template<> struct nexpr_code<ndb::expression_types::statement> { static constexpr const char* value = "[STATEMENT]"; };


    template<expression_forms Form>
    struct expression_form;

    template<>
    struct expression_form<expression_forms::op_args>
    {
        template<class Type, class Native_expression, class... Args>
        static void make(Native_expression& s)
        {
            s += nexpr_code<Type>::value;
            (Args::make(s), ...);
        }
    };
    template<>
    struct expression_form<expression_forms::a_op_b>
    {
        template<class Type, class Native_expression, class T1, class T2>
        static void make(Native_expression& s)
        {
            T1::make(s);
            s += nexpr_code<Type>::value;
            T2::make(s);
        }
    };

    template<>
    struct expression_form<expression_forms::op_a_b>
    {
        template<class Type, class Native_expression, class T1, class T2>
        static void make(Native_expression& s)
        {
            s += nexpr_code<Type>::value;
            T1::make(s);
            T2::make(s);
        }
    };




    // generic
    template<class Type>
    struct expression_type
    {
        // make all args
        template<class Native_expression, class... Args>
        static void make(Native_expression& s)
        {
            expression_form<Type::form>::template make<Type, Native_expression, Args...>(s);
        }
    };

//
    /*
    template<>
    struct expression_type<ndb::expression_types::statement>
    {
        template<class... Args>
        static void make(std::string& s, std::tuple<Args...> args)
        {
            std::cout << sizeof...(Args);

            //(expression_type<typename Args::type>::make(s, args), ...);
        }
    };*/
} // ndb

#endif // EXPRESSION_TYPE_NDB
