//
// Created by HP on 01.11.2024.
//

#ifndef EVENTBUS_HPP
#define EVENTBUS_HPP
#include <typeindex>

#include "Event.hpp"
#include "common/Logger.hpp"
#include "spdlog/spdlog.h"

namespace events {
    class IEventCallback {
    private:
        virtual void call(Event &e) = 0;

    public:
        virtual ~IEventCallback() = default;
        void execute(Event &e) { call(e); }
    };

    template<typename TEvent>
    class EventCallback : public IEventCallback {
        using Callback = std::function<void(TEvent &)>;

    private:
        Callback m_callback;

        void call(Event &e) override { m_callback(static_cast<TEvent &>(e)); }

    public:
        explicit EventCallback(Callback callback) : m_callback{callback} {}
    };

    using HandlerList = std::list<std::unique_ptr<IEventCallback>>;


    class EventBus {
        using Logger = std::shared_ptr<engine::Logger>;
        using Subscribers = std::unordered_map<std::type_index, std::unique_ptr<HandlerList>>;

    private:
        Logger m_logger;
        Subscribers m_subscribers;

    public:
        explicit EventBus(Logger logger);

        template<typename TEvent>
        void subscribe(std::function<void(TEvent &)> callback) {
            static_assert(std::is_base_of_v<Event, TEvent>, "TEvent is not derived from Event");

            if (!m_subscribers[typeid(TEvent)].get()) {
                m_subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            auto subscriber = std::make_unique<EventCallback<TEvent>>(callback);
            m_subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
        }

        template<typename TEvent, typename... TArgs>
        void emit(TArgs &&...args) {
            if (auto handlers = m_subscribers[typeid(TEvent)].get(); handlers) {
                TEvent event{std::forward<TArgs>(args)...};
                for (auto itr = handlers->begin(); itr != handlers->end(); ++itr) {
                    itr->get()->execute(event);
                }
            }
        }

        // clears the subscribers lists
        void clear_subscribers() { m_subscribers.clear(); }
    };

} // namespace events

#endif // EVENTBUS_HPP
