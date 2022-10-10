#include "transformations.h"
#include "point3d.h"
#include <cmath>

// helper function for 2D rotations
std::pair<float, float> rotate2D(float x, float y, float angle)
{
    float sin, cos;
    sincosf(angle, &sin, &cos); // in Mac OSX sincosf may not be found, try __sincosf. But you need to use sincosf for submission.

    return {cos*x-sin*y, sin*x+cos*y};
}


Transformation::Transformation(const Shape& shape)
    : sub_shape{shape.clone()}
{

}

//Scaled 
Scaled::Scaled(const Shape& shape, const Point3D& s_)
    : Transformation{shape}, s{s_}{};

Shape Scaled::clone_impl() const{
    return {std::make_shared<Scaled>(sub_shape, s)};
}

AABB Scaled::getBounds_impl() const {
    AABB bounds = sub_shape.getBounds();
    bounds.min *= s;
    bounds.max *= s;
    return bounds;
}

bool Scaled::isInside_impl(const Point3D& p) const {
    return sub_shape.isInside(p / s);
}

Shape Shape::scaled(Point3D factor) const{
    return Scaled(*this, factor).clone_impl();
}

//Translated
Translated::Translated(const Shape& shape, const Point3D& t_)
    :Transformation{shape}
    {
        t = t_;
    }

Shape Translated::clone_impl() const{
    return {std::make_shared<Translated>(sub_shape, t)};
}

AABB Translated::getBounds_impl() const {
    AABB bounds = sub_shape.getBounds();
    bounds.min += t;
    bounds.max += t;
    return bounds;
}

bool Translated::isInside_impl(const Point3D& p) const {
    Point3D pTranslated = p - t;
    return sub_shape.isInside(pTranslated);
}

Shape Shape::translated(Point3D offset) const {
    return Translated(*this, offset).clone_impl();
}

//Rotation
Rotated::Rotated(const Shape& shape, const Axis& axis_, const float& angle_)
    :Transformation{shape}
    {
        angle = angle_; 
        axis = axis_;
    }

Shape Rotated::clone_impl() const{
    return {std::make_shared<Rotated>(sub_shape, axis, angle)};
}

AABB Rotated::getBounds_impl() const {
    AABB bounds = sub_shape.getBounds();
    Point3D min = bounds.min;
    Point3D max = bounds.max;
    if (axis == Axis::X){
        std::pair<float,float> rotateMin = rotate2D(min.y, min.z, angle);
        bounds.min = Point3D(min.x, rotateMin.first, rotateMin.second);

        std::pair<float,float> rotateMax = rotate2D(max.y, max.z, angle);
        bounds.max = Point3D(max.x, rotateMax.first, rotateMax.second);
    }
    else if (axis == Axis::Y){
        std::pair<float,float> rotateMin = rotate2D(min.z, min.x, angle);
        bounds.min = Point3D(rotateMin.second, min.y, rotateMin.first);

        std::pair<float,float> rotateMax = rotate2D(max.z, max.x, angle);
        bounds.max = Point3D(rotateMax.second, max.y, rotateMax.first);
    }
    else {
        std::pair<float,float> rotateMin = rotate2D(min.x, min.y, angle);
        bounds.min = Point3D(rotateMin.first, rotateMin.second, min.z);

        std::pair<float,float> rotateMax = rotate2D(max.x, max.y, angle);
        bounds.max = Point3D(rotateMax.first, rotateMax.second, max.z);
    }
    return bounds;
}

bool Rotated::isInside_impl(const Point3D& p) const {
    Point3D pRotated = Point3D(1.0f);
    float negAngle = -angle;
    if (axis == Axis::X){
        std::pair<float,float> rotate = rotate2D(p.y, p.z, negAngle);
        pRotated = Point3D(p.x, rotate.first, rotate.second);
    }
    else if(axis == Axis::Y){
        std::pair<float,float> rotate = rotate2D(p.z, p.x, negAngle);
        pRotated = Point3D(rotate.second, p.y, rotate.first);
    }
    else {
        std::pair<float,float> rotate = rotate2D(p.x, p.y, negAngle);
        pRotated = Point3D(rotate.first, rotate.second, p.z);
    }
    return sub_shape.isInside(pRotated);
}

Shape Shape::rotated(Axis axis, float angle) const {
    return Rotated(*this, axis, angle).clone_impl();
}