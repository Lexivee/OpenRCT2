#pragma once

#include "../Limits.h"
#include "../ride/TrackData.h"
#include "TreeContainer.h"

#include <array>
#include <optional>
#include <tuple>
#include <vector>

struct Vehicle;
using PaintStructSequenceMapping = std::array<std::vector<uint32_t>, 4>;

struct VehicleKey
{
    uint32_t SpriteDirection;
    uint32_t Pitch;
    uint32_t NumPeeps;

    VehicleKey()
        : SpriteDirection(0)
        , Pitch(0)
        , NumPeeps(0)
    {
    }
};

struct PaintStructDescriptorKey
{
    PaintStructDescriptorKey()
        : Element(0)
        , Direction(0)
        , TrackSequence(0)
    {
    }

    uint32_t Element;
    uint32_t Direction;
    uint32_t TrackSequence;
    std::array<VehicleKey, OpenRCT2::Limits::MaxTrainsPerRide + 1> VehicleKey;
};

struct ImageIdOffset
{
    std::string Id;
    TreeContainer<uint32_t> Entries;
};

struct HeightSupportsTable
{
    std::string Id;
    uint32_t HeightOffset;
    std::map<uint8_t, uint32_t> Segments;
};

using PaintStructEdgesTable = std::vector<edge_t>;
struct PaintStructDescriptor
{
    enum class SupportsType
    {
        WoodenA
    };
    enum class FloorType
    {
        Cork
    };
    enum class FenceType
    {
        Ropes
    };
    enum class ImageIdBase
    {
        Car0
    };
    enum class PaintType
    {
        AddImageAsParent,
        AddImageAsChild,
    };
    enum class Scheme
    {
        Misc,
        Track
    };

    enum class Colour
    {
        VehicleBody,
        VehicleTrim,
        PeepTShirt
    };

    // key fields
    PaintStructDescriptorKey Key;

    // output fields
    std::optional<SupportsType> Supports;
    std::optional<FloorType> Floor;
    PaintStructEdgesTable* Edges = nullptr;
    std::optional<FenceType> Fences;
    std::optional<PaintType> PaintType;
    std::optional<Scheme> ImageIdScheme;
    ImageIdBase ImageIdBase;
    Colour PrimaryColour;
    uint32_t PrimaryColourIndex;
    Colour SecondaryColour;
    uint32_t SecondaryColourIndex;

    ImageIdOffset* ImageIdOffset = nullptr;
    uint32_t ImageIdOffsetIndex;

    CoordsXYZ Offset;
    BoundBoxXYZ BoundBox;
    HeightSupportsTable* HeightSupports = nullptr;

    //to-do : in the future, send a list of vehicle pointers so we can get the peep tshirt colours from every vehicle...
    void Paint(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, const Vehicle* vehicle) const;

    void ToJson(json_t& json) const;

    PaintStructDescriptor();
};

