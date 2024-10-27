//
// Created by HP on 27.10.2024.
//

#include "TileMapLoader.hpp"
#include <fstream>
#include "spdlog/logger.h"

namespace engine {
    TileMapLoader::TileMapLoader(Logger logger, int tile_size) :
        m_logger{logger}, m_index_map{}, m_tile_size{tile_size} {}

    void TileMapLoader::load_index_map(std::filesystem::path const &path) {
        std::ifstream file{path.string()};
        if (!file.is_open()) {
            throw std::runtime_error(std::format("could not open tilemap index file: {}", path.string()));
        }

        auto split = [](std::string const &str, std::string const &delimiter) -> std::vector<std::string> {
            std::vector<std::string> tokens;

            auto start = 0;
            auto end = str.find(delimiter);

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
            std::vector<int> row;
            for (auto const &word: split(line, ",")) {
                int number;
                auto [ptr, ec] = std::from_chars(word.data(), word.data() + word.size(), number);
                if (ec != std::errc()) {
                    file.close();
                    throw std::runtime_error(std::format("could not convert map index {} to std::size_t", word));
                }
                row.push_back(number);
            }
            m_index_map.push_back(row);
        }
        file.close();
    }
    int TileMapLoader::size() const { return m_tile_size; }
} // namespace engine
