#include "Map.h"
#include "../Engine/Core/Settings/Config.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

Map::Map() : mapConfigLoaded(false) {}
Map::~Map() {}

void Map::OnLoaded(OnLoadedCallback callback) {
    onLoadedCallback = callback;
}

void Map::OnLoading(OnLoadingCallback callback) {
    onLoadingCallback = callback;
}

void Map::OnMapStart(OnMapStartCallback callback) {
    onMapStartCallback = callback;
}

void Map::Load(const std::string& name, OnMapStartCallback onStart) {
    std::cout << "[Map] Starting to load: " << name << std::endl;

    auto originalCallback       = onMapStartCallback;
    onMapStartCallback          = onStart;
    std::string mapDir          = "../Game/Maps/" + name + "/";
    std::string mapConfigPath   = mapDir + "Map.conf";

    /* If unpacked Map-File */
    if(std::filesystem::exists(mapDir) && std::filesystem::is_directory(mapDir)) {
        if(std::filesystem::exists(mapConfigPath)) {
            if(mapConfig.LoadFromFile(mapConfigPath)) {
                mapConfigLoaded = true;

                // Maps/Waterworld/
                /*std::string oldbg = mapConfig.GetString("Loading.Background");
                std::cout << "CONF_BG: " << oldbg << std::endl;
                mapConfig.Set("Loading.Background", "Maps/" + name + "/" + oldbg);*/

            } else {
                std::cout << "[Map] Warning: Failed to load map configuration from " << mapConfigPath << std::endl;
            }
        } else {
            std::cout << "[Map] Warning: Map.conf not found at " << mapConfigPath << std::endl;
        }
    } else {
        std::cout << "[Map] Error: Map directory not found: " << mapDir << std::endl;
    }

    if(onMapStartCallback) {
        onMapStartCallback(name, mapConfig);
    }

    // Load asynchronously to avoid blocking the main thread
    std::thread loadingThread([this, name]() {
        // Simulate loading process with callbacks
        const std::vector<std::string> steps = {
            "Loading terrain",
            "Loading textures",
            "Loading objects",
            "Loading entities",
            "Placing Balls",
            "Placing Event-Boxes",
            "Finalizing map"
        };

        const int totalSteps = steps.size();

        for(int i = 0; i < totalSteps; ++i) {
            if(onLoadingCallback) {
                onLoadingCallback(steps[i], i + 1, totalSteps, ((float)(i + 1) / totalSteps * 100.0f));
            }

            // Simulate loading time
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }

        std::cout << "[Map] Finished loading: " << name << std::endl;

        if(onLoadedCallback) {
            onLoadedCallback();
        }
    });

    loadingThread.detach();
}
