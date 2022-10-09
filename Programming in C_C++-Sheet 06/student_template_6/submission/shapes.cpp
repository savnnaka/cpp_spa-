#include "shapes.h"

#include "transformations.h"
#include "operations.h"

#include <math.h>
#include <memory>
#include <stdexcept>

Shape::Shape(std::shared_ptr<Shape>&& shape) noexcept
    : instance{std::move(shape)} // take ownership
{

}

Shape Shape::clone() const
{
    // if this shape just contains a pointer, we can return a simple copy
    if (instance)
        return *this;

    // otherwise, call the derived clone implementation
    return clone_impl();
}

AABB Shape::getBounds() const {
    // call the nested shape (if any)
    if (instance)
        return instance->getBounds();

    // otherwise, call the derived getBounds implementation
    return getBounds_impl();
}

bool Shape::isInside(const Point3D& p) const
{
    // call the nested shape (if any)
    if (instance)
        return instance->isInside(p);

    // otherwise, call the derived isInside implementation
    return isInside_impl(p);
}

Shape Shape::clone_impl() const
{
    // no default implementation available (but cannot set = 0, since we want to have instances of Shape)
    // if you get this error, you forgot to implement the override
    throw std::logic_error("clone called on an abstract shape");
}

AABB Shape::getBounds_impl() const
{
    // fallback default implementation
    return AABB{-1.0f, 1.0f};
}

bool Shape::isInside_impl(const Point3D&) const
{
    // no fallback implementation (but cannot set = 0, since we want to have instances of Shape)
    // if you get this error, you forgot to implement the override
    throw std::logic_error("isInside called on an abstract shape");
}

Shape Empty::clone_impl() const
{
    return {std::make_shared<Empty>()};
}

bool Empty::isInside_impl(const Point3D&) const {
    return false;
}

AABB Empty::getBounds_impl() const
{
    return AABB{};
}

Shape Cube::clone_impl() const
{
    std::shared_ptr<Shape> shapeptr = std::make_shared<Cube>();
    return shapeptr;
}

bool Cube::isInside_impl(const Point3D& p) const {
    return getBounds().contains(p);
}

Shape Sphere::clone_impl() const
{
    return {std::make_shared<Sphere>()}; //!!
}

bool Sphere::isInside_impl(const Point3D& p) const 
{
    float innerSum = powf(p.x, 2.0f) + powf(p.y, 2) + powf(p.z, 2);
    return pow(innerSum, 0.5) <= 1;
}

Shape Cylinder::clone_impl() const
{
    return {std::make_shared<Cylinder>()}; //??
}

bool Cylinder::isInside_impl(const Point3D& p) const 
{
    float innerSum = powf(p.x, 2) + powf(p.y, 2);
    float squareRoot = powf(innerSum, 0.5);

    return (squareRoot <= 1.0f) && (-1.0f <= p.z) && (p.z <= 1.0f);
}

Shape Octahedron::clone_impl() const
{
    //Shape* oc = new Octahedron(*this);
    return {std::make_shared<Octahedron>()}; //??
}

bool Octahedron::isInside_impl(const Point3D& p) const 
{   // sum of absolutes smaller 1
    return (abs(p.x) + abs(p.y) + abs(p.z)) <= 1.0f;
}

  // operators
Shape Shape::operator&(const Shape& other) const
{ //and
    // sub shape und isInside_impl() define new shape
    // and objekt erstellen
    And neu = And(*this, other);
    // return a Shape clone, not an AND
    return neu.clone_impl();
};

// or
Shape Shape::operator|(const Shape& other) const{
    Or neu = Or(*this, other);
    return neu.clone_impl();
};

Shape Shape::operator^(const Shape& other) const
{
    Xor neu = Xor(*this, other);
    return neu.clone_impl();

};
Shape Shape::operator!() const
{ //not
    Not neu = Not(*this);
    return neu.clone_impl();
};

Shape Shape::operator+(const Shape& other) const
{
    return Shape::operator^(other).Shape::operator|(Shape::operator&(other));
};

Shape Shape::operator-(const Shape& other) const
{
    return Shape::operator&(other.Shape::operator!());
};