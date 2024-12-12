//
// Created by HP on 10.12.2024.
//
#pragma once

#include <typeindex>


#include "common/Logger.hpp"
#include "entt/entt.hpp"

namespace ecs {
    using Entityid = entt::entity;
    using registry = std::shared_ptr<entt::registry>;

    class Entity {
    private:
        Entityid m_id;
        class Registry *m_registry;

    public:
        explicit Entity(Entityid id, Registry *registry);

        // Needed for usage in std containers
        bool operator==(const Entity &) const;
        bool operator!=(const Entity &) const;
        bool operator>(const Entity &) const;
        bool operator<(const Entity &) const;

        // Tag & Group management
        void tag(std::string const &tag);
        [[nodiscard]] bool has_tag(std::string const &tag) const;
        void group(std::string const &group);
        [[nodiscard]] bool has_group(std::string const &group) const;
        void kill() const;
        [[nodiscard]] Entityid get_id() const;

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

    class System {
    protected:
        registry m_registry;

    public:
        explicit System(registry registry);

        virtual ~System() = default;
    };

    class Registry {
        using Systems = std::unordered_map<std::type_index, std::shared_ptr<System>>;
        using Logger = std::shared_ptr<engine::Logger>;
        using Entities = std::set<Entityid>;

    private:
        // Map of active systems
        // [index = system typeid]
        registry m_registry;
        Logger m_logger;
        Systems m_systems;
        // Entities to be added/removed when update is called
        Entities m_to_be_added_entities;
        Entities m_to_be_removed_entities;
        std::unordered_map<Entityid, std::string> m_tag_per_entity;
        std::unordered_map<std::string, Entity> m_Entityper_tag;

        std::unordered_map<std::string, std::set<Entity>> m_Entitygroups;
        std::unordered_map<Entityid, std::string> m_groupe_per_entity;

    public:
        explicit Registry(Logger logger);

        void clear();
        Entity create_entity();
        void remove_entity(Entity const &entity);

        template<typename T, typename... TArgs>
        void add_component(Entity const &entity, TArgs &&...args);
        template<typename T>
        void remove_component(Entity const &entity);
        template<typename T>
        [[nodiscard]] bool has_component(Entity entity) const;
        template<typename T>
        T &get_component(Entity const &entity);

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
        void add_tag(Entity entity, std::string const &tag);
        [[nodiscard]] bool has_tag(Entity entity, std::string const &tag) const;
        [[nodiscard]] Entity get_entity(std::string const &tag) const;
        void remove_tag(Entity entity);

        // Group management
        void add_to_group(Entity entity, std::string const &group);
        [[nodiscard]] bool has_group(Entity entity, std::string const &group) const;
        [[nodiscard]] std::vector<Entity> get_entities_from_group(std::string const &group) const;
        void remove_from_group(Entity entity);
    };

    template<typename T, typename... TArgs>
    void Entity::add_component(TArgs &&...args) {
        m_registry->add_component<T>(*this, std::forward<TArgs>(args)...);
    }
    template<typename T>
    void Entity::remove_component() const {
        m_registry->remove_component<T>(*this);
    }
    template<typename T>
    bool Entity::has_component() const {
        return m_registry->has_component<T>(*this);
    }
    template<typename T>
    T &Entity::get_component() {
        return m_registry->get_component<T>(*this);
    }

    template<typename T>
    T &Entity::get_component() const {
        return m_registry->get_component<T>(*this);
    }

    ///// Registry
    template<typename T, typename... TArgs>
    void Registry::add_component(Entity const &entity, TArgs &&...args) {
        m_registry->emplace<T>(entity.get_id(), std::forward<TArgs>(args)...);
    }
    template<typename T>
    void Registry::remove_component(Entity const &entity) {
        if (auto const removed = m_registry->remove<T>(entity.get_id()); removed == 0) {
            m_logger->warn("Could not remove component {} from entity {}", typeid(T).name(), entity.get_id());
        }
    }
    template<typename T>
    bool Registry::has_component(Entity entity) const {
        return m_registry->all_of<T>(entity.get_id());
    }
    template<typename T>
    T &Registry::get_component(Entity const &entity) {
        return m_registry->get<T>(entity.get_id());
    }
    template<typename T, typename... TArgs>
    void Registry::add_system(TArgs &&...args) {
        auto new_system = std::make_shared<T>(m_registry, std::forward<TArgs>(args)...);
        auto const [pos, ok] = m_systems.insert(std::make_pair(std::type_index(typeid(T)), new_system));
        if (!ok) {
            throw std::runtime_error(std::format("Inserting system {} failed", typeid(T).name()));
        }
    }
    template<typename T>
    void Registry::remove_system() {
        if (auto to_remove = m_systems.find(std::type_index(typeid(T))); to_remove != m_systems.end()) {
            m_systems.erase(to_remove);
        }
    }
    template<typename T>
    bool Registry::has_system() const {
        return m_systems.find(std::type_index(typeid(T)) != m_systems.cend());
    }
    template<typename T>
    T &Registry::get_system() const {
        if (auto system = m_systems.find(std::type_index(typeid(T))); system != m_systems.end()) {
            return *std::static_pointer_cast<T>(system->second);
        }
        throw std::out_of_range(std::format("Invalid system {}", typeid(T).name()));
    }

} // namespace ecs
