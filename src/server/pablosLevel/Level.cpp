/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Level
*/

/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Level
*/

#include "Level.hpp"

bool Level::nextLvl()
{
    this->clear();
    this->currentLvl += 1;
    std::string name("./levels/level"), line;
    name.append(std::to_string(currentLvl));
    std::fstream lvlFile(name, std::ios_base::in);
    Wave tmp;
    if (lvlFile.is_open() == false)
        return false;
    std::getline(lvlFile, line);
    this->mapId = std::atoi(line.c_str());
    std::getline(lvlFile, line);
    this->mapSpeed = std::atoi(line.c_str());
    std::getline(lvlFile, line);
    this->bossCountdown = std::atoi(line.c_str());

    while (std::getline(lvlFile, line)) {
        tmp.minSpawned = std::atoi(line.c_str());
        std::getline(lvlFile, line);
        tmp.maxSpawned = std::atoi(line.c_str());
        std::getline(lvlFile, line);
        tmp.typesFlag = std::atoi(line.c_str());
        std::getline(lvlFile, line);
        tmp.spawnInterval = std::atof(line.c_str());
        this->levelWaves.push_back(tmp);
    }
    lvlFile.close();
    return true;
}

void Level::loadLvlNb(int nb)
{
    std::fstream lvlFile(std::string("./levels/level").append(std::to_string(nb)), std::ios_base::in);
    std::string line;
    Wave tmp;
    tmp.finishedFlag = false;
    tmp.endless = false;
    this->currentLvl = 1;

    std::getline(lvlFile, line);
    this->mapId = std::atoi(line.c_str());
    std::getline(lvlFile, line);
    this->mapSpeed = std::atoi(line.c_str());
    std::getline(lvlFile, line);
    this->bossCountdown = std::atoi(line.c_str());

    while (std::getline(lvlFile, line)) {
        tmp.minSpawned = std::atoi(line.c_str());
        std::getline(lvlFile, line);
        tmp.maxSpawned = std::atoi(line.c_str());
        std::getline(lvlFile, line);
        tmp.typesFlag = std::atoi(line.c_str());
        std::getline(lvlFile, line);
        tmp.spawnInterval = std::atof(line.c_str());
        srand(time(NULL));
        tmp.spawned = rand() % tmp.maxSpawned + tmp.minSpawned;
        this->levelWaves.push_back(tmp);
    }
    lvlFile.close();
    this->currentLvl = 1;
}

void Level::createLvl()
{
    int nb = 0;
    int count;
    Wave tmp;
    Level toWrite;
    std::string str;
    tmp.finishedFlag = false;
    tmp.endless = false;

    std::cout << "What background should we use?" << std::endl;
    std::cin >> toWrite.mapId;
    std::cout << "How fast should the background be?" << std::endl;
    std::cin >> toWrite.mapSpeed;
    std::cout << "After how many seconds should the boss show up?" << std::endl;
    std::cin >> toWrite.bossCountdown;
    std::cout << "How many enemy waves should there be?" << std::endl;
    std::cin >> count;

    for (int i = 0; i < count; i++) {
        tmp.typesFlag = 0;
        tmp.maxSpawned = 0;
        tmp.minSpawned = 0;
        tmp.spawnInterval = 0.0;
        std::cout << "\n   Describe wave nb " << i+1 << std::endl;
        std::cout << "How many enemies should there be at least?" << std::endl;
        std::cin >> tmp.minSpawned;
        std::cout << "How many enemies should there be at max?" << std::endl;
        std::cin >> tmp.maxSpawned;
        std::cout << "Should there be enemies of type ORB?";

        for (int x = 0; tmp.typesFlag == 0; x++) {
            std::cout << " [+/-]" << std::endl;
            std::cin >> str;
            if (str.compare("+") == 0) {
                tmp.typesFlag += 1;
            } else if (str.compare("-") == 0);
                std::cout << "--We need more enemy types!--" << std::endl;
        }
        std::cout << "what should the spawn interval be?" << std::endl;
        std::cin >> tmp.spawnInterval;
        toWrite.levelWaves.push_back(tmp);
    }
    std::fstream LvlFile = this->createFile();
    this->writeLvlToFile(LvlFile, toWrite);
}

std::fstream Level::createFile()
{
    std::string name("./levels/level");
    name.append(std::to_string(this->countLvls()));
    std::fstream LvlFile(name, std::fstream::out);
    return (LvlFile);
}

void Level::writeLvlToFile(std::fstream &lvlFile, Level &toWrite)
{
    lvlFile << toWrite.mapId << std::endl;
    lvlFile << toWrite.mapSpeed << std::endl;
    lvlFile << toWrite.bossCountdown << std::endl;

    for (auto i : toWrite.levelWaves) {
        lvlFile << i.minSpawned << std::endl;
        lvlFile << i.maxSpawned << std::endl;
        lvlFile << i.typesFlag << std::endl;
        lvlFile << i.spawnInterval << std::endl;
    }
    lvlFile.close();
}

int Level::countLvls()
{
    auto dirIter = std::filesystem::directory_iterator("./levels");
    int fileCount = 0;

    for (auto& entry : dirIter)
        if (entry.is_regular_file())
            ++fileCount;
    return fileCount+1;
}

void Level::print()
{
    std::cout << "mapId=" << this->mapId << std::endl;
    std::cout << "mapSpeed=" << this->mapSpeed << std::endl;
    std::cout << "bossCountdown=" << this->bossCountdown << std::endl;
    std::cout << std::endl;


    for (int i = 0; i < this->levelWaves.size(); i++) {
    std::cout << "-- content for wave " << i+1 << " --"<< std::endl;
        std::cout << "minSpawned=" << levelWaves[i].minSpawned << std::endl;
        std::cout << "maxSpawned=" << levelWaves[i].maxSpawned << std::endl;
        std::cout << "typesFlag=" << levelWaves[i].typesFlag << std::endl;
        std::cout << "spawnInterval=" << levelWaves[i].spawnInterval << std::endl << std::endl;
    }
}

void Level::clear()
{
    this->bossCountdown = 0;
    this->levelWaves.clear();
    this->mapId = 0;
    this->mapSpeed = 0;
}

void Level::startLvl()
{
    this->clock.start();
    this->waveNb = 0;
}

void Level::exec()
{
    int spawns = 0;
    double intervals = (double)clock.interval().count();
    int types = levelWaves[waveNb].typesFlag;
    spawns = intervals;

    if (intervals > levelWaves[waveNb].spawnInterval && levelWaves[waveNb].spawned > 0) {
        clock._enemy = GetNow();
        std::cout << "spawning enemy\n"; // create enemy here
        levelWaves[waveNb].spawned--;
    }
    if (clock.boss().count() > bossCountdown) {
        clock._boss = GetNow();
        std::cout << "spawning boss\n"; // create boss here
    }
    // if (allEnemiesDead) { //pseudo code
    //     waveNb += 1;
    //     if (waveNb >= levelWaves.size() && bossDead)
    //         if (this->nextLvl() == false)
    //             over = true;
    //         else
    //             currentLvl += 1;
    // }
    return;
}