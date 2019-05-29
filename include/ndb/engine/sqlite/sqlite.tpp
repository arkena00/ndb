#include <ndb/expression/deduction.hpp>
#include <ndb/expression/sql/native.hpp>
#include <ndb/expression/sql/code.hpp>

#include <ndb/engine/sqlite/function.hpp>
#include <ndb/engine/sqlite/type.hpp>
#include <ndb/engine/sqlite/query.hpp>

#include <ndb/expression/operator.hpp>

#include <ndb/option.hpp>
#include <iostream> // query_debug
#include <ndb/table.hpp>

namespace ndb
{
    template<class Database>
    auto sqlite::exec(const std::string& str_statement) const
    {
        sqlite_query<Database> statement{ str_statement };
        return exec<Database>(statement);
    }

    template<class Database, class Query_option, class Expr>
    auto sqlite::exec(const Expr& expr) const
    {
        constexpr auto str_query = ndb::native_expression<Expr, ndb::sqlite>{};
        sqlite_query<Database> query{ str_query.data() };

        // bind values from expression
        expr.eval([&](auto&& e)
                  {
                      using expr_type = std::decay_t<decltype(e)>;
                      // e is expr_value
                      if constexpr (ndb::expr_is_value<expr_type>)
                      {
                          query.bind(std::get<0>(e.args()));
                      }
                  });

        using Result_type = typename
        std::conditional_t<
            ndb::has_option_v<query_option::object, Query_option>
            , typename ndb::deductions::table<std::decay_t<Expr>>::Detail_::object_type
            , ndb::line<Database>
        >;

        return exec<Database, Result_type>(std::move(query));
    }

    template<class Database, class Result_type>
    auto sqlite::exec(const sqlite_query<Database>& query) const
    {
        return query.template exec<Result_type>();
    }

    template<class Database>
    void sqlite::make()
    {
        exec<Database>(std::string("PRAGMA foreign_keys = ON;"));

        using Model = typename Database::model;

        std::string output;

        if constexpr (!std::is_same_v<typename Model::Detail_::edges, void>)
        {
            ndb::for_each<Model::Detail_::edges>([&output](auto, auto&& edge)
            {
                using Edge = std::decay_t<decltype(edge)>;

                output += "\ncreate table if not exists `" + std::string(ndb::edge_name<Edge>) + "` (";
                output += "\n" + std::string(ndb::edge_name<Edge>) + std::string(ndb::field_name<Edge::source_field>) + " integer, ";
                output += "\n" + std::string(ndb::edge_name<Edge>) + std::string(ndb::field_name<Edge::target_field>) + " integer ";
                output += ");";
            });
        }

        ndb::for_each_entity<Model>([this, &output](auto&&, auto&& table)
        {
            using Table = std::decay_t<decltype(table)>;

            output += "\ncreate table if not exists `" + std::string(ndb::table_name<Table>) + "` (";

            ndb::for_each_entity(table, [&output](auto&& i, auto&& field)
            {
                using Field = std::decay_t<decltype(field)>;

                if constexpr (decltype(i){} != 0) output += ",";

                // field id
                output += "\n" + std::string(ndb::field_name<Field>);
                bool need_size = false;

                // field type
                using field_value_type = typename std::decay_t<decltype(field)>::value_type;
                using field_storage_type = ndb::storage_type_t<ndb::sqlite, ndb_type_t<field_value_type, Database>>;

                if constexpr (std::is_same_v<int64_, field_storage_type>) output += " integer ";
                if constexpr (std::is_same_v<double_, field_storage_type>) output += " float ";
                if constexpr (std::is_same_v<string_, field_storage_type>)
                {
                    output += " text ";
                    need_size = true;
                }
                if constexpr (std::is_same_v<byte_array_, field_storage_type>) output += " blob ";

                // field size
                if (field.detail_.size > 0 && need_size) output += "(" + std::to_string(field.detail_.size) + ")";

                // field option
                if (field.detail_.is_primary || field.detail_.is_oid) output += " primary key";
                if (field.detail_.is_auto_increment) output += " autoincrement";
                if (field.detail_.is_not_null) output += " not null";
                if (field.detail_.is_unique) output += " unique";

                if constexpr (ndb::is_field_entity<Field>)
                {
                    /*
                    auto store_type = typename Field::type{};

                    //output += "\n\tentity type " + std::string(store_type.detail_.name);
                    output += "\n\tentity count : " + std::to_string(field.detail_.size);
                    output += "\n\ttotal size : " + std::to_string(store_type.detail_.size);

                    if constexpr (ndb::is_field_entity_vector<Field>) output += " VECTOR";*/
                }
            });

            // table options
            using Table_option = typename Table::Detail_::option;
            ndb::for_each<Table_option>([&output](auto&& i, auto&& type)
            {
                using Option = std::decay_t<decltype(type)>;

                if constexpr (decltype(i){} != 0) output += ",";

                // unique
                if constexpr(ndb::is_same_tpl_v<ndb::table_option::unique, Option>)
                {
                    output += "unique(";
                    // list fields
                    ndb::for_each<Option>([&output](auto&& i, auto&& type)
                    {
                        if constexpr (decltype(i){} != 0) output += ",";
                        using Field = std::decay_t<decltype(type)>;
                        output += ndb::field_name<Field>;
                    });

                    output += ")"; // unique(
                }
            });

            output += ");";

            // exec create table
            exec<Database>(output);
            output = "";
        });
    }

    template<class Expr>
    std::string sqlite::to_string(const Expr&)
    {
        constexpr auto str_query = ndb::native_expression<Expr, ndb::sqlite>{};
        return str_query.data();
    }
} // ndb
