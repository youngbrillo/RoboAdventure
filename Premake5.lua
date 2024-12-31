workspace "Robo Quest"
    configurations { "Debug", "Release" }
	
    targetdir "%{wks.location}/Build/%{cfg.buildcfg}"
	objdir "%{wks.location}/Build/Intermediates/%{cfg.buildcfg}"
    -- location "Build/"

    startproject "TestBed"

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

group "Samples"
	-- include "Projects/Editor"
	include "Projects/Game"
	include "Projects/TestBed"
group ""

group "Libraries"
	include "Source"
	include "Vendors/imgui"
group ""
