#include "test.h"

testbed::TestManager* testbed::TestManager::instance = nullptr;

testbed::TestManager::TestManager()
    : entries({})
    , currentIndex(0)
    , defaultIndex(0)
{
}

testbed::TestManager::~TestManager()
{
}

testbed::TestManager& testbed::TestManager::Get()
{
    // TODO: insert return statement here
    if (instance == nullptr)
        instance = new TestManager();

    return *instance;
}

int testbed::TestManager::AddEntry(std::string name, std::string category, TestGenerationFunction func)
{
    Get().entries.emplace_back(name, category, func);
    return Get().entries.size() - 1;
}

testbed::Test* testbed::TestManager::GetDefaultTest()
{
    return TestManager::GetTestAt(Get().defaultIndex);
}

testbed::Test* testbed::TestManager::GetTestAt(int index)
{
    if (index < 0 || index >= Get().entries.size())
        return nullptr;

    TestEntry entry = Get().entries[index];
    return entry.generator();
}

void testbed::TestManager::listenforInputs()
{
    if (IsKeyPressed(KEY_R))
    {
        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
                wantNewTest = true;
            else
                wantRestartTest = true;
        }
    }

    if (wantNewTest)
    {
        wantNewTest = false;
        if (ActiveTest) {
            ActiveTest->Unset();
            delete ActiveTest;
        }
        ActiveTest = GetTestAt(currentIndex);

        if (ActiveTest)
        {
            ActiveTest->Set();
        }
    }
    else if (wantRestartTest && ActiveTest)
    {
        ActiveTest->Unset();
        ActiveTest->Set();
        wantRestartTest = false;
    }


}

void testbed::TestManager::Update(float dt)
{
    if (ActiveTest) ActiveTest->Update(dt);

}

void testbed::TestManager::Draw()
{
    if (ActiveTest) {
        ClearBackground(ActiveTest->backgroundColor);
        ActiveTest->Draw();
    }
    else
    {
        ClearBackground(WHITE);
    }
}

void testbed::TestManager::Inspect()
{
    DrawMenu(ActiveTest);
    if (ActiveTest) ActiveTest->InspectWrapper();
}

void testbed::TestManager::DrawMenu(Test* active)
{

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Controls"))
        {
            if (ImGui::MenuItem("Restart", "Ctrl+R"))
            {
                wantRestartTest = true;
            }
            if (ImGui::MenuItem("Reload", "Ctrl+Shift+R"))
            {
                wantNewTest = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tests"))
        {
            for (int i = 0; i < entries.size(); i++)
            {
                if (ImGui::MenuItem(entries[i].name.c_str()))
                {
                    currentIndex = i;
                    wantNewTest = true;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

////#include <gamelib/core/utils/serialization/commonSerialization.h>
#include <gamelib/gameLib.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <filesystem>

void testbed::TestManager::SaveSettings(const char* fileLocation)
{
    auto& i = Get();
    //YAML::Emitter out;
    //out << YAML::BeginMap;
    //out << YAML::Key << "currentIndex" << YAML::Value << i.currentIndex;
    //out << YAML::Key << "defaultIndex" << YAML::Value << i.defaultIndex;
    //out << YAML::EndMap;


    //std::ofstream fileout(fileLocation);
    //fileout << out.c_str();

}

void testbed::TestManager::LoadSettings(const char* fileLocation)
{
    std::filesystem::path fp = fileLocation;

    if (!std::filesystem::exists(fp))
        return;
    //std::ifstream stream(fp);
    //std::stringstream strStream;
    //strStream << stream.rdbuf();
    //auto& i = Get();

    //YAML::Node root = YAML::Load(strStream.str());
    //i.currentIndex = root["currentIndex"].as<int>();
    //i.defaultIndex = root["defaultIndex"].as<int>();

}

void testbed::Test::InspectWrapper()
{
    if (!isInspecting) 
        return;
    ImGui::Begin(name.c_str(), &isInspecting);
        this->Inspect();
    ImGui::End();
}
