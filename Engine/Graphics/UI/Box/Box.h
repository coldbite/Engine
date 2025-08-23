#pragma once

#include "../Alignment.h"
#include "../../RGBA.h"
#include "../Animator.h"

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;

        struct Alignment {
            HorizontalAlignment horizontal;
            VerticalAlignment vertical;

            Alignment(HorizontalAlignment h = HorizontalAlignment::LEFT,
                         VerticalAlignment v = VerticalAlignment::TOP)
                : horizontal(h), vertical(v) {}

            // Common alignment presets
            static const Alignment TOP_LEFT;
            static const Alignment TOP_CENTER;
            static const Alignment TOP_RIGHT;
            static const Alignment CENTER_LEFT;
            static const Alignment CENTER;
            static const Alignment CENTER_RIGHT;
            static const Alignment BOTTOM_LEFT;
            static const Alignment BOTTOM_CENTER;
            static const Alignment BOTTOM_RIGHT;
        };

        class Box {
        public:
            Box(float width = 100.0f, float height = 100.0f);

            void Update(float deltaTime);
            void Render(IRenderingAPI& context);
            void Render(IRenderingAPI& context, float x, float y);
            void Render(IRenderingAPI& context, const Alignment& alignment);

            // Position and size
            void SetSize(float width, float height) { this->width = width; this->height = height; }
            void SetWidth(float width) { this->width = width; }
            void SetHeight(float height) { this->height = height; }
            void SetMargin(float x, float y);
            void SetMargin(float top, float right, float bottom, float left);

            float GetWidth() const { return width; }
            float GetHeight() const { return height; }

            // Appearance
            void SetColor(IColor* color);
            void SetBorderColor(IColor* color);
            void SetBorderWidth(float width) { this->borderWidth = width; }
            void SetShadow(float radius, IColor* color, float offsetX = 2.0f, float offsetY = 2.0f);

            float GetBorderWidth() const { return borderWidth; }

            // Animation system  
            TextAnimator& GetAnimator() { return animator; }

        private:
            float width, height;
            IColor* color;
            IColor* borderColor;
            float borderWidth;
            float m_marginTop, m_marginRight, m_marginBottom, m_marginLeft;
            
            // Shadow properties
            float shadowRadius;
            IColor* shadowColor;
            float shadowOffsetX, shadowOffsetY;
            
            TextAnimator animator;
        };
    }
}
