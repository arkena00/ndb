#ifndef ENGINE_SQLITE_CONNECTION_H_NDB
#define ENGINE_SQLITE_CONNECTION_H_NDB

#include <sqlite3.h>

#include <ndb/error.hpp>
#include <ndb/setup.hpp>

#include <experimental/filesystem>

namespace ndb
{
    namespace fs = std::experimental::filesystem;

    class sqlite_connection
    {
    public:
        sqlite_connection(const std::string& db_name, const std::string& path = "") :
            database_{ nullptr },
            path_{ path }
        {
            if (path_.empty()) path_ = "./";
            if (!fs::exists(path_)) fs::create_directory(path_);
            std::string fullpath = path_ + "/" + db_name + setup<sqlite>::ext;

            auto status = sqlite3_open(fullpath.c_str(), &database_);

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
        std::string path_;
    };

} // ndb

#endif // ENGINE_SQLITE_CONNECTION_H_NDB