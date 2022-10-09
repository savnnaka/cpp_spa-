#include "shapes.h"
#include "transformations.h"
#include "operations.h"
#include "voxel_grid.h"

#include "shapes_test.h"

// example shapes
// TODO: enable this in the CMakelists.txt when you are finished with your implementation to see some example shapes
#ifdef ENABLE_DEMO_CODE
Shape outline(const VoxelGrid& vg) {
    constexpr float voxel_size = 1.0f/VoxelGrid::level_of_detail;
    Shape s {vg.clone()}; // create a clone - this creates a reference that can be used multiple times without copying
    return (s.translated({voxel_size, voxel_size, 0.0f})|s.translated({voxel_size, -voxel_size, 0.0f})|s.translated({-voxel_size, voxel_size, 0.0f})|s.translated({-voxel_size, -voxel_size, 0.0f}))^s;
}

Shape cpp2022() {
    const Shape o    = Cylinder{} - Cylinder{}.scaled({0.5f, 0.5f, 1.0f});
    const Shape c    = o - Cube{}.scaled({0.5f, 1.0f, 1.0f}).translated({0.5f, 0.0f, 0.0f});
    const Shape plus = Cube{}.scaled({0.2f, 0.6f, 1.0f}) + Cube{}.scaled({0.6f, 0.2f, 1.0f});
    const Shape cpp  = c + plus.translated({0.8f, 0.0f, 0.0f}) + plus.translated({2.3f, 0.0f, 0.0f});
    const Shape zero = o.scaled({0.6f, 1.0f, 1.0f});
    const Shape two  = zero - Cube{}.scaled({0.6f, 0.5f, 1.0f}).translated({0.0f, 0.5f, 0.0f}) + Cube{}.scaled({0.6f, 0.15f, 1.0f}).translated({0.0f, 0.85f, 0.0f})
                     + Cube{}.scaled({0.6f, 0.15f, 1.0f}).rotated(Axis::Z, -40.0f*static_cast<float>(M_PI)/180.0f).translated({0.0f, 0.35f, 0.0f})
                     + Cylinder{}.scaled({0.15f, 0.15f, 1.0f}).translated({0.45f, 0.0f, 0.0f}) + Cylinder{}.scaled({0.15f, 0.15f, 1.0f}).translated({-0.45f, 0.75f, 0.0f});
    const Shape one  = Cube{}.scaled({0.15f, 1.0f, 1.0f}).translated({0.15f, 0.0f, 0.0f}) + Cube{}.scaled({0.3f, 0.15f, 1.0f}).rotated(Axis::Z, -27.0f*static_cast<float>(M_PI)/180.0f).translated({-0.1f, -0.75f, 0.0f});

    const Shape twenty_twenty_two = two+zero.translated({1.4f, 0.0f, 0.0f})+two.translated({2.8f, 0.0f, 0.0f})+two.translated({4.2f, 0.0f, 0.0f});

    return (cpp+twenty_twenty_two.translated({3.8f, 0.0f, 0.0f})).scaled({1.0f, 1.0f, 0.1f});
}

// base case for variadic template
Shape shape_list() {
    return Empty{}.clone();
}

// variadic template
// you can create arbitrarily long shape lists with this, you don't have to understand how this works (yet)
#if __cpp_lib_concepts >= 202002L
// only available if concepts are supported by your c++ standard library
template<ShapeFullyImplemented FirstShape, ShapeFullyImplemented... RemainingShapes>
#else
template<typename FirstShape, typename... RemainingShapes>
#endif
Shape shape_list(FirstShape first, RemainingShapes... rest) {
    return first + shape_list(rest...).translated({2.0f, 0.0f, 0.0f});
}

Shape composite_shape() {
    return (((!(Sphere{}-Cube{}.scaled(0.5f).translated(0.5f)))&Cylinder{})+Octahedron{}.scaled(0.5f).translated({0.0f, 0.0f, -0.5f})).rotated(Axis::Z, -45.0f*static_cast<float>(M_PI)/180.0f);
}

Shape example_shape() {
    // return shape_list(Cube{}, Sphere{}, Octahedron{}, Cylinder{});
    // return composite_shape();
    return outline(cpp2022());
}
#else
// fallback example shape (returns a simple cube)
Shape example_shape() {
    return Cube{}.clone();
}
#endif // end example shapes


/// implementation of your custom shape (bonus task)
Shape your_shape() {
    // return whatever shape you like in here - this file does not influence the evaluation of your submission (except potential bonus points)

    return example_shape();
}
