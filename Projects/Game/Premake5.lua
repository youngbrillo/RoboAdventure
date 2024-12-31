project "Robo Adventure (Game)"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"
    staticruntime "Off"
    debuggertype "NativeOnly"
    architecture "x86_64"
    files { "source/**.cpp", "source/**.hpp", "source/**.c",  "source/**.h"}

    -- includes and linking -------------------------------------
    externalincludedirs {"../../Vendor/include"}
    includedirs {"../../Source",}
    includedirs {"../../Vendor/imgui"}
    
	links { "gamelib", "raylib", "glfw3", "WinMM", "yamlcpp", "lua", "box2dd", "imgui"}
    libdirs { "../../Vendor/lib"}

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