/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Level
*/

#include "Level.hpp"

bool isFloat(std::string myString) {
    std::stringstream iss;
    iss << myString;
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

std::string Level::getBackground() {
    int finished = 0;
    std::string bg;

    while (!finished) {
        std::cout << "What background should we use?" << std::endl;
        bg.clear();
        std::cin >> bg;
        for (int i = 0; i != bg.size() - 1; i++)
            if (!isdigit(bg[i]))
                continue;
        if (std::find(std::begin(this->_backGrounds), std::end(this->_backGrounds), std::stoi(bg)) != std::end(this->_backGrounds))
            finished = 1;
    }
    return (bg);
}

std::string Level::getMusic() {
    int finished = 0;
    std::string music;

    while (!finished) {
        std::cout << "What Music should be played when the boss hasn't spawned yet?" << std::endl;
        music.clear();
        std::cin >> music;
        for (int i = 0; i != music.size() - 1; i++)
            if (!isdigit(music[i]))
                continue;
        if (std::stoi(music) >= 0 || std::stoi(music) < this->_songs)
            finished = 1;
    }
    return (music);
}

std::string Level::getMin() {
    int finished = 0;
    std::string min;

    while (!finished) {
        std::cout << "How many enemies should there be at least?" << std::endl;
        min.clear();
        std::cin >> min;
        finished = 1;
        for (int i = 0; i != min.size() - 1; i++)
            if (!isdigit(min[i])) {
                finished = 0;
                continue;
            }
        if (std::stoi(min) < 1)
            finished = 0;
    }
    return (min);
}

std::string Level::getMax(int min) {
    int finished = 0;
    std::string max;

    while (!finished) {
        std::cout << "How many enemies should there be at max?" << std::endl;
        max.clear();
        std::cin >> max;
        finished = 1;
        for (int i = 0; i != max.size() - 1; i++)
            if (!isdigit(max[i])) {
                finished = 0;
                continue;
            }
        if (std::stoi(max) < min)
            finished = 0;
    }
    return (max);
}

std::string Level::getInterval() {
    int finished = 0;
    std::string interval;

    while (!finished) {
        std::cout << "What should the spawn interval be?" << std::endl;
        interval.clear();
        std::cin >> interval;
        if (!isFloat(interval))
            continue;
        if (std::stof(interval) > 0)
            finished = 1;
    }
    return (interval);
}

std::string Level::getEnemy() {
    int finished = 0;
    int finished2 = 0;
    std::string enemy;
    std::vector<int> enemys;

    while (!finished) {
        while (!finished2) {
            enemy.clear();
            std::cout << "What Enemy should spawn?" << std::endl;
            std::cin >> enemy;
            for (int i = 0; i != enemy.size() - 1; i++)
                if (!isdigit(enemy[i]))
                    continue;
            if (std::stoi(enemy) >= 0 && std::stoi(enemy) <= this->_enemys &&
                std::find(std::begin(enemys), std::end(enemys), std::stoi(enemy)) == std::end(enemys)) {
                finished2 = 1;
                enemys.push_back(std::stoi(enemy));
            }
        }
        finished2 = 0;
        std::cout << "More Enemys? (yes | no is everything else)" << std::endl;
        enemy.clear();
        std::cin >> enemy;
        if (enemy != "yes" || enemys.size() > this->_enemys)
            finished = 1;
    }
    enemy.clear();
    for (int i = 0; i < enemys.size(); i++)
        enemy += std::to_string(enemys[i]) + " ";
    enemy.pop_back();
    return (enemy);
}

std::string Level::getBoss() {
    int finished = 0;
    std::string boss;

    while (!finished) {
        std::cout << "What Boss should be here?" << std::endl;
        boss.clear();
        std::cin >> boss;
        if (boss == "-1")
            break;
        for (int i = 0; i != boss.size() - 1; i++)
            if (!isdigit(boss[i]))
                continue;
        if (std::stoi(boss) >= 0 && std::stoi(boss) <= this->_boss)
            finished = 1;
    }
    return (boss);
}

int Level::getWaves() {
    int finished = 0;
    std::string waves;

    while (!finished) {
        std::cout << "How many enemy waves should there be?" << std::endl;
        waves.clear();
        std::cin >> waves;
        for (int i = 0; i != waves.size() - 1; i++)
            if (!isdigit(waves[i]))
                continue;
        if (std::stoi(waves) > 0)
            finished = 1;
    }
    return (std::stoi(waves));
}

void Level::createLvl() {
    int count;
    std::string toWrite;
    std::string str;

    toWrite += this->getBackground() + '\n';
    toWrite += this->getMusic() + '\n';
    count = this->getWaves();

    for (int i = 0; i < count; i++) {
        std::cout << "\n   Describe wave nb " << i + 1 << std::endl;
        str = this->getMin();
        toWrite += str + '\n';
        toWrite += this->getMax(std::stoi(str)) + '\n';
        toWrite += this->getInterval() + '\n';
        toWrite += this->getEnemy() + '\n';
        toWrite += this->getBoss() + '\n';
    }
    toWrite.pop_back();
    std::fstream LevelFile = this->createFile();
    LevelFile << toWrite;
    LevelFile.close();
}

std::fstream Level::createFile() {
    std::string name("./src/server/levels/level");
    name.append(std::to_string(this->countLvls()));
    std::fstream LevelFile(name, std::fstream::out);
    return (LevelFile);
}

int Level::countLvls() {
    auto dirIter = std::filesystem::directory_iterator("./src/server/levels");
    int fileCount = 0;

    for (auto& entry : dirIter)
        if (entry.is_regular_file())
            ++fileCount;
    return fileCount + 1;
}