//
// Created by HP on 18.10.2024.
//

#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

namespace system {
    class Movement : public System {
    public:
        Movement() = default;

        void update();
    };
} // namespace system
#endif // MOVEMENT_HPP
