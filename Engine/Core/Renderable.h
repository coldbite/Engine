#pragma once

#include <string>
#include <memory>

namespace Engine {
    class Renderable {
    public:
        Renderable(const std::string& name = "Unnamed");
        virtual ~Renderable() = default;
        
        const std::string& GetName() const { return name; }
        void SetName(const std::string& newName) { name = newName; }
        
        bool IsVisible() const { return visible; }
        void SetVisible(bool isVisible) { visible = isVisible; }
        
        // Pure virtual function for future rendering implementation
        virtual void PrepareForRendering() = 0;
        
    private:
        std::string name;
        bool visible;
    };
    
    using RenderablePtr = std::shared_ptr<Renderable>;
}