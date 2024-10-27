//
// Created by HP on 21.10.2024.
//

#ifndef ASSETSTORE_HPP
#define ASSETSTORE_HPP
#include <filesystem>
#include <string>
#include <unordered_map>

namespace spdlog {
    class logger;
}

struct SDL_Renderer;
struct SDL_Texture;

namespace engine {
    class AssetStore {
        struct TextureDeleter {
            void operator()(SDL_Texture *) const noexcept;
        };

    private:
        using Store = std::unordered_map<std::string, std::unique_ptr<SDL_Texture, TextureDeleter>>;
        using Logger = std::shared_ptr<spdlog::logger>;

        Logger m_logger;
        Store m_texture_store;

    public:
        explicit AssetStore(Logger logger);
        ~AssetStore(); // has to be defined explicitly
        void clear_assets();
        void add_texture(SDL_Renderer *renderer, std::string const &asset_id, std::filesystem::path const &file_path);


        [[nodiscard]] SDL_Texture *get_texture(std::string const &asset_id) const;
    };
} // namespace engine

#endif // ASSETSTORE_HPP
