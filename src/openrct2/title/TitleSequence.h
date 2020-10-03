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
#include "../localisation/Localisation.h"
#include "../openrct2/core/IStream.hpp"

#include <memory>

#define TITLE_COMMAND_SCENARIO_LENGTH 64

struct TitleCommand
{
    uint8_t Type;
    union
    {
        uint8_t SaveIndex; // LOAD (this index is internal only)
        struct             // LOCATION
        {
            uint8_t X;
            uint8_t Y;
        };
        uint8_t Rotations; // ROTATE (counter-clockwise)
        uint8_t Zoom;      // ZOOM
        struct             // FOLLOW
        {
            uint16_t SpriteIndex;
            utf8 SpriteName[USER_STRING_MAX_LENGTH];
        };
        uint8_t Speed;                                // SPEED
        uint16_t Milliseconds;                        // WAIT
        utf8 Scenario[TITLE_COMMAND_SCENARIO_LENGTH]; // LOADSC
    };
};

struct TitleSequence
{
    std::string Name;
    std::string Path;

    std::vector<TitleCommand> Commands;
    std::vector<std::string> Saves;

    bool IsZip = false;
};

struct TitleSequenceParkHandle
{
    std::string HintPath;
    std::unique_ptr<OpenRCT2::IStream> Stream;
};

enum TITLE_SCRIPT
{
    TITLE_SCRIPT_UNDEFINED = 0xFF,
    TITLE_SCRIPT_WAIT = 0,
    TITLE_SCRIPT_LOCATION,
    TITLE_SCRIPT_ROTATE,
    TITLE_SCRIPT_ZOOM,
    TITLE_SCRIPT_FOLLOW,
    TITLE_SCRIPT_RESTART,
    TITLE_SCRIPT_LOAD,
    TITLE_SCRIPT_END,
    TITLE_SCRIPT_SPEED,
    TITLE_SCRIPT_LOOP,
    TITLE_SCRIPT_ENDLOOP,
    TITLE_SCRIPT_LOADSC,
};

constexpr const utf8* TITLE_SEQUENCE_EXTENSION = ".parkseq";
constexpr uint8_t SAVE_INDEX_INVALID = UINT8_MAX;

std::unique_ptr<TitleSequence> CreateTitleSequence();
std::unique_ptr<TitleSequence> LoadTitleSequence(const std::string& path);
std::unique_ptr<TitleSequenceParkHandle> TitleSequenceGetParkHandle(const TitleSequence& seq, size_t index);

bool TitleSequenceSave(const TitleSequence& seq);
bool TitleSequenceAddPark(TitleSequence& seq, const utf8* path, const utf8* name);
bool TitleSequenceRenamePark(TitleSequence& seq, size_t index, const utf8* name);
bool TitleSequenceRemovePark(TitleSequence& seq, size_t index);

bool TitleSequenceIsLoadCommand(const TitleCommand& command);
