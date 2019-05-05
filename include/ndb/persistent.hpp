#ifndef PERSISTENT_H_NDB
#define PERSISTENT_H_NDB

#include <ndb/preprocessor.hpp>
#include <ndb/query.hpp>

#include <string>

ndb_table(ndb_persistent_table
    , ndb_field(key, std::string, ndb::size<255>, ndb::option<ndb::field_option::unique>)
    , ndb_field(value, std::string, ndb::size<255>)
)

ndb_model(ndb_persistent_model, ndb_persistent_table)

ndb_project(ndb_persistent
    , ndb_database(ndb_persistent_database, ndb_persistent_model, ndb::sqlite)
)

namespace ndb
{
    class persistent_group
    {
      public:
        persistent_group(const persistent_group* parent_group, std::string name)
            : name_{ std::move(name) }
            , path_{ parent_group ? (parent_group->path() + "." + name_) : name_ }
        {
            // connect when root is instantiated
            if (parent_group == nullptr)
            {
                ndb::connect<ndb::databases::ndb_persistent::ndb_persistent_database_>();
            }
        }

        persistent_group(std::string name) : persistent_group(nullptr, std::move(name)) {}

        const std::string& name() const { return name_; }
        const std::string& path() const { return path_; }

      private:
        std::string name_;
        std::string path_;
    };

    template<class T>
    class persistent
    {
        using Database = ndb::databases::ndb_persistent::ndb_persistent_database_;

      public:
        persistent(const persistent_group* group, std::string name, const T& default_value = T{})
            : name_{ std::move(name) }
            , path_{ group->path() + "." + name_ }
            , value_{}
            , default_value_{ default_value }
        {
            auto res = ndb::query<Database>()
                       << (ndb::get(ndb::models::ndb_persistent_model.ndb_persistent_table.value)
                       << ndb::source(ndb::models::ndb_persistent_model.ndb_persistent_table)
                       << ndb::filter(ndb::models::ndb_persistent_model.ndb_persistent_table.key == path_));
            if (res.has_result())
            {
                value_ = res[0][ndb::models::ndb_persistent_model.ndb_persistent_table.value];
            }
            else
                value_ = default_value_;
        }

        operator T() const { return value_; }

        const T& get() const { return value_; }
        const std::string& name() const { return name_; }
        const std::string& path() const { return path_; }
        const T& default_value() const { return default_value_; }

        void operator=(T value)
        {
            // ignore update if value didn't change
            if (value_ == value)
                return;

            value_ = std::move(value);
            ndb::query<Database>()
            << (ndb::add_ignore(ndb::models::ndb_persistent_model.ndb_persistent_table.key = path_, ndb::models::ndb_persistent_model.ndb_persistent_table.value = value_));
        }

      private:
        std::string name_;
        std::string path_;
        T value_;
        T default_value_;
    };
} // ndb

#endif // PERSISTENT_H_NDB
