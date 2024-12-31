project "gamelib"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    architecture "x86_64"
    files {"gamelib/**.cpp", "gamelib/**.hpp", "gamelib/**.c", "gamelib/**.h"}

    externalincludedirs { "../Vendor/include" }
    includedirs {  "../Vendors/imgui"}
    libdirs { "../Vendors/lib"}
    links {  "raylib", "glfw3", "WinMM", "yamlcpp", "lua", "box2dd", "imgui"}


    
    filter { "configurations:Debug" }
        runtime "Debug"
		symbols "On"
	filter {}

    filter { "configurations:Release" }
        runtime "Release"
	filter {}
