#pragma once
#include "Angle.hpp"
#include "Basic.hpp"
#include <cassert>
#include <functional>
#include <optional>
#include <ostream>

namespace otm
{
namespace detail
{
template <class T, size_t L>
struct VecBase0
{
    template <class... Args>
    constexpr VecBase0(Args ... args) noexcept
        : data{static_cast<T>(args)...}
    {
    }

    T data[L]{};
};

template <class T, size_t L>
struct VecBase : VecBase0<T, L>
{
    template <class... Args>
    constexpr VecBase(Args ... args) noexcept
        : VecBase0<T, L>{args...}
    {
    }
};

template <class T>
struct VecBase<T, 2> : VecBase0<T, 2>
{
    template <class... Args>
    constexpr VecBase(Args ... args) noexcept
        : VecBase0<T, 2>{args...}
    {
    }

    [[nodiscard]] Angle<RadR, CommonFloat<T>> ToAngle() const noexcept
    {
        auto& v = static_cast<const Vector<T, 2>&>(*this);
        return Atan2(v[1], v[0]);
    }
};

template <class T>
struct VecBase<T, 3> : VecBase0<T, 3>
{
    static const Vector<T, 3> forward;
    static const Vector<T, 3> backward;
    static const Vector<T, 3> right;
    static const Vector<T, 3> left;
    static const Vector<T, 3> up;
    static const Vector<T, 3> down;

    constexpr static Vector<T, 3> Forward() noexcept
    {
        return {1, 0, 0};
    }

    constexpr static Vector<T, 3> Backward() noexcept
    {
        return -Forward();
    }

    constexpr static Vector<T, 3> Right() noexcept
    {
        return {0, 1, 0};
    }

    constexpr static Vector<T, 3> Left() noexcept
    {
        return -Right();
    }

    constexpr static Vector<T, 3> Up() noexcept
    {
        return {0, 0, 1};
    }

    constexpr static Vector<T, 3> Down() noexcept
    {
        return -Up();
    }

    template <class... Args>
    constexpr VecBase(Args ... args) noexcept
        : VecBase0<T, 3>{args...}
    {
    }

    template <class U>
    constexpr auto operator^(const Vector<U, 3>& b) const noexcept
    {
        auto& a = this->data;
        return Vector{a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
    }

    constexpr Vector<T, 3>& operator^=(const Vector<T, 3>& b) noexcept
    {
        *this = *this ^ b;
        return static_cast<Vector<T, 3>&>(*this);
    }

    template <class F>
    [[nodiscard]] Vector<std::common_type_t<T, F>, 3> RotatedBy(const Quaternion<F>& q) const noexcept;

    template <class F>
    void RotateBy(const Quaternion<F>& q) noexcept;
};

template <class T, size_t L>
struct UnitVecBase
{
};

template <class T>
struct UnitVecBase<T, 3>
{
    static const UnitVec<T, 3> forward;
    static const UnitVec<T, 3> backward;
    static const UnitVec<T, 3> right;
    static const UnitVec<T, 3> left;
    static const UnitVec<T, 3> up;
    static const UnitVec<T, 3> down;

    constexpr static UnitVec<T, 3> Forward() noexcept
    {
        return {1, 0, 0};
    }

    constexpr static UnitVec<T, 3> Backward() noexcept
    {
        return {-1, 0, 0};
    }

    constexpr static UnitVec<T, 3> Right() noexcept
    {
        return {0, 1, 0};
    }

    constexpr static UnitVec<T, 3> Left() noexcept
    {
        return {0, -1, 0};
    }

    constexpr static UnitVec<T, 3> Up() noexcept
    {
        return {0, 0, 1};
    }

    constexpr static UnitVec<T, 3> Down() noexcept
    {
        return {0, 0, -1};
    }
};
}

struct All
{
};

class DivByZero final : public std::logic_error
{
public:
    DivByZero()
        : logic_error{"Division by zero"}
    {
    }

    explicit DivByZero(const std::string& s)
        : logic_error{s}
    {
    }

    explicit DivByZero(const char* s)
        : logic_error{s}
    {
    }
};

template <class T, size_t L>
struct Vector : detail::VecBase<T, L>
{
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    struct iterator;
    struct const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static const Vector zero;
    static const Vector one;

    constexpr static Vector Zero() noexcept
    {
        return {};
    }

    constexpr static Vector One() noexcept
    {
        return {All{}, 1};
    }

    [[nodiscard]] static Vector Rand(const Vector& min, const Vector& max) noexcept
    {
        Vector v;
        for (size_t i = 0; i < L; ++i)
            v[i] = otm::Rand(min[i], max[i]);
        return v;
    }

    [[nodiscard]] static Vector Rand(T min, T max) noexcept
    {
        return Vector{[min, max]
        {
            return otm::Rand(min, max);
        }};
    }

    constexpr Vector(All, T x) noexcept
        : Vector{[x]
        {
            return x;
        }}
    {
    }

    template <class Fn, std::enable_if_t<std::is_invocable_v<Fn>, int>  = 0>
    explicit constexpr Vector(Fn&& fn) noexcept
    {
        Transform([&fn](T)
        {
            return fn();
        });
    }

    constexpr Vector() noexcept
        : detail::VecBase<T, L>{}
    {
    }

    explicit constexpr Vector(T x) noexcept
        : detail::VecBase<T, L>{x}
    {
    }

    template <class... Args>
    constexpr Vector(T x, T y, Args ... args) noexcept
        : detail::VecBase<T, L>{x, y, args...}
    {
    }

    ~Vector() = default;
    constexpr Vector(const Vector&) noexcept = default;
    constexpr Vector(Vector&&) noexcept = default;
    constexpr Vector& operator=(const Vector&) noexcept = default;
    constexpr Vector& operator=(Vector&&) noexcept = default;

    template <class T2, size_t L2>
    explicit constexpr Vector(const Vector<T2, L2>& r) noexcept
    {
        Assign(r);
    }

    template <class T2, size_t L2, class... Args>
    constexpr Vector(const Vector<T2, L2>& v, T x, Args ... args) noexcept
    {
        static_assert(sizeof...(Args) + L2 < L, "Too many arguments");
        ((Assign(v) << x) << ... << static_cast<T>(args));
    }

    /**
     * \brief Assign elements of other vector to this. The value of the unassigned elements does not change.
     * \return Iterator pointing next to the last element assigned
     * \note Does nothing if offset is out of range
     */
    template <class T2, size_t L2>
    constexpr iterator Assign(const Vector<T2, L2>& other, ptrdiff_t offset = 0) noexcept
    {
        // ReSharper disable once CppInitializedValueIsAlwaysRewritten
        size_t size = 0; // Initialization is required for constexpr

        if (offset >= 0)
        {
            size = Min(L - Min(L, static_cast<size_t>(offset)), L2);
            for (size_t i = 0; i < size; ++i)
                (*this)[i + offset] = static_cast<T>(other[i]);
        }
        else
        {
            size = Min(L, L2 - Min(L2, static_cast<size_t>(-offset)));
            for (size_t i = 0; i < size; ++i)
                (*this)[i] = static_cast<T>(other[i - offset]);
        }

        return begin() + size;
    }

    template <class T2>
    constexpr bool operator==(const Vector<T2, L>& r) const noexcept
    {
        static_assert(std::is_integral_v<T> && std::is_integral_v<T2>,
            "Can't compare equality between floating point types. Use IsNearlyEqual() instead.");

        for (size_t i = 0; i < L; ++i)
            if ((*this)[i] != r[i])
                return false;
        return true;
    }

    template <class T2>
    constexpr bool operator!=(const Vector<T2, L>& r) const noexcept
    {
        return !(*this == r);
    }

    template <class T2, size_t L2>
    constexpr bool operator==(const Vector<T2, L2>&) const noexcept
    {
        return false;
    }

    template <class T2, size_t L2>
    constexpr bool operator!=(const Vector<T2, L2>&) const noexcept
    {
        return true;
    }

    [[nodiscard]] constexpr T LenSqr() const noexcept
    {
        return *this | *this;
    }

    [[nodiscard]] CommonFloat<T> Len() const noexcept
    {
        return std::sqrt(ToFloat(LenSqr()));
    }

    [[nodiscard]] constexpr T DistSqr(const Vector& v) const noexcept
    {
        return (*this - v).LenSqr();
    }

    [[nodiscard]] CommonFloat<T> Dist(const Vector& v) const noexcept
    {
        return (*this - v).Len();
    }

    /**
     * \brief Normalize this vector
     * \throws DivByZero if IsNearlyZero(LenSqr())
     */
    void Normalize()
    {
        if (!TryNormalize())
            throw DivByZero{};
    }

    bool TryNormalize() noexcept
    {
        static_assert(std::is_same_v<T, CommonFloat<T>>, "Can't use Normalize() for this type. Use Unit() instead.");
        const auto lensqr = LenSqr();
        if (lensqr <= kSmallNumV<T>)
            return false;
        *this /= std::sqrt(ToFloat(lensqr));
        return true;
    }

    constexpr void Clamp(T min, T max) noexcept
    {
        Transform([&](T x)
        {
            return Clamp(x, min, max);
        });
    }

    /**
     * \brief Get normalized vector
     * \return Normalized vector or nullopt if length is zero
     */
    [[nodiscard]] std::optional<UnitVec<CommonFloat<T>, L>> Unit() const noexcept;

    [[nodiscard]] Matrix<T, 1, L>& AsRowMatrix() noexcept
    {
        return reinterpret_cast<Matrix<T, 1, L>&>(*this);
    }

    [[nodiscard]] const Matrix<T, 1, L>& AsRowMatrix() const noexcept
    {
        return reinterpret_cast<const Matrix<T, 1, L>&>(*this);
    }

    [[nodiscard]] Matrix<T, L, 1>& AsColMatrix() noexcept
    {
        return reinterpret_cast<Matrix<T, L, 1>&>(*this);
    }

    [[nodiscard]] const Matrix<T, L, 1>& AsColMatrix() const noexcept
    {
        return reinterpret_cast<const Matrix<T, L, 1>&>(*this);
    }

    [[nodiscard]] constexpr Matrix<T, 1, L> ToRowMatrix() const noexcept;
    [[nodiscard]] constexpr Matrix<T, L, 1> ToColMatrix() const noexcept;

    constexpr T& operator[](size_t i) noexcept
    {
        assert(i < L);
        return this->data[i];
    }

    constexpr T operator[](size_t i) const noexcept
    {
        assert(i < L);
        return this->data[i];
    }

    [[nodiscard]] constexpr T& at(size_t i)
    {
        if (i >= L)
            OutOfRange();
        return this->data[i];
    }

    [[nodiscard]] constexpr T at(size_t i) const
    {
        if (i >= L)
            OutOfRange();
        return this->data[i];
    }

    template <class Fn>
    constexpr Vector& Transform(const Vector& other, Fn&& fn) noexcept(std::is_nothrow_invocable_v<Fn, T, T>)
    {
        for (size_t i = 0; i < L; ++i)
            (*this)[i] = fn((*this)[i], other[i]);

        return *this;
    }

    template <class Fn>
    constexpr Vector& Transform(Fn&& fn) noexcept(std::is_nothrow_invocable_v<Fn, T>)
    {
        for (size_t i = 0; i < L; ++i)
            (*this)[i] = fn((*this)[i]);

        return *this;
    }

    constexpr void Negate() noexcept
    {
        Transform(std::negate<>{});
    }

    constexpr Vector operator-() const noexcept
    {
        auto v = *this;
        v.Negate();
        return v;
    }

    constexpr Vector& operator+=(const Vector& v) noexcept
    {
        return Transform(v, std::plus<>{});
    }

    constexpr Vector& operator-=(const Vector& v) noexcept
    {
        return Transform(v, std::minus<>{});
    }

    constexpr Vector& operator*=(const Vector& v) noexcept
    {
        return Transform(v, std::multiplies<>{});
    }

    constexpr Vector& operator*=(T f) noexcept
    {
        return Transform([f](T v) -> T
        {
            return v * f;
        });
    }

    constexpr Vector& operator/=(T f) noexcept
    {
        return Transform([f](T v) -> T
        {
            return v / f;
        });
    }

    template <class U>
    constexpr auto operator+(const Vector<U, L>& v) const noexcept
    {
        Vector<std::common_type_t<T, U>, L> r;
        for (size_t i = 0; i < L; ++i)
            r[i] = (*this)[i] + v[i];
        return r;
    }

    template <class U>
    constexpr auto operator-(const Vector<U, L>& v) const noexcept
    {
        Vector<std::common_type_t<T, U>, L> r;
        for (size_t i = 0; i < L; ++i)
            r[i] = (*this)[i] - v[i];
        return r;
    }

    template <class U>
    constexpr auto operator*(const Vector<U, L>& v) const noexcept
    {
        Vector<std::common_type_t<T, U>, L> r;
        for (size_t i = 0; i < L; ++i)
            r[i] = (*this)[i] * v[i];
        return r;
    }

    template <class U>
    constexpr auto operator*(U f) const noexcept
    {
        using V = std::common_type_t<T, U>;
        Vector<V, L> v = *this;
        v *= static_cast<V>(f);
        return v;
    }

    template <class U>
    constexpr auto operator/(U f) const noexcept
    {
        using V = std::common_type_t<T, U>;
        Vector<V, L> v = *this;
        v /= static_cast<V>(f);
        return v;
    }

    template <class T2>
    constexpr std::common_type_t<T, T2> operator|(const Vector<T2, L>& v) const noexcept
    {
        std::common_type_t<T, T2> t{};
        for (size_t i = 0; i < L; ++i)
            t += (*this)[i] * v[i];
        return t;
    }

    constexpr iterator operator<<(T v) noexcept
    {
        return begin() << v;
    }

    constexpr iterator operator>>(T& v) noexcept
    {
        return begin() >> v;
    }

    constexpr const_iterator operator>>(T& v) const noexcept
    {
        return begin() >> v;
    }

    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return this->data;
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return this->data;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return this->data;
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return this->data + L;
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return this->data + L;
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return this->data + L;
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator{end()};
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator{end()};
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator{cend()};
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator{begin()};
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator{begin()};
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator{cbegin()};
    }

    struct const_iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        constexpr const_iterator() noexcept = default;

        constexpr explicit operator const T*() const noexcept
        {
            return ptr;
        }

        constexpr T operator*() const noexcept
        {
            return *ptr;
        }

        constexpr T operator[](difference_type n) const noexcept
        {
            return ptr[n];
        }

        constexpr const T* operator->() const noexcept
        {
            return ptr;
        }

        constexpr const_iterator& operator++() noexcept
        {
            ++ptr;
            return *this;
        }

        constexpr const_iterator operator++(int) noexcept
        {
            const_iterator it = *this;
            ++ptr;
            return it;
        }

        constexpr const_iterator& operator--() noexcept
        {
            --ptr;
            return *this;
        }

        constexpr const_iterator operator--(int) noexcept
        {
            const_iterator it = *this;
            --ptr;
            return it;
        }

        constexpr const_iterator& operator+=(ptrdiff_t n) noexcept
        {
            ptr += n;
            return *this;
        }

        constexpr const_iterator operator+(ptrdiff_t n) const noexcept
        {
            return const_iterator{ptr + n};
        }

        constexpr const_iterator& operator-=(ptrdiff_t n) noexcept
        {
            ptr -= n;
            return *this;
        }

        constexpr const_iterator operator-(ptrdiff_t n) const noexcept
        {
            return const_iterator{ptr - n};
        }

        constexpr ptrdiff_t operator-(const const_iterator& rhs) const noexcept
        {
            return ptr - rhs.ptr;
        }

        constexpr const_iterator& operator>>(T& v) noexcept
        {
            v = **this;
            return ++*this;
        }

        constexpr bool operator==(const const_iterator& it) const noexcept
        {
            return ptr == it.ptr;
        }

        constexpr bool operator!=(const const_iterator& it) const noexcept
        {
            return ptr != it.ptr;
        }

        constexpr bool operator<(const const_iterator& it) const noexcept
        {
            return ptr < it.ptr;
        }

        constexpr bool operator>(const const_iterator& it) const noexcept
        {
            return ptr > it.ptr;
        }

        constexpr bool operator<=(const const_iterator& it) const noexcept
        {
            return ptr <= it.ptr;
        }

        constexpr bool operator>=(const const_iterator& it) const noexcept
        {
            return ptr >= it.ptr;
        }

    protected:
        friend Vector;

        constexpr const_iterator(const T* data) noexcept
            : ptr{const_cast<T*>(data)}
        {
        }

        T* ptr = nullptr;
    };

    struct iterator : const_iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        constexpr iterator() noexcept = default;

        constexpr explicit operator T*() const noexcept
        {
            return this->ptr;
        }

        constexpr T& operator*() const noexcept
        {
            return *this->ptr;
        }

        constexpr T& operator[](ptrdiff_t n) const noexcept
        {
            return this->ptr[n];
        }

        constexpr T* operator->() const noexcept
        {
            return this->ptr;
        }

        constexpr iterator& operator++() noexcept
        {
            ++this->ptr;
            return *this;
        }

        constexpr iterator operator++(int) noexcept
        {
            iterator it = *this;
            ++this->ptr;
            return it;
        }

        constexpr iterator& operator--() noexcept
        {
            --this->ptr;
            return *this;
        }

        constexpr iterator operator--(int) noexcept
        {
            iterator it = *this;
            --this->ptr;
            return it;
        }

        constexpr iterator& operator+=(ptrdiff_t n) noexcept
        {
            this->ptr += n;
            return *this;
        }

        constexpr iterator operator+(ptrdiff_t n) const noexcept
        {
            return iterator{this->ptr + n};
        }

        constexpr iterator& operator-=(ptrdiff_t n) noexcept
        {
            this->ptr -= n;
            return *this;
        }

        constexpr iterator operator-(ptrdiff_t n) const noexcept
        {
            return iterator{this->ptr - n};
        }

        constexpr ptrdiff_t operator-(const iterator& rhs) const noexcept
        {
            return this->ptr - rhs.ptr;
        }

        constexpr iterator& operator<<(T v) noexcept
        {
            **this = v;
            return ++*this;
        }

        constexpr iterator& operator>>(T& v) noexcept
        {
            v = **this;
            return ++*this;
        }

    protected:
        friend Vector;

        constexpr iterator(pointer data) noexcept
            : const_iterator{data}
        {
        }
    };

private:
    [[noreturn]] static void OutOfRange()
    {
        throw std::out_of_range{"Vector out of range"};
    }
};

template <class... Args>
Vector(Args ...) -> Vector<std::common_type_t<Args...>, sizeof...(Args)>;

template <class T, size_t L, class... Args>
Vector(Vector<T, L>, Args ...) -> Vector<std::common_type_t<T, Args...>, L + sizeof...(Args)>;

template <class F, class T, size_t L>
constexpr auto operator*(F f, const Vector<T, L>& v) noexcept
{
    return v * f;
}

template <class T, size_t L>
std::ostream& operator<<(std::ostream& os, const Vector<T, L>& v)
{
    os << v[0];
    for (size_t i = 1; i < L; ++i)
        os << ' ' << v[i];
    return os;
}

template <class T, size_t L>
std::istream& operator>>(std::istream& is, Vector<T, L>& v)
{
    for (size_t i = 0; i < L; ++i)
        is >> v[i];
    return is;
}

template <class T, size_t L>
struct UnitVec : detail::UnitVecBase<T, L>
{
    static_assert(std::is_floating_point_v<T>);

    [[nodiscard]] static UnitVec Rand() noexcept
    {
        Vector<T, L> v;
        v.Transform([](auto&&...)
        {
            return Gauss<T, T>(0, 1);
        });
        if (auto u = v.Unit())
            return *u;
        return Rand();
    }

    void RotateBy(const Quaternion<T>& q) noexcept
    {
        static_assert(L == 3);
        reinterpret_cast<Vector<T, 3>&>(*this).RotateBy(q);
    }

    [[nodiscard]] UnitVec RotatedBy(const Quaternion<T>& q) const noexcept
    {
        static_assert(L == 3);
        return static_cast<UnitVec>(
            reinterpret_cast<const Vector<T, 3>&>(*this).RotatedBy(q)
        );
    }

    constexpr T operator[](size_t i) const noexcept
    {
        return v[i];
    }

    [[nodiscard]] constexpr const Vector<T, L>& Get() const noexcept
    {
        return v;
    }

    constexpr operator const Vector<T, L>&() const noexcept
    {
        return v;
    }

    constexpr const Vector<T, L>* operator->() const noexcept
    {
        return &v;
    }

    constexpr const Vector<T, L>& operator*() const noexcept
    {
        return v;
    }

private:
    template <class, class>
    friend struct Angle;
    friend Vector<T, L>;
    friend detail::UnitVecBase<T, L>;

    template <class... Args>
    constexpr UnitVec(Args ... args) noexcept
        : v{static_cast<T>(args)...}
    {
    }

    constexpr UnitVec(const Vector<T, L>& v) noexcept
        : v{v}
    {
    }

    Vector<T, L> v{};
};

template <class T, size_t L>
std::optional<UnitVec<CommonFloat<T>, L>> Vector<T, L>::Unit() const noexcept
{
    const auto lensqr = LenSqr();
    if (lensqr <= kSmallNumV<T>)
        return {};
    return UnitVec{*this / std::sqrt(ToFloat(lensqr))};
}

template <class Ratio, class T>
UnitVec<CommonFloat<T>, 2> Angle<Ratio, T>::ToVector() const noexcept
{
    return {{Cos(*this), Sin(*this)}};
}

template <class T, size_t L>
inline const Vector<T, L> Vector<T, L>::zero = Zero();

template <class T, size_t L>
inline const Vector<T, L> Vector<T, L>::one = One();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::forward = Forward();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::backward = Backward();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::right = Right();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::left = Left();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::up = Up();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::down = Down();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::forward = Forward();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::backward = Backward();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::right = Right();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::left = Left();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::up = Up();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::down = Down();
}
