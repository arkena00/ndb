#ifndef ENGINE_SQLITE_QUERY_H_NDB
#define ENGINE_SQLITE_QUERY_H_NDB

#include <ndb/type.hpp>
#include <ndb/engine.hpp>
#include <ndb/engine/native_query.hpp>
#include <ndb/engine/sqlite/type.hpp>
#include <ndb/result.hpp>
#include <ndb/type.hpp>

#include <sqlite3.h>

namespace ndb
{
    template<class Engine>
    class engine_connection;

    template<class Database>
    class native_query<Database, ndb::sqlite>
    {
    public:
        using Engine = ndb::sqlite;
        using statement_type = std::string;

        native_query() = default;

        native_query(statement_type str_statement) :
            statement_{ nullptr }
        {
            prepare(std::move(str_statement));
        }

        native_query(native_query&& other) noexcept :
            statement_ { std::exchange(other.statement_, nullptr) }
            , str_statement_{ std::move(other.str_statement_) }
            , bind_index_{ other.bind_index_ }
        {}

        native_query(const native_query&) = delete;
        native_query& operator=(const native_query&) = delete;

        ~native_query()
        {
            sqlite3_finalize(statement_);
        }

        void bind_clear()
        {
            bind_index_ = 1;
            sqlite3_clear_bindings(statement_);
        }

        template<class Expr>
        void append(const Expr& expr)
        {
            constexpr auto native_expr = ndb::native_expression<Expr, Engine>{};
            str_statement_ += std::string(native_expr.data());
        }

        void prepare(statement_type str_statement)
        {
#ifdef NDB_DEBUG_QUERY
            std::cout << "[ndb:debug_query]" << str_statement << std::endl;
#endif

            sqlite3_finalize(statement_);
            str_statement_ = std::move(str_statement);
            bind_index_ = 1;

            auto status = sqlite3_prepare_v2(connection(), str_statement_.c_str(), -1, &statement_, nullptr);

            if (status != SQLITE_OK)
            {
                std::string error = sqlite3_errmsg(connection());
                ndb_error("query error : " + error + "\nquery : " + str_statement_);
            }
        }

        template<class T>
        void bind_value(const T& value)
        {
            using storage_type = ndb::storage_type_t<Engine, ndb_type_t<T, Database>>;

            if constexpr (std::is_same_v<int_, storage_type>) sqlite3_bind_int(statement_, bind_index_, static_cast<cpp_type_t<int_, Database>>(value));
            else if constexpr (std::is_same_v<int64_, storage_type>) sqlite3_bind_int64(statement_, bind_index_, static_cast<cpp_type_t<int64_, Database>>(value));
            else if constexpr (std::is_same_v<double_, storage_type>) sqlite3_bind_double(statement_, bind_index_, static_cast<cpp_type_t<double_, Database>>(value));
            else if constexpr (std::is_same_v<string_, storage_type>)
            {
                auto data = ndb::type_data(value);
                constexpr int char_size = sizeof(std::remove_pointer_t<decltype(data)>);

                if constexpr (char_size == 1)
                    sqlite3_bind_text(statement_, bind_index_, data, -1, SQLITE_TRANSIENT);
                else if constexpr (char_size == 2)
                    sqlite3_bind_text16(statement_, bind_index_, data, -1, SQLITE_TRANSIENT);
                else ncx_error(T, "unsupported string format");
            }
            else if constexpr (std::is_same_v<byte_array_, storage_type>) sqlite3_bind_blob(statement_, bind_index_, ndb::type_data(value), static_cast<int>(value.size()), SQLITE_TRANSIENT);
            else ncx_error(T, "type unknown, add a custom type or use an engine type");
            bind_index_++;
        }

        template<class T>
        void bind(const T& value)
        {
            if constexpr (ndb::is_custom_type_v<T, Database>) bind_value(ndb::custom_type<T, Database>::internal_encode(value));
            else bind_value(value);
        }

        template<class Result_type = ndb::line<Database>>
        auto exec() const
        {
            int step = SQLITE_DONE;

            ndb::result<Database, Result_type> result;

            step = sqlite3_step(statement_);

            // select
            while (step == SQLITE_ROW)
            {
                ndb::line<Database> line;
                int field_count = sqlite3_column_count(statement_);

                for(int field_it = 0; field_it < field_count; field_it++)
                {
                    //const char* table_name = sqlite3_column_table_name(statement, field_it);
                    auto field_name = std::string{ sqlite3_column_name(statement_, field_it) };

                    int field_type_id = sqlite3_column_type(statement_, field_it);
                    sqlite3_value* field_value = sqlite3_column_value(statement_, field_it);

                    const char* data = nullptr;
                    int data_size = 0;

                    switch(field_type_id)
                    {
                        case ndb::engine_type_id<sqlite, int64_>::value:
                            line.add(field_name, ndb::type_make<cpp_type_t<int64_, Database>>(sqlite3_value_int64(field_value)));
                            break;

                        case ndb::engine_type_id<sqlite, double_>::value:
                            line.add(field_name, ndb::type_make<cpp_type_t<double_, Database>>(sqlite3_value_double(field_value)));
                            break;

                        case ndb::engine_type_id<sqlite, string_>::value:
                            line.add(field_name, ndb::type_make<cpp_type_t<string_, Database>>(reinterpret_cast<const char*>(sqlite3_value_text(field_value))));
                            break;

                        case ndb::engine_type_id<sqlite, byte_array_>::value:
                            data = reinterpret_cast<const char*>(sqlite3_value_blob(field_value));
                            data_size = sqlite3_value_bytes(field_value);
                            line.add(field_name, ndb::type_make<cpp_type_t<byte_array_, Database>>(data, data_size));
                            break;

                        case ndb::engine_type_id<sqlite, null_>::value:
                            line.add(field_name, ndb::null_type{} );
                            break;

                        default:
                            ndb_error("unknown engine type");
                    } // switch
                } // for

                if (field_count > 0)
                {
                    auto result_entity = ndb::internal::result_encoder<Result_type, Database>::decode(line);
                    result.add(std::move(result_entity));
                }
                step = sqlite3_step(statement_);
            } // while

            return result;
        }

        const char* c_str() const
        {
            return str_statement_.c_str();
        }

        const std::string& str() const
        {
            return str_statement_;
        }

        ndb::engine_connection<sqlite>& connection() const
        {
            return ndb::engine<ndb::sqlite>::get().template connection<Database>();
        }

    private:
        sqlite3_stmt* statement_;
        statement_type str_statement_;
        int bind_index_;
    };

    template<class Database>
    using sqlite_query = native_query<Database, ndb::sqlite>;

} // ndb

#endif // ENGINE_SQLITE_QUERY_H_NDB