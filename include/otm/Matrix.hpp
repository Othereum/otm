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
			static constexpr Matrix<T, L, L> Identity() noexcept;

			// Matrix that assigned other matrix to the identity matrix
			template <class T2, size_t R2, size_t C2>
			static constexpr Matrix<T, L, L> Identity(const Matrix<T2, R2, C2>& other,
			                                          const Vector<ptrdiff_t, 2>& offset = {}) noexcept;
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
		using value_type = Vector<T, C>;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = Vector<T, C>&;
		using const_reference = const Vector<T, C>&;
		using pointer = Vector<T, C>*;
		using const_pointer = const Vector<T, C>*;
		using iterator = Vector<T, C>*;
		using const_iterator = const Vector<T, C>*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		
		constexpr Matrix() noexcept = default;

		constexpr Matrix(All, T x) noexcept
		{
			for (auto& v : varr) for (auto& c : v) c = x;
		}

		template <class T2, size_t R2, size_t C2>
		explicit constexpr Matrix(const Matrix<T2, R2, C2>& other, const Vector<ptrdiff_t, 2>& offset = {})
		{
			Assign(other);
		}

		template <class... Args>
		explicit(sizeof...(Args) == 0)
		constexpr Matrix(T x, Args... args) noexcept
		{
			static_assert(sizeof...(Args) < R*C, "Too many arguments");
			Assign({x, static_cast<T>(args)...});
		}

		/**
		 * \brief Assign elements of other matrix to this. The value of the unassigned elements does not change.
		 * \note Does nothing if offset is out of range
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

		// Assign elements from initializer list. The value of the unassigned elements does not change.
		constexpr void Assign(std::initializer_list<T> list) noexcept
		{
			auto it = list.begin();
			for (size_t i=0; i<R && it != list.end(); ++i)
				for (size_t j=0; j<C && it != list.end(); ++j)
					varr[i][j] = *it++;
		}
		
		constexpr bool operator==(const Matrix& b) const noexcept
		{
			for (size_t i=0; i<R; ++i)
				if (Row(i) != b[i]) return false;

			return true;
		}

		constexpr auto& operator[](size_t i) noexcept { return varr[i]; }
		constexpr auto& operator[](size_t i) const noexcept { return varr[i]; }
		constexpr auto& Row(size_t i) noexcept { return varr[i]; }
		[[nodiscard]] constexpr auto& Row(size_t i) const noexcept { return varr[i]; }

		[[nodiscard]] constexpr auto Col(size_t c) const noexcept
		{
			Vector<T, R> v;
			
			for (size_t r = 0; r < R; ++r)
				v[r] = varr[r][c];
			
			return v;
		}

		constexpr void ColAssign(size_t c, const Vector<T, R>& v) noexcept
		{
			for (size_t r = 0; r < R; ++r)
				varr[r][c] = v[r];
		}

		[[nodiscard]] constexpr auto& AsVectors() noexcept { return varr; }
		[[nodiscard]] constexpr auto& AsVectors() const noexcept { return varr; }
		[[nodiscard]] auto& AsFlatArr() noexcept { return reinterpret_cast<T(&)[R*C]>(varr); }
		[[nodiscard]] auto& AsFlatArr() const noexcept { return reinterpret_cast<const T(&)[R*C]>(varr); }

		constexpr Matrix operator+(const Matrix& b) const noexcept
		{
			auto c = *this;
			return c += b;
		}

		constexpr Matrix& operator+=(const Matrix& b) noexcept
		{
			for (auto i = 0; i < R; ++i) varr[i] += b[i];
			return *this;
		}

		constexpr Matrix operator-(const Matrix& b) const noexcept
		{
			auto c = *this;
			return c -= b;
		}

		constexpr Matrix& operator-=(const Matrix& b) noexcept
		{
			for (auto i = 0; i < R; ++i) varr[i] -= b[i];
			return *this;
		}

		constexpr Matrix operator*(T f) const noexcept
		{
			auto c = *this;
			return c *= f;
		}

		constexpr Matrix& operator*=(T f) noexcept
		{
			for (auto i = 0; i < R; ++i) varr[i] *= f;
			return *this;
		}

		constexpr Matrix operator/(T f) const noexcept
		{
			auto c = *this;
			return c /= f;
		}

		constexpr Matrix& operator/=(T f) noexcept
		{
			for (auto i = 0; i < R; ++i) varr[i] /= f;
			return *this;
		}

		template <class T2, size_t C2>
		constexpr Matrix<std::common_type_t<T, T2>, R, C2> operator*(const Matrix<T2, C, C2>& b) const noexcept
		{
			Matrix<std::common_type_t<T, T2>, R, C2> c;
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
					t[j][i] = varr[i][j];
			return t;
		}

		[[nodiscard]] constexpr iterator begin() noexcept { return varr; }
		[[nodiscard]] constexpr const_iterator begin() const noexcept { return varr; }
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept { return varr; }

		[[nodiscard]] constexpr iterator end() noexcept { return varr + R; }
		[[nodiscard]] constexpr const_iterator end() const noexcept { return varr + R; }
		[[nodiscard]] constexpr const_iterator cend() const noexcept { return varr + R; }
		
		[[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
		[[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{end()}; }
		[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{cend()}; }

		[[nodiscard]] constexpr reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }
		[[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator{begin()}; }
		[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{cbegin()}; }

	private:
		Vector<T, C> varr[R];
	};

	template <class T, size_t R, size_t C, std::convertible_to<T> F>
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

		template <class T, size_t L>
		constexpr Matrix<T, L, L> MatrixBase<T, L, L>::Identity() noexcept
		{
			Matrix<T, L, L> matrix;
			for (size_t i = 0; i < L; ++i)
				matrix[i][i] = 1;
			return matrix;
		}

		template <class T, size_t L>
		template <class T2, size_t R2, size_t C2>
		constexpr Matrix<T, L, L> MatrixBase<T, L, L>::Identity(const Matrix<T2, R2, C2>& other, const Vector<ptrdiff_t, 2>& offset) noexcept
		{
			auto m = Identity();
			m.Assign(other, offset);
			return m;
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
	
	template <class T, size_t L>
	const Matrix<T, 1, L>& Vector<T, L>::RowMatrix() const noexcept
	{
		return reinterpret_cast<const Matrix<T, 1, L>&>(*this);
	}

	template <class T, size_t L>
	const Matrix<T, L, 1>& Vector<T, L>::ColMatrix() const noexcept
	{
		return reinterpret_cast<const Matrix<T, L, 1>&>(*this);
	}
}
