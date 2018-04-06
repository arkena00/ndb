#ifndef ENGINE_H_NDB
#define ENGINE_H_NDB

namespace ndb
{
    template<class Engine>
    class engine
    {
    public:
        static Engine& get()
        {
            return Engine::instance();
        }

        template<class Database>
        static void connect()
        {
            get().template connect<Database>();
        }
    };

    template<class Database, class Engine = typename Database::engine>
    void connect()
    {
        ndb::engine<Engine>::template connect<Database>();
    }
} // ndb

#endif // ENGINE_H_NDB
