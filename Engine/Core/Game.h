#pragma once

#include "Engine.h"
#include "RenderManager.h"
#include "Settings/Options.h"
#include <memory>

namespace Engine {
    class Game {
    public:
        Game();
        virtual ~Game();
        
        bool Initialize();
        void Run();
        void Shutdown();
        
        virtual void OnInit() {}
        virtual void OnUpdate(float /*deltaTime*/) {}
        virtual void OnRender() {}
        virtual void OnShutdown() {}
        
        template<typename EnumType>
        void AddOptions() {
            Settings::OptionsManager::GetInstance().AddOptions<EnumType>();
        }
        
        template<typename EnumType, typename T>
        void SetOption(EnumType option, const T& value) {
            Settings::OptionsManager::GetInstance().SetOption(option, value);
        }
        
        template<typename EnumType>
        bool HasOption(EnumType option) const {
            return Settings::OptionsManager::GetInstance().HasOption(option);
        }
        
        template<typename EnumType, typename T>
        T GetOption(EnumType option, const T& defaultValue = T{}) const {
            return Settings::OptionsManager::GetInstance().GetOption<EnumType, T>(option, defaultValue);
        }
        
        template<typename EnumType>
        void PrintOptions() const {
            Settings::OptionsManager::GetInstance().PrintOptions<EnumType>();
        }
        
    protected:
        Engine& GetEngine() { return Engine::GetInstance(); }
        
    private:
        void SetupEventHandlers();
        void RunDemo();
        
        std::unique_ptr<RenderManager> renderManager;
        bool isInitialized;
    };
}