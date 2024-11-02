//
// Created by HP on 02.11.2024.
//

#ifndef KEYPRESSEDEVENT_HPP
#define KEYPRESSEDEVENT_HPP
#include <SDL_keycode.h>
#include "eventBus/Event.hpp"

namespace events {

    class KeyPressedEvent : public Event {
    public:
        SDL_Keycode keycode;

        explicit KeyPressedEvent(SDL_Keycode keycode) : keycode(keycode) {}
    };

} // namespace events

#endif // KEYPRESSEDEVENT_HPP
