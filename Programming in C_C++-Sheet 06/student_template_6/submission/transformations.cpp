#include "transformations.h"
#include "point3d.h"
#include <algorithm>
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
        std::pair<float,float> rotateMinMax = rotate2D(min.y, max.z, angle);
        std::pair<float,float> rotateMaxMin = rotate2D(max.y, min.z, angle);
        std::pair<float,float> rotateMax = rotate2D(max.y, max.z, angle);

        float minY = std::min(std::min(rotateMin.second,rotateMax.second), 
                                std::min(rotateMinMax.second, rotateMaxMin.second));
        float minZ = std::min(std::min(rotateMin.first,rotateMax.first), 
                                std::min(rotateMinMax.first, rotateMaxMin.first));
        bounds.min = Point3D(min.x, minY, minZ);

        float maxY = std::max(std::max(rotateMin.second,rotateMax.second), 
                            std::max(rotateMinMax.second, rotateMaxMin.second));
        float maxZ = std::max(std::max(rotateMin.first,rotateMax.first),
                            std::max(rotateMinMax.first, rotateMaxMin.first));
        bounds.max = Point3D(max.x, maxY, maxZ);
    }
    else if (axis == Axis::Y){
        std::pair<float,float> rotateMin = rotate2D(min.z, min.x, angle);
        std::pair<float,float> rotateMinMax = rotate2D(min.z, max.x, angle);
        std::pair<float,float> rotateMaxMin = rotate2D(max.z, min.x, angle);
        std::pair<float,float> rotateMax = rotate2D(max.z, max.x, angle);

        float minZ = std::min(std::min(rotateMin.second,rotateMax.second), 
                                std::min(rotateMinMax.second, rotateMaxMin.second));
        float minX = std::min(std::min(rotateMin.first,rotateMax.first), 
                                std::min(rotateMinMax.first, rotateMaxMin.first));
        bounds.min = Point3D(minX, min.y, minZ);

        float maxZ = std::max(std::max(rotateMin.second,rotateMax.second), 
                                std::max(rotateMinMax.second, rotateMaxMin.second));
        float maxX = std::max(std::max(rotateMin.first,rotateMax.first), 
                                std::max(rotateMinMax.first, rotateMaxMin.first));
        bounds.max = Point3D(maxX, max.y, maxZ);
    }
    else {
        std::pair<float,float> rotateMin = rotate2D(min.x, min.y, angle);
        std::pair<float,float> rotateMinMax = rotate2D(min.x, max.y, angle);
        std::pair<float,float> rotateMaxMin = rotate2D(max.x, min.y, angle);
        std::pair<float,float> rotateMax = rotate2D(max.x, max.y, angle);

        float minX = std::min(std::min(rotateMin.second,rotateMax.second), 
                                std::min(rotateMinMax.second, rotateMaxMin.second));
        float minY = std::min(std::min(rotateMin.first,rotateMax.first), 
                                std::min(rotateMinMax.first, rotateMaxMin.first));
        bounds.min = Point3D(minX, minY, min.z);

        float maxX = std::max(std::max(rotateMin.second,rotateMax.second), 
                                std::max(rotateMinMax.second, rotateMaxMin.second));
        float maxY = std::max(std::max(rotateMin.first,rotateMax.first), 
                                std::max(rotateMinMax.first, rotateMaxMin.first));
        bounds.max = Point3D(maxX, maxY, max.z);
    }
    return bounds;
}

bool Rotated::isInside_impl(const Point3D& p) const {
    Point3D pRotated = p;
    float negAngle = -angle;
    if (axis == Axis::X){
        std::pair<float,float> rotate = rotate2D(p.y, p.z, negAngle);
        pRotated = Point3D(p.x, rotate.second, rotate.first);
    }
    else if(axis == Axis::Y){
        std::pair<float,float> rotate = rotate2D(p.z, p.x, negAngle);
        pRotated = Point3D(rotate.first, p.y, rotate.second);
    }
    else {
        std::pair<float,float> rotate = rotate2D(p.x, p.y, negAngle);
        pRotated = Point3D(rotate.second, rotate.first, p.z);
    }
    return sub_shape.isInside(pRotated);
}

Shape Shape::rotated(Axis axis, float angle) const {
    return Rotated(*this, axis, angle).clone_impl();
}