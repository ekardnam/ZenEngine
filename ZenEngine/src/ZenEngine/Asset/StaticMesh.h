#pragma once

#include <glm/glm.hpp>
#include "AssetInstance.h"

#include "Serialization.h"
#include "ZenEngine/Renderer/VertexArray.h"

namespace ZenEngine
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec2 TexCoord;
        glm::vec3 Normal;

        template<typename Archive>
        void Serialize(Archive &inArchive)
        {
            inArchive(Position, TexCoord, Normal); 
        }
    };

    class StaticMesh : public AssetInstance
    {
    public:
        IMPLEMENT_ASSET_CLASS(ZenEngine::StaticMesh)


        void SetVertices(const std::vector<Vertex> &inVertices) { mVertices = inVertices; mTainted = true; }
        void SetIndices(const std::vector<uint32_t> &inIndices) { mIndices = inIndices; mTainted = true; }
        const std::vector<Vertex> &GetVertices() { return mVertices; }
        const std::vector<uint32_t> &GetIndices() { return mIndices; }
        
        void PushVertex(Vertex inVertex) { mVertices.push_back(inVertex); mTainted = true; }
        void PushTriangle(uint32_t inIndices[3]) { for (int i = 0; i < 3; ++i) PushIndex(inIndices[i]); mTainted = true; }
        void PushIndex(uint32_t inIndex) {  mIndices.push_back(inIndex); mTainted = true; }

        std::shared_ptr<VertexArray> CreateOrGetVertexArray();
    private:
        std::vector<Vertex> mVertices;
        std::vector<uint32_t> mIndices;
        bool mTainted = false;

        std::shared_ptr<VertexArray> mVertexArray;

        template<typename Archive>
        void Serialize(Archive &inArchive)
        {
            inArchive(mVertices, mIndices); 
        }
        
        friend class cereal::access;
    };

    class OBJImporter : public AssetImporterFor<StaticMesh>
    {
    public:
        virtual const char *GetName() const { return "OBJImporter"; }
        virtual std::vector<std::string> ProvidesForExtensions() const 
        {
            return { ".obj" };
        }

        virtual std::vector<std::shared_ptr<AssetInstance>> Import(const std::filesystem::path &inFilepath) override;
    };
}

CEREAL_REGISTER_TYPE(ZenEngine::StaticMesh);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ZenEngine::AssetInstance, ZenEngine::StaticMesh)