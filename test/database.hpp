#ifndef NDB_DATABASE_HPP
#define NDB_DATABASE_HPP

#include <ndb/preprocessor.hpp>
#include <ndb/database.hpp>
#include <ndb/model.hpp>
#include <ndb/table.hpp>
#include <ndb/field.hpp>
#include <ndb/entity.hpp>
#include <ndb/expression.hpp>

namespace fields
{
    template<class Table>
    struct id : public ndb::field<Table, int, ndb::size<sizeof(int)>> { ndb_internal_field_op };
}

namespace tables
{
    template<class Model>
    struct user : ndb::table<Model>
    {
        using id_ = fields::id<user>; static constexpr id_ id {};
        struct name_ : ndb::field<user, std::string, ndb::size<255>> {} name;

        using Detail_ = ndb::table_detail<
        ndb::entity<id_, name_>
        >;
    };

    template<class Model>
    struct movie : ndb::table<Model>
    {
        using id_ = fields::id<movie>; static constexpr id_ id {};
        static constexpr struct name_ : ndb::field<movie, std::string, ndb::size<255>> { ndb_internal_field_op } name {};
        static constexpr struct image_ : ndb::field<movie, std::string, ndb::size<255>> { ndb_internal_field_op } image {};
        static constexpr struct user_ : ndb::field<movie, tables::user<Model>> {} user {};

        using Detail_ = ndb::table_detail
        <
        ndb::entity<id_, name_, image_>
        >;
    };

    template<class Model>
    struct music : ndb::table<Model>
    {
        using id_ = fields::id<music>; static constexpr id_ id {};
        static constexpr struct name_ : ndb::field<music, std::string, ndb::size<255>> { ndb_internal_field_op } name {};
        static constexpr struct image_ : ndb::field<music, std::string, ndb::size<255>> { ndb_internal_field_op } image {};
        //static constexpr struct user_ : ndb::field<movie, tables::user> {} user {};

        using Detail_ = ndb::table_detail
        <
        ndb::entity<id_, name_, image_>
        >;
    };
}

struct models
{
    struct library_
    {
        using movie_ = tables::movie<library_>; static constexpr movie_ movie {};
        using user_ = tables::user<library_>; static constexpr user_ user {};
        using music_ = tables::music<library_>; static constexpr music_ music {};

        using Detail_ = ndb::model_detail
        <
        ndb::entity< user_, music_, movie_>
        >;
    };

    static constexpr const models::library_ library = {};
};

namespace databases
{
    struct project
    {
        static constexpr auto name = "test";

        static constexpr struct alpha_ : ndb::database<project, models::library_, ndb::mongo>{} alpha{};
        static constexpr struct zeta_ : ndb::database<project, models::library_, ndb::sqlite>{} zeta{};

        using Detail_ = ndb::database_detail<ndb::entity<alpha_, zeta_>>;
    };
} // databases

namespace dbs
{
    using alpha = databases::project::alpha_;
    using zeta = databases::project::zeta_;
}



/*
using Movie = entity<id, name, image>;
struct movie : db::entity<models::library::movie>
{
    int id_;
    std::string name_;
    ndb_serialize(id_ & name_)
    movie(int id) : entity(id) {}
};
*/

#endif //NDB_DATABASE_HPP