/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world BasicWorldGenerator.cpp
*/

#include <ctime>
#include "indiestudio/world/BasicWorldGenerator.hpp"

namespace IndieStudio {

    void BasicWorldGenerator::generate(MapPattern *pattern)
    {
        std::srand(std::time(NULL));

        // We place a wall each two tiles, one line of two.
        for (int x = 2; x < pattern->getWidth() - 2; x += 2)
            for (int z = 2; z < pattern->getHeight() - 2; z += 2)
                pattern->set(x, 1, z, MapPattern::TileType::INNER_WALL_BLOCK);

        // We remove some breakable blocks.
        for (int x = 1; x < pattern->getWidth() - 1; x += 1)
            for (int z = 1; z < pattern->getHeight() - 1; z += 1)
                if (pattern->get(x, 1, z) == MapPattern::TileType::BREAKABLE_BLOCK
                && std::rand() % HOLE_RAND_MAX < HOLE_RATE)
                    pattern->set(x, 1, z, MapPattern::TileType::EMPTY);
    }

}