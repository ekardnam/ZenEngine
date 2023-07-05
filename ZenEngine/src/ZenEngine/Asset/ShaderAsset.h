#pragma once

#include "Asset.h"
#include "ZenEngine/Renderer/Shader.h"
#include "ZenEngine/ShaderCompiler/ShaderReflector.h"
#include "ZenEngine/Renderer/RendererAPI.h"

namespace ZenEngine
{
    class ShaderLoader;
    
    class ShaderAsset : public Asset
    {
    public:
        IMPLEMENT_ASSET_CLASS(ZenEngine::ShaderAsset)
        using Loader = ShaderLoader;
        ShaderAsset() = default;

        std::shared_ptr<Shader> CreateOrGetShaderProgram();
        Shader::ShaderUniformInfo GetShaderUniforms();
    
        void SetName(const std::string &inName) { mName = inName; }
        void SetSourceCode(const std::string &inSource) { mSourceCode = inSource; mTainted = true; }

        const std::string &GetName() const { return mName; }
        const std::string &GetSourceCode() const { return mSourceCode; }
    private:
        std::string mName;
        std::string mSourceCode;

        bool mTainted = false;
        std::shared_ptr<Shader> mShaderProgram;        
    };

    class ShaderLoader : public AssetLoader
    {
    public:
        IMPLEMENT_LOADER_CLASS(ZenEngine::ShaderLoader)

        virtual bool Save(const std::shared_ptr<Asset> &inAssetInstance, const AssetInfo &inAsset) const override;
        virtual std::shared_ptr<Asset> Load(const AssetInfo &inAsset) const override;
        virtual bool CanLoad(const std::filesystem::path &inFilepath) const override;
        virtual std::pair<UUID, const char*> GetAssetIdAssetClass(const std::filesystem::path &inFilepath) const override;

    private:
        UUID GetAssetId(const std::filesystem::path &inFilepath) const;
    };
}