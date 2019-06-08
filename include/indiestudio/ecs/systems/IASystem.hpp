/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IA
*/

#pragma once

#include <optional>

#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/IWorld.hpp"
#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"
#include "indiestudio/gameplay/BombFactory.hpp"

namespace IndieStudio {
struct Tile {
    std::size_t delta;
    MapPattern::TileType type;
};

using Hitmap = std::vector<std::vector<Tile>>;

template <typename ManagerType>
class Poti {
public:

    enum class Direction {
        LEFT,
        RIGHT,
        TOP,
        BOT,
        NONE
    };

    struct Coord {
        std::size_t x;
        std::size_t y;
    };

    struct PathData {
        Direction dir;
        std::size_t pathLen;
    };


    std::optional<std::pair<Direction, std::size_t>> search_for(ManagerType &manager, MapPattern::TileType target, MapPattern *map, std::size_t x, std::size_t y, bool should_go_through_explosions) {
        Hitmap hitmap = this->init_hitmap(manager, map, should_go_through_explosions);
        this->fill_hitmap(hitmap, {x, y}, 0);
        std::optional<std::pair<Coord, std::size_t>> target_coord = this->find_nearest_target(target, hitmap);
        if (target_coord == std::nullopt) {
            return std::nullopt;
        }
        this->reset_hitmap(hitmap);
        this->fill_hitmap(hitmap, {target_coord.value().first.x, target_coord.value().first.y}, 0);
        return std::make_optional(std::make_pair(this->get_direction(hitmap, {x, y}), target_coord.value().second));
    }

private:

    Hitmap init_hitmap(ManagerType &manager, MapPattern *map, bool should_go_through_explosion) {
        Hitmap hitmap;
        std::cout << "Creating hitmap: " << std::endl;
        for (std::size_t i = 0 ; i < map->getHeight() ; i++) {
            hitmap.push_back(std::vector<Tile>());
            for (std::size_t j = 0 ; j < map->getWidth() ; j++) {
                std::cout << static_cast<int>(map->get(j, 1, i)) << " ";
                hitmap[i].push_back({static_cast<std::size_t>(-1), map->get(j, 1, i)});
            }
            std::cout << std::endl;
        }
        this->add_futur_explosion(manager, hitmap, should_go_through_explosion);
        return hitmap;
    }

    void add_futur_explosion(ManagerType &manager, Hitmap &hitmap, bool should_go_through_explosion) {
        manager.template forEntitiesWith<ECS::Component::IsBomb>([&manager, should_go_through_explosion, &hitmap](auto &bomb, [[gnu::unused]] std::size_t id) {
            auto &bomb_pos = manager.template getComponent<ECS::Component::Position>(bomb);
            auto &bomb_range = manager.template getComponent<ECS::Component::ExplosionRange>(bomb);
            std::pair<short, short> bomb_coord = MapPattern::positionToTile(bomb_pos.x, bomb_pos.z);

            for (std::size_t i = 0 ; i < bomb_range.explosionRangeUp ; i++) {
                auto &tile = hitmap[bomb_coord.second - i][bomb_coord.first];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (should_go_through_explosion) {
                    tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
                } else {
                    tile.type = MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION;
                }
            }

            for (std::size_t i = 0 ; i < bomb_range.explosionRangeDown ; i++) {
                auto &tile = hitmap[bomb_coord.second + i][bomb_coord.first];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (should_go_through_explosion) {
                    tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
                } else {
                    tile.type = MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION;
                }
            }

            for (std::size_t i = 0 ; i < bomb_range.explosionRangeLeft ; i++) {
                auto &tile = hitmap[bomb_coord.second][bomb_coord.first - i];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (should_go_through_explosion) {
                    tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
                } else {
                    tile.type = MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION;
                }
            }

            for (std::size_t i = 0 ; i < bomb_range.explosionRangeRight ; i++) {
                auto &tile = hitmap[bomb_coord.second][bomb_coord.first + i];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (should_go_through_explosion) {
                    tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
                } else {
                    tile.type = MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION;
                }
            }

        });
    }

    void fill_hitmap(Hitmap &hitmap, Coord coord, std::size_t current) {
        hitmap[coord.y][coord.x].delta = current;
        if (is_solid(hitmap[coord.y][coord.x])) {
            return;
        }

        if (hitmap[coord.y][coord.x - 1].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x - 1, coord.y}, current + 1);
        }
        if (hitmap[coord.y][coord.x + 1].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x + 1, coord.y}, current + 1);
        }
        if (hitmap[coord.y - 1][coord.x].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x, coord.y - 1}, current + 1);
        }
        if (hitmap[coord.y + 1][coord.x].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x, coord.y + 1}, current + 1);
        }
    }

    bool is_solid(const Tile &tile) {
        switch (tile.type) {
            case MapPattern::TileType::EMPTY: return false;
            case MapPattern::TileType::PLAYER: return false;
            case MapPattern::TileType::POWER_UP: return false;
            case MapPattern::TileType::POTENTIAL_EXPLOSION: return false;
            default: return true;
        }
    }

    void reset_hitmap(Hitmap &hitmap) {
        std::cout << "Displaying hitmap" << std::endl;
        for (auto &tmp : hitmap) {
            for (auto &tile : tmp) {
                if (tile.delta == static_cast<std::size_t>(-1)) {
                    std::cout << "\\ ";
                } else {
                    std::cout << tile.delta << " ";
                }
                tile.delta = -1;
            }
            std::cout << std::endl;
        }
    }

    std::optional<std::pair<Coord, std::size_t>> find_nearest_target(MapPattern::TileType target, const Hitmap &hitmap) {
        std::size_t nearest = -1;
        Coord nearest_target = {0, 0};

        for (std::size_t y = 0 ; y < hitmap.size() ; y++) {
            for (std::size_t x = 0 ; x < hitmap[y].size() ; x++) {
                if (hitmap[y][x].type == target) {
                    is_nearest(hitmap, nearest, nearest_target, {x, y});
                }
            }
        }
        if (nearest == static_cast<std::size_t>(-1)) {
            return std::nullopt;
        }
        return std::make_optional(std::make_pair(nearest_target, nearest));
    }

    void is_nearest(const Hitmap &hitmap, std::size_t &nearest, Coord &nearest_target, Coord current_coord) {
        if (hitmap[current_coord.y][current_coord.x].delta < nearest) {
            nearest = hitmap[current_coord.y][current_coord.x].delta;
            nearest_target = current_coord;
        }
    }

    Direction get_direction(const Hitmap &hitmap, Coord coord) {
        std::size_t tmp = -1;
        Direction direction = Direction::LEFT;

        if (coord.x > 0 && hitmap[coord.y][coord.x - 1].delta < tmp) {
            tmp = hitmap[coord.y][coord.x - 1].delta;
            direction = Direction::LEFT;
        }
        if (coord.x < hitmap[coord.y].size() && hitmap[coord.y][coord.x + 1].delta < tmp) {
            tmp = hitmap[coord.y][coord.x + 1].delta;
            direction = Direction::RIGHT;
        }
        if (coord.y > 0 && hitmap[coord.y - 1][coord.x].delta < tmp) {
            tmp = hitmap[coord.y - 1][coord.x].delta;
            direction = Direction::TOP;
        }
        if (coord.y < hitmap.size() && hitmap[coord.y + 1][coord.x].delta < tmp) {
            tmp = hitmap[coord.y + 1][coord.x].delta;
            direction = Direction::BOT;
        }
        return direction;
    }

};
}

namespace IndieStudio::ECS::System {

    using namespace ECS::Event;

    template<typename ManagerType>
    class IASystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override {
            (void) world;

            manager.template forEntitiesWith<IA>(
                [&manager, world, this](auto &data, [[gnu::unused]] auto id) {
                    auto &ia = manager.template getComponent<IA>(data);
                    auto &movement = manager.template getComponent<Movement>(data);
                    auto &position = manager.template getComponent<Position>(data);

                    std::optional<typename Poti<ManagerType>::Direction> decision = this->execute_action(manager, ia.current_action, position, world);
                    if (!decision) {
                            ia.current_action = this->select_action();
                            decision = this->execute_action(manager, ia.current_action, position, world);
                    }

                    if (this->emergency_move(manager, decision, position, world)) {
                        ia.current_action = IA::Action::NOTHING;
                    }

                    this->move(decision, movement);
                }
            );
        }

        void move(std::optional<typename Poti<ManagerType>::Direction> direction, Movement &movement) {
                movement.up = false;
                movement.down = false;
                movement.left = false;
                movement.right = false;
                std::cout << "Moving at ";
            if (!direction.has_value()) {
                std::cout << "nothing" << std::endl;
            } else if (direction.value() == Poti<ManagerType>::Direction::BOT) {
                std::cout << "bot" << std::endl;
                movement.down = true;
            } else if (direction.value() == Poti<ManagerType>::Direction::TOP) {
                std::cout << "top" << std::endl;
                movement.up = true;
            } else if (direction.value() == Poti<ManagerType>::Direction::LEFT) {
                std::cout << "left" << std::endl;
                movement.left = true;
            } else if (direction.value() == Poti<ManagerType>::Direction::RIGHT) {
                std::cout << "right" << std::endl;
                movement.right = true;
            } else {
                std::cout << "nothing" << std::endl;
            }
        }

        bool emergency_move(ManagerType &manager, std::optional<typename Poti<ManagerType>::Direction> &decision, Position position, IWorld *world) {

            MapPattern *tilemap = world->getPattern();
            std::cout << "Emergency move" << std::endl;

            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> empty = this->marron.search_for(manager, MapPattern::TileType::EMPTY, tilemap, coord.first, coord.second, true);
            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> player = this->marron.search_for(manager, MapPattern::TileType::PLAYER, tilemap, coord.first, coord.second, true);
            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> powerup = this->marron.search_for(manager, MapPattern::TileType::POWER_UP, tilemap, coord.first, coord.second, true);
            if (empty.has_value() && empty.value().second == 0) {
                std::cout << "Safe on empty" << std::endl;
                return false;
            }
            if (player.has_value() && player.value().second == 0) {
                std::cout << "Safe on player" << std::endl;
                return false;
            }
            if (powerup.has_value() && powerup.value().second == 0) {
                std::cout << "Safe on powerup" << std::endl;
                return false;
            }
            if (!empty.has_value() && !player.has_value() && !powerup.has_value()) {
                std::cout << "No safe place. Just die. sorry" << std::endl;
                decision = std::make_optional(Poti<ManagerType>::Direction::NONE);
                return true;
            }

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> nearest;
            if (empty.has_value() && (player.has_value() && empty.value().second < player.value().second) && (powerup.has_value() && empty.value().second < powerup.value().second)) {
                std::cout << "Nearest on empty" << std::endl;
                nearest = empty;
            }
            if (player.has_value() && (empty.has_value() && player.value().second < empty.value().second) && (powerup.has_value() && player.value().second < powerup.value().second)) {
                std::cout << "Nearest on player" << std::endl;
                nearest = player;
            }
            if (powerup.has_value() && (empty.has_value() && powerup.value().second < empty.value().second) && (player.has_value() && powerup.value().second < player.value().second)) {
                std::cout << "Nearest on powerup" << std::endl;
                nearest = powerup;
            }

            decision = std::make_optional(nearest.value().first);
            return true;
        }

        IA::Action select_action() {
            int rand = std::rand() % 3;

            switch (rand) {
                case 0: return IA::Action::ATK;
                case 1: return IA::Action::WALL;
                case 2: return IA::Action::PICKUP;
                default: return IA::Action::NOTHING;
            }
        }

        std::optional<typename Poti<ManagerType>::Direction> execute_action(ManagerType &manager, IA::Action action, Position position, IWorld *world) {
            switch (action) {
                case IA::Action::ATK: return atk_player(manager, position, world);
                case IA::Action::WALL: return destroy_wall(manager, position, world);
                case IA::Action::PICKUP: return pick_powerup(manager, position, world);
                default: return std::nullopt;
            }
        }

        std::optional<typename Poti<ManagerType>::Direction> atk_player(ManagerType &manager, Position position, IWorld *world) {
            MapPattern *tilemap = world->getPattern();

            std::cout << "Attacking!" << std::endl;
            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);
            std::cout << "Player position: " << coord.first << "x " << coord.second << "y " << std::endl;

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> decision = this->marron.search_for(manager, MapPattern::TileType::PLAYER, tilemap, coord.first, coord.second, false);

            if (decision == std::nullopt) {
                std::cout << "No player in range :(!" << std::endl;
                return std::nullopt;
            }

            if (decision.value().second <= 3) {
                std::cout << "Attacked! :D i finished my move!" << std::endl;
                // TODO: poser bombe
                // TODO: ajouter du bruit aléatoire, et prendre en compte les stats du player
                return std::nullopt;
            }

            return std::make_optional(decision.value().first);
        }

        std::optional<typename Poti<ManagerType>::Direction> destroy_wall(ManagerType &manager, Position position, IWorld *world) {
            MapPattern *tilemap = world->getPattern();

            std::cout << "Breacking wall!" << std::endl;
            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);
            std::cout << "Player position: " << coord.first << "x " << coord.second << "y " << std::endl;

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> decision = this->marron.search_for(manager, MapPattern::TileType::BREAKABLE_BLOCK, tilemap, coord.first, coord.second, false);

            if (decision == std::nullopt) {
                std::cout << "No wall in range :(!" << std::endl;
                return std::nullopt;
            }

            if (decision.value().second == 1) {
                std::cout << "Breacked wall!" << std::endl;
                // TODO: poser bombe
                // l'action sera reroll, mais cancel immédiatement à l iteration suivante pour échapper à sa propre bombe
                return std::nullopt;
            }

            return std::make_optional(decision.value().first);
        }

        std::optional<typename Poti<ManagerType>::Direction> pick_powerup(ManagerType &manager, Position position, IWorld *world) {
            MapPattern *tilemap = world->getPattern();

            std::cout << "Picking powerup!" << std::endl;
            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);
            std::cout << "Player position: " << coord.first << "x " << coord.second << "y " << std::endl;

            if (tilemap->get(coord.second, 1, coord.first) == MapPattern::TileType::POWER_UP) {
                std::cout << "Powerup picked!" << std::endl;
                // TODO: apply powerup effect
                // TODO: delete le powerup
                // TODO: update la tilemap
                return std::nullopt;
            }

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> decision = this->marron.search_for(manager, MapPattern::TileType::POWER_UP, tilemap, coord.first, coord.second, false);
            if (decision == std::nullopt) {
                std::cout << "No powerup in range :(!" << std::endl;
                return std::nullopt;
            }

            return std::make_optional(decision.value().first);
        }

    protected:
    private:
        Poti<ManagerType> marron;
    };
}