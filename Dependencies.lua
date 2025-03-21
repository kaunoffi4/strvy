

VULKAN_SDK = os.getenv("VULKAN_SDK");

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/strvy/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/strvy/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/strvy/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/strvy/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/strvy/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/strvy/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/strvy/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/strvy/vendor/ImGuizmo"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["Assimp"] = "%{wks.location}/strvy/vendor/assimp/assimp-5.4.3/install/include"



LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

LibraryDir["AssimpPath"] = "%{wks.location}/strvy/vendor/assimp/assimp-5.4.3/install/lib"

Library["Assimp_Debug"] = "%{LibraryDir.AssimpPath}/assimp-vc143-mtd.lib"

Library["Assimp_Release"] = "%{LibraryDir.AssimpPath}/assimp-vc143-mt.lib"
