#pragma once

#include <optional>
#include "Editor.h"
#include "ZenEngine/Asset/AssetManager.h"
#include "ZenEngine/Renderer/Texture2D.h"

namespace ZenEngine
{

    class AssetBrowser : public EditorWindow
    {
    public:
        AssetBrowser() : EditorWindow("Asset Browser", true)
        {
            ZE_ASSERT_CORE_MSG(sAssetBrowserInstance == nullptr, "An AssetBrowser already exists");
            sAssetBrowserInstance = this;
            mCurrentDirectory = mBaseDirectory;
        }

        virtual void OnRegister() override;
        virtual void OnRenderWindow() override;

        void SetAssetIcon(const char *inAssetClassName, const std::filesystem::path &inIconPath);

        const std::filesystem::path &GetCurrentDirectory() const { return mCurrentDirectory; } 

        static AssetBrowser &Get() { ZE_ASSERT_CORE_MSG(sAssetBrowserInstance != nullptr, "An AssetBrowser does not exist!"); return *sAssetBrowserInstance; }
    private:
        std::shared_ptr<Texture2D> mFolderIcon;
        std::shared_ptr<Texture2D> mDefaultFileIcon;

        std::unordered_map<const char *, std::shared_ptr<Texture2D>> mAssetIcons;

        // TODO change this
        std::filesystem::path mBaseDirectory = "Assets";
        std::filesystem::path mCurrentDirectory;

        std::optional<std::filesystem::path> mCurrentlyRenaming;
        std::string mNewFilename;

        static AssetBrowser *sAssetBrowserInstance;

        uint8_t *LoadIcon(const std::filesystem::path &inIconPath);
        void FreeIcon(uint8_t *inImage);

        void CreateFolder();
        void DoneRenaming();
        void StartRenaming(const std::filesystem::path &inFilepath);
        void Move(const std::filesystem::path &inOld, const std::filesystem::path &inDirectory);
    };
}