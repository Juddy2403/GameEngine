#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN

#include <glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32

#include <glm/glm.hpp>
#include "VulkanUtil.h"
#include "Shader.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "meshes/3DMesh.h"
#include "Level.h"
#include "RenderPass.h"
#include <GraphicsPipeline.h>
#include "TimeManager.h"
#include "Camera.h"
#include "SwapChain.h"
#include <unordered_set>

namespace VulkanEngine
{
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    enum class ShadingMode
    {
        lambertMode,
        normalMode,
        specularMode,
        combinedMode
    };

    class VulkanBase
    {
    public:
        void Run()
        {
            InitWindow();
            InitVulkan();
            MainLoop();
            Cleanup();
        }

        static VkPhysicalDevice m_PhysicalDevice;
        static VkDevice m_Device;
        static VkExtent2D m_SwapChainExtent;
        static VkQueue m_GraphicsQueue;
    private:
        void InitVulkan();
        void MainLoop();
        void Cleanup() const;
        void CreateSurface();

        ShadingMode m_ShadingMode = ShadingMode::combinedMode;
        static uint32_t m_Width;
        static uint32_t m_Height;
        static bool m_HasWindowResized;
        Camera m_Camera{};
        Shader m_3DShader{
            "shaders/CompiledSpv/3Dshader.vert.spv",
            "shaders/CompiledSpv/3Dshader.frag.spv" };
        Shader m_2DShader{
            "shaders/CompiledSpv/2Dshader.vert.spv",
            "shaders/CompiledSpv/2Dshader.frag.spv" };

        CommandPool m_CommandPool{};
        CommandBuffer m_CommandBuffer{};
        Level m_Level{};
        RenderPass m_RenderPass{};
        GraphicsPipeline m_3DGraphicsPipeline{};
        GraphicsPipeline m_2DGraphicsPipeline{};
        glm::vec2 m_LastMousePos{};
        std::unordered_set<int> m_PressedKeys;
        GLFWwindow* m_Window = nullptr;

        void InitWindow();
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice vkDevice) const;
        void DrawFrame(uint32_t imageIndex);
        SwapChain m_SwapChain{};
        VkQueue m_PresentQueue = VK_NULL_HANDLE;
        void PickPhysicalDevice() const;
        bool IsDeviceSuitable(VkPhysicalDevice device) const;
        void CreateLogicalDevice();

        VkInstance m_Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        size_t m_CurrentFrame = 0;

        static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void SetupDebugMessenger();
        static std::vector<const char*> GetRequiredExtensions();
        static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        void CreateInstance();
        void CreateSyncObjects();
        void DrawFrame();

        void KeyEvent(int key, int scancode, int action, int mods);
        void MouseMove(GLFWwindow* window, double xpos, double ypos);
        void MouseEvent(GLFWwindow* window, int button, int action, int mods);
        void ProcessInput();
        static void WindowResized(GLFWwindow* window, int width, int height);

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType
            , const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
        {
            std::cerr << "validation layer: " << pCallbackData->pMessage << '\n';
            return VK_FALSE;
        }
    };
}
