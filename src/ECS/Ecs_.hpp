//
// Created by HP on 10.12.2024.
//
#pragma once

#include <typeindex>


#include "common/Logger.hpp"
#include "entt/entt.hpp"

namespace ecs {
    using entity_id = entt::entity;
    using registry = std::shared_ptr<entt::registry>;

    class Entity_ {
    private:
        entity_id m_id;
        class Registry_ *m_registry;

    public:
        explicit Entity_(entity_id id, Registry_ *registry);

        // Needed for usage in std containers
        bool operator==(const Entity_ &) const;
        bool operator!=(const Entity_ &) const;
        bool operator>(const Entity_ &) const;
        bool operator<(const Entity_ &) const;

        // Tag & Group management
        void tag(std::string const &tag);
        [[nodiscard]] bool has_tag(std::string const &tag) const;
        void group(std::string const &group);
        [[nodiscard]] bool has_group(std::string const &group) const;
        void kill() const;
        [[nodiscard]] entity_id get_id() const;

        template<typename T, typename... TArgs>
        void add_component(TArgs &&...);
        template<typename T>
        void remove_component() const;
        template<typename T>
        [[nodiscard]] bool has_component() const;
        template<typename T>
        T &get_component();
        template<typename T>
        T &get_component() const;
    };

    class System_ {
    private:
        registry m_registry;

    public:
        using Entities = std::vector<Entity_>;

        explicit System_(registry registry);

        virtual ~System_() = default;
    };

    class Registry_ {
        using Systems = std::unordered_map<std::type_index, std::shared_ptr<System_>>;
        using Logger = std::shared_ptr<engine::Logger>;
        using Entities = std::set<entity_id>;

    private:
        // Map of active systems
        // [index = system typeid]
        registry m_registry;
        Logger m_logger;
        Systems m_systems;
        // Entities to be added/removed when update is called
        Entities m_to_be_added_entities;
        Entities m_to_be_removed_entities;
        std::unordered_map<entity_id, std::string> m_tag_per_entity;
        std::unordered_map<std::string, Entity_> m_Entityper_tag;

        std::unordered_map<std::string, std::set<Entity_>> m_Entitygroups;
        std::unordered_map<entity_id, std::string> m_groupe_per_entity;

    public:
        explicit Registry_(Logger logger);

        void clear();
        Entity_ create_entity();
        void remove_entity(Entity_ const &entity);

        template<typename T, typename... TArgs>
        void add_component(Entity_ const &entity, TArgs &&...args);
        template<typename T>
        void remove_component(Entity_ const &entity);
        template<typename T>
        [[nodiscard]] bool has_component(Entity_ entity) const;
        template<typename T>
        T &get_component(Entity_ const &entity);

        template<typename T, typename... TArgs>
        void add_system(TArgs &&...args);
        template<typename T>
        void remove_system();
        template<typename T>
        [[nodiscard]] bool has_system() const;
        template<typename T>
        T &get_system() const;
        void update();

        // Tag management
        void add_tag(Entity_ entity, std::string const &tag);
        [[nodiscard]] bool has_tag(Entity_ entity, std::string const &tag) const;
        [[nodiscard]] Entity_ get_entity(std::string const &tag) const;
        void remove_tag(Entity_ entity);

        // Group management
        void add_to_group(Entity_ entity, std::string const &group);
        [[nodiscard]] bool has_group(Entity_ entity, std::string const &group) const;
        [[nodiscard]] std::vector<Entity_> get_entities_from_group(std::string const &group) const;
        void remove_from_group(Entity_ entity);
    };

    template<typename T, typename... TArgs>
    void Entity_::add_component(TArgs &&...args) {
        m_registry->add_component<T>(*this, std::forward<TArgs>(args)...);
    }
    template<typename T>
    void Entity_::remove_component() const {
        m_registry->remove_component<T>(*this);
    }
    template<typename T>
    bool Entity_::has_component() const {
        return m_registry->has_component<T>(*this);
    }
    template<typename T>
    T &Entity_::get_component() {
        return m_registry->get_component<T>(*this);
    }

    template<typename T>
    T &Entity_::get_component() const {
        return m_registry->get_component<T>(*this);
    }

    ///// Registry
    template<typename T, typename... TArgs>
    void Registry_::add_component(Entity_ const &entity, TArgs &&...args) {
        m_registry->emplace<T>(entity.get_id(), std::forward<TArgs>(args)...);
    }
    template<typename T>
    void Registry_::remove_component(Entity_ const &entity) {
        if (auto const removed = m_registry->remove<T>(entity.get_id()); removed == 0) {
            m_logger->warn("Could not remove component {} from entity {}", typeid(T).name(), entity.get_id());
        }
    }
    template<typename T>
    bool Registry_::has_component(Entity_ entity) const {
        return m_registry->all_of<T>(entity.get_id());
    }
    template<typename T>
    T &Registry_::get_component(Entity_ const &entity) {
        return m_registry->get<T>(entity.get_id());
    }
    template<typename T, typename... TArgs>
    void Registry_::add_system(TArgs &&...args) {
        auto new_system = std::make_shared<T>(std::forward<TArgs>(args)...);
        auto const [pos, ok] = m_systems.insert(std::make_pair(std::type_index(typeid(T)), new_system));
        if (!ok) {
            throw std::runtime_error(std::format("Inserting system {} failed", typeid(T).name()));
        }
    }
    template<typename T>
    void Registry_::remove_system() {
        if (auto to_remove = m_systems.find(std::type_index(typeid(T))); to_remove != m_systems.end()) {
            m_systems.erase(to_remove);
        }
    }
    template<typename T>
    bool Registry_::has_system() const {
        return m_systems.find(std::type_index(typeid(T)) != m_systems.cend());
    }
    template<typename T>
    T &Registry_::get_system() const {
        if (auto system = m_systems.find(std::type_index(typeid(T))); system != m_systems.end()) {
            return std::static_pointer_cast<T>(system->second);
        }
        throw std::out_of_range(std::format("Invalid system {}", typeid(T).name()));
    }

} // namespace ecs
