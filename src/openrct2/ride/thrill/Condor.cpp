/*****************************************************************************
 * Copyright (c) 2014-2023 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "meta/Condor.h"

#include "../../common.h"
#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../../scenario/Scenario.h"
#include "../Ride.h"
#include "../Track.h"
#include "../TrackPaint.h"
#include "../Vehicle.h"
#include "../VehiclePaint.h"
#include "../../core/DataSerialiser.h"

#include <cstring>
#include <algorithm>

enum
{
    SPR_ROTO_DROP_TOWER_SEGMENT = 14558,
    SPR_ROTO_DROP_TOWER_SEGMENT_TOP = 14559,
    SPR_ROTO_DROP_TOWER_BASE = 14560,
    SPR_ROTO_DROP_TOWER_BASE_SEGMENT = 14561,
    SPR_ROTO_DROP_TOWER_BASE_90_DEG = 14562,
    SPR_ROTO_DROP_TOWER_BASE_SEGMENT_90_DEG = 14563,
};

/**
 *
 *  rct2: 0x006D5DA9
 */
void VehicleVisualCondor(
    PaintSession& session, int32_t x, int32_t imageDirection, int32_t y, int32_t z, const Vehicle* vehicle,
    const CarEntry* carEntry)
{
    imageDirection = OpenRCT2::Entity::Yaw::YawTo32(imageDirection);

    auto imageFlags = ImageId(0, vehicle->colours.Body, vehicle->colours.Trim);
    if (vehicle->IsGhost())
    {
        imageFlags = ConstructionMarker;
    }

    ImageId image_id;
    int32_t baseImage_id = (carEntry->base_image_id + 4) + ((vehicle->animation_frame / 4) & 0x3);
    if (vehicle->restraints_position >= 64)
    {
        baseImage_id += 7;
        baseImage_id += (vehicle->restraints_position / 64);
    }

    // Draw back:
    image_id = imageFlags.WithIndex(baseImage_id);
    PaintAddImageAsParent(session, image_id, { 0, 0, z }, { 2, 2, 41 }, { -11, -11, z + 1 });

    // Draw front:
    image_id = imageFlags.WithIndex(baseImage_id + 4);
    PaintAddImageAsParent(session, image_id, { 0, 0, z }, { 16, 16, 41 }, { -5, -5, z + 1 });

    if (vehicle->num_peeps > 0 && !vehicle->IsGhost())
    {
        uint8_t riding_peep_sprites[64];
        std::fill_n(riding_peep_sprites, sizeof(riding_peep_sprites), 0xFF);
        for (int32_t i = 0; i < vehicle->num_peeps; i++)
        {
            uint8_t cl = (i & 3) * 16;
            cl += (i & 0xFC);
            cl += vehicle->animation_frame / 4;
            cl += (imageDirection / 8) * 16;
            cl &= 0x3F;
            riding_peep_sprites[cl] = vehicle->peep_tshirt_colours[i];
        }

        // Draw riding peep sprites in back to front order:
        for (int32_t j = 0; j <= 48; j++)
        {
            int32_t i = (j % 2) ? (48 - (j / 2)) : (j / 2);
            if (riding_peep_sprites[i] != 0xFF)
            {
                baseImage_id = carEntry->base_image_id + 20 + i;
                if (vehicle->restraints_position >= 64)
                {
                    baseImage_id += 64;
                    baseImage_id += vehicle->restraints_position / 64;
                }
                image_id = ImageId(baseImage_id, riding_peep_sprites[i]);
                PaintAddImageAsChild(session, image_id, { 0, 0, z }, { { -5, -5, z + 1 }, { 16, 16, 41 } });
            }
        }
    }

    assert(carEntry->effect_visual == 1);
    // Although called in original code, effect_visual (splash effects) are not used for many rides and does not make sense so
    // it was taken out
}

/** rct2: 0x00886194 */
static void PaintCondorBase(
    PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = track_map_3x3[direction][trackSequence];

    int32_t edges = edges_3x3[trackSequence];

    WoodenASupportsPaintSetup(session, (direction & 1), 0, height, session.TrackColours[SCHEME_MISC]);

    const StationObject* stationObject = ride.GetStationObject();

    TrackPaintUtilPaintFloor(session, edges, session.TrackColours[SCHEME_SUPPORTS], height, floorSpritesMetalB, stationObject);

    TrackPaintUtilPaintFences(
        session, edges, session.MapPosition, trackElement, ride, session.TrackColours[SCHEME_TRACK], height, fenceSpritesMetalB,
        session.CurrentRotation);

    if (trackSequence == 0)
    {
        auto imageId = session.TrackColours[SCHEME_TRACK].WithIndex(
            (direction & 1 ? SPR_ROTO_DROP_TOWER_BASE_90_DEG : SPR_ROTO_DROP_TOWER_BASE));
        PaintAddImageAsParent(session, imageId, { 0, 0, height }, { 2, 2, 27 }, { 8, 8, height + 3 });

        imageId = session.TrackColours[SCHEME_TRACK].WithIndex(
            (direction & 1 ? SPR_ROTO_DROP_TOWER_BASE_SEGMENT_90_DEG : SPR_ROTO_DROP_TOWER_BASE_SEGMENT));
        PaintAddImageAsParent(session, imageId, { 0, 0, height + 32 }, { 2, 2, 30 }, { 8, 8, height + 32 });

        imageId = session.TrackColours[SCHEME_TRACK].WithIndex(
            (direction & 1 ? SPR_ROTO_DROP_TOWER_BASE_SEGMENT_90_DEG : SPR_ROTO_DROP_TOWER_BASE_SEGMENT));
        PaintAddImageAsParent(session, imageId, { 0, 0, height + 64 }, { 2, 2, 30 }, { 8, 8, height + 64 });

        PaintUtilSetVerticalTunnel(session, height + 96);
        PaintUtilSetSegmentSupportHeight(session, SEGMENTS_ALL, 0xFFFF, 0);

        PaintUtilSetGeneralSupportHeight(session, height + 96, 0x20);

        return;
    }

    int32_t blockedSegments = 0;
    switch (trackSequence)
    {
        case 1:
            blockedSegments = SEGMENT_B8 | SEGMENT_C8 | SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC;
            break;
        case 2:
            blockedSegments = SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC;
            break;
        case 3:
            blockedSegments = SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC | SEGMENT_D4 | SEGMENT_C0;
            break;
        case 4:
            blockedSegments = SEGMENT_B4 | SEGMENT_C8 | SEGMENT_B8;
            break;
        case 5:
            blockedSegments = SEGMENT_BC | SEGMENT_D4 | SEGMENT_C0;
            break;
        case 6:
            blockedSegments = SEGMENT_B4 | SEGMENT_C8 | SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0;
            break;
        case 7:
            blockedSegments = SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0 | SEGMENT_D4 | SEGMENT_BC;
            break;
        case 8:
            blockedSegments = SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0;
            break;
    }
    PaintUtilSetSegmentSupportHeight(session, blockedSegments, 0xFFFF, 0);
    PaintUtilSetSegmentSupportHeight(session, SEGMENTS_ALL & ~blockedSegments, height + 2, 0x20);
    PaintUtilSetGeneralSupportHeight(session, height + 32, 0x20);
}

/** rct2: 0x008861A4 */
static void PaintCondorTowerSection(
    PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackSequence == 1)
    {
        return;
    }

    auto imageId = session.TrackColours[SCHEME_TRACK].WithIndex(SPR_ROTO_DROP_TOWER_SEGMENT);
    PaintAddImageAsParent(session, imageId, { 0, 0, height }, { 2, 2, 30 }, { 8, 8, height });

    const TileElement* nextTileElement = reinterpret_cast<const TileElement*>(&trackElement) + 1;
    if (trackElement.IsLastForTile() || trackElement.GetClearanceZ() != nextTileElement->GetBaseZ())
    {
        imageId = session.TrackColours[SCHEME_TRACK].WithIndex(SPR_ROTO_DROP_TOWER_SEGMENT_TOP);
        PaintAddImageAsChild(session, imageId, { 0, 0, height }, { { 8, 8, height }, { 2, 2, 30 } });
    }

    PaintUtilSetSegmentSupportHeight(session, SEGMENTS_ALL, 0xFFFF, 0);

    PaintUtilSetVerticalTunnel(session, height + 32);
    PaintUtilSetGeneralSupportHeight(session, height + 32, 0x20);
}

/**
 * rct2: 0x00886074
 */
TRACK_PAINT_FUNCTION GetTrackPaintFunctionCondor(int32_t trackType)
{
    switch (trackType)
    {
        case TrackElemType::TowerBase:
            return PaintCondorBase;

        case TrackElemType::TowerSection:
            return PaintCondorTowerSection;
    }

    return nullptr;
}

static constexpr const CoordsXY word_9A3AB4[4] = {
    { 0, 0 },
    { 0, -96 },
    { -96, -96 },
    { -96, 0 },
};

static constexpr const CoordsXY StartLocations[] = { { 48, 0 },    { 30, 38 },   { -11, 47 }, { -43, 21 },
                                                   { -43, -21 }, { -11, -47 }, { 30, -38 } };


static constexpr const uint32_t StartDirections[] = {8, 3, 31, 26, 22, 17, 13};

CondorVehicleData::CondorVehicleData()
    : VehicleIndex(0)
{
}

CondorVehicleData ::~CondorVehicleData()
{
}

VehicleDataType CondorVehicleData::GetType() const
{
    return VehicleDataType::Condor;
}

void CondorVehicleData::Serialise(DataSerialiser& stream)
{
    stream << VehicleIndex;
}

RideDataType CondorRideData::GetType() const
{
    return RideDataType::Condor;
}

CondorRideData::CondorRideData()
    : State(CondorRideState::Waiting)
    , VehiclesZ(0)
    , TowerTop(0)
    , TowerBase(0)
{
}


static uint32_t CondorGetTowerHeight(const Vehicle& vehicle)
{
    TileCoordsXYZ result;

    TileElement* tileElement = MapGetTrackElementAtOfType(vehicle.TrackLocation, vehicle.GetTrackType());
    if (tileElement == nullptr)
        return 0;

    while (!tileElement->IsLastForTile())
    {
        tileElement++;

        if (tileElement->IsGhost())
            continue;

        if (tileElement->GetType() != TileElementType::Track)
            continue;

        const auto* trackElement = tileElement->AsTrack();
        if (trackElement->GetRideIndex() != vehicle.ride)
            continue;

        if (trackElement->GetTrackType() != TrackElemType::TowerSection
            && trackElement->GetTrackType() != TrackElemType::TowerBase)
            continue;

        int32_t tileTop = tileElement->ClearanceHeight;
        if (result.z < tileTop)
            result.z = tileTop;
    }

    return result.ToCoordsXYZ().z;
}

void CondorCreateVehicle(
    Vehicle* vehicle, Ride* ride, int32_t vehicleIndex, const CoordsXYZ& carPosition, TrackElement* trackElement)
{
    // Loc6DDCA4:
    vehicle->TrackSubposition = VehicleTrackSubposition::Default;
    vehicle->TrackLocation = carPosition;
    vehicle->current_station = trackElement->GetStationIndex();

    vehicle->SetTrackDirection(0);
    vehicle->SetTrackType(trackElement->GetTrackType());
    vehicle->track_progress = 0;
    vehicle->SetState(Vehicle::Status::MovingToEndOfStation);
    vehicle->update_flags = 0;

    //place the car in a circle, centered around the tower
    auto centerOffset = CoordsXY{ 16, 16 };
    auto chosenLoc = carPosition + CoordsXYZ{ StartLocations[vehicleIndex % 7] + centerOffset, 0 };
    vehicle->MoveTo(chosenLoc);

    //set the angle of the vehicle
    //32 sprites for a complete rotation
    vehicle->sprite_direction = StartDirections[vehicleIndex % 7];
    //vehicle->sprite_direction = 9;
    vehicle->Pitch = 0;

    vehicle->VehicleData = std::make_unique<CondorVehicleData>();
    auto condorData = static_cast<CondorVehicleData*>(vehicle->VehicleData.get());
    condorData->VehicleIndex = vehicleIndex;

    if (vehicleIndex == 0)
    {
        ride->Data = std::make_unique<CondorRideData>();

        auto condorRideData = static_cast<CondorRideData*>(ride->Data.get());
        condorRideData->State = CondorRideState::Waiting;
        condorRideData->TowerTop = CondorGetTowerHeight(*vehicle) - 64;
        condorRideData->TowerBase = vehicle->z;
        condorRideData->VehiclesZ = vehicle->z;
    }


    auto rideData = static_cast<CondorRideData*>(ride->Data.get());
    if (rideData != nullptr)
    {
        rideData->Vehicles.push_back(vehicle);
    }
}

static void CondorRideUpdateWating(Ride& ride)
{
    auto condorRideData = static_cast<CondorRideData*>(ride.Data.get());

    //check if all vehicles are departing
    bool allDeparting = true;
    const auto& vehicles = ride.vehicles;
    const auto numVehicles = ride.NumTrains;

    for (uint8_t vehicleIndex = 0; vehicleIndex < numVehicles; ++vehicleIndex)
    {
        auto* vehicle = GetEntity<Vehicle>(vehicles[vehicleIndex]);
        if (vehicle != nullptr)
        {
            if (vehicle->status != Vehicle::Status::Departing)
            {
                allDeparting = false;
                break;
            }
        }
    }

    if (allDeparting)
    {
        for (uint8_t vehicleIndex = 0; vehicleIndex < numVehicles; ++vehicleIndex)
        {
            auto* vehicle = GetEntity<Vehicle>(vehicles[vehicleIndex]);
            if (vehicle != nullptr)
            {
                vehicle->SetState(Vehicle::Status::Travelling);
            }
        }
        condorRideData->State = CondorRideState::Climbing;
    }
}

static void CondorRideUpdateClimbing(Ride& ride)
{
    auto condorRideData = static_cast<CondorRideData*>(ride.Data.get());
    if (condorRideData != nullptr)
    {
        int32_t height = condorRideData->VehiclesZ + 1;
        if (height > condorRideData->TowerTop)
        {
            height = condorRideData->TowerTop;
            condorRideData->State = CondorRideState::Falling;
        }
        condorRideData->VehiclesZ = height;
    }
}

static void CondorRideUpdateFalling(Ride& ride)
{
    auto condorRideData = static_cast<CondorRideData*>(ride.Data.get());
    if (condorRideData != nullptr)
    {
        int32_t height = condorRideData->VehiclesZ - 1;
        if (height < condorRideData->TowerBase)
        {
            height = condorRideData->TowerBase;
            condorRideData->State = CondorRideState::Waiting;
        }
        condorRideData->VehiclesZ = height;
    }
}


void CondorRideUpdate(Ride& ride)
{
    auto condorRideData = static_cast<CondorRideData*>(ride.Data.get());
    if (condorRideData != nullptr)
    {
        switch (condorRideData->State)
        {
            case CondorRideState::Waiting:
                CondorRideUpdateWating(ride);
                break;
            case CondorRideState::Climbing:
                CondorRideUpdateClimbing(ride);
                break;
            case CondorRideState::Falling:
                CondorRideUpdateFalling(ride);
                break;
        }
    }
}

void CondorUpdateDeparting(Vehicle& vehicle)
{
}

void CondorUpdateTravelling(Vehicle& vehicle)
{
    //auto condorData = static_cast<CondorVehicleData*>(vehicle.VehicleData.get());
    auto ride = vehicle.GetRide();
    auto condorRideData = static_cast<CondorRideData*>(ride->Data.get());


    if (condorRideData->State == CondorRideState::Climbing || condorRideData->State == CondorRideState::Falling)
    {
        auto target = vehicle.GetLocation();
        target.z = condorRideData->VehiclesZ;

        vehicle.MoveTo(target);
        vehicle.TrackLocation.z = target.z;
        vehicle.Invalidate();
    }
    else
    {
        vehicle.SetState(Vehicle::Status::Arriving);
    }
}

void CondorUpdateMotion(Vehicle& vehicle)
{
    vehicle.current_station = StationIndex::FromUnderlying(0);
    vehicle.velocity = 0;
    vehicle.acceleration = 0;
    vehicle.SetState(Vehicle::Status::WaitingForPassengers);
}

void CondorUpdateWaitingForDepart(Vehicle& vehicle)
{
}
