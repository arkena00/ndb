#ifndef ENGINE_SQLITE_CONNECTION_H_NDB
#define ENGINE_SQLITE_CONNECTION_H_NDB

#include <sqlite3.h>

#include <ndb/engine/connection_param.hpp>
#include <ndb/error.hpp>
#include <ndb/option.hpp>
#include <ndb/setup.hpp>

#include <experimental/filesystem>
#include <unordered_map>

namespace ndb
{
    namespace fs = std::experimental::filesystem;

    class sqlite_connection
    {
    public:
        sqlite_connection(ndb::connection_param params) :
            database_{ nullptr },
            params_{ std::move(params) }
        {
            if (params_.path.empty()) params_.path = "./";
            if (!fs::exists(params_.path)) fs::create_directory(params_.path);
            std::string fullpath = params_.path + "/" + params_.db_name + setup<sqlite>::ext;

            int native_flag = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
            if ((int)params_.flag & (int)ndb::connection_flag::read_only)
            {
                native_flag = SQLITE_OPEN_READONLY;
            }

            auto status = sqlite3_open_v2(fullpath.c_str(), &database_, native_flag, nullptr);

            if (status != SQLITE_OK) ndb_error("database connection failed");
        }

        ~sqlite_connection()
        {
            sqlite3_close(database_);
        }

        sqlite3* database()
        {
            return database_;
        }

    private:
        sqlite3* database_;

        ndb::connection_param params_;
    };

} // ndb

#endif // ENGINE_SQLITE_CONNECTION_H_NDB