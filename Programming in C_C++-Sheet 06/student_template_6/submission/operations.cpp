#include "operations.h"
#include <cstddef>
#include <utility>

Operation::Operation(const Shape& shape_a, const Shape& shape_b)
    : sub_shape_a{shape_a.clone()}, sub_shape_b{shape_b.clone()}
{

}

AABB Operation::getBounds_impl() const
{
    return sub_shape_a.getBounds()+sub_shape_b.getBounds();
}

// AND 

And::And(const Shape& shape_a, const Shape& shape_b)
    : Operation(shape_a, shape_b)   // call constructor of base class
{
    //Operation(std::forward<Shape&&>(std::move(shape_a)), std::forward<Shape&&>(std::move(shape_b)));
    //Operation(std::forward(shape_a), std::forward(shape_b));  
}

Shape And::clone_impl() const
{
    return {std::make_shared<And>(this->sub_shape_a, this->sub_shape_b)};
}

bool And::isInside_impl(const Point3D& p) const 
{   
    return this->sub_shape_a.isInside(p) && this->sub_shape_b.isInside(p);
}

// OR

Or::Or(const Shape& shape_a, const Shape& shape_b)
    : Operation(shape_a, shape_b)    // call constructor of base class
{
     
}

Shape Or::clone_impl() const
{
    return {std::make_shared<Or>(this->sub_shape_a, this->sub_shape_b)};
}

bool Or::isInside_impl(const Point3D& p) const 
{   
    return this->sub_shape_a.isInside(p) || this->sub_shape_b.isInside(p);
}

// XOR

Xor::Xor(const Shape& shape_a, const Shape& shape_b)
    : Operation(shape_a, shape_b)    // call constructXor of base class
{
    // call clone_impl(), ist aber privat   
}

Shape Xor::clone_impl() const
{
    return {std::make_shared<Xor>(this->sub_shape_a, this->sub_shape_b)};
}

bool Xor::isInside_impl(const Point3D& p) const 
{   
    return this->sub_shape_a.isInside(p) ^ this->sub_shape_b.isInside(p);
}

// Not

Not::Not(const Shape& shape_a)
    : Operation(std::move(shape_a), std::move(shape_a))    // call constructNot of base class
{
    // call clone_impl(), ist aber privat   
}

Shape Not::clone_impl() const
{
    return {std::make_shared<Not>(this->sub_shape_a)};
}

bool Not::isInside_impl(const Point3D& p) const 
{   
    return !this->sub_shape_a.isInside(p);
}