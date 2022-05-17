/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../common.h"
#include "../ride/RideTypes.h"
#include "../world/Location.hpp"

struct Peep;
struct Guest;
struct TileElement;

class GuestPathfinding
{
public:
    virtual ~GuestPathfinding() = default;

    /**
     * Given a peep 'peep' at tile 'loc', who is trying to get to 'gPeepPathFindGoalPosition', decide the direction the peep
     * should walk in from the current tile.
     *
     * @param loc Reference to the peep's current tile location
     * @param peep Reference to the current peep struct
     * @param pathOverQueues Whether or not its ok to pathfind over queues (eg. staff members don't care about queues when
     * pathfinding)
     * @return The direction the peep should walk in
     */
    virtual Direction ChooseDirection(const TileCoordsXYZ& loc, Peep& peep, bool pathOverQueues = false) = 0;

    /**
     * Test whether the given tile can be walked onto, if the peep is currently at height currentZ and
     * moving in direction currentDirection
     *
     * @param tileElement A pointer to the tile that is being tested
     * @param currentZ The height coord the peep is at
     * @param currentDirection The direction the peep is facing in
     * @return True if the given tile can be walked onto, false otherwise
     */
    static bool IsValidPathZAndDirection(TileElement* tileElement, int32_t currentZ, int32_t currentDirection);

    /**
     * Overall guest pathfinding AI. Sets up Peep::DestinationX/DestinationY (which they move to in a
     * straight line, no pathfinding). Called whenever the guest has arrived at their previously set destination.
     *
     * @param peep A reference to a guest struct
     * @returns 0 if the guest has successfully had a new destination set up, nonzero otherwise.
     */
    virtual int32_t CalculateNextDestination(Guest& peep) = 0;

    /**
     * Set a new destination for a 'peep' to try and get to
     *
     * @param peep A reference to a Peep struct
     * @param dest A reference to a target destination coord
     * @returns 0 if a new destination was set, nonzero otherwise
     */
    virtual int32_t SetNewDestination(Peep& peep, const TileCoordsXYZ& dest) = 0;
};

class OriginalPathfinding final : public GuestPathfinding
{
public:
    Direction ChooseDirection(const TileCoordsXYZ& loc, Peep& peep, bool pathOverQueues = false) final override;

    int32_t CalculateNextDestination(Guest& peep) final override;

    int32_t SetNewDestination(Peep& peep, const TileCoordsXYZ& dest) final override;

private:
    int32_t GuestPathFindParkEntranceEntering(Peep& peep, uint8_t edges);

    int32_t GuestPathFindPeepSpawn(Peep& peep, uint8_t edges);

    int32_t GuestPathFindParkEntranceLeaving(Peep& peep, uint8_t edges);
};

extern std::unique_ptr<GuestPathfinding> gGuestPathfinder;

#if defined(DEBUG_LEVEL_1) && DEBUG_LEVEL_1
#    define PATHFIND_DEBUG                                                                                                     \
        0 // Set to 0 to disable pathfinding debugging;
          // Set to 1 to enable pathfinding debugging.

// When PATHFIND_DEBUG is 1 (nonzero):
// The following calls configure debug logging for the given peep
// If they're a guest, pathfinding will be logged if they have PEEP_FLAGS_TRACKING set
// If they're staff, pathfinding will be logged if their name is "Mechanic Debug"
void PathfindLoggingEnable(Peep& peep);
void PathfindLoggingDisable();
#endif // defined(DEBUG_LEVEL_1) && DEBUG_LEVEL_1
