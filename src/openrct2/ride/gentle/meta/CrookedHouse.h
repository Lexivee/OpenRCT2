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
constexpr const RideTypeDescriptor CrookedHouseRTD =
{
    SET_FIELD(AlternateType, RIDE_TYPE_NULL),
    SET_FIELD(Category, RIDE_CATEGORY_GENTLE),
    SET_FIELD(EnabledTrackPieces, {}),
    SET_FIELD(ExtraTrackPieces, {}),
    SET_FIELD(CoveredTrackPieces, {}),
    SET_FIELD(StartTrackPiece, TrackElemType::FlatTrack3x3),
    SET_FIELD(TrackPaintFunction, GetTrackPaintFunctionCrookedHouse),
    SET_FIELD(Flags, {
        RideTypeFlags::HasSinglePieceStation,
        RideTypeFlags::CannotHaveGaps,
        RideTypeFlags::HasLoadOptions,
        RideTypeFlags::HasNoTrack,
        RideTypeFlags::VehicleIsIntegral,
        RideTypeFlags::TrackNoWalls,
        RideTypeFlags::FlatRide,
        RideTypeFlags::InRide,
        RideTypeFlags::AllowMusic,
        RideTypeFlags::HasEntranceExit,
        RideTypeFlags::SingleSession,
        RideTypeFlags::ListVehiclesSeparately } ),
    SET_FIELD(RideModes, EnumsToFlags(RideMode::CrookedHouse)),
    SET_FIELD(DefaultMode, RideMode::CrookedHouse),
    SET_FIELD(OperatingSettings, { 0, 0, 0, 0, 0, 0 }),
    SET_FIELD(Naming, { STR_RIDE_NAME_CROOKED_HOUSE, STR_RIDE_DESCRIPTION_CROOKED_HOUSE }),
    SET_FIELD(NameConvention, { RideComponentType::Building, RideComponentType::Structure, RideComponentType::Station }),
    SET_FIELD(EnumName, nameof(RIDE_TYPE_CROOKED_HOUSE)),
    SET_FIELD(AvailableBreakdowns, 0),
    SET_FIELD(Heights, { 16, 96, 3, 2, }),
    SET_FIELD(MaxMass, 255),
    SET_FIELD(LiftData, { OpenRCT2::Audio::SoundId::Null, 5, 5 }),
    SET_FIELD(RatingsCalculationFunction, ride_ratings_calculate_crooked_house),
    SET_FIELD(RatingsMultipliers, { 15, 8, 0 }),
    SET_FIELD(UpkeepCosts, { 30, 1, 0, 0, 0, 0 }),
    SET_FIELD(BuildCosts, { 32.50_GBP, 1.00_GBP, 1, }),
    SET_FIELD(DefaultPrices, { 6, 0 }),
    SET_FIELD(DefaultMusic, MUSIC_OBJECT_GENTLE),
    SET_FIELD(PhotoItem, ShopItem::Photo),
    SET_FIELD(BonusValue, 22),
    SET_FIELD(ColourPresets, TRACK_COLOUR_PRESETS(
        { COLOUR_BRIGHT_RED, COLOUR_BLACK, COLOUR_GREY },
    )),
    SET_FIELD(ColourPreview, { 0, 0 }),
    SET_FIELD(ColourKey, RideColourKey::Ride),
    SET_FIELD(Name, "crooked_house"),
};
// clang-format on
