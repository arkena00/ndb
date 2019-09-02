#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>

#include <ndb/persistent.hpp>

struct browser : ndb::persistent_group
{
    using persistent_group::persistent_group;

    ndb::persistent<std::string> home{ this, "home", "www.google.fr" };

    struct : ndb::persistent_group
    {
        using persistent_group::persistent_group;

        ndb::persistent<bool> auto_load{ this, "auto_load", false };
        ndb::persistent<int> history_size{ this, "history_size", 10 };
    } tabs{ this, "tabs" };
};

int main()
{
    ndb::initializer<ndb::sqlite> init;

    ::browser browser{ "browser" };

    std::cout << "\n" << browser.home.path() << " : " << browser.home.get();
    std::cout << "\n" << browser.tabs.auto_load.path() << " : " << browser.tabs.auto_load.get();
    std::cout << "\n" << browser.tabs.history_size.path() << " : " << browser.tabs.history_size.get();

    browser.home = "https://zestedesavoir.com";
    browser.tabs.auto_load = true;
    browser.tabs.history_size = 200;

    return 0;
}