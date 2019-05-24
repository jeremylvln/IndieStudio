/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** mod AMod.hpp
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include "indiestudio/common/ILogger.hpp"

namespace IndieStudio {

    class AMod {
    public:
        struct Descriptor
        {
            std::string id;
            std::string name;
            std::string author;
            std::string version;
            std::function<AMod *(void)> entryPoint;
        };

        virtual ~AMod() {}
        virtual void onEnable() = 0;
        virtual void onDisable() = 0;

        void setLogger(std::unique_ptr<ILogger> &logger)
        {
            this->logger = std::move(logger);
        }
    
    protected:
        std::unique_ptr<ILogger> logger;

    private:
    };

}

#define EXPORT_MOD(id, name, author, version, clazz)                                               \
    extern "C" {                                                                                   \
        static IndieStudio::AMod *createMod()                                                      \
        {                                                                                          \
            return new clazz();                                                                    \
        }                                                                                          \
                                                                                                   \
        IndieStudio::AMod::Descriptor MOD_DESCRIPTOR = { id, name, author, version, &createMod };  \
    }