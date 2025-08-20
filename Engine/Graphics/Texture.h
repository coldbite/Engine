#pragma once
#ifdef _WIN32
    #include <windows.h>
    #include <gl/gl.h>
#endif
#include <string>
#include <stdexcept>

namespace Engine {
    namespace Graphics {
        class Texture {
            public:
#ifdef _WIN32
                GLuint id;
#else
                unsigned int id;
#endif
                std::string name;
                int width;
                int height;
        };
    }
}
