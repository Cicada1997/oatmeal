#include "Vector.hh"

// Compound-assignment implementations

Vector& Vector::operator+=(const Vector& rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vector& Vector::operator-=(const Vector& rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector& Vector::operator*=(const Vector& rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

Vector& Vector::operator/=(const Vector& rhs) noexcept {
    x /= rhs.x; // no check for zero: caller’s responsibility
    y /= rhs.y;
    return *this;
}

Vector& Vector::operator*=(double scalar) noexcept {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector& Vector::operator/=(double scalar) noexcept {
    x /= scalar; // no check for zero
    y /= scalar;
    return *this;
}

// Binary operators delegating to compound versions

Vector Vector::operator+(const Vector& rhs) const noexcept {
    Vector tmp = *this;
    tmp += rhs;
    return tmp;
}

Vector Vector::operator-(const Vector& rhs) const noexcept {
    Vector tmp = *this;
    tmp -= rhs;
    return tmp;
}

Vector Vector::operator*(const Vector& rhs) const noexcept {
    Vector tmp = *this;
    tmp *= rhs;
    return tmp;
}

Vector Vector::operator/(const Vector& rhs) const noexcept {
    Vector tmp = *this;
    tmp /= rhs;
    return tmp;
}

Vector Vector::operator*(double scalar) const noexcept {
    Vector tmp = *this;
    tmp *= scalar;
    return tmp;
}

Vector Vector::operator/(double scalar) const noexcept {
    Vector tmp = *this;
    tmp /= scalar;
    return tmp;
}

// Non-member scalar-first overloads

Vector operator*(double scalar, const Vector& v) noexcept {
    return v * scalar;  // uses Coordinate::operator*(double)
}

Vector operator/(double scalar, const Vector& v) noexcept {
    // If you want (scalar / each component):
    return Vector(scalar / v.x, scalar / v.y);
}


