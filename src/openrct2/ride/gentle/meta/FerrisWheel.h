/*****************************************************************************
 * Copyright (c) 2014-2023 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../../../sprites.h"
#include "../../RideData.h"
#include "../../ShopItem.h"
#include "../../Track.h"

// clang-format off
constexpr const RideTypeDescriptor FerrisWheelRTD =
{
    SET_FIELD(AlternateType, RIDE_TYPE_NULL),
    SET_FIELD(Category, RIDE_CATEGORY_GENTLE),
    SET_FIELD(EnabledTrackPieces, {}),
    SET_FIELD(ExtraTrackPieces, {}),
    SET_FIELD(CoveredTrackPieces, {}),
    SET_FIELD(StartTrackPiece, TrackElemType::FlatTrack1x4C),
    SET_FIELD(TrackPaintFunction, GetTrackPaintFunctionFerrisWheel),
    SET_FIELD(Flags, { {
            RideTypeFlags::HasTrackColourMain,
            RideTypeFlags::HasSinglePieceStation,
            RideTypeFlags::CannotHaveGaps,
            RideTypeFlags::HasNoTrack,
            RideTypeFlags::VehicleIsIntegral,
            RideTypeFlags::TrackNoWalls,
            RideTypeFlags::FlatRide,
            RideTypeFlags::HasVehicleColours,
            RideTypeFlags::AllowMusic,
            RideTypeFlags::HasEntranceExit,
            RideTypeFlags::SingleSession,
            RideTypeFlags::SlightlyInterestingToLookAt,
            RideTypeFlags::ListVehiclesSeparately } } ),
    SET_FIELD(RideModes, EnumsToFlags(RideMode::ForwardRotation, RideMode::BackwardRotation)),
    SET_FIELD(DefaultMode, RideMode::ForwardRotation),
    SET_FIELD(OperatingSettings, { 1, 3, 0, 0, 0, 0 }),
    SET_FIELD(Naming, { STR_RIDE_NAME_FERRIS_WHEEL, STR_RIDE_DESCRIPTION_FERRIS_WHEEL }),
    SET_FIELD(NameConvention, { RideComponentType::Wheel, RideComponentType::Structure, RideComponentType::Station }),
    SET_FIELD(EnumName, nameof(RIDE_TYPE_FERRIS_WHEEL)),
    SET_FIELD(AvailableBreakdowns, (1 << BREAKDOWN_SAFETY_CUT_OUT)),
    SET_FIELD(Heights, { 16, 176, 3, 2, }),
    SET_FIELD(MaxMass, 255),
    SET_FIELD(LiftData, { OpenRCT2::Audio::SoundId::Null, 5, 5 }),
    SET_FIELD(RatingsCalculationFunction, ride_ratings_calculate_ferris_wheel),
    SET_FIELD(RatingsMultipliers, { 60, 20, 10 }),
    SET_FIELD(UpkeepCosts, { 50, 1, 0, 0, 0, 0 }),
    SET_FIELD(BuildCosts, { 112.50_GBP, 1.00_GBP, 1, }),
    SET_FIELD(DefaultPrices, { 10, 0 }),
    SET_FIELD(DefaultMusic, MUSIC_OBJECT_SUMMER),
    SET_FIELD(PhotoItem, ShopItem::Photo),
    SET_FIELD(BonusValue, 45),
    SET_FIELD(ColourPresets, TRACK_COLOUR_PRESETS(
        { COLOUR_BLACK, COLOUR_BLACK, COLOUR_BLACK },
        { COLOUR_GREY, COLOUR_GREY, COLOUR_GREY },
    )),
    SET_FIELD(ColourPreview, { SPR_RIDE_DESIGN_PREVIEW_FERRIS_WHEEL_TRACK, 0 }),
    SET_FIELD(ColourKey, RideColourKey::Ride),
    SET_FIELD(Name, "ferris_wheel"),
};
// clang-format on
