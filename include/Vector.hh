#ifndef COORDINATE_H
#define COORDINATE_H

class Vector 
{
    public:
        double x, y;

        // Constructor
        Vector(double x_ = 0.0, double y_ = 0.0) noexcept
            : x(x_), y(y_) {}

        // Compound-assignment operators
        Vector& operator+=(const Vector& rhs) noexcept;
        Vector& operator-=(const Vector& rhs) noexcept;
        Vector& operator*=(const Vector& rhs) noexcept; // component-wise
        Vector& operator/=(const Vector& rhs) noexcept; // component-wise

        Vector& operator*=(double scalar) noexcept;
        Vector& operator/=(double scalar) noexcept;

        // Binary arithmetic operators (non-mutating)
        Vector operator+(const Vector& rhs) const noexcept;
        Vector operator-(const Vector& rhs) const noexcept;
        Vector operator*(const Vector& rhs) const noexcept; // component-wise
        Vector operator/(const Vector& rhs) const noexcept; // component-wise

        Vector operator*(double scalar) const noexcept;
        Vector operator/(double scalar) const noexcept;

        // Unary operators
        Vector operator+() const noexcept { return *this; }
        Vector operator-() const noexcept { return Vector(-x, -y); }
};

// Non-member overloads for scalar first
Vector operator*(double scalar, const Vector& v) noexcept;
Vector operator/(double scalar, const Vector& v) noexcept; // optionally, component-wise

#endif // COORDINATE_H

