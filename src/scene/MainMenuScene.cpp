/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** scene MainMenuScene.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/scene/MainMenuScene.hpp"
#include "indiestudio/scene/NewGameScene.hpp"
#include "indiestudio/Singleton.hpp"
#include "indiestudio/common/Error.hpp"
#include "indiestudio/scene/PlayScene.hpp"

namespace IndieStudio {

    sf::Sound MainMenuScene::BACKGROUND_MUSIC = sf::Sound();

    void MainMenuScene::initialize(SceneManager::Scene &scene)
    {
        SoundManager &soundManager = static_cast<SoundManager &>(
            Game::INSTANCE->getSoundManager());
        BACKGROUND_MUSIC.setBuffer(soundManager.getSound("assets/sounds/shooting_stars.ogg").buffer);
        BACKGROUND_MUSIC.play();
        BACKGROUND_MUSIC.setPlayingOffset(sf::seconds(21));
        BACKGROUND_MUSIC.setLoop(true);

        scene.scene->addSkyDomeSceneNode(scene.manager->textureManager->getTexture("assets/textures/skydome.jpg").content);

        scene.scene->addCameraSceneNode(0, irr::core::vector3df(50, 0, 0), irr::core::vector3df(0, 0, 0));

        setupMenu(scene);
        setupCharacterExplosion(scene);
        setupTravelling(scene);

        scene.onEvent = [&](const irr::SEvent &event) {
            return onEvent(scene, event);
        };
    }

    void MainMenuScene::setupMenu(SceneManager::Scene &scene)
    {
        auto guiEnv = scene.scene->getGUIEnvironment();
        auto guiRoot = scene.gui;

        guiEnv->addImage(
            scene.manager->textureManager->getTexture("assets/textures/title.png").content,
            irr::core::position2di(300, 75), true, guiRoot
        );

        irr::core::vector2di pos(515, 500);
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_PLAY_NEW, L"New Game");
        pos.Y += 60;
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_PLAY_LOAD, L"Load Game");
        pos.Y += 60;
        guiEnv->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}),
            guiRoot, BUTTON_ID_QUIT, L"Quit");
    }

    void MainMenuScene::setupCharacterExplosion(SceneManager::Scene &scene)
    {
        irr::scene::IAnimatedMeshSceneNode* jesusModel = scene.scene->addAnimatedMeshSceneNode(
            scene.scene->getMesh("assets/models/jesus.md3"), 0, 666);

        if (!jesusModel)
            throw RessourceError("Failed to load player.md3");

        jesusModel->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        jesusModel->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/jesus.png").content);
        jesusModel->setAnimationSpeed(30);
        jesusModel->setScale(irr::core::vector3df(0.7f ,0.7f ,0.7f));
        jesusModel->setPosition(irr::core::vector3df(0, -5, 0));
        jesusModel->setRotation(irr::core::vector3df(0, 180, 0));
        irr::scene::ISceneNodeAnimator *anim = scene.scene->createFlyCircleAnimator({(float)15 + (rand() % 10), (float)15 + (rand() % 10), (float)15 + (rand() % 10)}, 100, 0.001, {15, 15, 15});
        jesusModel->addAnimator(anim);
        anim->drop();


        irr::scene::IAnimatedMeshSceneNode* anms = scene.scene->addAnimatedMeshSceneNode(
            scene.scene->getMesh("assets/models/player.md3"), 0, 42);

        if (!anms)
            throw RessourceError("Failed to load player.md3");

        anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        anms->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/player_white.png").content);
        anms->setFrameLoop(0, 27);
        anms->setAnimationSpeed(30);
        anms->setScale(irr::core::vector3df(8.f,8.f,8.f));
        anms->setAnimationSpeed(18);
        anms->setPosition(irr::core::vector3df(0, -7, 0));

        for (int i = 0; i < 150; i++) {
            int randX = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 100 * 15;
            int randY = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 100 * 15;
            int randZ = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 100 * 15;

            irr::scene::ISceneNodeAnimator *anim = scene.scene->createFlyStraightAnimator(
                anms->getAbsolutePosition(), irr::core::vector3df(randX, randY, randZ),
                15000, true, true
            );

            irr::scene::IAnimatedMeshSceneNode *clone = (irr::scene::IAnimatedMeshSceneNode *)anms->clone();
            clone->addAnimator(anim);

            anim = scene.scene->createRotationAnimator(
                irr::core::vector3df(rand() % 3, rand() % 3, rand() % 2 + 1));
            clone->addAnimator(anim);

            int randTexture = rand() % 4;

            if (randTexture == 0)
                clone->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/player_black.png").content);
            else if (randTexture == 1)
                clone->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/player_pink.png").content);
            else if (randTexture == 2)
                clone->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/player_red.png").content);
            else
                clone->setMaterialTexture(0, scene.manager->textureManager->getTexture("assets/textures/player_white.png").content);
        }
    }

    void MainMenuScene::setupTravelling(SceneManager::Scene &scene)
    {
        irr::core::array<irr::core::vector3df> points;
        irr::scene::ICameraSceneNode *cam = scene.scene->getActiveCamera();
        irr::core::vector3df pos(cam->getAbsolutePosition());
        int step = 2;

        while (pos.X >= 0) {
            points.push_back(pos);
            pos.X -= step;
            pos.Z += step;
        }
        while (pos.X >= -50) {
            points.push_back(pos);
            pos.X -= step;
            pos.Z -= step;
        }
        while (pos.X <= 0) {
            points.push_back(pos);
            pos.X += step;
            pos.Z -= step;
        }
        while (pos.X <= 50) {
            points.push_back(pos);
            pos.X += step;
            pos.Z += step;
        }

        auto device = Singleton::getDevice();
        irr::scene::ISceneNodeAnimator* sceneAnimator =
            scene.scene->createFollowSplineAnimator(
                device->getTimer()->getTime(), points, 2);

        cam->addAnimator(sceneAnimator);
        sceneAnimator->drop();
    }

    bool MainMenuScene::onEvent(SceneManager::Scene &scene, const irr::SEvent &event)
    {
        static irr::gui::IGUIWindow *msgBox = nullptr;

        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
                if (!msgBox) {
                    msgBox = Singleton::getDevice()->getGUIEnvironment()->addMessageBox(
                    L"Quit", L"Are you sure?", true,
                    irr::gui::EMBF_YES | irr::gui::EMBF_NO, scene.gui, MSG_BOX_QUIT);
                    msgBox->setDraggable(false);
                } else {
                    msgBox->remove();
                    msgBox = nullptr;
                }
                return true;
            }
        } else if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller->getID() == BUTTON_ID_PLAY_NEW) {
                scene.manager->setActiveScene(SceneManager::NEW_GAME_ID);
                return true;
            } else if (event.GUIEvent.Caller->getID() == BUTTON_ID_QUIT) {
                msgBox = Singleton::getDevice()->getGUIEnvironment()->addMessageBox(
                    L"Quit", L"Are You Sure ?", true,
                    irr::gui::EMBF_YES | irr::gui::EMBF_NO, scene.gui, MSG_BOX_QUIT);
                msgBox->setDraggable(false);
                return true;
            } else if (event.GUIEvent.Caller->getID() == BUTTON_ID_PLAY_LOAD) {
                scene.scene->getGUIEnvironment()->addFileOpenDialog(L"Select your world file", true, scene.gui);
                return true;
            }
        } else if (event.EventType == irr::EET_GUI_EVENT) {
            if (event.GUIEvent.EventType == irr::gui::EGET_MESSAGEBOX_CANCEL
            || event.GUIEvent.EventType == irr::gui::EGET_MESSAGEBOX_NO
            || event.GUIEvent.EventType == irr::gui::EGET_MESSAGEBOX_OK) {
                msgBox = nullptr;
            } else if (event.GUIEvent.EventType == irr::gui::EGET_MESSAGEBOX_YES
            && event.GUIEvent.Caller->getID() == MSG_BOX_QUIT) {
                Singleton::getDevice()->closeDevice();
            } else if (event.GUIEvent.EventType == irr::gui::EGET_FILE_SELECTED) {
                const wchar_t *path = static_cast<irr::gui::IGUIFileOpenDialog *>(
                    event.GUIEvent.Caller)->getFileName();

                if (path == nullptr)
                    return true;

                std::wstring tmp = std::wstring(path);

                SceneManager::Scene &playScene = scene.manager->getScene(SceneManager::PLAY_ID);

                WorldManager &manager = static_cast<WorldManager &>(
                    Game::INSTANCE->getWorldManager());

                try {
                    World *world = manager.load(std::string(tmp.begin(), tmp.end()));

                    BACKGROUND_MUSIC.stop();
                    PlayScene::initialize(playScene);
                    scene.manager->setActiveScene(SceneManager::PLAY_ID);
                    world->focusECS(playScene);
                } catch (const IndieError &ex) {
                    std::string tmp(ex.what());
                    msgBox = Singleton::getDevice()->getGUIEnvironment()->addMessageBox(
                    L"Failed to load the map", std::wstring(tmp.begin(), tmp.end()).c_str(), true,
                    irr::gui::EMBF_OK, scene.gui, MSG_BOX_QUIT);
                    msgBox->setDraggable(false);
                }

                return true;
            }
        }

        return false;
    }

}