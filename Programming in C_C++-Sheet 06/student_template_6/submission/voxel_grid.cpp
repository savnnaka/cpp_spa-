#include "voxel_grid.h"

#include "transformations.h"
#include "operations.h"

#include <cassert>
#include <iostream>
#include <sstream>

VoxelGrid::VoxelGrid(const Shape& shape)
{
    throw std::logic_error("task 6.4 b)");
    (void) shape; // silence unused parameter warning
}

std::tuple<uint32_t, uint32_t, uint32_t> VoxelGrid::getResolution() const
{
    return {res_x, res_y, res_z};
}

VoxelSlice VoxelGrid::extractSlice(Axis axis, uint32_t slice) const
{
    throw std::logic_error("task 6.4 d)");
    (void) axis, (void) slice; // silence unused parameter warning
}

Shape VoxelGrid::clone_impl() const
{
    return {std::make_shared<VoxelGrid>(*this)};
}

AABB VoxelGrid::getBounds_impl() const
{
    return bounds;
}

bool VoxelGrid::isInside_impl(const Point3D& p) const
{
    throw std::logic_error("task 6.4 f)");
    (void) p; // silence unused parameter warning
}

bool VoxelGrid::isSet(uint32_t x, uint32_t y, uint32_t z) const
{
    // When running in debug mode, these will check whether the supplied indices are valid or "trap" to the debugger.
    // When no debugger is running, failing the assertion will terminate the program immediately.
    // When compiled in release mode, assert() does nothing.
    assert(x < res_x);
    assert(y < res_y);
    assert(z < res_z);

    throw std::logic_error("task 6.4 c)");
}

Point3D VoxelGrid::voxelCenter(uint32_t x, uint32_t y, uint32_t z) const
{
    throw std::logic_error("task 6.4 a)");
    (void) x, (void) y, (void) z; // silence unused parameter warning
}

std::ostream& operator<<(std::ostream& ostream, const VoxelSlice& slice)
{
    throw std::logic_error("task 6.4 e)");
    (void) slice; // silence unused parameter warning

    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const VoxelGrid& vg)
{
    uint32_t res_z;
    std::tie(std::ignore, std::ignore, res_z) = vg.getResolution();

    for (uint32_t z=0; z<res_z; ++z) {
        auto slice = vg.extractSlice(Axis::Z, z);
        ostream << slice << std::endl;
    }
    return ostream;
}
