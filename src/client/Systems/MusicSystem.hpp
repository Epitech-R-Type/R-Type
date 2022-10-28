/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicSystem
*/

#pragma once

#include <raylib.h>
#include <cmrc/cmrc.hpp>
#include "../../shared/Systems/ISystem.hpp"

CMRC_DECLARE(client);

struct Song {
    const std::string path;
};

static std::vector<Song> Songs {
    {"resources/menu.mp3"}
};

class MusicSystem : public System {
    public:
        MusicSystem();
        ~MusicSystem();
        void apply();
        void changeSong(int ID);

    private:
        Music _music;
        float _volume = 0.5;
        cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
};

