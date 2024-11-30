//
// Created by HP on 21.10.2024.
//

#ifndef ASSETSTORE_HPP
#define ASSETSTORE_HPP
#include <SDL_ttf.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include "common/Logger.hpp"

struct SDL_Renderer;
struct SDL_Texture;

namespace engine {
    class AssetStore {
        struct TextureDeleter {
            void operator()(SDL_Texture *) const noexcept;
        };

        struct FontDeleter {
            void operator()(TTF_Font *) const noexcept;
        };

    private:
        using Store = std::unordered_map<std::string, std::unique_ptr<SDL_Texture, TextureDeleter>>;
        using FontStore = std::unordered_map<std::string, std::unique_ptr<TTF_Font, FontDeleter>>;
        using Logger = std::shared_ptr<engine::Logger>;

        Logger m_logger;
        Store m_texture_store;
        FontStore m_font_store;

    public:
        explicit AssetStore(Logger logger);
        ~AssetStore(); // has to be defined explicitly
        void clear_assets();
        void add_texture(SDL_Renderer *renderer, std::string const &asset_id, std::filesystem::path const &file_path);
        [[nodiscard]] SDL_Texture *get_texture(std::string const &asset_id) const;

        void add_font(std::string const &asset_id, std::filesystem::path const &file_path, std::size_t font_size);
        [[nodiscard]] TTF_Font *get_font(std::string const &asset_id) const;
    };
} // namespace engine

#endif // ASSETSTORE_HPP
