#include "voxel_grid.h"

#include "point3d.h"
#include "transformations.h"
#include "operations.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <sys/types.h>

VoxelGrid::VoxelGrid(const Shape& shape)
{
    Point3D extents = shape.getBounds().extents();
    Point3D resolution = extents * this->level_of_detail;
    uint32_t resX = static_cast<uint32_t>(resolution.x);
    uint32_t resY = static_cast<uint32_t>(resolution.y);
    uint32_t resZ = static_cast<uint32_t>(resolution.z);
    if (!std::isinf(extents.x))
        this->res_x = resX >= 1 ? resX : 1;
    else throw std::logic_error("task 6.4 b)"); 
    if (!std::isinf(extents.y))
        this->res_y = resY >= 1 ? resY : 1;
    else throw std::logic_error("task 6.4 b)"); 
    if (!std::isinf(extents.z))
        this->res_z = resZ >= 1 ? resZ : 1;
    else throw std::logic_error("task 6.4 b)"); 
    this->voxels.resize(resX*resY*resZ+resY*resZ+resZ);
    for (uint32_t x = 0; x < resX; x++){
        for (uint32_t y = 0; y < resY; y++) {
            for (uint32_t z = 0; z < resZ; z++){
                if (shape.isInside(voxelCenter(x,y,z)))
                    voxels[x*this->res_y*this->res_z+y*this->res_z+z] = true;
                else voxels[x*this->res_y*this->res_z+y*this->res_z+z] = false;
            }
        }
    }
}

std::tuple<uint32_t, uint32_t, uint32_t> VoxelGrid::getResolution() const
{
    return {res_x, res_y, res_z};
}

VoxelSlice VoxelGrid::extractSlice(Axis axis, uint32_t slice) const
{
    if (Axis::Z == axis) {
        VoxelSlice Slice = VoxelSlice(res_y, res_x);
        for (uint32_t x = 0; x < this->res_x; x++){
            for (uint32_t y = 0; y < this->res_y; y++){
                Slice.data[y][x] = isSet(x, y, slice);
            }
        }
        return Slice;
    }
    else if (Axis::X == axis) {
        VoxelSlice Slice = VoxelSlice(res_z, res_y);
        for (uint32_t y = 0; y < this->res_y; y++){
            for (uint32_t z = 0; z < this->res_z; z++){
                Slice.data[z][y] = isSet(slice, y, z);
            }
        }
        return Slice;
    }
    else {
        VoxelSlice Slice = VoxelSlice(res_x, res_z);
        for (uint32_t x = 0; x < this->res_x; x++){
            for (uint32_t z = 0; z < this->res_z; z++){
                Slice.data[x][z] = isSet(x, slice, z);
            }
        }
        return Slice;
    }
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
    //throw std::logic_error("task 6.4 f)");
    //(void) p; // silence unused parameter warning
    AABB boundingBox = this->getBounds_impl();
    if(p > boundingBox.max || p < boundingBox.min)
        return false;
    // pVector relativ zum VoxelGrid
    Point3D pVector = p - boundingBox.min;
    uint32_t xCord = static_cast<uint32_t>(pVector.x);
    uint32_t yCord = static_cast<uint32_t>(pVector.x);
    uint32_t zCord = static_cast<uint32_t>(pVector.x);

    return this->isSet(xCord, yCord, zCord);
}

bool VoxelGrid::isSet(uint32_t x, uint32_t y, uint32_t z) const
{
    // When running in debug mode, these will check whether the supplied indices are valid or "trap" to the debugger.
    // When no debugger is running, failing the assertion will terminate the program immediately.
    // When compiled in release mode, assert() does nothing.
    assert(x < res_x);
    assert(y < res_y);
    assert(z < res_z);

    return this->voxels.at(x*res_y+res_z + y*res_z + z);
    }

Point3D VoxelGrid::voxelCenter(uint32_t x, uint32_t y, uint32_t z) const
{
    AABB boundingBox = this->getBounds_impl();
    Point3D extents = boundingBox.extents();
    Point3D stepsize = Point3D(extents.x / static_cast<float>(x), 
                                extents.y / static_cast<float>(y), 
                                extents.z / static_cast<float>(z));
    Point3D center = stepsize * static_cast<float>(+0.5*y+0.5*z+0.5);
    return center + boundingBox.min;
}

std::ostream& operator<<(std::ostream& ostream, const VoxelSlice& slice)
{
    for (uint32_t x=0; x < slice.data.size(); x++){
        for (uint32_t y=0; y < slice.data[x].size(); y++){
            if(slice.data[x][y]) std::cout << "X";
            else std::cout << ".";
            std::cout << " ";
        }
        std::cout << "\n";
    }
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
