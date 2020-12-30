/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "TileElement.h"

#include "../core/Guard.hpp"
#include "../interface/Window.h"
#include "../localisation/Localisation.h"
#include "../ride/Track.h"
#include "Banner.h"
#include "LargeScenery.h"
#include "Location.hpp"
#include "Scenery.h"

uint8_t TileElementBase::GetType() const
{
    return this->type & TILE_ELEMENT_TYPE_MASK;
}

void TileElementBase::SetType(uint8_t newType)
{
    this->type &= ~TILE_ELEMENT_TYPE_MASK;
    this->type |= (newType & TILE_ELEMENT_TYPE_MASK);
}

Direction TileElementBase::GetDirection() const
{
    return this->type & TILE_ELEMENT_DIRECTION_MASK;
}

void TileElementBase::SetDirection(Direction direction)
{
    this->type &= ~TILE_ELEMENT_DIRECTION_MASK;
    this->type |= (direction & TILE_ELEMENT_DIRECTION_MASK);
}

Direction TileElementBase::GetDirectionWithOffset(uint8_t offset) const
{
    return ((this->type & TILE_ELEMENT_DIRECTION_MASK) + offset) & TILE_ELEMENT_DIRECTION_MASK;
}

bool TileElementBase::IsLastForTile() const
{
    return (this->Flags & TILE_ELEMENT_FLAG_LAST_TILE) != 0;
}

void TileElementBase::SetLastForTile(bool on)
{
    if (on)
        Flags |= TILE_ELEMENT_FLAG_LAST_TILE;
    else
        Flags &= ~TILE_ELEMENT_FLAG_LAST_TILE;
}

bool TileElementBase::IsGhost() const
{
    return (this->Flags & TILE_ELEMENT_FLAG_GHOST) != 0;
}

void TileElementBase::SetGhost(bool isGhost)
{
    if (isGhost)
    {
        this->Flags |= TILE_ELEMENT_FLAG_GHOST;
    }
    else
    {
        this->Flags &= ~TILE_ELEMENT_FLAG_GHOST;
    }
}

bool tile_element_is_underground(TileElement* tileElement)
{
    do
    {
        tileElement++;
        if ((tileElement - 1)->IsLastForTile())
            return false;
    } while (tileElement->GetType() != TILE_ELEMENT_TYPE_SURFACE);
    return true;
}

BannerIndex TileElement::GetBannerIndex() const
{
    rct_scenery_entry* sceneryEntry;

    switch (GetType())
    {
        case TILE_ELEMENT_TYPE_LARGE_SCENERY:
            sceneryEntry = AsLargeScenery()->GetEntry();
            if (sceneryEntry == nullptr || sceneryEntry->large_scenery.scrolling_mode == SCROLLING_MODE_NONE)
                return BANNER_INDEX_NULL;

            return AsLargeScenery()->GetBannerIndex();
        case TILE_ELEMENT_TYPE_WALL:
            sceneryEntry = AsWall()->GetEntry();
            if (sceneryEntry == nullptr || sceneryEntry->wall.scrolling_mode == SCROLLING_MODE_NONE)
                return BANNER_INDEX_NULL;

            return AsWall()->GetBannerIndex();
        case TILE_ELEMENT_TYPE_BANNER:
            return AsBanner()->GetIndex();
        default:
            return BANNER_INDEX_NULL;
    }
}

void TileElement::SetBannerIndex(BannerIndex bannerIndex)
{
    switch (GetType())
    {
        case TILE_ELEMENT_TYPE_WALL:
            AsWall()->SetBannerIndex(bannerIndex);
            break;
        case TILE_ELEMENT_TYPE_LARGE_SCENERY:
            AsLargeScenery()->SetBannerIndex(bannerIndex);
            break;
        case TILE_ELEMENT_TYPE_BANNER:
            AsBanner()->SetIndex(bannerIndex);
            break;
        default:
            log_error("Tried to set banner index on unsuitable tile element!");
            Guard::Assert(false);
    }
}

void TileElement::RemoveBannerEntry()
{
    auto bannerIndex = GetBannerIndex();
    auto banner = GetBanner(bannerIndex);
    if (banner != nullptr)
    {
        window_close_by_number(WC_BANNER, bannerIndex);
        *banner = {};
    }
}

ride_id_t TileElement::GetRideIndex() const
{
    switch (GetType())
    {
        case TILE_ELEMENT_TYPE_TRACK:
            return AsTrack()->GetRideIndex();
        case TILE_ELEMENT_TYPE_ENTRANCE:
            return AsEntrance()->GetRideIndex();
        case TILE_ELEMENT_TYPE_PATH:
            return AsPath()->GetRideIndex();
        default:
            return RIDE_ID_NULL;
    }
}

void TileElement::ClearAs(uint8_t newType)
{
    type = newType;
    Flags = 0;
    base_height = MINIMUM_LAND_HEIGHT;
    clearance_height = MINIMUM_LAND_HEIGHT;
    std::fill_n(pad_04, sizeof(pad_04), 0x00);
    std::fill_n(pad_08, sizeof(pad_08), 0x00);
}

void TileElementBase::Remove()
{
    tile_element_remove(static_cast<TileElement*>(this));
}

// Rotate both of the values amount
const QuarterTile QuarterTile::Rotate(uint8_t amount) const
{
    switch (amount)
    {
        case 0:
            return QuarterTile{ *this };
        case 1:
        {
            auto rotVal1 = _val << 1;
            auto rotVal2 = rotVal1 >> 4;
            // Clear the bit from the tileQuarter
            rotVal1 &= 0b11101110;
            // Clear the bit from the zQuarter
            rotVal2 &= 0b00010001;
            return QuarterTile{ static_cast<uint8_t>(rotVal1 | rotVal2) };
        }
        case 2:
        {
            auto rotVal1 = _val << 2;
            auto rotVal2 = rotVal1 >> 4;
            // Clear the bit from the tileQuarter
            rotVal1 &= 0b11001100;
            // Clear the bit from the zQuarter
            rotVal2 &= 0b00110011;
            return QuarterTile{ static_cast<uint8_t>(rotVal1 | rotVal2) };
        }
        case 3:
        {
            auto rotVal1 = _val << 3;
            auto rotVal2 = rotVal1 >> 4;
            // Clear the bit from the tileQuarter
            rotVal1 &= 0b10001000;
            // Clear the bit from the zQuarter
            rotVal2 &= 0b01110111;
            return QuarterTile{ static_cast<uint8_t>(rotVal1 | rotVal2) };
        }
        default:
            log_error("Tried to rotate QuarterTile invalid amount.");
            return QuarterTile{ 0 };
    }
}

uint8_t TileElementBase::GetOccupiedQuadrants() const
{
    return Flags & TILE_ELEMENT_OCCUPIED_QUADRANTS_MASK;
}

void TileElementBase::SetOccupiedQuadrants(uint8_t quadrants)
{
    Flags &= ~TILE_ELEMENT_OCCUPIED_QUADRANTS_MASK;
    Flags |= (quadrants & TILE_ELEMENT_OCCUPIED_QUADRANTS_MASK);
}

int32_t TileElementBase::GetBaseZ() const
{
    return base_height * COORDS_Z_STEP;
}

void TileElementBase::SetBaseZ(int32_t newZ)
{
    base_height = (newZ / COORDS_Z_STEP);
}

int32_t TileElementBase::GetClearanceZ() const
{
    return clearance_height * COORDS_Z_STEP;
}

void TileElementBase::SetClearanceZ(int32_t newZ)
{
    clearance_height = (newZ / COORDS_Z_STEP);
}
