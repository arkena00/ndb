#ifndef LINE_H_NDB
#define LINE_H_NDB

#include <ndb/engine/type.hpp>
#include <ndb/utility.hpp>
#include <ndb/error.hpp>
#include <unordered_map>
#include <vector>

namespace ndb
{
    template<class Engine>
    class line
    {
    public:
        void add(int field_id, ndb::value field_value)
        {
            // value accessible by field
            if (field_id >= 0) value_index_.emplace(field_id, values_.size());
            values_.emplace_back(std::move(field_value));
        }

        size_t field_count() const
        {
            return values_.size();
        }

        ndb::value operator[](unsigned int index) const
        {
            if (index >= values_.size()) ndb_error("Out of range");
            return values_.at(index);
        }

        template<class Field>
        typename native_type<Engine, typename Field::value_type>::type operator[](const Field& f) const
        {
            if (!value_index_.count(ndb::field_id<Field>)) ndb_error("Field does not exist in the result, check the select clause");
            return values_.at(value_index_.at(ndb::field_id<Field>)).template get<typename native_type<Engine, typename Field::value_type>::type>();
        }

    private:
        std::vector<ndb::value> values_;
        std::unordered_map<int, int> value_index_;
    };
} // ndb



#endif // LINE_H_NDB