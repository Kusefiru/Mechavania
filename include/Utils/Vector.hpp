#ifndef MECHA_VECTOR_HPP
#define MECHA_VECTOR_HPP

template <typename T>
class Vector2 {
	public:
		Vector2() : x(0), y(0) {}
		Vector2(const T& x, const T& y) : x(x), y(y) {}
		Vector2(const Vector2<T>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

		T x, y;
};

// -v
template<typename T>
inline Vector2<T> operator-(const Vector2<T>& v) { return Vector2<T>(-v.x, -v.y); }

// l + r
template<typename T>
inline Vector2<T> operator+(const Vector2<T>& l, const Vector2<T>& r) { return Vector2<T>(l.x + r.x, l.y + r.y);}

// l += r
template<typename T>
inline Vector2<T>& operator+=(Vector2<T>& l, const Vector2<T>& r) { l.x += r.x; l.y += r.y; return l; }

// l - r
template<typename T>
inline Vector2<T> operator-(const Vector2<T>& l, const Vector2<T>& r) { return Vector2<T>(l.x - r.x, l.y - r.y);}

// l -= r
template<typename T>
inline Vector2<T>& operator-=(Vector2<T>& l, const Vector2<T>& r) { l.x -= r.x; l.y -= r.y; return l; }

// l == r ?
template<typename T>
inline Vector2<T> operator==(const Vector2<T>& l, const Vector2<T>& r) { return l.x == r.x && l.y == r.y; }

// l != r ?
template<typename T>
inline Vector2<T> operator!=(const Vector2<T>& l, const Vector2<T>& r) { return l.x != r.x || l.y != r.y; }

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

#endif