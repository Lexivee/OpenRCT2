/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include <openrct2/common.h>
#include <openrct2/ride/Ride.h>

const_utf8string RideNames[RCT2_RIDE_TYPE_COUNT] = {
    "SpiralRollerCoaster",
    "StandUpRollerCoaster",
    "SuspendedSwingingCoaster",
    "InvertedRollerCoaster",
    "JuniorRollerCoaster",
    "MiniatureRailway",
    "Monorail",
    "MiniSuspendedCoaster",
    "BoatHire",
    "WoodenWildMouse",
    "Steeplechase",
    "CarRide",
    "LaunchedFreefall",
    "BobsleighCoaster",
    "ObservationTower",
    "LoopingRollerCoaster",
    "DinghySlide",
    "MineTrainCoaster",
    "Chairlift",
    "CorkscrewRollerCoaster",
    "Maze",
    "SpiralSlide",
    "GoKarts",
    "LogFlume",
    "RiverRapids",
    "Dodgems",
    "SwingingShip",
    "SwingingInverterShip",
    "FoodStall",
    "1D",
    "DrinkStall",
    "1F",
    "Shop",
    "MerryGoRound",
    "22",
    "InformationKiosk",
    "Toilets",
    "FerrisWheel",
    "MotionSimulator",
    "3DCinema",
    "TopSpin",
    "SpaceRings",
    "ReverseFreefallCoaster",
    "Lift",
    "VerticalDropRollerCoaster",
    "CashMachine",
    "Twist",
    "HauntedHouse",
    "FirstAid",
    "Circus",
    "GhostTrain",
    "TwisterRollerCoaster",
    "WoodenRollerCoaster",
    "SideFrictionRollerCoaster",
    "WildMouse",
    "MultiDimensionRollerCoaster",
    "MultiDimensionRollerCoasterAlt",
    "FlyingRollerCoaster",
    "FlyingRollerCoasterAlt",
    "VirginiaReel",
    "SplashBoats",
    "MiniHelicopters",
    "LayDownRollerCoaster",
    "SuspendedMonorail",
    "LayDownRollerCoasterAlt",
    "ReverserRollerCoaster",
    "HeartlineTwisterCoaster",
    "MiniGolf",
    "GigaCoaster",
    "RoToDrop",
    "FlyingSaucers",
    "CrookedHouse",
    "MonorailCycles",
    "CompactInvertedCoaster",
    "WaterCoaster",
    "AirPoweredVerticalCoaster",
    "InvertedHairpinCoaster",
    "MagicCarpet",
    "SubmarineRide",
    "RiverRafts",
    "50",
    "Enterprise",
    "52",
    "53",
    "54",
    "55",
    "InvertedImpulseCoaster",
    "MiniRollerCoaster",
    "MineRide",
    "59",
    "LimLaunchedRollerCoaster",
};

const_utf8string TrackNames[256] = {
    "Flat",
    "EndStation",
    "BeginStation",
    "MiddleStation",
    "25DegUp",
    "60DegUp",
    "FlatTo25DegUp",
    "25DegUpTo60DegUp",
    "60DegUpTo25DegUp",
    "25DegUpToFlat",
    "25DegDown",
    "60DegDown",
    "FlatTo25DegDown",
    "25DegDownTo60DegDown",
    "60DegDownTo25DegDown",
    "25DegDownToFlat",
    "LeftQuarterTurn5Tiles",
    "RightQuarterTurn5Tiles",
    "FlatToLeftBank",
    "FlatToRightBank",
    "LeftBankToFlat",
    "RightBankToFlat",
    "BankedLeftQuarterTurn5Tiles",
    "BankedRightQuarterTurn5Tiles",
    "LeftBankTo25DegUp",
    "RightBankTo25DegUp",
    "25DegUpToLeftBank",
    "25DegUpToRightBank",
    "LeftBankTo25DegDown",
    "RightBankTo25DegDown",
    "25DegDownToLeftBank",
    "25DegDownToRightBank",
    "LeftBank",
    "RightBank",
    "LeftQuarterTurn5Tiles25DegUp",
    "RightQuarterTurn5Tiles25DegUp",
    "LeftQuarterTurn5Tiles25DegDown",
    "RightQuarterTurn5Tiles25DegDown",
    "SBendLeft",
    "SBendRight",
    "LeftVerticalLoop",
    "RightVerticalLoop",
    "LeftQuarterTurn3Tiles",
    "RightQuarterTurn3Tiles",
    "LeftQuarterTurn3TilesBank",
    "RightQuarterTurn3TilesBank",
    "LeftQuarterTurn3Tiles25DegUp",
    "RightQuarterTurn3Tiles25DegUp",
    "LeftQuarterTurn3Tiles25DegDown",
    "RightQuarterTurn3Tiles25DegDown",
    "LeftQuarterTurn1Tile",
    "RightQuarterTurn1Tile",
    "LeftTwistDownToUp",
    "RightTwistDownToUp",
    "LeftTwistUpToDown",
    "RightTwistUpToDown",
    "HalfLoopUp",
    "HalfLoopDown",
    "LeftCorkscrewUp",
    "RightCorkscrewUp",
    "LeftCorkscrewDown",
    "RightCorkscrewDown",
    "FlatTo60DegUp",
    "60DegUpToFlat",
    "FlatTo60DegDown",
    "60DegDownToFlat",
    "TowerBase",
    "TowerSection",
    "FlatCovered",
    "25DegUpCovered",
    "60DegUpCovered",
    "FlatTo25DegUpCovered",
    "25DegUpTo60DegUpCovered",
    "60DegUpTo25DegUpCovered",
    "25DegUpToFlatCovered",
    "25DegDownCovered",
    "60DegDownCovered",
    "FlatTo25DegDownCovered",
    "25DegDownTo60DegDownCovered",
    "60DegDownTo25DegDownCovered",
    "25DegDownToFlatCovered",
    "LeftQuarterTurn5TilesCovered",
    "RightQuarterTurn5TilesCovered",
    "SBendLeftCovered",
    "SBendRightCovered",
    "LeftQuarterTurn3TilesCovered",
    "RightQuarterTurn3TilesCovered",
    "LeftHalfBankedHelixUpSmall",
    "RightHalfBankedHelixUpSmall",
    "LeftHalfBankedHelixDownSmall",
    "RightHalfBankedHelixDownSmall",
    "LeftHalfBankedHelixUpLarge",
    "RightHalfBankedHelixUpLarge",
    "LeftHalfBankedHelixDownLarge",
    "RightHalfBankedHelixDownLarge",
    "LeftQuarterTurn1Tile60DegUp",
    "RightQuarterTurn1Tile60DegUp",
    "LeftQuarterTurn1Tile60DegDown",
    "RightQuarterTurn1Tile60DegDown",
    "Brakes",
    "RotationControlToggle",
    "Inverted90DegUpToFlatQuarterLoop_Maze",
    "LeftQuarterBankedHelixLargeUp",
    "RightQuarterBankedHelixLargeUp",
    "LeftQuarterBankedHelixLargeDown",
    "RightQuarterBankedHelixLargeDown",
    "LeftQuarterHelixLargeUp",
    "RightQuarterHelixLargeUp",
    "LeftQuarterHelixLargeDown",
    "RightQuarterHelixLargeDown",
    "25DegUpLeftBanked",
    "25DegUpRightBanked",
    "Waterfall",
    "Rapids",
    "OnRidePhoto",
    "25DegDownLeftBanked",
    "25DegDownRightBanked",
    "WaterSplash",
    "FlatTo60DegUpLongBase",
    "60DegUpToFlatLongBase",
    "Whirlpool",
    "Down60ToFlatLongBase",
    "FlatToDown60LongBase",
    "CableLiftHill",
    "ReverseWhoaBellySlope",
    "ReverseWhoaBellyVertical",
    "90DegUp",
    "90DegDown",
    "60DegUpTo90DegUp",
    "90DegDownTo60DegDown",
    "90DegUpTo60DegUp",
    "60DegDownTo90DegDown",
    "BrakeForDrop",
    "LeftEighthToDiag",
    "RightEighthToDiag",
    "LeftEighthToOrthogonal",
    "RightEighthToOrthogonal",
    "LeftEighthBankToDiag",
    "RightEighthBankToDiag",
    "LeftEighthBankToOrthogonal",
    "RightEighthBankToOrthogonal",
    "DiagFlat",
    "Diag25DegUp",
    "Diag60DegUp",
    "DiagFlatTo25DegUp",
    "Diag25DegUpTo60DegUp",
    "Diag60DegUpTo25DegUp",
    "Diag25DegUpToFlat",
    "Diag25DegDown",
    "Diag60DegDown",
    "DiagFlatTo25DegDown",
    "Diag25DegDownTo60DegDown",
    "Diag60DegDownTo25DegDown",
    "Diag25DegDownToFlat",
    "DiagFlatTo60DegUp",
    "Diag60DegUpToFlat",
    "DiagFlatTo60DegDown",
    "Diag60DegDownToFlat",
    "DiagFlatToLeftBank",
    "DiagFlatToRightBank",
    "DiagLeftBankToFlat",
    "DiagRightBankToFlat",
    "DiagLeftBankTo25DegUp",
    "DiagRightBankTo25DegUp",
    "Diag25DegUpToLeftBank",
    "Diag25DegUpToRightBank",
    "DiagLeftBankTo25DegDown",
    "DiagRightBankTo25DegDown",
    "Diag25DegDownToLeftBank",
    "Diag25DegDownToRightBank",
    "DiagLeftBank",
    "DiagRightBank",
    "LogFlumeReverser",
    "SpinningTunnel",
    "LeftBarrelRollUpToDown",
    "RightBarrelRollUpToDown",
    "LeftBarrelRollDownToUp",
    "RightBarrelRollDownToUp",
    "LeftBankToLeftQuarterTurn3Tiles25DegUp",
    "RightBankToRightQuarterTurn3Tiles25DegUp",
    "LeftQuarterTurn3Tiles25DegDownToLeftBank",
    "RightQuarterTurn3Tiles25DegDownToRightBank",
    "PoweredLift",
    "LeftLargeHalfLoopUp",
    "RightLargeHalfLoopUp",
    "RightLargeHalfLoopDown",
    "LeftLargeHalfLoopDown",
    "LeftFlyerTwistUp",
    "RightFlyerTwistUp",
    "LeftFlyerTwistDown",
    "RightFlyerTwistDown",
    "FlyerHalfLoopUp",
    "FlyerHalfLoopDown",
    "LeftFlyerCorkscrewUp",
    "RightFlyerCorkscrewUp",
    "LeftFlyerCorkscrewDown",
    "RightFlyerCorkscrewDown",
    "HeartlineTransferUp",
    "HeartlineTransferDown",
    "LeftHeartlineRoll",
    "RightHeartlineRoll",
    "MiniGolfHoleA",
    "MiniGolfHoleB",
    "MiniGolfHoleC",
    "MiniGolfHoleD",
    "MiniGolfHoleE",
    "MultidimInvertedFlatTo90DegQuarterLoopDown",
    "90DegToInvertedFlatQuarterLoopUp",
    "InvertedFlatTo90DegQuarterLoopDown",
    "LeftCurvedLiftHill",
    "RightCurvedLiftHill",
    "LeftReverser",
    "RightReverser",
    "AirThrustTopCap",
    "AirThrustVerticalDown",
    "AirThrustVerticalDownToLevel",
    "BlockBrakes",
    "LeftBankedQuarterTurn3Tile25DegUp",
    "RightBankedQuarterTurn3Tile25DegUp",
    "LeftBankedQuarterTurn3Tile25DegDown",
    "RightBankedQuarterTurn3Tile25DegDown",
    "LeftBankedQuarterTurn5Tile25DegUp",
    "RightBankedQuarterTurn5Tile25DegUp",
    "LeftBankedQuarterTurn5Tile25DegDown",
    "RightBankedQuarterTurn5Tile25DegDown",
    "25DegUpToLeftBanked25DegUp",
    "25DegUpToRightBanked25DegUp",
    "LeftBanked25DegUpTo25DegUp",
    "RightBanked25DegUpTo25DegUp",
    "25DegDownToLeftBanked25DegDown",
    "25DegDownToRightBanked25DegDown",
    "LeftBanked25DegDownTo25DegDown",
    "RightBanked25DegDownTo25DegDown",
    "LeftBankedFlatToLeftBanked25DegUp",
    "RightBankedFlatToRightBanked25DegUp",
    "LeftBanked25DegUpToLeftBankedFlat",
    "RightBanked25DegUpToRightBankedFlat",
    "LeftBankedFlatToLeftBanked25DegDown",
    "RightBankedFlatToRightBanked25DegDown",
    "LeftBanked25DegDownToLeftBankedFlat",
    "RightBanked25DegDownToRightBankedFlat",
    "FlatToLeftBanked25DegUp",
    "FlatToRightBanked25DegUp",
    "LeftBanked25DegUpToFlat",
    "RightBanked25DegUpToFlat",
    "FlatToLeftBanked25DegDown",
    "FlatToRightBanked25DegDown",
    "LeftBanked25DegDownToFlat",
    "RightBanked25DegDownToFlat",
    "LeftQuarterTurn1Tile90DegUp",
    "RightQuarterTurn1Tile90DegUp",
    "LeftQuarterTurn1Tile90DegDown",
    "RightQuarterTurn1Tile90DegDown",
    "Multidim90DegUpToInvertedFlatQuarterLoop",
    "MultidimFlatTo90DegDownQuarterLoop",
    "Elem255",
};

// clang-format off
const_utf8string FlatTrackNames[256] = {
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "1x4_A",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "2x2",
    "4x4",
    "",
    "",
    "",
    "",
    "1x5",
    "",
    "1x1_A",
    "1x4_B",
    "",
    "1x1_B",
    "1x4_C",
    "3x3",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};
// clang-format on

const_utf8string TrackElemNames[] = {
    "TrackElemType::Flat",
    "TrackElemType::EndStation",
    "TrackElemType::BeginStation",
    "TrackElemType::MiddleStation",
    "TrackElemType::Up25",
    "TrackElemType::Up60",
    "TrackElemType::FlatToUp25",
    "TrackElemType::Up25ToUp60",
    "TrackElemType::Up60ToUp25",
    "TrackElemType::Up25ToFlat",
    "TrackElemType::Down25",
    "TrackElemType::Down60",
    "TrackElemType::FlatToDown25",
    "TrackElemType::Down25ToDown60",
    "TrackElemType::Down60ToDown25",
    "TrackElemType::Down25ToFlat",
    "TrackElemType::LeftQuarterTurn5Tiles",
    "TrackElemType::RightQuarterTurn5Tiles",
    "TrackElemType::FlatToLeftBank",
    "TrackElemType::FlatToRightBank",
    "TrackElemType::LeftBankToFlat",
    "TrackElemType::RightBankToFlat",
    "TrackElemType::BankedLeftQuarterTurn5Tiles",
    "TrackElemType::BankedRightQuarterTurn5Tiles",
    "TrackElemType::LeftBankToUp25",
    "TrackElemType::RightBankToUp25",
    "TrackElemType::Up25ToLeftBank",
    "TrackElemType::Up25ToRightBank",
    "TrackElemType::LeftBankToDown25",
    "TrackElemType::RightBankToDown25",
    "TrackElemType::Down25ToLeftBank",
    "TrackElemType::Down25ToRightBank",
    "TrackElemType::LeftBank",
    "TrackElemType::RightBank",
    "TrackElemType::LeftQuarterTurn5TilesUp25",
    "TrackElemType::RightQuarterTurn5TilesUp25",
    "TrackElemType::LeftQuarterTurn5TilesDown25",
    "TrackElemType::RightQuarterTurn5TilesDown25",
    "TrackElemType::SBendLeft",
    "TrackElemType::SBendRight",
    "TrackElemType::LeftVerticalLoop",
    "TrackElemType::RightVerticalLoop",
    "TrackElemType::LeftQuarterTurn3Tiles",
    "TrackElemType::RightQuarterTurn3Tiles",
    "TrackElemType::LeftBankedQuarterTurn3Tiles",
    "TrackElemType::RightBankedQuarterTurn3Tiles",
    "TrackElemType::LeftQuarterTurn3TilesUp25",
    "TrackElemType::RightQuarterTurn3TilesUp25",
    "TrackElemType::LeftQuarterTurn3TilesDown25",
    "TrackElemType::RightQuarterTurn3TilesDown25",
    "TrackElemType::LeftQuarterTurn1Tile",
    "TrackElemType::RightQuarterTurn1Tile",
    "TrackElemType::LeftTwistDownToUp",
    "TrackElemType::RightTwistDownToUp",
    "TrackElemType::LeftTwistUpToDown",
    "TrackElemType::RightTwistUpToDown",
    "TrackElemType::HalfLoopUp",
    "TrackElemType::HalfLoopDown",
    "TrackElemType::LeftCorkscrewUp",
    "TrackElemType::RightCorkscrewUp",
    "TrackElemType::LeftCorkscrewDown",
    "TrackElemType::RightCorkscrewDown",
    "TrackElemType::FlatToUp60",
    "TrackElemType::Up60ToFlat",
    "TrackElemType::FlatToDown60",
    "TrackElemType::Down60ToFlat",
    "TrackElemType::TowerBase",
    "TrackElemType::TowerSection",
    "TrackElemType::FlatCovered",
    "TrackElemType::Up25Covered",
    "TrackElemType::Up60Covered",
    "TrackElemType::FlatToUp25Covered",
    "TrackElemType::Up25ToUp60Covered",
    "TrackElemType::Up60ToUp25Covered",
    "TrackElemType::Up25ToFlatCovered",
    "TrackElemType::Down25Covered",
    "TrackElemType::Down60Covered",
    "TrackElemType::FlatToDown25Covered",
    "TrackElemType::Down25ToDown60Covered",
    "TrackElemType::Down60ToDown25Covered",
    "TrackElemType::Down25ToFlatCovered",
    "TrackElemType::LeftQuarterTurn5TilesCovered",
    "TrackElemType::RightQuarterTurn5TilesCovered",
    "TrackElemType::SBendLeftCovered",
    "TrackElemType::SBendRightCovered",
    "TrackElemType::LeftQuarterTurn3TilesCovered",
    "TrackElemType::RightQuarterTurn3TilesCovered",
    "TrackElemType::LeftHalfBankedHelixUpSmall",
    "TrackElemType::RightHalfBankedHelixUpSmall",
    "TrackElemType::LeftHalfBankedHelixDownSmall",
    "TrackElemType::RightHalfBankedHelixDownSmall",
    "TrackElemType::LeftHalfBankedHelixUpLarge",
    "TrackElemType::RightHalfBankedHelixUpLarge",
    "TrackElemType::LeftHalfBankedHelixDownLarge",
    "TrackElemType::RightHalfBankedHelixDownLarge",
    "TrackElemType::LeftQuarterTurn1TileUp60",
    "TrackElemType::RightQuarterTurn1TileUp60",
    "TrackElemType::LeftQuarterTurn1TileDown60",
    "TrackElemType::RightQuarterTurn1TileDown60",
    "TrackElemType::Brakes",
    "TrackElemType::RotationControlToggle",
    "TrackElemType::InvertedUp90ToFlatQuarterLoopAlias",
    "TrackElemType::LeftQuarterBankedHelixLargeUp",
    "TrackElemType::RightQuarterBankedHelixLargeUp",
    "TrackElemType::LeftQuarterBankedHelixLargeDown",
    "TrackElemType::RightQuarterBankedHelixLargeDown",
    "TrackElemType::LeftQuarterHelixLargeUp",
    "TrackElemType::RightQuarterHelixLargeUp",
    "TrackElemType::LeftQuarterHelixLargeDown",
    "TrackElemType::RightQuarterHelixLargeDown",
    "TrackElemType::Up25LeftBanked",
    "TrackElemType::Up25RightBanked",
    "TrackElemType::Waterfall",
    "TrackElemType::Rapids",
    "TrackElemType::OnRidePhoto",
    "TrackElemType::Down25LeftBanked",
    "TrackElemType::Down25RightBanked",
    "TrackElemType::Watersplash",
    "TrackElemType::FlatToUp60LongBase",
    "TrackElemType::Up60ToFlatLongBase",
    "TrackElemType::Whirlpool",
    "TrackElemType::Down60ToFlatLongBase",
    "TrackElemType::FlatToDown60LongBase",
    "TrackElemType::CableLiftHill",
    "TrackElemType::ReverseFreefallSlope",
    "TrackElemType::ReverseFreefallVertical",
    "TrackElemType::Up90",
    "TrackElemType::Down90",
    "TrackElemType::Up60ToUp90",
    "TrackElemType::Down90ToDown60",
    "TrackElemType::Up90ToUp60",
    "TrackElemType::Down60ToDown90",
    "TrackElemType::BrakeForDrop",
    "TrackElemType::LeftEighthToDiag",
    "TrackElemType::RightEighthToDiag",
    "TrackElemType::LeftEighthToOrthogonal",
    "TrackElemType::RightEighthToOrthogonal",
    "TrackElemType::LeftEighthBankToDiag",
    "TrackElemType::RightEighthBankToDiag",
    "TrackElemType::LeftEighthBankToOrthogonal",
    "TrackElemType::RightEighthBankToOrthogonal",
    "TrackElemType::DiagFlat",
    "TrackElemType::DiagUp25",
    "TrackElemType::DiagUp60",
    "TrackElemType::DiagFlatToUp25",
    "TrackElemType::DiagUp25ToUp60",
    "TrackElemType::DiagUp60ToUp25",
    "TrackElemType::DiagUp25ToFlat",
    "TrackElemType::DiagDown25",
    "TrackElemType::DiagDown60",
    "TrackElemType::DiagFlatToDown25",
    "TrackElemType::DiagDown25ToDown60",
    "TrackElemType::DiagDown60ToDown25",
    "TrackElemType::DiagDown25ToFlat",
    "TrackElemType::DiagFlatToUp60",
    "TrackElemType::DiagUp60ToFlat",
    "TrackElemType::DiagFlatToDown60",
    "TrackElemType::DiagDown60ToFlat",
    "TrackElemType::DiagFlatToLeftBank",
    "TrackElemType::DiagFlatToRightBank",
    "TrackElemType::DiagLeftBankToFlat",
    "TrackElemType::DiagRightBankToFlat",
    "TrackElemType::DiagLeftBankToUp25",
    "TrackElemType::DiagRightBankToUp25",
    "TrackElemType::DiagUp25ToLeftBank",
    "TrackElemType::DiagUp25ToRightBank",
    "TrackElemType::DiagLeftBankToDown25",
    "TrackElemType::DiagRightBankToDown25",
    "TrackElemType::DiagDown25ToLeftBank",
    "TrackElemType::DiagDown25ToRightBank",
    "TrackElemType::DiagLeftBank",
    "TrackElemType::DiagRightBank",
    "TrackElemType::LogFlumeReverser",
    "TrackElemType::SpinningTunnel",
    "TrackElemType::LeftBarrelRollUpToDown",
    "TrackElemType::RightBarrelRollUpToDown",
    "TrackElemType::LeftBarrelRollDownToUp",
    "TrackElemType::RightBarrelRollDownToUp",
    "TrackElemType::LeftBankToLeftQuarterTurn3TilesUp25",
    "TrackElemType::RightBankToRightQuarterTurn3TilesUp25",
    "TrackElemType::LeftQuarterTurn3TilesDown25ToLeftBank",
    "TrackElemType::RightQuarterTurn3TilesDown25ToRightBank",
    "TrackElemType::PoweredLift",
    "TrackElemType::LeftLargeHalfLoopUp",
    "TrackElemType::RightLargeHalfLoopUp",
    "TrackElemType::RightLargeHalfLoopDown",
    "TrackElemType::LeftLargeHalfLoopDown",
    "TrackElemType::LeftFlyerTwistUp",
    "TrackElemType::RightFlyerTwistUp",
    "TrackElemType::LeftFlyerTwistDown",
    "TrackElemType::RightFlyerTwistDown",
    "TrackElemType::FlyerHalfLoopUp",
    "TrackElemType::FlyerHalfLoopDown",
    "TrackElemType::LeftFlyerCorkscrewUp",
    "TrackElemType::RightFlyerCorkscrewUp",
    "TrackElemType::LeftFlyerCorkscrewDown",
    "TrackElemType::RightFlyerCorkscrewDown",
    "TrackElemType::HeartLineTransferUp",
    "TrackElemType::HeartLineTransferDown",
    "TrackElemType::LeftHeartLineRoll",
    "TrackElemType::RightHeartLineRoll",
    "TrackElemType::MinigolfHoleA",
    "TrackElemType::MinigolfHoleB",
    "TrackElemType::MinigolfHoleC",
    "TrackElemType::MinigolfHoleD",
    "TrackElemType::MinigolfHoleE",
    "TrackElemType::MultiDimInvertedFlatToDown90QuarterLoop",
    "TrackElemType::Up90ToInvertedFlatQuarterLoop",
    "TrackElemType::InvertedFlatToDown90QuarterLoop",
    "TrackElemType::LeftCurvedLiftHill",
    "TrackElemType::RightCurvedLiftHill",
    "TrackElemType::LeftReverser",
    "TrackElemType::RightReverser",
    "TrackElemType::AirThrustTopCap",
    "TrackElemType::AirThrustVerticalDown",
    "TrackElemType::AirThrustVerticalDownToLevel",
    "TrackElemType::BlockBrakes",
    "TrackElemType::LeftBankedQuarterTurn3TileUp25",
    "TrackElemType::RightBankedQuarterTurn3TileUp25",
    "TrackElemType::LeftBankedQuarterTurn3TileDown25",
    "TrackElemType::RightBankedQuarterTurn3TileDown25",
    "TrackElemType::LeftBankedQuarterTurn5TileUp25",
    "TrackElemType::RightBankedQuarterTurn5TileUp25",
    "TrackElemType::LeftBankedQuarterTurn5TileDown25",
    "TrackElemType::RightBankedQuarterTurn5TileDown25",
    "TrackElemType::Up25ToLeftBankedUp25",
    "TrackElemType::Up25ToRightBankedUp25",
    "TrackElemType::LeftBankedUp25ToUp25",
    "TrackElemType::RightBankedUp25ToUp25",
    "TrackElemType::Down25ToLeftBankedDown25",
    "TrackElemType::Down25ToRightBankedDown25",
    "TrackElemType::LeftBankedDown25ToDown25",
    "TrackElemType::RightBankedDown25ToDown25",
    "TrackElemType::LeftBankedFlatToLeftBankedUp25",
    "TrackElemType::RightBankedFlatToRightBankedUp25",
    "TrackElemType::LeftBankedUp25ToLeftBankedFlat",
    "TrackElemType::RightBankedUp25ToRightBankedFlat",
    "TrackElemType::LeftBankedFlatToLeftBankedDown25",
    "TrackElemType::RightBankedFlatToRightBankedDown25",
    "TrackElemType::LeftBankedDown25ToLeftBankedFlat",
    "TrackElemType::RightBankedDown25ToRightBankedFlat",
    "TrackElemType::FlatToLeftBankedUp25",
    "TrackElemType::FlatToRightBankedUp25",
    "TrackElemType::LeftBankedUp25ToFlat",
    "TrackElemType::RightBankedUp25ToFlat",
    "TrackElemType::FlatToLeftBankedDown25",
    "TrackElemType::FlatToRightBankedDown25",
    "TrackElemType::LeftBankedDown25ToFlat",
    "TrackElemType::RightBankedDown25ToFlat",
    "TrackElemType::LeftQuarterTurn1TileUp90",
    "TrackElemType::RightQuarterTurn1TileUp90",
    "TrackElemType::LeftQuarterTurn1TileDown90",
    "TrackElemType::RightQuarterTurn1TileDown90",
    "TrackElemType::MultiDimUp90ToInvertedFlatQuarterLoop",
    "TrackElemType::MultiDimFlatToDown90QuarterLoop",
    "TrackElemType::MultiDimInvertedUp90ToFlatQuarterLoop",
};

const_utf8string RideCodeNames[RCT2_RIDE_TYPE_COUNT] = {
    "spiral_rc",
    "stand_up_rc",
    "suspended_swinging_rc",
    "inverted_rc",
    "junior_rc",
    "miniature_railway",
    "monorail",
    "mini_suspended_rc",
    "boat_hire",
    "wooden_wild_mouse",
    "steeplechase",
    "car_ride",
    "launched_freefall",
    "bobsleigh_rc",
    "observation_tower",
    "looping_rc",
    "dinghy_slide",
    "mine_train_rc",
    "chairlift",
    "corkscrew_rc",
    "maze",
    "spiralslide",
    "go_karts",
    "log_flume",
    "river_rapids",
    "dodgems",
    "pirate_ship",
    "swinging_inverter_ship",
    "food_stall",
    "1D",
    "drink_stall",
    "1F",
    "shop",
    "merry_go_round",
    "22",
    "information_kiosk",
    "toilets",
    "ferris_wheel",
    "motion_simulator",
    "3d_cinema",
    "top_spin",
    "space_rings",
    "reverse_freefall_rc",
    "lift",
    "vertical_drop_rc",
    "cash_machine",
    "twist",
    "haunted_house",
    "first_aid",
    "circus_show",
    "ghost_train",
    "twister_rc",
    "wooden_rc",
    "side_friction_rc",
    "wild_mouse",
    "multi_dimension_rc",
    "multi_dimension_rc_alt",
    "flying_rc",
    "flying_rc_alt",
    "virginia_reel",
    "splash_boats",
    "mini_helicopters",
    "lay_down_rc",
    "suspended_monorail",
    "lay_down_rc_alt",
    "reverser_rc",
    "heartline_twister_rc",
    "mini_golf",
    "giga_rc",
    "roto_drop",
    "flying_saucers",
    "crooked_house",
    "monorail_cycles",
    "compact_inverted_rc",
    "water_rc",
    "air_powered_vertical_rc",
    "inverted_hairpin_rc",
    "magic_carpet",
    "submarine_ride",
    "river_rafts",
    "50",
    "enterprise",
    "52",
    "53",
    "54",
    "55",
    "inverted_impulse_rc",
    "mini_rc",
    "mine_ride",
    "59",
    "lim_launched_rc",
};

const_utf8string TrackCodeNames[256] = {
    "flat",
    "end_station",
    "begin_station",
    "middle_station",
    "25_deg_up",
    "60_deg_up",
    "flat_to_25_deg_up",
    "25_deg_up_to_60_deg_up",
    "60_deg_up_to_25_deg_up",
    "25_deg_up_to_flat",
    "25_deg_down",
    "60_deg_down",
    "flat_to_25_deg_down",
    "25_deg_down_to_60_deg_down",
    "60_deg_down_to_25_deg_down",
    "25_deg_down_to_flat",
    "left_quarter_turn_5",
    "right_quarter_turn_5",
    "flat_to_left_bank",
    "flat_to_right_bank",
    "left_bank_to_flat",
    "right_bank_to_flat",
    "banked_left_quarter_turn_5",
    "banked_right_quarter_turn_5",
    "left_bank_to_25_deg_up",
    "right_bank_to_25_deg_up",
    "25_deg_up_to_left_bank",
    "25_deg_up_to_right_bank",
    "left_bank_to_25_deg_down",
    "right_bank_to_25_deg_down",
    "25_deg_down_to_left_bank",
    "25_deg_down_to_right_bank",
    "left_bank",
    "right_bank",
    "left_quarter_turn_5_25_deg_up",
    "right_quarter_turn_5_25_deg_up",
    "left_quarter_turn_5_25_deg_down",
    "right_quarter_turn_5_25_deg_down",
    "s_bend_left",
    "s_bend_right",
    "left_vertical_loop",
    "right_vertical_loop",
    "left_quarter_turn_3",
    "right_quarter_turn_3",
    "left_quarter_turn_3_bank",
    "right_quarter_turn_3_bank",
    "left_quarter_turn_3_25_deg_up",
    "right_quarter_turn_3_25_deg_up",
    "left_quarter_turn_3_25_deg_down",
    "right_quarter_turn_3_25_deg_down",
    "left_quarter_turn_1",
    "right_quarter_turn_1",
    "left_twist_down_to_up",
    "right_twist_down_to_up",
    "left_twist_up_to_down",
    "right_twist_up_to_down",
    "half_loop_up",
    "half_loop_down",
    "left_corkscrew_up",
    "right_corkscrew_up",
    "left_corkscrew_down",
    "right_corkscrew_down",
    "flat_to_60_deg_up",
    "60_deg_up_to_flat",
    "flat_to_60_deg_down",
    "60_deg_down_to_flat",
    "tower_base",
    "tower_section",
    "flat_covered",
    "25_deg_up_covered",
    "60_deg_up_covered",
    "flat_to_25_deg_up_covered",
    "25_deg_up_to_60_deg_up_covered",
    "60_deg_up_to_25_deg_up_covered",
    "25_deg_up_to_flat_covered",
    "25_deg_down_covered",
    "60_deg_down_covered",
    "flat_to_25_deg_down_covered",
    "25_deg_down_to_60_deg_down_covered",
    "60_deg_down_to_25_deg_down_covered",
    "25_deg_down_to_flat_covered",
    "left_quarter_turn_5_covered",
    "right_quarter_turn_5_covered",
    "s_bend_left_covered",
    "s_bend_right_covered",
    "left_quarter_turn_3_covered",
    "right_quarter_turn_3_covered",
    "left_half_banked_helix_up_small",
    "right_half_banked_helix_up_small",
    "left_half_banked_helix_down_small",
    "right_half_banked_helix_down_small",
    "left_half_banked_helix_up_large",
    "right_half_banked_helix_up_large",
    "left_half_banked_helix_down_large",
    "right_half_banked_helix_down_large",
    "left_quarter_turn_1_60_deg_up",
    "right_quarter_turn_1_60_deg_up",
    "left_quarter_turn_1_60_deg_down",
    "right_quarter_turn_1_60_deg_down",
    "brakes",
    "rotation_control_toggle",
    "inverted_90_deg_up_to_flat_quarter_loop_maze",
    "left_quarter_banked_helix_large_up",
    "right_quarter_banked_helix_large_up",
    "left_quarter_banked_helix_large_down",
    "right_quarter_banked_helix_large_down",
    "left_quarter_helix_large_up",
    "right_quarter_helix_large_up",
    "left_quarter_helix_large_down",
    "right_quarter_helix_large_down",
    "25_deg_up_left_banked",
    "25_deg_up_right_banked",
    "waterfall",
    "rapids",
    "on_ride_photo",
    "25_deg_down_left_banked",
    "25_deg_down_right_banked",
    "water_splash",
    "flat_to_60_deg_up_long_base",
    "60_deg_up_to_flat_long_base",
    "whirlpool",
    "flat_to_60_deg_down_long_base",
    "60_deg_up_to_flat_long_base122",
    "cable_lift_hill",
    "reverse_freefall_slope",
    "reverse_freefall_vertical",
    "90_deg_up",
    "90_deg_down",
    "60_deg_up_to_90_deg_up",
    "90_deg_down_to_60_deg_down",
    "90_deg_up_to_60_deg_up",
    "60_deg_down_to_90_deg_down",
    "brake_for_drop",
    "left_eighth_to_diag",
    "right_eighth_to_diag",
    "left_eighth_to_orthogonal",
    "right_eighth_to_orthogonal",
    "left_eighth_bank_to_diag",
    "right_eighth_bank_to_diag",
    "left_eighth_bank_to_orthogonal",
    "right_eighth_bank_to_orthogonal",
    "diag_flat",
    "diag_25_deg_up",
    "diag_60_deg_up",
    "diag_flat_to_25_deg_up",
    "diag_25_deg_up_to_60_deg_up",
    "diag_60_deg_up_to_25_deg_up",
    "diag_25_deg_up_to_flat",
    "diag_25_deg_down",
    "diag_60_deg_down",
    "diag_flat_to_25_deg_down",
    "diag_25_deg_down_to_60_deg_down",
    "diag_60_deg_down_to_25_deg_down",
    "diag_25_deg_down_to_flat",
    "diag_flat_to_60_deg_up",
    "diag_60_deg_up_to_flat",
    "diag_flat_to_60_deg_down",
    "diag_60_deg_down_to_flat",
    "diag_flat_to_left_bank",
    "diag_flat_to_right_bank",
    "diag_left_bank_to_flat",
    "diag_right_bank_to_flat",
    "diag_left_bank_to_25_deg_up",
    "diag_right_bank_to_25_deg_up",
    "diag_25_deg_up_to_left_bank",
    "diag_25_deg_up_to_right_bank",
    "diag_left_bank_to_25_deg_down",
    "diag_right_bank_to_25_deg_down",
    "diag_25_deg_down_to_left_bank",
    "diag_25_deg_down_to_right_bank",
    "diag_left_bank",
    "diag_right_bank",
    "log_flume_reverser",
    "spinning_tunnel",
    "left_barrel_roll_up_to_down",
    "right_barrel_roll_up_to_down",
    "left_barrel_roll_down_to_up",
    "right_barrel_roll_down_to_up",
    "left_bank_to_left_quarter_turn_3_25_deg_up",
    "right_bank_to_right_quarter_turn_3_25_deg_up",
    "left_quarter_turn_3_25_deg_down_to_left_bank",
    "right_quarter_turn_3_25_deg_down_to_right_bank",
    "powered_lift",
    "left_large_half_loop_up",
    "right_large_half_loop_up",
    "right_large_half_loop_down",
    "left_large_half_loop_down",
    "left_flyer_twist_up",
    "right_flyer_twist_up",
    "left_flyer_twist_down",
    "right_flyer_twist_down",
    "flyer_half_loop_up",
    "flyer_half_loop_down",
    "left_flyer_corkscrew_up",
    "right_flyer_corkscrew_up",
    "left_flyer_corkscrew_down",
    "right_flyer_corkscrew_down",
    "heartline_transfer_up",
    "heartline_transfer_down",
    "left_heartline_roll",
    "right_heartline_roll",
    "mini_golf_hole_a",
    "mini_golf_hole_b",
    "mini_golf_hole_c",
    "mini_golf_hole_d",
    "mini_golf_hole_e",
    "multidim_inverted_flat_to_90_deg_quarter_loop_down",
    "90_deg_to_inverted_flat_quarter_loop_up",
    "inverted_flat_to_90_deg_quarter_loop_down",
    "left_curved_lift_hill",
    "right_curved_lift_hill",
    "left_reverser",
    "right_reverser",
    "air_thrust_top_cap",
    "air_thrust_vertical_down",
    "air_thrust_vertical_down_to_level",
    "block_brakes",
    "left_banked_quarter_turn_3_25_deg_up",
    "right_banked_quarter_turn_3_25_deg_up",
    "left_banked_quarter_turn_3_25_deg_down",
    "right_banked_quarter_turn_3_25_deg_down",
    "left_banked_quarter_turn_5_25_deg_up",
    "right_banked_quarter_turn_5_25_deg_up",
    "left_banked_quarter_turn_5_25_deg_down",
    "right_banked_quarter_turn_5_25_deg_down",
    "25_deg_up_to_left_banked_25_deg_up",
    "25_deg_up_to_right_banked_25_deg_up",
    "left_banked_25_deg_up_to_25_deg_up",
    "right_banked_25_deg_up_to_25_deg_up",
    "25_deg_down_to_left_banked_25_deg_down",
    "25_deg_down_to_right_banked_25_deg_down",
    "left_banked_25_deg_down_to_25_deg_down",
    "right_banked_25_deg_down_to_25_deg_down",
    "left_banked_flat_to_left_banked_25_deg_up",
    "right_banked_flat_to_right_banked_25_deg_up",
    "left_banked_25_deg_up_to_left_banked_flat",
    "right_banked_25_deg_up_to_right_banked_flat",
    "left_banked_flat_to_left_banked_25_deg_down",
    "right_banked_flat_to_right_banked_25_deg_down",
    "left_banked_25_deg_down_to_left_banked_flat",
    "right_banked_25_deg_down_to_right_banked_flat",
    "flat_to_left_banked_25_deg_up",
    "flat_to_right_banked_25_deg_up",
    "left_banked_25_deg_up_to_flat",
    "right_banked_25_deg_up_to_flat",
    "flat_to_left_banked_25_deg_down",
    "flat_to_right_banked_25_deg_down",
    "left_banked_25_deg_down_to_flat",
    "right_banked_25_deg_down_to_flat",
    "left_quarter_turn_1_90_deg_up",
    "right_quarter_turn_1_90_deg_up",
    "left_quarter_turn_1_90_deg_down",
    "right_quarter_turn_1_90_deg_down",
    "multidim_90_deg_up_to_inverted_flat_quarter_loop",
    "multidim_flat_to_90_deg_down_quarter_loop",
    "elem_255",
};