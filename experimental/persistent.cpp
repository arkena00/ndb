#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>

#include <ndb/persistent.hpp>

struct nxi : ndb::persistent_group
{
    using persistent_group::persistent_group;

    struct : ndb::persistent_group
    {
        using persistent_group::persistent_group;

        ndb::persistent<std::string> home{ this, "home", "test" };
        ndb::persistent<bool> zeta{ this, "zeta" };

        struct : ndb::persistent_group
        {
            using persistent_group::persistent_group;

            ndb::persistent<int> min_width{ this, "min_width" };
            ndb::persistent<std::string> max_width{ this, "max_width" };
        } tabs{ this, "tabs" };
    } browser{ this, "browser" };
};


/*
ndb_persistent_root(nxi)
    ndb_persistent_group(browser)
        ndb_persistent(home, std::string, "test")

        ndb_persistent_group(tabs)
            ndb_persistent(max_width, std::string, "test")
        ndb_persistent_group_end(tabs)
    ndb_persistent_group_end(browser)
ndb_persistent_root_end(nxi)*/

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main()
{
    using ndb::sqlite;

    try
    {
        ndb::initializer<sqlite> init;

        ndb::connection_param cp;

        ::nxi nxi{ "nxi" };
/*
        std::cout << "\n" << nxi.browser.home.path() << " : " << nxi.browser.home.get();
        std::cout << "\n" << nxi.browser.tabs.max_width.path() << " : " << nxi.browser.tabs.max_width.get();
        std::cout << "\n" << nxi.browser.tabs.min_width.path() << " : " << nxi.browser.tabs.min_width.get();
        std::cout << "\n" << nxi.browser.zeta.path() << " : " << nxi.browser.zeta.get();
*/

        nxi.browser.home = std::string("dev.neuroshok.com");
        nxi.browser.tabs.max_width = "10";
        nxi.browser.tabs.min_width = 20;
        nxi.browser.zeta = true;

        for (auto& item : nxi.list())
        {
            std::visit(overloaded {
                [](auto arg) { std::cout << arg->path() << ": " << arg->get() << '\n'; }
                , [](ndb::persistent_group* arg) { std::cout << "group" << arg->name() << '\n'; }
            }, item);
        }

    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}