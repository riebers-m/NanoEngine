//
// Created by HP on 10.12.2024.
//

#include "ECS.hpp"

namespace ecs {
    Entity::Entity(Entityid id, Registry *registry) : m_id{id}, m_registry{registry} {}
    bool Entity::operator==(const Entity &other) const { return m_id == other.m_id; }
    bool Entity::operator!=(const Entity &other) const { return m_id != other.m_id; }
    bool Entity::operator>(const Entity &other) const { return m_id > other.m_id; }
    bool Entity::operator<(const Entity &other) const { return m_id < other.m_id; }
    void Entity::tag(std::string const &tag) { m_registry->add_tag(*this, tag); }
    [[nodiscard]] bool Entity::has_tag(std::string const &tag) const { return m_registry->has_tag(*this, tag); }
    void Entity::group(std::string const &group) { m_registry->add_to_group(*this, group); }
    [[nodiscard]] bool Entity::has_group(std::string const &group) const { return m_registry->has_group(*this, group); }
    void Entity::kill() const { m_registry->remove_entity(*this); }
    [[nodiscard]] Entityid Entity::get_id() const { return m_id; }

    System::System(registry registry) : m_registry{std::move(registry)} {}

    Registry::Registry(Logger logger) : m_registry{std::make_shared<entt::registry>()}, m_logger{std::move(logger)} {}
    void Registry::clear() { m_registry->clear(); }
    Entity Registry::create_entity() { return Entity{m_registry->create(), this}; }
    void Registry::remove_entity(Entity const &entity) { m_to_be_removed_entities.emplace(entity.get_id()); }
    void Registry::update() {
        for (auto const entity_id: m_to_be_removed_entities) {
            m_registry->destroy(entity_id);
        }
        m_to_be_removed_entities.clear();
    }
    void Registry::add_tag(Entity entity, std::string const &tag) {
        m_tag_per_entity.insert({entity.get_id(), tag});
        m_Entityper_tag.insert({tag, entity});
    }
    bool Registry::has_tag(Entity entity, std::string const &tag) const {
        if (!m_tag_per_entity.contains(entity.get_id())) {
            return false;
        }
        return m_tag_per_entity.find(entity.get_id())->second == tag;
    }
    Entity Registry::get_entity(std::string const &tag) const { return m_Entityper_tag.at(tag); }
    void Registry::remove_tag(Entity entity) {
        if (auto itr = m_tag_per_entity.find(entity.get_id()); itr != m_tag_per_entity.end()) {
            m_Entityper_tag.erase(itr->second);
            m_tag_per_entity.erase(itr);
        }
    }
    void Registry::add_to_group(Entity entity, std::string const &group) {
        m_Entitygroups.insert({group, std::set<Entity>{}});
        m_Entitygroups[group].insert(entity);
        m_groupe_per_entity.insert({entity.get_id(), group});
    }
    bool Registry::has_group(Entity entity, std::string const &group) const {
        if (auto const itr = m_groupe_per_entity.find(entity.get_id()); itr != m_groupe_per_entity.end()) {
            return itr->second == group;
        }
        return false;
    }
    std::vector<Entity> Registry::get_entities_from_group(std::string const &group) const {
        auto const group_set = m_Entitygroups.at(group);
        return std::vector<Entity>{group_set.cbegin(), group_set.cend()};
    }
    void Registry::remove_from_group(Entity entity) {
        if (auto group_tag = m_groupe_per_entity.find(entity.get_id()); group_tag != m_groupe_per_entity.end()) {
            if (auto group = m_Entitygroups.find(group_tag->second); group != m_Entitygroups.end()) {
                if (auto const Entityin_group = group->second.find(entity); Entityin_group != group->second.end()) {
                    group->second.erase(Entityin_group);
                    m_groupe_per_entity.erase(group_tag);
                }
            }
        }
    }
} // namespace ecs
