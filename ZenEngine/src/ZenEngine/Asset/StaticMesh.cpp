#include "StaticMesh.h"

#include "ZenEngine/Renderer/VertexBuffer.h"
#include "ZenEngine/Renderer/IndexBuffer.h"

#include "OBJ_Loader.h"

namespace ZenEngine
{

    std::shared_ptr<VertexArray> StaticMesh::CreateOrGetVertexArray()
    {
        if (mVertexArray == nullptr || mTainted)
        {
            BufferLayout layout{
                { ShaderDataType::Float3, "Position" },
                { ShaderDataType::Float3, "Normal" },
                { ShaderDataType::Float2, "TexCoord" }
            };
            auto vb = VertexBuffer::Create((float*)mVertices.data(), mVertices.size() * sizeof(Vertex));
            vb->SetLayout(layout);

            auto ib = IndexBuffer::Create(mIndices.data(), mIndices.size());

            mVertexArray = VertexArray::Create();
            mVertexArray->AddVertexBuffer(vb);
            mVertexArray->SetIndexBuffer(ib);
            mTainted = false;
        }

        return mVertexArray;
    }

    std::vector<ImportedAsset> OBJImporter::Import(const std::filesystem::path &inFilepath)
    {
        objl::Loader loader;
        bool loadout = loader.LoadFile(inFilepath.string());
        std::vector<ImportedAsset> assets;
        if (!loadout)
        {
            ZE_CORE_ERROR("Could not open {} as OBJ file", inFilepath);
            return assets;
        }
        
        for (int i = 0; i < loader.LoadedMeshes.size(); ++i)
        {
            objl::Mesh curMesh = loader.LoadedMeshes[i];
            std::shared_ptr<StaticMesh> mesh = std::make_unique<StaticMesh>();

            for (auto &vertex : curMesh.Vertices)
            {
                mesh->PushVertex({
                    .Position = { vertex.Position.X, vertex.Position.Y, vertex.Position.Z },
                    .Normal = { vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z },
                    .TexCoord = { vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y }
                });
            }

            mesh->SetIndices(curMesh.Indices);

            auto importedFilename = inFilepath.filename().replace_extension(".zasset");
            ImportedAsset importedAsset;
            importedAsset.Filename = importedFilename;
            importedAsset.ClassName = StaticMesh::GetStaticAssetClassName();
            importedAsset.Instance = mesh;
            assets.push_back(importedAsset);
            /*file << "Material: " << curMesh.MeshMaterial.name << "\n";
            file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
            file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
            file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
            file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
            file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
            file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
            file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
            file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
            file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
            file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
            file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
            file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";*/
        }
        return assets;
    }

}