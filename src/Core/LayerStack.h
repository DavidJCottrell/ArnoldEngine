#pragma once

#include "aepch.h"
#include "Layer.h"

namespace AE::Core
{
    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *overlay);
        void PopLayer(Layer *layer);
        void PopOverlay(Layer *overlay);

        /*
            Vector of pointers because Layer is a base class that other layers inherit.
            Storing derived classes would cause object slicing, whereas storing pointers
            to different derived layer types maintains their full functionality.
        */
        std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer *>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer *> m_Layers;
        std::vector<Layer *>::iterator m_LayerInsert;
    };
}