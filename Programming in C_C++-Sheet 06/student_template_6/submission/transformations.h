#pragma once

#include "shapes.h"

#include <memory>

/// shared abstract transformation base class containing a nested shape to be transformed
class Transformation : public Shape {
protected:
    Transformation(const Shape& shape);
public:
    Shape sub_shape;
};

// of course, one could implement all these transformations jointly as a single transformation matrix.
// for simplicity, we don't do that here.

class Scaled : public Transformation {
    public:
        Point3D s;
        Shape clone_impl() const override;
        AABB getBounds_impl() const override;
        bool isInside_impl(const Point3D& p) const override;
        Scaled(const Shape& shape, const Point3D& s);
};

class Translated : public Transformation {
    public:
        Point3D t;

        Translated(const Shape& shape, const Point3D& s);
        Shape clone_impl() const override;
        AABB getBounds_impl() const override;
        bool isInside_impl(const Point3D& p) const override;
};

class Rotated : public Transformation {
    public:
        float angle;
        Axis axis;

        Shape clone_impl() const override;
        AABB getBounds_impl() const override;
        bool isInside_impl(const Point3D& p) const override;

        Rotated(const Shape& shape, const Axis& axis, const float& angle);
};