#ifndef ENGINE_POSTGRE_QUERY_H_NDB
#define ENGINE_POSTGRE_QUERY_H_NDB

#include <ndb/line.hpp>
#include <ndb/engine.hpp>
#include <ndb/engine/postgre/postgre.hpp>
#include <ndb/result.hpp>

#include <libpq-fe.h>

#include <string>

namespace ndb
{
    template<class Database>
    class postgre_query
    {
    public:
        using Engine = typename Database::engine;

        postgre_query(std::string str_query) :
            str_query_{ std::move(str_query) },
            data_count_{ 0 }
        {}

        ~postgre_query()
        {

        }

        template<class T>
        void bind_value(const T& value)
        {
            //data_values_.push_back('a');
            //data_values_.push_back('b');
            data_sizes_.push_back(2);
        }

        template<class T>
        void bind(const T& value)
        {
            if constexpr (ndb::is_native_type_v<ndb::postgre, T>) bind_value(value);
            else bind_value(ndb::custom_type<T, Database>::encode(value)); //check encoders if you have an error here
        };

        template<class Result_type = ndb::line<Database>>
        auto exec() const
        {
            ndb::result<Database, Result_type> result;

            const char* values = data_values_.data();
            auto values_address = std::addressof(values);

            PGresult* res = PQexecParams(connection(), str_query_.c_str(),
                                         data_count_,
                                         nullptr,
                                         values_address,
                                         data_sizes_.data(), nullptr, 0);

            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                auto error_message = std::string(PQerrorMessage(connection()));
                PQclear(res);
                ndb_error("query error : " + error_message);
            }

            PQclear(res);

            return result;
        }

        postgre_connection& connection() const
        {
            return ndb::engine<ndb::postgre>::get().connection<Database>();
        }

    private:
        std::string str_query_;
        std::vector<char> data_values_;
        std::vector<int> data_sizes_;
        unsigned int data_count_;
    };
} // ndb

#endif // ENGINE_POSTGRE_QUERY_H_NDB