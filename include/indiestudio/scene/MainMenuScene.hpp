/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene MainMenuScene.hpp
*/

#pragma once

#include "indiestudio/scene/SceneManager.hpp"

namespace IndieStudio {

    class MainMenuScene {
    public:
        static void initialize(SceneManager::Scene &scene);
    
    protected:
    private:
        static void setupMenu(SceneManager::Scene &scene);
        static void setupCharacterExplosion(SceneManager::Scene &scene);
        static void setupTravelling(SceneManager::Scene &scene);
        static bool onEvent(SceneManager::Scene &scene, const irr::SEvent &event);

        enum {
            BUTTON_ID_PLAY,
            BUTTON_ID_SETTING,
            BUTTON_ID_HOW_TO_PLAY,
            BUTTON_ID_QUIT
        };
    };

}