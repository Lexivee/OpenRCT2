#pragma once

#include <cstdint>
#include <vector>
#include "TrackData.h"

/// <summary>
/// RideBaseBuilder only supports square bases for now
/// Non-square bases requires specific handling for the track mapping
/// </summary>
class RideBaseBuilder
{
public:
    RideBaseBuilder(uint8_t sizeX, uint8_t sizeY);

    void SwapTiles(uint8_t index0, uint8_t index1);
    void SetTileClearance(uint8_t index, int32_t clearance);
    uint8_t GetTileIndex(uint8_t x, uint8_t y) const;

    std::vector<PreviewTrack> GetBlocks() const;
    std::vector<uint8_t> GetEdges() const;
    std::vector<std::vector<uint8_t>> GetMapping() const;
    std::vector<uint8_t> GetSequences() const;
private:
    std::vector<PreviewTrack> _blocks;
    std::vector<uint8_t> _edges;
    std::vector<uint8_t> _sequences;
    std::vector<std::vector<uint8_t>> _mapping;
    uint8_t _sizeX, _sizeY;

    void BuildBlocks();
    void BuildEdges();
    void BuildMapping();
    void BuildSequences();
};
