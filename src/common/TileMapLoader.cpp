//
// Created by HP on 27.10.2024.
//

#include "TileMapLoader.hpp"
#include <fstream>
#include "spdlog/logger.h"

namespace engine {
    TileMapLoader::TileMapLoader(int tile_size) : m_index_map{}, m_tile_size{tile_size} {}

    void TileMapLoader::load_index_map(std::filesystem::path const &path) {
        std::ifstream file{path.string()};
        if (!file.is_open()) {
            throw std::runtime_error(std::format("could not open tilemap index file: {}", path.string()));
        }

        auto split = [](std::string const &str, std::string const &delimiter) -> std::vector<std::string> {
            std::vector<std::string> tokens;

            std::size_t start = 0;
            std::size_t end = str.find(delimiter);

            while (end != std::string::npos) {
                tokens.push_back(str.substr(start, end - start));
                start = end + 1;
                end = str.find(delimiter, start);
            }
            tokens.push_back(str.substr(start, end));
            return tokens;
        };

        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::pair<int, int>> row;
            for (auto const &word: split(line, ",")) {
                int x_coord{};
                int y_coord{};
                auto [ptr_x, ec_x] = std::from_chars(word.data(), word.data() + 1, y_coord);
                auto [ptr_y, ec_y] = std::from_chars(word.data() + 1, word.data() + word.size(), x_coord);
                if (ec_x != std::errc() || ec_y != std::errc()) {
                    file.close();
                    throw std::runtime_error(std::format("could not convert map index {}", word));
                }
                row.emplace_back(x_coord, y_coord);
            }
            m_index_map.push_back(row);
        }
        file.close();
    }
    size_t TileMapLoader::tile_size() const { return m_tile_size; }
    size_t TileMapLoader::map_width() const { return m_index_map.at(0).size() * m_tile_size; }
    size_t TileMapLoader::map_height() const { return m_index_map.size() * m_tile_size; }
} // namespace engine
