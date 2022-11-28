#pragma once
#include <elastic/detail/type_traits.hpp>
#include <iterator>
#include <span>
#include <streambuf>
#include <vector>

namespace elastic
{
    namespace detail
    {
        template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
        class basic_streambuf
        {
        public:
            using iterator = std::vector<_Ty, _Alloc>::iterator;
            using const_iterator = typename std::vector<_Ty, _Alloc>::const_iterator;
            using value_type = _Ty;
            using size_type = std::size_t;
            using reference = _Ty&;
            using const_reference = const _Ty&;
            using pointer = _Ty*;
            using const_pointer = const pointer;

            basic_streambuf()
                : buffer_()
            {
            }

            basic_streambuf(size_type capa)
                : buffer_(capa)
            {
            }

            basic_streambuf(std::span<_Ty> data)
                : basic_streambuf()
            {
                std::copy(data.begin(), data.end(), std::back_inserter(buffer_));
            }

            basic_streambuf(const basic_streambuf& buf)
                : basic_streambuf(std::span<_Ty>(buf.data(), buf.size()))
            {
            }

            virtual ~basic_streambuf() = default;

        public:
            value_type& operator[](size_type pos)
            {
                return buffer_.at(pos);
            }

        public:
            size_type size() noexcept
            {
                return buffer_.size();
            }

            const size_type size() const noexcept
            {
                return buffer_.size();
            }

            pointer data() noexcept
            {
                return buffer_.data();
            }

            const pointer data() const noexcept
            {
                return buffer_.data();
            }

            pointer rdata() noexcept
            {
                return buffer_.data() + rpos_;
            }

            const pointer rdata() const noexcept
            {
                return buffer_.data() + rpos_;
            }

            pointer wdata() noexcept
            {
                return buffer_.data() + wpos_;
            }

            const pointer wdata() const noexcept
            {
                return buffer_.data() + wpos_;
            }

            constexpr iterator begin() noexcept
            {
                return buffer_.begin();
            }

            constexpr const_iterator begin() const noexcept
            {
                return buffer_.begin();
            }

            constexpr iterator end() noexcept
            {
                return buffer_.end();
            }

            constexpr const_iterator end() const noexcept
            {
                return buffer_.end();
            }

            void resize(size_type bytes)
            {
                buffer_.resize(bytes);
            }

            void clear() noexcept
            {
                wpos_ = 0;
                rpos_ = 0;
                buffer_.clear();
            }

            auto erase(const_iterator& where)
            {
                consume(-1);

                return buffer_.erase(where);
            }

            auto erase(const_iterator& begin, const_iterator& end)
            {
                auto distance = std::distance(begin, end);

                consume(-distance);

                return buffer_.erase(begin, end);
            }

            void commit(int32_t bytes)
            {
                wpos_ += bytes;
            }

            void consume(int32_t bytes)
            {
                rpos_ += bytes;
            }

            template <typename _U>
            _U read()
            {
                constexpr auto sz = sizeof(_U);

                _U value{};

                read(&value, sz);

                return value;
            }

            void read(_Ty* dest, size_type bytes)
            {
                if (bytes + rpos_ > buffer_.size())
                    throw std::runtime_error("out of range");

                _Ty value{};
                std::memcpy(dest, buffer_.data() + rpos_, bytes);

                consume(bytes);

                return value;
            }

        private:
            int32_t wpos_;
            int32_t rpos_;
            std::vector<_Ty, _Alloc> buffer_;
        };
    } // namespace detail
} // namespace elastic