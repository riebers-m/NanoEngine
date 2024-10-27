//
// Created by HP on 27.10.2024.
//

#ifndef TILEMAPLOADER_HPP
#define TILEMAPLOADER_HPP
#include <filesystem>

namespace spdlog {
    class logger;
}

namespace engine {
    using IndexContainer = std::vector<std::vector<int>>;
    class TileMapLoader {
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        Logger m_logger;
        IndexContainer m_index_map;
        int m_tile_size;

    public:
        explicit TileMapLoader(Logger logger, int tile_size = 32);

        void load_index_map(std::filesystem::path const &path);
        [[nodiscard]] int size() const;

        IndexContainer::iterator begin() { return m_index_map.begin(); }
        IndexContainer::iterator end() { return m_index_map.end(); }
    };

} // namespace engine

#endif // TILEMAPLOADER_HPP
