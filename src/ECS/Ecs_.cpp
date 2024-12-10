//
// Created by HP on 10.12.2024.
//

#include "Ecs_.hpp"

namespace ecs {
    Entity_::Entity_(entity_id id, Registry_ *registry) : m_id{id}, m_registry{registry} {}
    bool Entity_::operator==(const Entity_ &other) const { return m_id == other.m_id; }
    bool Entity_::operator!=(const Entity_ &other) const { return m_id != other.m_id; }
    bool Entity_::operator>(const Entity_ &other) const { return m_id > other.m_id; }
    bool Entity_::operator<(const Entity_ &other) const { return m_id < other.m_id; }
    void Entity_::tag(std::string const &tag) {}
    [[nodiscard]] bool Entity_::has_tag(std::string const &tag) const { return false; }
    void Entity_::group(std::string const &group) {}
    [[nodiscard]] bool Entity_::has_group(std::string const &group) const { return false; }
    void Entity_::kill() { m_registry->remove_entity(*this); }
    [[nodiscard]] entity_id Entity_::get_id() const { return m_id; }


    Registry_::Registry_(Logger logger) : m_logger{std::move(logger)} {}
    void Registry_::clear() { m_registry.clear(); }
    Entity_ Registry_::create_entity() { return Entity_{m_registry.create(), this}; }
    void Registry_::remove_entity(Entity_ const &entity) { m_to_be_removed_entities.emplace(entity.get_id()); }
    void Registry_::update() {
        for (auto const entity_id: m_to_be_removed_entities) {
            m_registry.destroy(entity_id);
        }
        m_to_be_removed_entities.clear();
    }
    void Registry_::add_tag(Entity_ entity, std::string const &tag) {
        m_tag_per_entity.insert({entity.get_id(), tag});
        m_Entityper_tag.insert({tag, entity});
    }
    bool Registry_::has_tag(Entity_ entity, std::string const &tag) const {
        if (!m_tag_per_entity.contains(entity.get_id())) {
            return false;
        }
        return m_tag_per_entity.find(entity.get_id())->second == tag;
    }
    Entity_ Registry_::get_entity(std::string const &tag) const { return m_Entityper_tag.at(tag); }
    void Registry_::remove_tag(Entity_ entity) {
        if (auto itr = m_tag_per_entity.find(entity.get_id()); itr != m_tag_per_entity.end()) {
            m_Entityper_tag.erase(itr->second);
            m_tag_per_entity.erase(itr);
        }
    }
    void Registry_::add_to_group(Entity_ entity, std::string const &group) {
        m_Entitygroups.insert({group, std::set<Entity_>{}});
        m_Entitygroups[group].insert(entity);
        m_groupe_per_entity.insert({entity.get_id(), group});
    }
    bool Registry_::has_group(Entity_ entity, std::string const &group) const {
        if (auto const itr = m_groupe_per_entity.find(entity.get_id()); itr != m_groupe_per_entity.end()) {
            return itr->second == group;
        }
        return false;
    }
    std::vector<Entity_> Registry_::get_entities_from_group(std::string const &group) const {
        auto const group_set = m_Entitygroups.at(group);
        return std::vector<Entity_>{group_set.cbegin(), group_set.cend()};
    }
    void Registry_::remove_from_group(Entity_ entity) {
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
