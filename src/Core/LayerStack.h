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
     * @brief Manages a stack of layers with separate overlay layers
     *
     * The LayerStack manages the ordering and updating of layers in the engine.
     * It supports two types of layers:
     * - Regular layers: Rendered in the middle of the stack
     * - Overlay layers: Always rendered on top of regular layers
     *
     * Layers are managed as pointers to allow polymorphic behavior of different
     * layer types while avoiding object slicing.
     */
    class LayerStack
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
        void PopLayer(Layer* layer);

        /**
         * @brief Removes an overlay layer from the stack
         * @param overlay Pointer to the overlay layer to remove
         */
        void PopOverlay(Layer* overlay);

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
