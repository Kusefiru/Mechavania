#ifndef MECHA_VECTOR_HPP
#define MECHA_VECTOR_HPP

template <class T>
class Vector2 {
	public:
		Vector2() : x(T), y(T) {}
		Vector2(T x, T y) : x(x), y(y) {}

		inline Vector2<T> operator+(const Vector2<T>& v2) { return Vector2(x + v2.x, y + v2.y); }
		inline Vector2<T> operator-(const Vector2<T>& v2) { return Vector2(x - v2.x, y - v2.y); }

		T x, y;
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

#endif