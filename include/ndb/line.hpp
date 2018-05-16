#ifndef LINE_H_NDB
#define LINE_H_NDB

#include <ndb/engine/type.hpp>
#include <ndb/utility.hpp>
#include <ndb/error.hpp>
#include <ndb/value.hpp>
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
            if (index >= values_.size()) ndb_error("ndb::value out of range");
            return values_.at(index);
        }

        template<class Field>
        auto operator[](const Field& f) const
        {
            if (!value_index_.count(ndb::field_id<Field>)) ndb_error("Field does not exist in the result, check the select clause");

            size_t index = value_index_.at(ndb::field_id<Field>);
            using native_type = typename native_type<Engine, typename Field::value_type>::type;
            using value_type = typename Field::value_type;

            if constexpr (ndb::is_native<value_type>)
            {
                return values_.at(index).template get<native_type>();
            }
            else return ndb::type<Engine>::decode<value_type>(values_.at(index).template get<native_type>());
        }

    private:
        std::vector<ndb::value> values_;
        std::unordered_map<size_t, size_t> value_index_;
    };
} // ndb



#endif // LINE_H_NDB