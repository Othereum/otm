# otm : Othereum's Math Library

Math library for 3D programming.

## Key features

* Lightweight, Header-only
* Modern (**C++20**)
* Generic
* constexpr

## Includes

* `Vector`, `Matrix` for linear algebra
* `Quaternion`, `Transform` for geometry
* `Angle`: No more confusing between radians and degrees
* Additional math functions

## Examples

### Vector

#### Creating

```cpp
// using Vec3 = Vector<float, 3>
Vec3 v1; // All components are initialized to 0
Vec3 v2{2, 3}; // (2, 3, 0)
// Vec3 v2{2, 3, 4, 5}; // ERROR: Too many arguments
Vector vv{1, 2llu, 2.3f, 4.5}; // Vector<double, 4>

Vec4 v3{v2}; // (2, 3, 0, 0)
Vec4 v4{v2, 1}; // (2, 3, 0, 1)
Vec2 v5{v4}; // (2, 3)

Vector<float, 10> v6{All{}, 3.14f}; // Set all components to 3.14
constexpr auto v7 = Vec3::One(); // (1, 1, 1)

Vec4 v8;
v8 << 1 << 2 << 3 << 4;

float x, y, z, w;
v8 >> x >> y >> z >> w;

std::cin >> v8;
std::cout << v8 << std::endl;
```

#### Operations

```cpp
v1.Len(); // sqrt(v1.LenSqr())
v1.Dist(v2); // sqrt(v1.DistSqr(v2))

v1.y = v2[1]; // x, y, z, w are only available for Vector<T, 1...4>

v1 *= 3; // multiply 3 to all components
v1 /= 2;

v1 *= v2; // multiply each component
v1 / v2;
v1 + v2;
v1 -= v2;

v1 | v2; // dot product
v1 ^ v2; // cross product. Only available for Vector<T, 3>

for (float x : v1) { ... }
v1.Transform([](float x) { return x + 1; });
v2.Transform(v2, std::plus<>{});

// I don't know if you need this, but it works anyway.
std::sort(v1.begin(), v1.end());
```

#### UnitVec

otm has special vector type `UnitVec`.

```cpp
void foo(const Vec3& direction);
```

can be changed to:

```cpp
void foo(const UVec3& direction);
```

Now foo() accepts only unit vectors:

```cpp
foo(v1.Unit());
```

`UnitVec` can be converted to `Vector` implicitly or explicitly by `.Get()`

```cpp
float s = direction.Get() | v2;
Vec3 v = direction;
```

### Matrix

```cpp
// using Mat4 = Matrix<float, 4, 4>;
Mat4 m1; // All elements are set to 0
Mat3 m2{
    1, 2, 3,
    4, 5, 6,
    7, 8//, 0
};

Mat4 m3 = Mat4::Identity(); // Only available for square matrix
m3[3] << 2 << 3 << 4;
/*
1, 0, 0, 0,
0, 1, 0, 0,
0, 0, 1, 0,
2, 3, 4, 1
*/

m3[2][3] = 14;
std::cout << m3 << std::endl;

Matrix<float, 2, 4> a;
Matrix<double, 4, 2> b;
Matrix<double, 2, 2> c = a * b;
c *= Mat2{}; // Only available for square matrix

Matrix<float, 4, 2> at = a.Transposed();
c.Transpose(); // Only available for square matrix

Vec3 pos{2, 4, 5};
Matrix<float, 1, 4> rv = pos.RowMatrix();
Matrix<float, 4, 1> cv = pos.ColMatrix();

Mat4 t1 = Mat4::Identity(rv, /*offset=*/{0, 3});
/* offset is optional
1, 0, 0, 0,
0, 1, 0, 0,
0, 0, 1, 0,
2, 4, 5, 1
*/

t1.Assign(m2, /*offset=*/{2, -1});
/* offset is optional
1, 0, 4, 5,
0, 1, 7, 8,
0, 0, 1, 0,
2, 4, 5, 1
*/
```
