//
// Created by HP on 09.12.2024.
//

#include "Script.hpp"
#include "components/Script.hpp"
systems::Script::Script() { require_component<components::Script>(); }
void systems::Script::update() {
    for (auto entity: get_entities()) {
        auto const script = entity.get_component<components::Script>();
        script.func();
    }
}
