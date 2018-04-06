#ifndef RESULT_H_NDB
#define RESULT_H_NDB

#include <ndb/line.hpp>
#include <vector>

namespace ndb
{
    template<class Engine>
    class result
    {
        using iterator = typename std::vector<ndb::line<Engine>>::iterator;

    private:
        std::vector<ndb::line<Engine>> line_list_;
        int _add_id;

    public:
        result() : _add_id(0) {}

        void add(ndb::line<Engine> l)
        {
            line_list_.push_back(std::move(l));
        }

        int add_id() const { return _add_id; }

        size_t size() const { return line_list_.size(); }

        bool has_result() const { return size() > 0; }

        ndb::line<Engine>& operator[](int index) { return line_list_.at(index); }

        iterator begin() { return line_list_.begin(); }
        iterator end() { return line_list_.end(); }

    };
} // ndb

#endif // RESULT_H_NDB