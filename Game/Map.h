#pragma once
#include "../Engine/Core/World.h"
#include <functional>
#include <string>

class Map : public Engine::World {
public:
    using OnLoadedCallback = std::function<void()>;
    using OnLoadingCallback = std::function<void(const std::string& message, int actual, int total, float percentage)>;

    Map();
    Map(std::string name);
    
    void OnLoaded(OnLoadedCallback callback);
    void OnLoading(OnLoadingCallback callback);
    void Load(const std::string& name);

private:
    OnLoadedCallback onLoadedCallback;
    OnLoadingCallback onLoadingCallback;
    std::string mapName;
};
