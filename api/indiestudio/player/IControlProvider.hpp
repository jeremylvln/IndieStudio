/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IControlProvider.hpp
*/

#pragma once

#include <deque>
#include <string>
#include <irrlicht.h>
#include "indiestudio/common/ByteBuffer.hpp"
#include "indiestudio/ecs/Events.hpp"

namespace IndieStudio {

	class IControlProvider {
	public:
		struct Mappings {
			ECS::Event::EventData up;
			ECS::Event::EventData left;
			ECS::Event::EventData down;
			ECS::Event::EventData right;
			ECS::Event::EventData drop;
		};

		virtual ~IControlProvider() = default;

		virtual void initConfigurationArea(irr::gui::IGUIEnvironment *guiEnv,
			irr::gui::IGUIElement *area, int playerIdx) = 0;
		virtual bool onConfigurationEvent(const irr::SEvent &event,
			irr::gui::IGUIElement *area, int playerIdx) = 0;
		
		virtual void onWorldQuit(int playerIdx) = 0;

		virtual Mappings getPlayerMappings(int playerIdx) const = 0;
		virtual std::deque<ECS::Event::EventData> pollEvents() = 0;

		virtual const std::string &getIconPath() const = 0;

		virtual void pack(ByteBuffer &buffer, int playerIdx) const = 0;
        virtual void unpack(ByteBuffer &buffer, int playerIdx) = 0;
	};

}
