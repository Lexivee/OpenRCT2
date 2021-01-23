#pragma once

#include "../common.h"

struct CoordsXYZ;
enum class EntityListId : uint8_t;
enum class SpriteIdentifier : uint8_t;

struct SpriteBase
{
    SpriteIdentifier sprite_identifier;
    // Valid values are EntityListId::...
    EntityListId linked_list_index;
    // Height from centre of sprite to bottom
    uint8_t sprite_height_negative;
    uint16_t sprite_index;
    uint16_t flags;
    int16_t x;
    int16_t y;
    int16_t z;
    // Width from centre of sprite to edge
    uint8_t sprite_width;
    // Height from centre of sprite to top
    uint8_t sprite_height_positive;
    // Screen Coordinates of sprite
    int16_t sprite_left;
    int16_t sprite_top;
    int16_t sprite_right;
    int16_t sprite_bottom;

    uint8_t sprite_direction;

    void MoveTo(const CoordsXYZ& newLocation);
    void Invalidate();
    template<typename T> bool Is() const;
    template<typename T> T* As()
    {
        return Is<T>() ? reinterpret_cast<T*>(this) : nullptr;
    }
    template<typename T> const T* As() const
    {
        return Is<T>() ? reinterpret_cast<const T*>(this) : nullptr;
    }
};

enum class MiscEntityType : uint8_t;
struct MiscEntity : SpriteBase
{
    MiscEntityType SubType;
    uint16_t frame;
};
