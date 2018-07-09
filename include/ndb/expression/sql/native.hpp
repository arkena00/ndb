#ifndef EXPRESSION_SQL_NATIVE_H_NDB
#define EXPRESSION_SQL_NATIVE_H_NDB

#include <ndb/expression/native.hpp>
#include <ndb/utility.hpp>

#include <string>
#include <array>

namespace ndb
{
    template<class Expr, class Engine>
    struct native_expression<Expr, Engine, expression_categories::sql>
    {
        static constexpr auto category = expression_categories::sql;

        constexpr native_expression() :
            size_{ 0 },
            value_index_{ 1 },
            data_{}
        {
            //ndb::expression_make<Expr>
            Expr::template make<Engine>(*this);
        }

        constexpr void append(char v)
        {
            data_[size_] = v;
            size_++;
        }

        constexpr void append(const char* v)
        {
            auto s = cx_str_len(v);
            for (int i = 0; i != s; i++) data_[size_ + i] = v[i];
            size_ += s;
        }

        constexpr void add_value()
        {
            data_[size_] = '0' + value_index_;
            size_ ++;
            value_index_++;
        }

        constexpr auto size() const
        {
            return size_;
        }


        std::string str() const
        {
            std::string result;
            for (int i = 0; i != Capacity; i++)
            {
                if (data_.at(i) != 0) result.push_back(data_.at(i));
            }

            return result;
        }

        static constexpr auto Capacity = 1000;

        size_t size_;
        unsigned int value_index_;
        std::array<char, Capacity> data_;
    };
} // ndb

#endif // EXPRESSION_SQL_NATIVE_H_NDB