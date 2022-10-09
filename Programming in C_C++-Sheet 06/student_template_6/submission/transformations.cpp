#include "transformations.h"
#include <cmath>

// helper function for 2D rotations
std::pair<float, float> rotate2D(float x, float y, float angle)
{
    float sin, cos;
    sincosf(angle, &sin, &cos); // in Mac OSX sincosf may not be found, try __sincosf. But you need to use sincosf for submission.

    return {cos*x-sin*y, sin*x+cos*y};
}


Transformation::Transformation(const Shape& shape)
    : sub_shape{shape.clone()}{};

// Scaled derives from Transformation, which derives from Shape
Scaled::Scaled(const Shape& shape, const Point3D& s_)
    :Transformation(shape){
        this->s = s_;
    }

Shape Scaled::clone_impl() const{
    return {std::make_shared<Scaled>(this->sub_shape, this->s)};
}

AABB Scaled::getBounds_impl() const {
    this->getBounds().min = this->getBounds().min * this->s;
    this->getBounds().max = this->getBounds().max * this->s;
    return this->getBounds();
}

bool Scaled::isInside_impl(const Point3D& p) const {
    Point3D pScaled = p / this->s;
    return this->isInside(pScaled);
}

Shape Shape::scaled(Point3D factor) const{
    return Scaled(*this, factor).clone_impl();
}

//Translated
Translated::Translated(const Shape& shape, const Point3D& t_)
    :Transformation(shape){
        t = t_;
    }

Shape Translated::clone_impl() const{
    return {std::make_shared<Translated>(this->sub_shape, this->t)};
}

AABB Translated::getBounds_impl() const {
    //this->getBounds().min.operator+=(this->t);
    //this->getBounds().max.operator+=(this->t);
    this->getBounds().min += this->t;
    this->getBounds().max += this->t;
    return this->getBounds();
}

bool Translated::isInside_impl(const Point3D& p) const {
    Point3D pTranslated = p - this->t;
    return this->isInside(pTranslated);
}

Shape Shape::translated(Point3D offset) const {
    return Translated(*this, offset).clone_impl();
}

//Rotation
Rotated::Rotated(const Shape& shape, const Axis& axis_, const float& angle_)
    :Transformation(shape){
        axis = axis_;
        angle = angle_;
    }

Shape Rotated::clone_impl() const{
    return {std::make_shared<Rotated>(this->sub_shape, this->axis, this->angle)};
}

AABB Rotated::getBounds_impl() const {
    Point3D min = this->getBounds().min;
    Point3D max = this->getBounds().max;
    if (this->axis == Axis::X){
        std::pair<float,float> rotateMin = rotate2D(min.y, min.z, this->angle);
        this->getBounds().min = Point3D(min.x, rotateMin.first, rotateMin.second);

        std::pair<float,float> rotateMax = rotate2D(max.y, max.z, this->angle);
        this->getBounds().max = Point3D(max.x, rotateMax.first, rotateMax.second);
    }
    else if (this->axis == Axis::Y){
        std::pair<float,float> rotateMin = rotate2D(min.z, min.x, this->angle);
        this->getBounds().min = Point3D(rotateMin.second, min.y, rotateMin.first);

        std::pair<float,float> rotateMax = rotate2D(max.z, max.x, this->angle);
        this->getBounds().max = Point3D(rotateMax.second, max.y, rotateMax.first);
    }
    else {
        std::pair<float,float> rotateMin = rotate2D(min.x, min.y, this->angle);
        this->getBounds().min = Point3D(rotateMin.first, rotateMin.second, min.z);

        std::pair<float,float> rotateMax = rotate2D(max.x, max.y, this->angle);
        this->getBounds().max = Point3D(rotateMax.first, rotateMax.second, max.z);
    }
    return this->getBounds();
}

bool Rotated::isInside_impl(const Point3D& p) const {
    Point3D pRotated = Point3D(1.0f);
    float negAngle = -angle;
    if (this->axis == Axis::X){
        std::pair<float,float> rotate = rotate2D(p.y, p.z, negAngle);
        pRotated = Point3D(p.x, rotate.first, rotate.second);
    }
    else if(this->axis == Axis::Y){
        std::pair<float,float> rotate = rotate2D(p.z, p.x, negAngle);
        pRotated = Point3D(rotate.second, p.y, rotate.first);
    }
    else {
        std::pair<float,float> rotate = rotate2D(p.x, p.y, negAngle);
        pRotated = Point3D(rotate.first, rotate.second, p.z);
    }
    return this->isInside(pRotated);
}

Shape Shape::rotated(Axis axis, float angle) const {
    return Rotated(*this, axis, angle).clone_impl();
}