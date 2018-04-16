#ifndef ENGINE_SQLITE_SQLITE_H_NDB
#define ENGINE_SQLITE_SQLITE_H_NDB

#include <sqlite3.h>

#include <ndb/engine/basic.hpp>
#include <ndb/error.hpp>
#include <ndb/value.hpp>
#include <ndb/result.hpp>
#include <ndb/setup.hpp>

#include <experimental/filesystem>
#include <unordered_map>

namespace ndb
{
    namespace fs = std::experimental::filesystem;

    template<class T>
    static constexpr bool is_native = std::is_same_v<std::decay_t<T>, int>
    || std::is_same_v<std::decay_t<T>, double>
    || std::is_same_v<std::decay_t<T>, std::string>;

    class sqlite_connection
    {
    public:
        sqlite_connection(const std::string& db_name) :
            database_{ nullptr }
        {
            if (!fs::exists(setup<sqlite>::path)) fs::create_directory(setup<sqlite>::path);

            std::string path = setup<sqlite>::path + db_name + setup<sqlite>::ext;
            auto status = sqlite3_open(path.c_str(), &database_);

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
    };


    class sqlite : public basic_engine<sqlite>
    {
    public:
        inline sqlite();

        template<class Database>
        inline void connect();

        template<class Database>
        inline sqlite_connection& connection() const;

        template<class T>
        inline void bind(sqlite3_stmt* statement, int bind_index, const T& v) const;

        template<class Database>
        inline void exec(const char* str_query) const;

        template<class Database>
        inline void exec(const std::string& str_query) const;

        template<class Database, class Expr>
        inline ndb::result<sqlite> exec(const Expr& expr) const;

        template<class Database>
        inline void make();

        inline static constexpr auto expr_category();

    private:
        std::unordered_map<int, std::unique_ptr<sqlite_connection>> connection_list_;
    };
} // ndb

#include <ndb/engine/sqlite/sqlite.tpp>

#endif // ENGINE_SQLITE_SQLITE_H_NDB