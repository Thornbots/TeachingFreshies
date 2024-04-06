/*
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef MODM_ALIGNED_STORAGE_HPP
#define MODM_ALIGNED_STORAGE_HPP

namespace modm
{
/// @cond
namespace aligned_storage_impl
{
template<size_t Cap>
union aligned_storage_helper
{
    struct double1 { double a; };
    struct double4 { double a[4]; };
    template<class T> using maybe = std::conditional_t<(Cap >= sizeof(T)), T, char>;
    char real_data[Cap];
    maybe<int> a;
    maybe<long> b;
    maybe<long long> c;
    maybe<void*> d;
    maybe<void(*)()> e;
    maybe<double1> f;
    maybe<double4> g;
    maybe<long double> h;
};
} // namespace aligned_storage_impl
/// @endcond

/**
 * Implementation of std::aligned_storage that avoids GCC bug #61458 which can
 * cause excessive size for types smaller than the maximum alignment.
 * See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61458
 *
 * The implementation is derived from:
 * https://github.com/WG21-SG14/SG14/blob/master/SG14/inplace_function.h
 */
template<size_t Cap, size_t Align = alignof(aligned_storage_impl::aligned_storage_helper<Cap>)>
struct aligned_storage {
    using type = std::aligned_storage_t<Cap, Align>;
};

template<size_t Cap, size_t Align = alignof(aligned_storage_impl::aligned_storage_helper<Cap>)>
using aligned_storage_t = typename aligned_storage<Cap, Align>::type;

static_assert(sizeof(aligned_storage_t<sizeof(void*)>) == sizeof(void*));
static_assert(alignof(aligned_storage_t<sizeof(void*)>) == alignof(void*));

} // namespace modm

#endif // MODM_ALIGNED_STORAGE_HPP
