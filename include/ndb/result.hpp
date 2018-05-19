#ifndef RESULT_H_NDB
#define RESULT_H_NDB

#include <ndb/line.hpp>
#include <vector>

namespace ndb
{
    template<class T, class Engine>
    struct result_encoder;

    // nop encoder
    template<class Engine>
    struct result_encoder<ndb::line<Engine>, Engine>
    {
        static auto decode(ndb::line<Engine>& line)
        {
            return line;
        }
    };

    template<class T, class Engine>
    class result
    {
        using iterator = typename std::vector<T>::iterator;

    private:
        std::vector<T> line_list_;
        int _add_id;

    public:
        result() : _add_id(0) {}



        void add(T l)
        {
            line_list_.push_back(std::move(l));
        }

        int add_id() const { return _add_id; }

        size_t size() const { return line_list_.size(); }

        bool has_result() const { return size() > 0; }

        T& operator[](int index) { return line_list_.at(index); }

        iterator begin() { return line_list_.begin(); }
        iterator end() { return line_list_.end(); }

    };
} // ndb

#endif // RESULT_H_NDB