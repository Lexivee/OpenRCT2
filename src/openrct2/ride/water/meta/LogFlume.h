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
constexpr const RideTypeDescriptor LogFlumeRTD =
{
    SET_FIELD(AlternateType, RIDE_TYPE_NULL),
    SET_FIELD(Category, RIDE_CATEGORY_WATER),
    SET_FIELD(EnabledTrackPieces, {TRACK_STRAIGHT, TRACK_STATION_END, TRACK_SLOPE, TRACK_S_BEND, TRACK_CURVE_SMALL, TRACK_ON_RIDE_PHOTO, TRACK_LOG_FLUME_REVERSER, TRACK_SLOPE_STEEP_DOWN}),
    SET_FIELD(ExtraTrackPieces, {}),
    SET_FIELD(CoveredTrackPieces, {}),
    SET_FIELD(StartTrackPiece, TrackElemType::EndStation),
    SET_FIELD(TrackPaintFunction, get_track_paint_function_log_flume),
    SET_FIELD(Flags, { TrackHasThreeColours, {
        RideTypeFlags::HasLeaveWhenAnotherVehicleArrivesAtStation,
        RideTypeFlags::CanSynchroniseAdjacentStations,
        RideTypeFlags::HasDataLogging,
        RideTypeFlags::HasDrops,
        RideTypeFlags::HasLoadOptions,
        RideTypeFlags::PeepWillRideAgain,
        RideTypeFlags::HasVehicleColours,
        RideTypeFlags::HasTrack,
        RideTypeFlags::SupportsMultipleTrackColour,
        RideTypeFlags::AllowMusic,
        RideTypeFlags::HasEntranceExit,
        RideTypeFlags::AllowMoreVehiclesThanStationFits,
        RideTypeFlags::HasAirTime,
        RideTypeFlags::ShowInTrackDesigner,
        RideTypeFlags::InterestingToLookAt } } ),
    SET_FIELD(RideModes, EnumsToFlags(RideMode::ContinuousCircuit)),
    SET_FIELD(DefaultMode, RideMode::ContinuousCircuit),
    SET_FIELD(OperatingSettings, { 0, 0, 0, 0, 0, 0 }),
    SET_FIELD(Naming, { STR_RIDE_NAME_LOG_FLUME, STR_RIDE_DESCRIPTION_LOG_FLUME }),
    SET_FIELD(NameConvention, { RideComponentType::Boat, RideComponentType::Track, RideComponentType::Station }),
    SET_FIELD(EnumName, nameof(RIDE_TYPE_LOG_FLUME)),
    SET_FIELD(AvailableBreakdowns, (1 << BREAKDOWN_SAFETY_CUT_OUT) | (1 << BREAKDOWN_CONTROL_FAILURE)),
    SET_FIELD(Heights, { 10, 24, 7, 9, }),
    SET_FIELD(MaxMass, 255),
    SET_FIELD(LiftData, { OpenRCT2::Audio::SoundId::Null, 5, 5 }),
    SET_FIELD(RatingsCalculationFunction, ride_ratings_calculate_log_flume),
    SET_FIELD(RatingsMultipliers, { 80, 34, 6 }),
    SET_FIELD(UpkeepCosts, { 80, 20, 0, 9, 0, 10 }),
    SET_FIELD(BuildCosts, { 22.50_GBP, 2.50_GBP, 40, }),
    SET_FIELD(DefaultPrices, { 20, 20 }),
    SET_FIELD(DefaultMusic, MUSIC_OBJECT_WATER),
    SET_FIELD(PhotoItem, ShopItem::Photo4),
    SET_FIELD(BonusValue, 65),
    SET_FIELD(ColourPresets, TRACK_COLOUR_PRESETS(
        { COLOUR_DARK_BROWN, COLOUR_DARK_BROWN, COLOUR_GREY },
        { COLOUR_OLIVE_GREEN, COLOUR_BLACK, COLOUR_GREY },
    )),
    SET_FIELD(ColourPreview, { SPR_RIDE_DESIGN_PREVIEW_LOG_FLUME_TRACK, SPR_RIDE_DESIGN_PREVIEW_LOG_FLUME_SUPPORTS }),
    SET_FIELD(ColourKey, RideColourKey::Ride),
    SET_FIELD(Name, "log_flume"),
    SET_FIELD(UpdateRotating, UpdateRotatingDefault),
    SET_FIELD(LightFXAddLightsMagicVehicle, nullptr),
    SET_FIELD(StartRideMusic, OpenRCT2::RideAudio::DefaultStartRideMusicChannel),
    SET_FIELD(DesignCreateMode, TrackDesignCreateMode::Default),
    SET_FIELD(MusicUpdateFunction, DefaultMusicUpdate),
    SET_FIELD(Classification, RideClassification::Ride),
    SET_FIELD(UpdateLeaveEntrance, PeepUpdateRideLeaveEntranceDefault),
    SET_FIELD(SpecialElementRatingAdjustment, SpecialTrackElementRatingsAjustment_LogFlume),
};
// clang-format on
