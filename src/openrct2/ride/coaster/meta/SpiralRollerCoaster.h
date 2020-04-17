/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../../RideData.h"
#include "../../Track.h"

// clang-format off
constexpr const RideTypeDescriptor SpiralRollerCoasterRTD =
{
    SET_FIELD(EnabledTrackPieces, (1ULL << TRACK_STRAIGHT) | (1ULL << TRACK_STATION_END) | (1ULL << TRACK_FLAT_ROLL_BANKING) | (1ULL << TRACK_SLOPE) | (1ULL << TRACK_SLOPE_STEEP) | (1ULL << TRACK_SLOPE_CURVE) | (1ULL << TRACK_SLOPE_CURVE_STEEP) | (1ULL << TRACK_S_BEND) | (1ULL << TRACK_CURVE_SMALL) | (1ULL << TRACK_CURVE) | (1ULL << TRACK_HELIX_SMALL) | (1ULL << TRACK_BRAKES) | (1ULL << TRACK_ON_RIDE_PHOTO) | (1ULL << TRACK_BLOCK_BRAKES) | (1ULL << TRACK_SLOPE_ROLL_BANKING) | (1ULL << TRACK_LIFT_HILL) | (1ULL << TRACK_LIFT_HILL_CURVED)),
    SET_FIELD(ExtraTrackPieces, (1ULL << TRACK_BOOSTER)),
    SET_FIELD(TrackPaintFunction, get_track_paint_function_mini_rc),
    SET_FIELD(Flags, RIDE_TYPE_FLAGS_TRACK_HAS_3_COLOURS | RIDE_TYPE_FLAG_HAS_LEAVE_WHEN_ANOTHER_VEHICLE_ARRIVES_AT_STATION |
                     RIDE_TYPE_FLAGS_COMMON_COASTER | RIDE_TYPE_FLAGS_COMMON_COASTER_NON_ALT | RIDE_TYPE_FLAG_HAS_LARGE_CURVES |
                     RIDE_TYPE_FLAG_PEEP_CHECK_GFORCES | RIDE_TYPE_FLAG_ALLOW_MULTIPLE_CIRCUITS),
    SET_FIELD(NameConvention, { RIDE_COMPONENT_TYPE_TRAIN, RIDE_COMPONENT_TYPE_TRACK, RIDE_COMPONENT_TYPE_STATION }),
};
// clang-format on
