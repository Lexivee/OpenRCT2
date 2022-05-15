/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "DefaultMode.h"

namespace OpenRCT2
{
    namespace RideModes
    {
        class Dodgems : public Default
        {
        public:
            virtual rct_string_id GetOperationErrorMessage(Ride* ride) const override;
            virtual bool FindVehicleToEnter(Guest* guest, Ride* ride, std::vector<uint8_t>& car_array) const override;
        };
    } // namespace RideModes
} // namespace OpenRCT2
