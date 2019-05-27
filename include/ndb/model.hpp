#ifndef MODEL_H_NDB
#define MODEL_H_NDB

#include <tuple>

namespace ndb
{
    template<class... Tables>
    struct model
    {
        template<size_t N>
        using item_at = std::tuple_element_t<N, std::tuple<Tables...>>;

        static constexpr auto count()
        {
            return sizeof...(Tables);
        }

    };

    template<class Model_tables, class Model_edges = void, class... Options>
    struct model_detail
    {
        using entity = Model_tables;
        using edges = Model_edges;
    };
} // ndb

#endif // MODEL_H_NDB
