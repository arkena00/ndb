#ifndef VALUE_H_NDB
#define VALUE_H_NDB

#include <ndb/error.hpp>

#include <string>
#include <vector>
#include <variant>

#include <ndb/engine/type.hpp>

namespace ndb
{
    class value
    {
    public:
        template<class T>
		value(T v) :
            value_{ std::move(v) }
        {}

        template<class T>
        const auto& get() const
        {
            try
            {
                return std::get<T>(value_);
            }
            catch (const std::exception&)
            {
                ndb_error("Can't get unknown type");
            }
        }

        bool is_null() const
        {
            return std::holds_alternative<null_type>(value_);
        }

    private:
        std::variant<ndb::null_type, int, double, float, std::string, std::vector<char>> value_;
    };
} // ndb

#endif // VALUE_H_NDB
