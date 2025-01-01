#pragma once
#define ENTT_NO_ETO

#include <entt/entt.hpp>
#include <raylib/raylib.h>
#include "UUID.h"

namespace glib
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(const Entity& e) = default;
        Entity(entt::entity handle, entt::registry* registry)
            : m_entityHandle(handle), m_world(registry)
        {
        }
        Entity(entt::entity handle, entt::registry& registry)
            : m_entityHandle(handle), m_world(&registry)
        {
        }

        const char* Name();
        glib::UUID GUID() const;
        Vector3& Position();
        Vector3& Size();
        Vector4& Quaternion();

        void SetVisibility(bool value);
        bool isVisible();
        void Enable(bool value);
        bool isEnabled();

        template<typename T>
        bool HasComponent()
        {
            return !!m_world->try_get<T>(m_entityHandle);
        }

        template<typename T, typename... Args>
        inline T& AddComponent(Args&&... args)
        {
            return m_world->emplace<T>(m_entityHandle, std::forward<Args>(args)...);
        }


        template<typename T, typename... Args>
        inline T& AddOrReplaceComponent(Args&&... args)
        {
            return m_world->emplace_or_replace<T>(m_entityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            return m_world->get<T>(m_entityHandle);
        }

        template<typename T>
        T& GetOrAddComponent()
        {
            return m_world->get_or_emplace<T>(m_entityHandle);
        }
        template<typename T>
        T* TryGetComponent()
        {
            return m_world->try_get<T>(m_entityHandle);
        }
        template<typename T>
        void RemoveComponent()
        {
            m_world->remove<T>(m_entityHandle);
        }
    public:
        void Destroy();
    public:
        operator bool() const { return m_entityHandle != entt::null; }
        operator entt::entity() const { return m_entityHandle; }
        operator uint32_t() const { return (uint32_t)m_entityHandle; }

        bool operator==(const Entity& other) const
        {
            return m_entityHandle == other.m_entityHandle && m_world == other.m_world;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }
    private:
        entt::entity m_entityHandle{ entt::null };
        entt::registry* m_world = nullptr;
    };
}



