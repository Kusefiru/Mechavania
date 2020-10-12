#ifndef MECHA_VECTOR_HPP
#define MECHA_VECTOR_HPP

/**
 * 2 sized vector implementation with operator overloads
 * Inspired by the SFML implementation which contains the following notice:
 */

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2020 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

template <typename T>
class Vector2 {
	public:
		Vector2() : x(0), y(0) {}
		Vector2(const T& x, const T& y) : x(x), y(y) {}
		Vector2(const Vector2<T>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
		
		inline explicit operator Vector2<int>() const { return Vector2<int>(static_cast<int>(x), static_cast<int>(y)); }

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

#endif // MECHA_VECTOR_HPP