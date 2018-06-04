#ifndef ENGINE_POSTGRE_POSTGRE_H_NDB
#define ENGINE_POSTGRE_POSTGRE_H_NDB

#include <libpq-fe.h>

#include <ndb/engine/basic.hpp>
#include <ndb/engine/postgre/connection.hpp>

#include <unordered_map>
#include <memory>

namespace ndb
{
    template<class Database>
    class postgre_query;

    template<class Database>
    class line;

    class postgre : public basic_engine<postgre>
    {
    public:
        inline postgre() = default;

        template<class Database>
        inline void connect(ndb::connection_param);

        template<class Database>
        inline postgre_connection& connection() const;

        template<class Database, class Result_type = ndb::line<Database>>
        inline auto exec(postgre_query<Database>& query) const;

        template<class Database, class Query_option, class Expr>
        inline auto exec(const Expr& expr) const;

        template<class Database>
        inline void make();

    private:
        std::unordered_map<int, std::unique_ptr<postgre_connection>> connections_;
    };
} // ndb

#include <ndb/engine/postgre/postgre.tpp>

#endif // ENGINE_POSTGRE_POSTGRE_H_NDB