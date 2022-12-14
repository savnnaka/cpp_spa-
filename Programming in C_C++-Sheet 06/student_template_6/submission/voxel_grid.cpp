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
    bounds = shape.getBounds();
    Point3D extents = shape.getBounds().extents();
    Point3D resolution = extents * level_of_detail;
    res_x = (resolution.x >= 1 && !std::isinf(resolution.x)) ? 
                    static_cast<uint32_t>(resolution.x) : 1;
    res_y = (resolution.y >= 1 && !std::isinf(resolution.y)) ? 
                    static_cast<uint32_t>(resolution.y) : 1;
    res_z = (resolution.z >= 1 && !std::isinf(resolution.z)) ? 
                    static_cast<uint32_t>(resolution.z) : 1;
    voxels.resize(res_x*res_y*res_z-1+res_y*res_z-1+res_z);
    for (uint32_t x = 0; x < res_x; x++){
        for (uint32_t y = 0; y < res_y; y++) {
            for (uint32_t z = 0; z < res_z; z++){
                if (shape.isInside(voxelCenter(x,y,z)))
                    voxels[x*res_y*res_z+y*res_z+z] = true;
                else voxels[x*res_y*res_z+y*res_z+z] = false;
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
        VoxelSlice Slice = VoxelSlice(res_x, res_y);
        for (uint32_t x = 0; x < res_x; x++){
            for (uint32_t y = 0; y < res_y; y++){
                Slice.data[y][x] = isSet(x, y, slice);
            }
        }
        return Slice;
    }
    else if (Axis::X == axis) {
        VoxelSlice Slice = VoxelSlice(res_y, res_z);
        for (uint32_t y = 0; y < res_y; y++){
            for (uint32_t z = 0; z < res_z; z++){
                Slice.data[z][y] = isSet(slice, y, z);
            }
        }
        return Slice;
    }
    else {
        VoxelSlice Slice = VoxelSlice(res_z, res_x);
        for (uint32_t x = 0; x < res_x; x++){
            for (uint32_t z = 0; z < res_z; z++){
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
    AABB boundingBox = getBounds();
    if( p.x >= boundingBox.max.x || p.x <= boundingBox.min.x ||
        p.y >= boundingBox.max.y || p.y <= boundingBox.min.y ||
        p.z >= boundingBox.max.z || p.z <= boundingBox.min.z)
        return false;
    else {
        Point3D pVector = p - boundingBox.min;
        //in welchem Voxel ist p?
        Point3D extents = boundingBox.extents();
        Point3D stepsize =  Point3D(extents.x / static_cast<float>(res_x), 
                                    extents.y / static_cast<float>(res_y), 
                                    extents.z / static_cast<float>(res_z));
        uint32_t xCord = static_cast<uint32_t>(pVector.x / stepsize.x);
        uint32_t yCord = static_cast<uint32_t>(pVector.y / stepsize.y);
        uint32_t zCord = static_cast<uint32_t>(pVector.z / stepsize.z);

        return isSet(xCord, yCord, zCord);
    }
}

bool VoxelGrid::isSet(uint32_t x, uint32_t y, uint32_t z) const
{
    // When running in debug mode, these will check whether the supplied indices are valid or "trap" to the debugger.
    // When no debugger is running, failing the assertion will terminate the program immediately.
    // When compiled in release mode, assert() does nothing.
    assert(x < res_x);
    assert(y < res_y);
    assert(z < res_z);

    return voxels.at(x*res_y*res_z + y*res_z + z);
    }

Point3D VoxelGrid::voxelCenter(uint32_t x, uint32_t y, uint32_t z) const
{
    AABB boundingBox = getBounds();
    Point3D extents  = boundingBox.extents();
    Point3D stepsize =  Point3D(extents.x / static_cast<float>(res_x), 
                                extents.y / static_cast<float>(res_y), 
                                extents.z / static_cast<float>(res_z));
    Point3D center = stepsize * Point3D(static_cast<float>(x+0.5),
                                        static_cast<float>(y+0.5),
                                        static_cast<float>(z+0.5));
    return center + boundingBox.min;
}

std::ostream& operator<<(std::ostream& ostream, const VoxelSlice& slice)
{
    for (uint32_t x=0; x < slice.data.size() ; x++){
        for (uint32_t y=0; y < slice.data[x].size(); y++){
            if(slice.data[x][y]) std::cout << "X";
            else std::cout << ".";
            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
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
