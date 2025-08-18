#include "Renderable.h"

namespace Engine {
    Renderable::Renderable(const std::string& name) 
        : name(name), visible(true) {
    }
}