#pragma once
#include "Matrix.hpp"

namespace otm
{
	template <class T>
	struct Quaternion
	{
		static_assert(std::is_floating_point_v<T>);
		
		static const Quaternion identity;

		[[nodiscard]] static constexpr Quaternion Identity() noexcept { return {}; }
		[[nodiscard]] static Quaternion Rand() noexcept { return {UVec3::Rand(), Rad::Rand()}; }

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

		explicit Quaternion(const Mat3& m) noexcept
		{
			if (const auto trace = m[0][0] + m[1][1] + m[2][2]; trace > 0)
			{
				const auto t = 0.5_f / sqrt(trace + 1);
				v[0] = (m[1][2] - m[2][1]) * t;
				v[1] = (m[2][0] - m[0][2]) * t;
				v[2] = (m[0][1] - m[1][0]) * t;
				s = 0.25_f / t;
			}
			else if (m[0][0] > m[1][1] && m[0][0] > m[2][2])
			{
				const auto t = 2 * sqrt(1 + m[0][0] - m[1][1] - m[2][2]);
				v[0] = 0.25_f * t;
				v[1] = (m[0][1] + m[1][0]) / t;
				v[2] = (m[0][2] + m[2][0]) / t;
				s = (m[1][2] - m[2][1]) / t;
			}
			else if (m[1][1] > m[2][2])
			{
				const auto t = 2 * sqrt(1 + m[1][1] - m[0][0] - m[2][2]);
				v[0] = (m[0][1] + m[1][0]) / t;
				v[1] = 0.25_f * t;
				v[2] = (m[1][2] + m[2][1] ) / t;
				s = (m[2][0] - m[0][2]) / t;
			}
			else
			{
				const auto t = 2 * sqrt(1 + m[2][2] - m[0][0] - m[1][1]);
				v[0] = (m[0][2] + m[2][0]) / t;
				v[1] = (m[1][2] + m[2][1]) / t;
				v[2] = 0.25_f * t;
				s = (m[0][1] - m[1][0]) / t;
			}
		}

		constexpr Quaternion& operator*=(const Quaternion& q) noexcept { return *this = *this * q; }
		constexpr Quaternion operator*(const Quaternion& q) const noexcept
		{
			return {s*q.v + q.s*v + (v^q.v), s*q.s - (v|q.v)};
		}

		constexpr void Invert() noexcept { v.Negate(); }
		constexpr Quaternion operator~() const noexcept { return {-v, s}; }
	};

	template <class T, class V = T>
	[[nodiscard]] bool IsEquivalent(const Quaternion<T>& a, const Quaternion<T>& b, V tolerance = kSmallNumV<V>) noexcept
	{
		return IsNearlyEqual(Vec3::one.RotatedBy(a), Vec3::one.RotatedBy(b), tolerance);
	}

	template <class T>
	template <class F>
	Vector<std::common_type_t<T, F>, 3> detail::VecBase<T, 3>::RotatedBy(const Quaternion<F>& q) const noexcept
	{
		using Tf = std::common_type_t<T, F>;
		auto& vr = static_cast<const Vector<T, 3>&>(*this);
		return (q * Quaternion<Tf>{Vector<Tf, 3>{vr}, 0} * ~q).v;
	}

	template <class T>
	template <class F>
	void detail::VecBase<T, 3>::RotateBy(const Quaternion<F>& q) noexcept
	{
		static_assert(std::is_same_v<std::remove_cvref_t<decltype(this->RotatedBy())>, std::remove_cvref_t<decltype(*this)>>);
		*this = this->RotatedBy(q);
	}

	template <class T>
	inline const Quaternion<T> Quaternion<T>::identity = Identity();
}
