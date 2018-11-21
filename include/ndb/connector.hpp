#ifndef CONNECTOR_H_NDB
#define CONNECTOR_H_NDB

#include <ndb/engine.hpp>

namespace ndb
{
    template<class Database>
    class connector
    {
    public:
        template<class... Args>
        connector(Args&&... args)
        {
            ndb::connect<Database>(std::forward<Args>(args)...);
        }

        ~connector()
        {
            // delete connection
        }
    };

} // ndb

#endif // CONNECTOR_H_NDB
