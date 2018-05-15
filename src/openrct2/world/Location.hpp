#pragma region Copyright (c) 2017 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#pragma once

#include <algorithm>
#include "../common.h"
#include <algorithm>

#define LOCATION_NULL     ((sint16)(uint16)0x8000)
#define RCT_XY8_UNDEFINED 0xFFFF
#define MakeXY16(x, y)    {(sint16)(x), (sint16)(y)}

#pragma pack(push, 1)
struct LocationXY8 {
    union {
        struct {
            uint8 x, y;
        };
        uint16 xy;
    };
};
assert_struct_size(LocationXY8, 2);

struct sLocationXY8 {
    sint8 x, y;
};
assert_struct_size(sLocationXY8, 2);

struct LocationXY16 {
    sint16 x, y;
};
assert_struct_size(LocationXY16, 4);


struct LocationXYZ16 {
    sint16 x, y, z;
};
assert_struct_size(LocationXYZ16, 6);
#pragma pack(pop)

constexpr sint32 COORDS_NULL = -1;

/**
 * Tile coordinates use 1 x/y increment per tile and 1 z increment per step.
 * Regular ('big', 'sprite') coordinates use 32 x/y increments per tile and 8 z increments per step.
 */
struct CoordsXY
{
    sint32 x = 0;
    sint32 y = 0;

    CoordsXY() = default;
    constexpr CoordsXY(sint32 _x, sint32 _y) :
        x(_x),
        y(_y)
    {
    }
};

struct TileCoordsXY
{
    TileCoordsXY() = default;
    TileCoordsXY(sint32 x_, sint32 y_) : x(x_), y(y_) {}
    explicit TileCoordsXY(CoordsXY c) : x(c.x / 32), y(c.y / 32) {}
    TileCoordsXY& operator+=(const TileCoordsXY rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    sint32 x = 0, y = 0;
};

struct CoordsXYZ
{
    sint32 x = 0;
    sint32 y = 0;
    sint32 z = 0;

    CoordsXYZ() = default;
    constexpr CoordsXYZ(sint32 _x, sint32 _y, sint32 _z)
        : x(_x),
          y(_y),
          z(_z)
    {
    }
};

struct TileCoordsXYZ
{
    TileCoordsXYZ() = default;
    TileCoordsXYZ(sint32 x_, sint32 y_, sint32 z_) : x(x_), y(y_), z(z_) {}
    explicit TileCoordsXYZ(CoordsXYZ c) : x(c.x / 32), y(c.y / 32), z(c.z / 8) {}
    TileCoordsXYZ& operator+=(const TileCoordsXY rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    sint32 x = 0, y = 0, z = 0;
};

struct CoordsXYZD
{
    sint32 x, y, z;
    uint8 direction;

    bool isNull() const { return x == COORDS_NULL; };
};

struct TileCoordsXYZD
{
    sint32 x, y, z;
    uint8 direction;

    bool isNull() const { return x == COORDS_NULL; };
};

/**
 * Represents a rectangular range of the map using regular coordinates (32 per tile).
 */
struct MapRange
{
    CoordsXY LeftTop;
    CoordsXY RightBottom;

    sint32 GetLeft() const { return LeftTop.x; }
    sint32 GetTop() const { return LeftTop.y; }
    sint32 GetRight() const { return RightBottom.x; }
    sint32 GetBottom() const { return RightBottom.y; }

    MapRange() : MapRange(0, 0, 0, 0) { }
    MapRange(sint32 left, sint32 top, sint32 right, sint32 bottom)
        : LeftTop(left, top),
          RightBottom(right, bottom)
    {
    }

    MapRange Normalise() const
    {
        auto result = MapRange(
            std::min(GetLeft(), GetRight()),
            std::min(GetTop(), GetBottom()),
            std::max(GetLeft(), GetRight()),
            std::max(GetTop(), GetBottom()));
        return result;
    }
};
