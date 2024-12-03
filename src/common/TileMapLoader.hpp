//
// Created by HP on 27.10.2024.
//

#ifndef TILEMAPLOADER_HPP
#define TILEMAPLOADER_HPP
#include <filesystem>

namespace engine {
    using IndexContainer = std::vector<std::vector<int>>;
    class TileMapLoader {

    private:
        IndexContainer m_index_map;
        int m_tile_size;

    public:
        explicit TileMapLoader(int tile_size = 32);

        void load_index_map(std::filesystem::path const &path);
        [[nodiscard]] size_t tile_size() const;
        // Number of tiles per row
        [[nodiscard]] size_t map_width() const;
        // Number of tiles per colum
        [[nodiscard]] size_t map_height() const;

        IndexContainer::iterator begin() { return m_index_map.begin(); }
        IndexContainer::iterator end() { return m_index_map.end(); }
    };

} // namespace engine

#endif // TILEMAPLOADER_HPP
