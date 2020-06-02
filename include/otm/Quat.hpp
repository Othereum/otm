#pragma once
#include "Vector.hpp"

namespace otm
{
	template <class T>
	struct Quaternion
	{
		static_assert(std::is_floating_point_v<T>);
		
		static const Quaternion identity;

		[[nodiscard]] static constexpr Quaternion Identity() noexcept { return {}; }
		
		Vector<T, 3> v;
		T s = 1;

		constexpr Quaternion() noexcept = default;
		constexpr Quaternion(const Vector<T, 3>& v, T s) noexcept: v{v}, s{s} {}
		constexpr Quaternion(T x, T y, T z, T w) noexcept: v{x, y, z}, s{w} {}
		explicit constexpr Quaternion(const Vector<T, 4>& v4) noexcept: v{v4}, s{v4.w} {}
		
		Quaternion(const UnitVec<T, 3>& axis, Angle<RadR, T> angle) noexcept
			:Quaternion{axis.Get() * Sin(angle / 2), Cos(angle / 2)}
		{
		}

		constexpr Quaternion& operator*=(const Quaternion& q) noexcept { return *this = *this * q; }
		constexpr Quaternion operator*(const Quaternion& q) const noexcept
		{
			return {s*q.v + q.s*v + (v^q.v), s*q.s - (v|q.v)};
		}

		constexpr void Invert() noexcept { v.Negate(); }
		constexpr Quaternion operator~() const noexcept { return {-v, s}; }
	};

	template <class T>
	template <class F>
	Vector<std::common_type_t<T, F>, 3> detail::VecBase<T, 3>::RotatedBy(const Quaternion<F>& q) const noexcept
	{
		using TF = std::common_type_t<T, F>;
		auto& v = static_cast<const Vector<T, 3>&>(*this);
		return (q * Quaternion<TF>{v, 0} * ~q).v;
	}

	template <class T>
	template <class F>
	void detail::VecBase2<T, 3>::RotateBy(const Quaternion<F>& q) noexcept
	{
		static_assert(std::is_same_v<std::remove_cvref_t<decltype(this->RotatedBy())>, std::remove_cvref_t<decltype(*this)>>);
		*this = this->RotatedBy();
	}

	template <class T>
	inline const Quaternion<T> Quaternion<T>::identity = Identity();
}
