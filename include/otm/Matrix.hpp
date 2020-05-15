#pragma once
#include "Vector.hpp"
#include "Quat.hpp"

namespace otm
{
	namespace detail
	{
		template <class T, size_t R, size_t C>
		struct MatrixBase {};

		template <class T, size_t L>
		struct MatrixBase<T, L, L>
		{
			constexpr void Transpose() noexcept;
			
			// Matrix with ones on the main diagonal and zeros elsewhere
			static constexpr Matrix Identity() noexcept
			{
				Matrix matrix;
				for (size_t i = 0; i < L; ++i)
					matrix[i][i] = 1;
				return matrix;
			}

			// Matrix that assigned other matrix to the identity matrix
			template <class T2, size_t R2, size_t C2>
			static constexpr Matrix Identity(const Matrix<T2, R2, C2>& other, const Vector<ptrdiff_t, 2>& offset = {}) noexcept
			{
				auto m = Identity();
				m.Assign(other, offset);
				return m;
			}
		};

		template <class T, size_t R, size_t C>
		struct MatrixGeometry : MatrixBase<T, R, C> {};

		template <std::floating_point T>
		struct MatrixGeometry<T, 3, 3> : MatrixBase<T, 3, 3>
		{
			static constexpr Matrix<T, 3, 3> SimpleViewProj(const Vector<T, 2>& scr) noexcept;
		};
		
		template <std::floating_point T>
		struct MatrixGeometry<T, 4, 4> : MatrixBase<T, 4, 4>
		{
			static constexpr Matrix<T, 4, 4> Translation(const Vector<T, 3>& pos) noexcept;
			static constexpr Matrix<T, 4, 4> Rotation(const Quaternion<T>& rot) noexcept;
			static constexpr Matrix<T, 4, 4> Scale(const Vector<T, 3>& scale) noexcept;
			static Matrix<T, 4, 4> LookAt(const Vector<T, 3>& eye, const Vector<T, 3>& target, const Vector<T, 3>& up);
		};
		
	}

	template <class T, size_t R, size_t C>
	struct Matrix : detail::MatrixGeometry<T, R, C>
	{
		constexpr Matrix() noexcept :arr{} {}

		constexpr Matrix(All, T x) noexcept
		{
			for (auto& v : flat) v = x;
		}

		template <class T2, size_t R2, size_t C2>
		explicit constexpr Matrix(const Matrix<T2, R2, C2>& other)
		{
			Assign(other);
		}

		template <class... Args>
		explicit(sizeof...(Args) == 0)
		constexpr Matrix(T x, Args... args) noexcept
			:arr{x, static_cast<T>(args)...}
		{
		}

		/**
		 * \brief Assign elements of other matrix to this. The value of the unassigned elements does not change.
		 * \param other Matrix to be assigned to this
		 * \param offset Must be (x < C && y < R)
		 */
		template <class T2, size_t R2, size_t C2>
		constexpr void Assign(const Matrix<T2, R2, C2>& other, const Vector<ptrdiff_t, 2>& offset = {}) noexcept
		{
			if (offset.y >= 0)
			{
				const auto size = Min(R - Min(R, offset.y), R2);
				for (size_t i = 0; i < size; ++i)
					(*this)[i + offset.y].Assign(other[i], offset.x);
			}
			else
			{
				const auto size = Min(R, R2 - Min(R2, -offset.y));
				for (size_t i = 0; i < size; ++i)
					(*this)[i].Assign(other[i - offset.y], offset.x);
			}
		}

		constexpr bool operator==(const Matrix&) const noexcept = default;

		constexpr auto& operator[](size_t i) noexcept { return m[i]; }
		constexpr auto& operator[](size_t i) const noexcept { return m[i]; }
		constexpr auto& Row(size_t i) noexcept { return m[i]; }
		[[nodiscard]] constexpr auto& Row(size_t i) const noexcept { return m[i]; }

		[[nodiscard]] constexpr auto Col(size_t c) const noexcept
		{
			Vector<T, R> v;
			
			for (size_t r = 0; r < R; ++r)
				v[r] = m[r][c];
			
			return v;
		}

		constexpr void ColAssign(size_t c, const Vector<T, R>& v) noexcept
		{
			for (size_t r = 0; r < R; ++r)
				m[r][c] = v[r];
		}

		[[nodiscard]] constexpr auto& AsVectors() noexcept { return m; }
		[[nodiscard]] constexpr auto& AsVectors() const noexcept { return m; }
		[[nodiscard]] constexpr auto& AsFloats() noexcept { return flat; }
		[[nodiscard]] constexpr auto& AsFloats() const noexcept { return flat; }

		constexpr Matrix operator+(const Matrix& b) const noexcept
		{
			auto c = *this;
			return c += b;
		}

		constexpr Matrix& operator+=(const Matrix& b) noexcept
		{
			for (auto i = 0; i < R; ++i) m[i] += b[i];
			return *this;
		}

		constexpr Matrix operator-(const Matrix& b) const noexcept
		{
			auto c = *this;
			return c -= b;
		}

		constexpr Matrix& operator-=(const Matrix& b) noexcept
		{
			for (auto i = 0; i < R; ++i) m[i] -= b[i];
			return *this;
		}

		constexpr Matrix operator*(T f) const noexcept
		{
			auto c = *this;
			return c *= f;
		}

		constexpr Matrix& operator*=(T f) noexcept
		{
			for (auto i = 0; i < R; ++i) m[i] *= f;
			return *this;
		}

		constexpr Matrix operator/(T f) const noexcept
		{
			auto c = *this;
			return c /= f;
		}

		constexpr Matrix& operator/=(T f) noexcept
		{
			for (auto i = 0; i < R; ++i) m[i] /= f;
			return *this;
		}

		template <size_t C2>
		constexpr Matrix<T, R, C2> operator*(const Matrix<T, C, C2>& b) const noexcept
		{
			Matrix<T, R, C2> c;
			for (size_t i = 0; i < R; ++i)
				for (size_t j = 0; j < C2; ++j)
					c[i][j] = Row(i) | b.Col(j);
			return c;
		}

		constexpr Matrix& operator*=(const Matrix& b) noexcept { return *this = *this * b; }

		[[nodiscard]] constexpr Matrix<T, C, R> Transposed() const noexcept
		{
			Matrix<T, C, R> t;
			for (auto i = 0; i < R; ++i)
				for (auto j = 0; j < C; ++j)
					t[j][i] = m[i][j];
			return t;
		}

	private:
		union
		{
			Vector<T, C> m[R];
			T arr[R][C];
			T flat[R*C];
		};
	};

	template <class F, class T, size_t R, size_t C>
	constexpr auto operator*(F f, const Matrix<T, R, C>& m) noexcept { return m * f; }

	template <class T, size_t R, size_t C>
	std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& m)
	{
		os << m[0];
		for (size_t i = 1; i < R; ++i)
			os << '\n' << m[i];
		return os;
	}

	namespace detail
	{
		template <class T, size_t L>
		constexpr void MatrixBase<T, L, L>::Transpose() noexcept
		{
			auto& self = static_cast<Matrix<T, L, L>&>(*this);
			for (size_t i=0; i<L; ++i) for (size_t j=0; j<L; ++j)
			{
				using std::swap;
				swap(self[j][i], self[i][j]);
			}
		}

		template <std::floating_point T>
		constexpr Matrix<T, 3, 3> MatrixGeometry<T, 3, 3>::SimpleViewProj(const Vector<T, 2>& scr) noexcept
		{
			Matrix<T, 3, 3> proj;
			proj[0][0] = 2 / scr.x;
			proj[1][1] = 2 / scr.y;
			proj[2][2] = 1;
			return proj;
		}

		template <std::floating_point T>
		constexpr Matrix<T, 4, 4> MatrixGeometry<T, 4, 4>::Translation(const Vector<T, 3>& pos) noexcept
		{
			auto t = Mat4::Identity();
			t[3] << pos.x << pos.y << pos.z;
			return t;
		}

		template <std::floating_point T>
		constexpr Matrix<T, 4, 4> MatrixGeometry<T, 4, 4>::Rotation(const Quaternion<T>& rot) noexcept
		{
			const auto& [x, y, z, w] = rot.data;
			
			return {
				1 - 2*(y*y + z*z),	2*(x*y + w*z),		2*(x*z - w*y),		0,
				2*(x*y - w*z),		1 - 2*(x*x + z*z),	2*(y*z + w*x),		0,
				2*(x*z + w*y),		2*(y*z - w*x),		1 - 2*(x*x + y*y),	0,
				0,					0,					0,					1
			};
		}

		template <std::floating_point T>
		constexpr Matrix<T, 4, 4> MatrixGeometry<T, 4, 4>::Scale(const Vector<T, 3>& scale) noexcept
		{
			auto s = Mat4::Identity();
			s[0][0] = scale.x;
			s[1][1] = scale.y;
			s[2][2] = scale.z;
			return s;
		}

		template <std::floating_point T>
		Matrix<T, 4, 4> MatrixGeometry<T, 4, 4>::LookAt(const Vector<T, 3>& eye, const Vector<T, 3>& target, const Vector<T, 3>& up)
		{
			auto k = target - eye; k.Normalize();
			auto i = up ^ k; i.Normalize();
			auto j = k ^ i; j.Normalize();
			Vec3 t{eye|i, eye|j, eye|k}; t.Negate();

			return {
				i.x, j.x, k.x, 0,
				i.y, j.y, k.y, 0,
				i.z, j.z, k.z, 0,
				t.x, t.y, t.z, 1
			};
		}
	}
}
