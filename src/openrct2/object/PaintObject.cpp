#include "PaintObject.h"

#include "../Context.h"
#include "../core/Console.hpp"
#include "../core/Json.hpp"
#include "../entity/EntityRegistry.h"
#include "../paint/Paint.h"
#include "../paint/Supports.h"
#include "../ride/Ride.h"
#include "../ride/RideData.h"
#include "../ride/TrackPaint.h"
#include "ObjectManager.h"
#include "ObjectRepository.h"

PaintObject::PaintObject()
{
}

void PaintObject::ReadJson(IReadObjectContext* context, json_t& root)
{
    try
    {
        if (root.contains("trackSequenceTables"))
        {
            auto trackSequenceTables = root["trackSequenceTables"];
            if (trackSequenceTables.is_array())
            {
                for (const auto& trackSequenceTable : trackSequenceTables)
                {
                    PaintStructSequenceMapping table;
                    auto sequences = trackSequenceTable["sequences"];
                    if (sequences.is_array())
                    {
                        size_t index = 0;
                        for (const auto& sequence : sequences)
                        {
                            if (sequence.is_array())
                            {
                                for (const auto& value : sequence)
                                {
                                    if (value.is_number())
                                        table[index].push_back(value);
                                }
                            }
                            index++;
                        }
                    }

                    const auto& trackElement = trackSequenceTable["trackElement"];
                    track_type_t element = 0;
                    if (trackElement == "flat_track_3x3")
                        element = TrackElemType::FlatTrack3x3;
                    _sequenceMappings[element] = table;
                }
            }
        }

        if (root.contains("edgesTables"))
        {
            auto edgesTables = root["edgesTables"];
            if (edgesTables.is_array())
            {
                for (const auto& edgeTable : edgesTables)
                {
                    PaintStructEdgesTable table;

                    auto edges = edgeTable["edges"];
                    if (edges.is_array())
                    {
                        for (const auto& edge : edges)
                        {
                            uint32_t result = 0;
                            if (edge.is_array())
                            {
                                for (const auto& side : edge)
                                {
                                    if (side == "ne")
                                        result |= EDGE_NE;
                                    else if (side == "se")
                                        result |= EDGE_SE;
                                    else if (side == "sw")
                                        result |= EDGE_SW;
                                    else if (side == "nw")
                                        result |= EDGE_NW;
                                }
                            }
                            table.push_back(static_cast<edge_t>(result));
                        }
                    }

                    auto id = edgeTable["id"];
                    _edgeMappings[id] = table;
                }
            }
        }

        if (root.contains("heightSupportsTables"))
        {
            auto heightTables = root["heightSupportsTables"];
            if (heightTables.is_array())
            {
                for (const auto& heightTable : heightTables)
                {
                    HeightSupportsTable table;
                    table.Id = heightTable["id"];
                    table.HeightOffset = heightTable["heightOffset"];

                    const auto& segments = heightTable["segments"];
                    for (const auto& segment : segments)
                    {
                        uint32_t trackSequence = segment["trackSequence"];
                        uint32_t edge = 0;

                        const auto& values = segment["values"];
                        for (const auto& value : values)
                        {
                            if (value == "b4")
                                edge |= SEGMENT_B4;
                            else if (value == "cc")
                                edge |= SEGMENT_CC;
                            else if (value == "bc")
                                edge |= SEGMENT_BC;
                            else if (value == "d4")
                                edge |= SEGMENT_D4;
                            else if (value == "c0")
                                edge |= SEGMENT_C0;
                            else if (value == "d0")
                                edge |= SEGMENT_D0;
                            else if (value == "b8")
                                edge |= SEGMENT_B8;
                            else if (value == "c8")
                                edge |= SEGMENT_C8;
                            else if (value == "c4")
                                edge |= SEGMENT_C4;
                        }

                        table.Segments[trackSequence] = edge;
                    }
                    _heightMapping[table.Id] = table;
                }
            }
        }

        if (root.contains("boundBoxes"))
        {
            const auto& boundBoxes = root["boundBoxes"];

            if (boundBoxes.is_array())
            {
                for (const auto& boundBox : boundBoxes)
                {
                    BoundBoxEntry entry;
                    if (boundBox.contains("id"))
                    {
                        entry.Id = boundBox["id"];
                    }

                    if (boundBox.contains("values"))
                    {
                        const auto& values = boundBox["values"];
                        for (const auto& value : values)
                        {
                            BoundBoxEntryValue entryValue;
                            if (value.contains("offset_x"))
                            {
                                auto offsetX = value["offset_x"];
                                if (offsetX.is_number())
                                    entryValue.Coords.x = offsetX;
                            }

                            if (value.contains("offset_y"))
                            {
                                auto offsetY = value["offset_y"];
                                if (offsetY.is_number())
                                    entryValue.Coords.y = offsetY;
                            }

                            if (value.contains("offset_z"))
                            {
                                auto offsetZ = value["offset_z"];
                                if (offsetZ.is_number())
                                    entryValue.Coords.z = offsetZ;
                            }

                            if (value.contains("bb_offset_x"))
                            {
                                auto bbOffsetX = value["bb_offset_x"];
                                if (bbOffsetX.is_number())
                                    entryValue.Boundbox.offset.x = bbOffsetX;
                            }

                            if (value.contains("bb_offset_y"))
                            {
                                auto bbOffsetY = value["bb_offset_y"];
                                if (bbOffsetY.is_number())
                                    entryValue.Boundbox.offset.y = bbOffsetY;
                            }

                            if (value.contains("bb_offset_z"))
                            {
                                auto bbOffsetZ = value["bb_offset_z"];
                                if (bbOffsetZ.is_number())
                                    entryValue.Boundbox.offset.z = bbOffsetZ;
                            }

                            if (value.contains("bb_length_x"))
                            {
                                auto bbLengthX = value["bb_length_x"];
                                if (bbLengthX.is_number())
                                    entryValue.Boundbox.length.x = bbLengthX;
                            }

                            if (value.contains("bb_length_y"))
                            {
                                auto bbLengthY = value["bb_length_y"];
                                if (bbLengthY.is_number())
                                    entryValue.Boundbox.length.y = bbLengthY;
                            }

                            if (value.contains("bb_length_z"))
                            {
                                auto bbLengthZ = value["bb_length_z"];
                                if (bbLengthZ.is_number())
                                    entryValue.Boundbox.length.z = bbLengthZ;
                            }

                            uint32_t trackSequence = 0;
                            if (value.contains("trackSequence"))
                            {
                                auto trackSequenceJson = value["trackSequence"];
                                if (trackSequenceJson.is_number())
                                    trackSequence = trackSequenceJson;
                            }
                            entry.Values[trackSequence] = entryValue;
                        }
                    }
                    _boundBoxMapping[entry.Id] = entry;
                }
            }
        }

        std::vector<ImageIdOffsetJson> offsetsJson;
        auto imageIdOffsets = root["imageIdOffsets"];
        if (imageIdOffsets.is_array())
        {
            for (const auto& imageIdOffset : imageIdOffsets)
            {
                ImageIdOffsetJson offsetJson;
                offsetJson.FromJson(imageIdOffset);
                offsetsJson.push_back(offsetJson);
            }
        }

        if (root.contains("vehicleIndices"))
        {
            const auto& vehicleIndices = root["vehicleIndices"];
            if (vehicleIndices.is_array())
            {
                for (const auto& vehicleIndex : vehicleIndices)
                {
                    _vehicleIndices.push_back(vehicleIndex);
                }
            }
        }

        auto paintStructs = root["paintStructs"];
        std::vector<PaintStructKey> keysJson;
        std::vector<PaintStructJson> paintsJson;
        if (paintStructs.is_array())
        {
            for (const auto& paintStruct : paintStructs)
            {
                PaintStructJson paintJson(*this);
                paintJson.FromJson(paintStruct);
                paintJson.Key.FromJson(paintStruct);
                paintsJson.push_back(paintJson);
            }
        }

        //generate the trees
        for (const auto& paintStructJson : paintsJson)
            keysJson.push_back(paintStructJson.Key);

        _paintStructsTree = std::make_unique<PaintStructTree>(keysJson);
        keysJson.clear();

        for (const auto& offsetJson : offsetsJson)
            keysJson.insert(keysJson.end(), offsetJson.Keys.begin(), offsetJson.Keys.end());

        for (const auto& offsetJson : offsetsJson)
        {
            ImageIdOffset offset(keysJson);
            offset.Id = offsetJson.Id;

            for (size_t index = 0; index < offsetJson.Keys.size(); index++)
            {
                const auto& key = offsetJson.Keys[index];
                const auto& values = offsetJson.Values[index];

                for (const auto& value : values)
                {
                    offset.Entries.Add(key, value);
                }
            }
            _imageIdOffsetMapping[offset.Id] = std::make_shared<ImageIdOffset>(offset);
        }

        for (const auto& paintStructJson : paintsJson)
        {
            const auto& key = paintStructJson.Key;
            auto value = paintStructJson.Value();
            auto ptr = std::make_shared<PaintStructDescriptor>(value);
            _paintStructsTree->Add(key, ptr);
        }
    }
    catch (JsonException& e)
    {
        Console::Error::WriteLine(e.what());
    }
}

void PaintObject::Load()
{
}

void PaintObject::Unload()
{
}

void PaintObject::Paint(
    PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement) const
{
    // for (const auto& paintStruct : _paintStructs)
    // paintStruct.Paint(session, ride, trackSequence, direction, height, trackElement);

    PaintStructKey key;
    key.Direction = direction;
    key.Element = trackElement.GetTrackType();

    auto it = _sequenceMappings.find(key.Element.value());
    if (it != _sequenceMappings.end())
        trackSequence = it->second[direction][trackSequence];
    key.TrackSequence = trackSequence;

    // check the first vehicle in the list
    // to-do: add vehicle state variables in the key for every vehicle index in the list
    Vehicle* vehicle = nullptr;
    if (!_vehicleIndices.empty())
    {
        // first, check if the paint struct key matches with the call
        if (ride.lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK)
        {
            vehicle = GetEntity<Vehicle>(ride.vehicles[_vehicleIndices[0]]);

            session.InteractionType = ViewportInteractionItem::Entity;
            session.CurrentlyDrawnEntity = vehicle;
        }
    }

    // to-do: in the future, add a key val for every vehicle index, not just the first in the list
    if (vehicle != nullptr)
    {
        key.VehicleNumPeeps[0] = vehicle->num_peeps;
        key.VehiclePitch[0] = vehicle->Pitch;
        key.VehicleSpriteDirection[0] = vehicle->sprite_direction;
    }

    auto paintStructs = _paintStructsTree->Get(key);
    if (paintStructs != nullptr)
    {
        for (const auto& paintStruct : *paintStructs)
        {
            paintStruct->Paint(session, ride, trackSequence, direction, height, trackElement, key, vehicle);
        }
    }
}

void PaintObject::LoadPaintObjects()
{
    auto& repoManager = OpenRCT2::GetContext()->GetObjectRepository();
    for (ObjectEntryIndex i = 0; i < std::size(RideTypeDescriptors); i++)
    {
        auto& rtd = GetRideTypeDescriptor(i);

        if (!rtd.PaintObjectId.empty())
        {
            auto repoItem = repoManager.FindObject(rtd.PaintObjectId);
            if (repoItem != nullptr)
            {
                auto test = repoManager.LoadObject(repoItem);

                if (repoItem->LoadedObject == nullptr)
                {
                    repoManager.RegisterLoadedObject(repoItem, std::move(test));

                    if (repoItem->LoadedObject != nullptr)
                        repoItem->LoadedObject->Load();
                }
            }
        }
    }
}
