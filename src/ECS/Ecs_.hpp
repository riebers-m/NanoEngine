//
// Created by HP on 10.12.2024.
//
#pragma once

#include <typeindex>


#include "common/Logger.hpp"
#include "entt/entt.hpp"

namespace ecs {
    using entity_id = entt::entity;

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
        void kill();
        [[nodiscard]] entity_id get_id() const;

        template<typename T, typename... TArgs>
        void add_component(TArgs &&...);
        template<typename T>
        void remove_component();
        template<typename T>
        [[nodiscard]] bool has_component() const;
        template<typename T>
        T &get_component();
        template<typename T>
        T &get_component() const;
    };

    class Registry_ {
        // using Systems = std::unordered_map<std::type_index, std::shared_ptr<System>>;
        using Logger = std::shared_ptr<engine::Logger>;
        using Entities = std::set<entity_id>;

    private:
        // Map of active systems
        // [index = system typeid]
        // Systems m_systems;
        entt::registry m_registry;
        Logger m_logger;
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
        // m_registry->add_component<T>(*this, std::forward<TArgs>(args)...);
    }
    template<typename T>
    void Entity_::remove_component() {
        // m_kregistry->remove_component<T>(*this);
    }
    template<typename T>
    bool Entity_::has_component() const {
        return false;
        // retkurn m_registry->has_component<T>(*this);
    }
    template<typename T>
    T &Entity_::get_component() {
        // return m_registry->get_component<T>(*this);
        return T{};
    }

    template<typename T>
    T &Entity_::get_component() const {
        // rketurn m_registry->get_component<T>(*this);
        return T{};
    }

} // namespace ecs
