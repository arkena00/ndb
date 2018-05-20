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
        static void connect(const std::string& path = "")
        {
            get().template connect<Database>(path);
        }
    };

    template<class Database, class Engine = typename Database::engine>
    void connect(const std::string& path = "")
    {
        ndb::engine<Engine>::template connect<Database>(path);
    }
} // ndb

#endif // ENGINE_H_NDB
