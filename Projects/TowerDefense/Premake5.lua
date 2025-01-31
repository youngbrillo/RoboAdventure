project "TowerDefense"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"
    staticruntime "Off"
    debuggertype "NativeOnly"
    architecture "x86_64"
    files { "source/**.cpp", "source/**.hpp", "source/**.c",  "source/**.h"}

    -- includes and linking -------------------------------------
    externalincludedirs {"../../Vendors/include"}
    includedirs {"../../Source",}
    includedirs {"../../Vendors/imgui"}
    
	links { "gamelib", "raylib", "glfw3", "WinMM", "yamlcpp", "lua", "box2dd", "imgui"}
    libdirs { "../../Vendors/lib"}

    -- filters ------------------------------------------
    filter { "configurations:Debug" }
        runtime "Debug"
		symbols "On"
	filter {}

    filter { "configurations:Release" }
        runtime "Release"
	filter {}

    -- post build commands ------------------------------------------
	postbuildcommands {
		-- '{COPYFILE} "%{wks.location}/Coral/Coral.Managed/Coral.Managed.runtimeconfig.json" "%{cfg.targetdir}"',
	}