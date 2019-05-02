#ifndef UTILITY_CX_TO_STRING_H_NDB
#define UTILITY_CX_TO_STRING_H_NDB

namespace ndb
{
    namespace internal
    {
        template<class T>
        constexpr unsigned int digit_count(T val)
        {
            unsigned int n = (val < 0);
            do
            {
                ++n;
            } while (val /= T{10});
            return n;
        }

        template<auto... Values>
        class cx_string_impl
        {
          public:
            static constexpr unsigned int size()
            {
                unsigned int s = 0;
                auto process = [&s](auto&& value)
                {
                    using value_type = std::decay_t<decltype(value)>;
                    if constexpr (std::is_same_v<value_type, char>)
                    {
                        s += 1;
                    }
                    else
                    {
                        s += digit_count(value);
                    }
                };
                (process(Values), ...);
                return s + 1; // + 1 to add '\0'
            }

            constexpr cx_string_impl()
                : data_{ 0 }
            {
                unsigned int index = 0;

                auto process = [&index, this](auto&& value)
                {
                    using value_type = std::decay_t<decltype(value)>;
                    if constexpr (std::is_same_v<value_type, char>)
                    {
                        data_[index++] = value;
                    }
                    else
                    {
                        unsigned int digits_count =  digit_count(value);
                        index += digits_count;
                        value_type v = value;
                        do
                        {
                            data_[--index] = char(v % 10 + '0');
                        } while (v /= value_type{10});
                        index += digits_count;
                    }
                };

                (process(Values), ...);
            }

            constexpr const char* get() const { return data_.data(); }

          private:
            std::array<char, size()> data_;
        };

        template<auto... Vs>
        static constexpr auto cx_string = cx_string_impl<Vs...>{};
    } // internal

    template<auto... Vs>
    static constexpr auto cx_string = internal::cx_string<Vs...>.get();
} // ndb

#endif // UTILITY_CX_STRING_H_NDB