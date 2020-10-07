#pragma once
#include "Basic.hpp"
#include <cassert>
#include <functional>
#include <optional>

namespace otm
{
struct All_T
{
};

constexpr All_T All;

class DivByZero final : public std::logic_error
{
  public:
    DivByZero() : logic_error{"Division by zero"}
    {
    }
};

template <class T, size_t L>
struct VecBase
{
    T arr[L];

    template <class... Args>
    constexpr VecBase(Args... args) noexcept : arr{args...}
    {
    }
};

template <class T>
struct VecBase<T, 2>
{
    union {
        struct
        {
            T x, y;
        };
        T arr[2];
    };

    template <class... Args>
    constexpr VecBase(Args... args) noexcept : arr{args...}
    {
    }
};

template <class T>
struct VecBase<T, 3>
{
    union {
        struct
        {
            T x, y, z;
        };
        T arr[3];
    };

    template <class... Args>
    constexpr VecBase(Args... args) noexcept : arr{args...}
    {
    }
};

template <class T>
struct VecBase<T, 4>
{
    union {
        struct
        {
            T x, y, z, w;
        };
        T arr[4];
    };

    template <class... Args>
    constexpr VecBase(Args... args) noexcept : arr{args...}
    {
    }
};

template <class T, size_t L>
struct Vector : VecBase<T, L>
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

    [[nodiscard]] static Vector Rand(const Vector& min, const Vector& max) noexcept
    {
        Vector v;
        std::transform(min.begin(), min.end(), max.begin(), v.begin(), [](T a, T b) { return otm::Rand(a, b); });
        return v;
    }

    [[nodiscard]] static Vector Rand(T min, T max) noexcept
    {
        Vector v;
        std::generate(v.begin(), v.end(), [&] { return otm::Rand(min, max); });
        return v;
    }

    constexpr Vector(All_T, T x) noexcept
    {
        std::fill(begin(), end(), x);
    }

    template <Arithmetic... Args>
    requires(sizeof...(Args) <= L) explicit(sizeof...(Args) == 1) constexpr Vector(Args... args) noexcept
        : VecBase<T, L>{args...}
    {
    }

    template <class T2, size_t L2, Arithmetic... Args>
    requires(Min(L, L2) + sizeof...(Args) <= L) explicit(sizeof...(Args) == 0) constexpr Vector(const Vector<T2, L2>& r,
                                                                                                Args... args) noexcept
        : VecBase<T, L>{NoInit{}}
    {
        auto it = std::copy_n(r.begin(), Min(L, L2), begin());
        (it << ... << args);
        std::fill(it, end(), 0);
    }

    constexpr Vector(const Vector&) noexcept = default;
    constexpr Vector& operator=(const Vector&) noexcept = default;

    template <class T2, size_t L>
    requires(L != L2) constexpr bool operator==(const Vector<T2, L2>& r) const noexcept
    {
        return false;
    }

    template <class T2>
    requires std::integral<T>&& std::integral<T2> constexpr bool operator==(const Vector<T2, L>& r) const noexcept
    {
        std::equal(begin(), end(), r.begin());
    }

    [[nodiscard]] constexpr T LenSqr() const noexcept
    {
        return *this | *this;
    }

    [[nodiscard]] auto Len() const noexcept
    {
        return Sqrt(LenSqr());
    }

    [[nodiscard]] constexpr T DistSqr(const Vector& v) const noexcept
    {
        return (*this - v).LenSqr();
    }

    [[nodiscard]] auto Dist(const Vector& v) const noexcept
    {
        return (*this - v).Len();
    }

    /**
     * @brief Normalize this vector
     * @throws DivByZero if IsNearlyZero(LenSqr())
     */
    void Normalize() requires std::floating_point<T>
    {
        if (!TryNormalize())
            throw DivByZero{};
    }

    bool TryNormalize() noexcept requires std::floating_point<T>
    {
        const auto lensqr = LenSqr();
        if (lensqr <= kSmallNumV<T>)
            return false;

        *this /= Sqrt(lensqr);
        return true;
    }

    /**
     * @brief Get normalized vector
     * @return Normalized vector or nullopt if length is zero
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

    [[nodiscard]] constexpr T* data() noexcept
    {
        return this->arr;
    }

    [[nodiscard]] constexpr const T* data() const noexcept
    {
        return this->arr;
    }

    [[nodiscard]] constexpr size_t size() const noexcept
    {
        return L;
    }

    constexpr T& operator[](size_t i) noexcept
    {
        assert(i < L);
        return this->arr[i];
    }

    constexpr const T& operator[](size_t i) const noexcept
    {
        assert(i < L);
        return this->arr[i];
    }

    [[nodiscard]] constexpr T& at(size_t i)
    {
        if (i >= L)
            OutOfRange();

        return this->arr[i];
    }

    [[nodiscard]] constexpr const T& at(size_t i) const
    {
        if (i >= L)
            OutOfRange();

        return this->arr[i];
    }

    constexpr void Negate() noexcept
    {
        std::transform(begin(), end(), begin(), std::negate<>{});
    }

    constexpr Vector operator-() const noexcept
    {
        auto v = *this;
        v.Negate();
        return v;
    }

    constexpr Vector& operator+=(const Vector& v) noexcept
    {
        Transform(v, std::plus<>{});
    }

    constexpr Vector& operator-=(const Vector& v) noexcept
    {
        Transform(v, std::minus<>{});
    }

    constexpr Vector& operator*=(const Vector& v) noexcept
    {
        Transform(v, std::multiplies<>{});
    }

    constexpr Vector& operator*=(T f) noexcept
    {
        Transform([f](T v) -> T { return v * f; });
    }

    constexpr Vector& operator/=(T f) noexcept
    {
        Transform([f](T v) -> T { return v / f; });
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

        constexpr const_iterator(const T* data) noexcept : ptr{const_cast<T*>(data)}
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

        constexpr iterator(pointer data) noexcept : const_iterator{data}
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
Vector(Args...) -> Vector<std::common_type_t<Args...>, sizeof...(Args)>;

template <class T, size_t L, class... Args>
Vector(Vector<T, L>, Args...) -> Vector<std::common_type_t<T, Args...>, L + sizeof...(Args)>;

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

template <std::floating_point T, size_t L>
struct UnitVec
{
    [[nodiscard]] constexpr static UVec3 Forward() noexcept;
    [[nodiscard]] constexpr static UVec3 Right() noexcept;
    [[nodiscard]] constexpr static UVec3 Up() noexcept;

    [[nodiscard]] static UnitVec Rand() noexcept
    {
        Vector<T, L> v;
        v.Transform([](auto&&...) { return Gauss<T, T>(0, 1); });
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
        return static_cast<UnitVec>(reinterpret_cast<const Vector<T, 3>&>(*this).RotatedBy(q));
    }

    constexpr T operator[](size_t i) const noexcept
    {
        return v[i];
    }

    [[nodiscard]] constexpr const Vector<T, L>& Get() const noexcept
    {
        return v;
    }

    constexpr operator const Vector<T, L> &() const noexcept
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
    constexpr UnitVec(Args... args) noexcept : v{static_cast<T>(args)...}
    {
    }

    constexpr UnitVec(const Vector<T, L>& v) noexcept : v{v}
    {
    }

    Vector<T, L> v{};
};

template <class T, size_t L>
[[nodiscard]] constexpr T Min(const Vector<T, L>& v) noexcept
{
    auto m = v[0];
    for (size_t i = 1; i < L; ++i)
        m = Min(m, v[i]);
    return m;
}

template <class T, size_t L>
[[nodiscard]] constexpr T Max(const Vector<T, L>& v) noexcept
{
    auto m = v[0];
    for (size_t i = 1; i < L; ++i)
        m = Max(m, v[i]);
    return m;
}

template <class T, size_t L, class V = T>
[[nodiscard]] constexpr bool IsNearlyEqual(const Vector<T, L>& a, const Vector<T, L>& b,
                                           V tolerance = kSmallNumV<V>) noexcept
{
    for (size_t i = 0; i < L; ++i)
        if (!IsNearlyEqual(a[i], b[i], tolerance))
            return false;
    return true;
}

template <class T, size_t L, class V = T>
[[nodiscard]] constexpr bool IsNearlyZero(const Vector<T, L>& a, V tolerance = kSmallNumV<V>) noexcept
{
    for (auto x : a)
        if (!IsNearlyZero(x, tolerance))
            return false;
    return true;
}

template <size_t L, class T, class U, class V>
[[nodiscard]] constexpr auto Lerp(const Vector<T, L>& a, const Vector<U, L>& b, V alpha) noexcept
{
    return a + alpha * (b - a);
}

template <class T, class U>
constexpr auto operator^(const Vector<T, 3>& a, const Vector<U, 3>& b) noexcept
{
    return Vector{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

template <class T>
constexpr Vector<T, 3>& operator^=(Vector<T, 3>& a, const Vector<T, 3>& b) noexcept
{
    return a = a ^ b;
}

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
} // namespace otm
