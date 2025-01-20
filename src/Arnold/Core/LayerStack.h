#pragma once

/**
 * @file
 * @brief Layer management system for organizing and updating game/application layers
 */

#include "aepch.h"
#include "Layer.h"

namespace AE::Core
{
    /**
     * @class LayerStack
     * @brief Manages a stack of layers with separate overlay layers
     *
     * The LayerStack provides ordered management of layers and overlays:
     * - Regular layers are rendered in the middle of the stack
     * - Overlay layers are always rendered on top
     * - Supports dynamic adding/removing of layers
     * - Maintains proper render and update order
     *
     * @note Layers are processed from bottom to top for updates, and top to bottom for events
     */
    class ARNOLD_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        /**
         * @brief Adds a regular layer to the stack
         * @param layer Pointer to the layer to add
         *
         * Regular layers are inserted before overlay layers to maintain proper rendering order
         */
        void PushLayer(Layer* layer);

        /**
         * @brief Adds an overlay layer to the stack
         * @param overlay Pointer to the overlay layer to add
         *
         * Overlay layers are always added to the end of the stack to render on top
         */
        void PushOverlay(Layer* overlay);

        /**
         * @brief Removes a regular layer from the stack
         * @param layer Pointer to the layer to remove
         */
        void PopLayer(const Layer* layer);

        /**
         * @brief Removes an overlay layer from the stack
         * @param overlay Pointer to the overlay layer to remove
         */
        void PopOverlay(const Layer* overlay);

        /**
         * @brief Gets iterator to the beginning of the layer stack
         * @return Iterator pointing to first layer
         */
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }

        /**
         * @brief Gets iterator to the end of the layer stack
         * @return Iterator pointing past the last layer
         */
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
}
