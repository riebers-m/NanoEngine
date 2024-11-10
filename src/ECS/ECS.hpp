//
// Created by HP on 18.10.2024.
//

#ifndef ECS_HPP
#define ECS_HPP
#include <bitset>
#include <deque>
#include <set>
#include <typeindex>
#include <unordered_map>

#include "const/Const.hpp"
#include "spdlog/logger.h"

namespace spdlog {
    class logger;
}
namespace ecs {
    ///////////////////////////////////////////////////////////////////////////////////////////
    /// Entity
    ///////////////////////////////////////////////////////////////////////////////////////////
    class Entity {
    private:
        std::size_t m_id;
        class Registry *m_registry;

    public:
        explicit Entity(std::size_t id, Registry *registry);
        Entity &operator=(Entity const &) = default;

        // Tag & Group management
        void tag(std::string const &tag);
        bool has_tag(std::string const &tag) const;
        void group(std::string const &group);
        bool has_group(std::string const &group) const;

        bool operator==(const Entity &) const;
        bool operator!=(const Entity &) const;
        bool operator>(const Entity &) const;
        bool operator<(const Entity &) const;

        void kill();

        [[nodiscard]] std::size_t get_id() const;
        template<typename T, typename... TArgs>
        void add_component(TArgs &&...);
        template<typename T>
        void remove_component();
        template<typename T>
        bool has_component() const;
        template<typename T>
        T &get_component();
        template<typename T>
        T &get_component() const;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////
    /// System
    ///////////////////////////////////////////////////////////////////////////////////////////
    using Signature = std::bitset<engine::MAX_COMPONENTS>;

    class System {
    private:
        using Entities = std::vector<Entity>;
        Signature m_component_signature;
        Entities m_entities;

    public:
        System();
        virtual ~System() = default;

        void add_entity(Entity entity);
        void remove_entity(Entity entity);
        [[nodiscard]] Entities get_entities() const;
        [[nodiscard]] Signature get_signature() const;

        template<typename T>
        void require_component();
    };

    ///////////////////////////////////////////////////////////////////////////////////////////
    /// Component
    ///////////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct is_simple_struct {
        // Check if T is a simple struct type
        static constexpr bool value = std::is_class_v<T> && !std::is_polymorphic_v<T>;
    };

    struct BaseComponent {
    protected:
        static std::size_t next_id;
    };

    template<typename T>
    class Component : public BaseComponent {
        static_assert(is_simple_struct<T>::value, "Component type is no aggregate type.");

    public:
        // Returns a unique id of Component<T>
        static std::size_t get_id() {
            static auto id = next_id++;
            return id;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////////////////
    /// Pool
    ///////////////////////////////////////////////////////////////////////////////////////////

    class BasePool {
    public:
        virtual ~BasePool() = default;
    };

    template<typename T>
    class Pool : public BasePool {
    private:
        using Container = std::vector<T>;

        Container m_data;

    public:
        explicit Pool(std::size_t size = 1000);
        ~Pool() override = default;

        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] std::size_t size() const;
        void resize(std::size_t new_size);
        void clear();
        void add(T const &);
        void set(std::size_t index, T const &);
        T &get(std::size_t index);
        T &operator[](std::size_t index);
    };


    ///////////////////////////////////////////////////////////////////////////////////////////
    /// Registry
    ///////////////////////////////////////////////////////////////////////////////////////////
    class Registry {
        using Components = std::vector<std::shared_ptr<BasePool>>;
        using Signatures = std::vector<Signature>;
        using Systems = std::unordered_map<std::type_index, std::shared_ptr<System>>;
        using Entities = std::set<Entity>;
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        std::size_t m_entity_count{};
        // Entities to be added/removed when update is called
        Entities m_to_be_added_entities;
        Entities m_to_be_removed_entities;
        // Each pool contains all the data for a certain component type
        // [vector index = component id, pool index = entity id]
        Components m_components;
        // The signature lets us know which component are turned on for an entity
        // [index = entity id]
        Signatures m_signatures;
        // Map of active systems
        // [index = system typeid]
        Systems m_systems;
        Logger m_logger;
        // Entity IDs who was previously removed
        std::deque<size_t> m_free_ids;

        std::unordered_map<std::size_t, std::string> m_tag_per_entity;
        std::unordered_map<std::string, Entity> m_entity_per_tag;

        std::unordered_map<std::string, std::set<Entity>> m_entity_groups;
        std::unordered_map<std::size_t, std::string> m_groupe_per_entity;

    public:
        explicit Registry(Logger logger);

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
        void add_entity_to_system(Entity const &entity);
        void remove_entity_from_system(Entity);
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

    ///////////////////////////////////////////////////////////////////////////////////////////
    /// Entity
    ///////////////////////////////////////////////////////////////////////////////////////////
    template<typename T, typename... TArgs>
    void Entity::add_component(TArgs &&...args) {
        m_registry->add_component<T>(*this, std::forward<TArgs>(args)...);
    }
    template<typename T>
    void Entity::remove_component() {
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
    ///////////////////////////////////////////////////////////////////////////////////////////
    /// System
    ///////////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    void System::require_component() {
        auto const component_id = Component<T>::get_id();
        m_component_signature.set(component_id);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////
    /// Pool
    ///////////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    Pool<T>::Pool(std::size_t size) : m_data() {
        resize(size);
    }
    template<typename T>
    bool Pool<T>::is_empty() const {
        return m_data.empty();
    }
    template<typename T>
    std::size_t Pool<T>::size() const {
        return m_data.size();
    }
    template<typename T>
    void Pool<T>::resize(std::size_t new_size) {
        m_data.resize(new_size);
    }
    template<typename T>
    void Pool<T>::clear() {
        m_data.clear();
    }
    template<typename T>
    void Pool<T>::add(T const &component) {
        m_data.push_back(component);
    }
    template<typename T>
    void Pool<T>::set(std::size_t index, T const &component) {
        if (index >= size()) {
            throw std::out_of_range(std::format("index {} out of range", index));
        }
        m_data[index] = component;
    }
    template<typename T>
    T &Pool<T>::get(std::size_t index) {
        if (index >= size()) {
            throw std::out_of_range(
                    std::format("index {} >= {} out of range for component pool {}", index, size(), typeid(T).name()));
        }
        return m_data[index];
    }
    template<typename T>
    T &Pool<T>::operator[](std::size_t index) {
        return get(index);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    /// Registry
    ///////////////////////////////////////////////////////////////////////////////////////////
    template<typename T, typename... TArgs>
    void Registry::add_component(Entity const &entity, TArgs &&...args) {
        auto const component_id = Component<T>::get_id();
        auto const entity_id = entity.get_id();

        if (entity_id >= m_signatures.size()) {
            throw std::out_of_range(std::format("Entity Id {} signature out of range", entity_id));
        }

        if (component_id >= m_components.size()) {
            m_components.resize((m_components.size() + 1) * 2, nullptr);
            m_logger->debug("resizing components vector to {}", m_components.size());
        }

        if (!m_components[component_id]) {
            m_components[component_id] = std::make_shared<Pool<T>>();
            m_logger->debug("adding component pool {} with id {}", typeid(T).name(), component_id);
        }

        auto component_pool = std::static_pointer_cast<Pool<T>>(m_components[component_id]);

        if (entity_id >= component_pool->size()) {
            component_pool->resize((component_pool->size() + 1) * 2);
            m_logger->debug("resizing component pool {} to {}", typeid(T).name(), component_pool->size());
        }
        T new_component{std::forward<TArgs>(args)...};

        component_pool->set(entity_id, new_component);
        m_signatures[entity_id].set(component_id);

        m_logger->debug("component id {} was added to entity id {}", component_id, entity_id);
    }
    template<typename T>
    void Registry::remove_component(Entity const &entity) {
        auto const component_id = Component<T>::get_id();
        auto const entity_id = entity.get_id();

        if (entity_id >= m_signatures.size()) {
            throw std::out_of_range(std::format("Entity Id {} signature out of range", entity_id));
        }

        m_signatures[entity_id].set(component_id, false);
        m_logger->debug("component id {} was removed to entity id {}", component_id, entity_id);
    }

    template<typename T>
    bool Registry::has_component(Entity entity) const {
        auto const component_id = Component<T>::get_id();
        auto const entity_id = entity.get_id();

        if (entity_id >= m_signatures.size()) {
            throw std::out_of_range(std::format("Entity Id {} signature out of range", entity_id));
        }
        return m_signatures[entity_id].test(component_id);
    }
    template<typename T>
    T &Registry::get_component(Entity const &entity) {
        auto const component_id = Component<T>::get_id();
        auto const entity_id = entity.get_id();

        if (component_id >= m_components.size()) {
            throw std::out_of_range(std::format("invalid component id {} >= {} for {}", component_id,
                                                m_components.size(), typeid(T).name()));
        }
        auto component = std::static_pointer_cast<Pool<T>>(m_components[component_id]);

        return component->get(entity_id);
    }


    template<typename T, typename... TArgs>
    void Registry::add_system(TArgs &&...args) {
        auto new_system = std::make_shared<T>(std::forward<TArgs>(args)...);
        auto const [pos, ok] = m_systems.insert(std::make_pair(std::type_index(typeid(T)), new_system));
        if (!ok) {
            throw std::runtime_error(std::format("inserting system {} failed", typeid(T).name()));
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
        return m_systems.find(std::type_index(typeid(T))) != m_systems.end();
    }
    template<typename T>
    T &Registry::get_system() const {
        if (auto system = m_systems.find(std::type_index(typeid(T))); system != m_systems.end()) {
            return *(std::static_pointer_cast<T>(system->second));
        }
        throw std::out_of_range(std::format("invalid system {}", typeid(T).name()));
    }

} // namespace ecs

#endif // ECS_HPP
