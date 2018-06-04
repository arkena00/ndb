#ifndef ENGINE_POSTGRE_CONNECTION_H_NDB
#define ENGINE_POSTGRE_CONNECTION_H_NDB

#include <libpq-fe.h>

#include <ndb/engine/connection_param.hpp>
#include <ndb/error.hpp>

#include <array>

namespace ndb
{
    class postgre_connection
    {
    public:
        postgre_connection(ndb::connection_param params) :
            connection_{ nullptr },
            params_{ std::move(params) }
        {
            unsigned int param_index = 0;
            std::array<const char*, 4> keys;
            std::array<const char*, 4> values;

            auto param_add = [&](const char* key, const char* value)
            {
                keys[param_index] = key;
                values[param_index] = value;
                param_index++;
            };

            param_add("user", params_.user.c_str());
            param_add("port", std::to_string(params_.port).c_str());
            param_add("host", params_.host.c_str());
            param_add("dbname", params_.db_name.c_str());
            param_add(nullptr, nullptr);

            // check server status
            PGPing server_status = PQpingParams(keys.data(), values.data(), 0);
            if (server_status != PQPING_OK) ndb_error("unable to connect to host : " + params_.host);

            connection_ = PQconnectdbParams(keys.data(), values.data(), 0);

            if (PQstatus(connection_) != CONNECTION_OK)
            {
                auto error_message = std::string(PQerrorMessage(connection_));
                PQfinish(connection_);
                ndb_error("database connection failed : " + error_message);
            }
        }

        operator PGconn*()
        {
            return connection_;
        }

        ~postgre_connection()
        {
            PQfinish(connection_);
        }

    private:
        PGconn* connection_;
        ndb::connection_param params_;
    };
} // ndb

#endif // ENGINE_POSTGRE_CONNECTION_H_NDB