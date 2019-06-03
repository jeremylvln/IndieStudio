/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** createNodeSystem
*/

#include "Components.hpp"
#include "indiestudio/Game.hpp"
#include "indiestudio/Singleton.hpp"
#include "BaseSystem.hpp"

using namespace IndieStudio::ECS::Component;

namespace Ecs {

    template <typename ManagerType>
    struct Initializer {

        // Create the node of an entity from its MeshPath
        static void createAllNode(ManagerType &manager)
        {
            manager.template forEntitiesWith<NodeCreate, MeshPath>(
            [&manager](auto &data, [[gnu::unused]] std::size_t id) {
                auto scenemg = IndieStudio::Singleton::getDevice()->getSceneManager();
                auto &mesh = manager.template getComponent<MeshPath>(data);
                auto node = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh(mesh.meshPath.c_str()));

        	    manager.template setComponent(data, Node(node));
                manager.template unsetComponent<NodeCreate>(data);
            });
        }

        // Set all the attributes of a node
        static void setupAllNode(ManagerType &manager)
        {
            manager.template forEntitiesWith<Node, Position, Scale, MaterialTexture, MaterialFlag>(
            [&manager](auto &data, [[gnu::unused]] std::size_t id) {
                auto driver = IndieStudio::Singleton::getDevice()->getVideoDriver();
                auto &node = manager.template getComponent<Node>(data);
                auto &position = manager.template getComponent<Position>(data);
                auto &scale = manager.template getComponent<Scale>(data);
                auto &materialTexture = manager.template getComponent<MaterialTexture>(data);
                auto &materialFlag = manager.template getComponent<MaterialFlag>(data);

                node.node->setMaterialTexture(materialTexture.layer, driver->getTexture(materialTexture.path.c_str()));
                node.node->setMaterialFlag(materialFlag.flag, materialFlag.present);
                node.node->setScale(irr::core::vector3df{scale.x, scale.y, scale.z});
                node.node->setPosition(irr::core::vector3df{position.x, position.y, position.z});
            });
        }

        static void initAllEntities(ManagerType &manager)
        {
            createAllNode(manager);
            setupAllNode(manager);
        }
    };
}