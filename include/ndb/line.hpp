#ifndef LINE_H_NDB
#define LINE_H_NDB

#include <ndb/engine/type.hpp>
#include <ndb/utility.hpp>
#include <ndb/error.hpp>
#include <map>
#include <unordered_map>

namespace ndb
{
    template<class Engine>
    class line
    {
    public:
        void add(int field_id, ndb::value field_value)
        {
            data_.emplace(field_id, std::move(field_value));
        }

        size_t field_count() const
        {
            return data_.size();
        }

        template<class Field>
        typename native_type<Engine, typename Field::value_type>::type operator[](const Field& f) const
        {
            if (!data_.count(ndb::field_id<Field>)) ndb_error("Field does not exist in the result, check the select clause");
            return data_.at(ndb::field_id<Field>).template get<typename native_type<Engine, typename Field::value_type>::type>();
        }

    private:
        std::unordered_map<int, ndb::value> data_;
    };
} // ndb



#endif // LINE_H_NDB