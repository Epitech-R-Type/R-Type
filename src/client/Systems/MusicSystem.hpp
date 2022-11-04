/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicSystem
*/

#pragma once

#include <raylib.h>
#include <cmrc/cmrc.hpp>
#include <queue>

#include "../../shared/Utilities/Utilities.hpp"
#include "../../shared/Systems/ISystem.hpp"

CMRC_DECLARE(client);

struct Song {
    const std::string path;
};

struct SFX {
    const std::string path;
};

static std::vector<Song> Songs {
    {"resources/song0.mp3"},
    {"resources/song1.mp3"}
};

static std::vector<SFX> SoundEffects {
    {"resources/gunShot1.wav"},
    {"resources/gunShot2.wav"},
    {"resources/gunShot3.wav"}
};

class MusicSystem : public System {
    public:
        static std::queue<SFXID> SFXQueue;

        MusicSystem(int ID);
        ~MusicSystem();
        void apply();
        void changeSong(int ID);
        void playSFX(int ID);

    private:
        Music _music;
        float _volume = 0.5;
        cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
        std::map<SFXID, Sound> SFXobjects;
};

