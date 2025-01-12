#include <gtest/gtest.h>
#include "Core/LayerStack.h"

// A simple test layer class for testing
class TestLayer : public AE::Core::Layer {
public:
    TestLayer(const std::string& name) : Layer(name) {}
};

class LayerStackTest : public ::testing::Test {
protected:
    AE::Core::LayerStack layerStack;
};

TEST_F(LayerStackTest, PushLayerTest) {
    // Create a test layer
    auto* layer = new TestLayer("TestLayer");

    // Add it to the stack
    layerStack.PushLayer(layer);

    // Check if the layer is in the stack
    bool found = false;
    for (auto* stackLayer : layerStack) {
        if (stackLayer == layer) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found) << "Layer should be found in the stack";
}

TEST_F(LayerStackTest, PushOverlayTest) {
    // Create test layers
    auto* regularLayer = new TestLayer("RegularLayer");
    auto* overlayLayer = new TestLayer("OverlayLayer");

    // Add them to the stack
    layerStack.PushLayer(regularLayer);
    layerStack.PushOverlay(overlayLayer);

    // Overlay should be at the end of the stack
    auto it = layerStack.begin();
    EXPECT_EQ(*it, regularLayer) << "Regular layer should be first";

    it = --layerStack.end();
    EXPECT_EQ(*it, overlayLayer) << "Overlay layer should be last";
}

TEST_F(LayerStackTest, PopLayerTest) {
    auto* layer = new TestLayer("TestLayer");
    layerStack.PushLayer(layer);
    layerStack.PopLayer(layer);

    // Check if the layer is removed
    bool found = false;
    for (auto* stackLayer : layerStack) {
        if (stackLayer == layer) {
            found = true;
            break;
        }
    }
    EXPECT_FALSE(found) << "Layer should be removed from the stack";

    // Clean up the layer since LayerStack no longer manages it
    delete layer;
}
