/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** BombFactory
*/

#pragma once

#include <iostream>
#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/Singleton.hpp"
#include "indiestudio/ecs/Initializer.hpp"

namespace IndieStudio {
    class BombFactory {
    	public:

            template <typename ManagerType>
            static void poseBomb(ManagerType &manager, irr::scene::ISceneManager *scenemg, float BombPosX, float BombPosZ, std::size_t playerID) {
                auto &newBomb = manager.addEntity();

                manager.setComponent(newBomb, MeshPath("assets/models/bomb.obj"));
                manager.setComponent(newBomb, MaterialTexture(0, "assets/textures/bomb.png"));
                manager.setComponent(newBomb, MaterialFlag(irr::video::EMF_LIGHTING, true));
                manager.setComponent(newBomb, Scale(3.5, 3.5, 3.5));
                manager.setComponent(newBomb, Position(BombPosX, 70, BombPosZ));
                manager.setComponent(newBomb, IsBomb());
                manager.setComponent(newBomb, ExplosionRange());
                manager.setComponent(newBomb, LifeTime());
                manager.setComponent(newBomb, PosedBy(playerID));
                manager.setComponent(newBomb, Setup());
                manager.setComponent(newBomb, NodeCreate());
                IndieStudio::Initializer<WorldECS>::initAllEntities(manager, scenemg);
            };
    };
}