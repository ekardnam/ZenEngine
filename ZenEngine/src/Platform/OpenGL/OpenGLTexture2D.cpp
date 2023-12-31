#include "OpenGLTexture2D.h"

#include "ZenEngine/Core/Macros.h"

namespace ZenEngine
{
    static GLenum Texture2DFormatToGLInternalFormat(Texture2D::Format inFormat)
    {
        switch (inFormat)
        {
        case Texture2D::Format::R8: return GL_R8;
        case Texture2D::Format::RGB8: return GL_RGB8;
        case Texture2D::Format::RGBA32F: return GL_RGBA32F;
        case Texture2D::Format::RGBA8: return GL_RGBA8; 
        default: ZE_ASSERT_CORE_MSG(false, "Could not convert Texture2D::Format!"); return 0;
        }
    }

    static GLenum Texture2DFormatToGLFormat(Texture2D::Format inFormat)
    {
        switch (inFormat)
        {
        case Texture2D::Format::R8: return GL_RED;
        case Texture2D::Format::RGB8: return GL_RGB;
        case Texture2D::Format::RGBA32F: return GL_RGBA32F;
        case Texture2D::Format::RGBA8: return GL_RGBA;
        default: ZE_ASSERT_CORE_MSG(false, "Could not convert Texture2D::Format!"); return 0;
        }
    }

    static GLenum Texture2DFilterToGLFilter(Texture2D::Filter inFilter)
    {
        switch (inFilter)
        {
        case Texture2D::Filter::Linear: return GL_LINEAR;
        case Texture2D::Filter::Nearest: return GL_NEAREST;
        default: ZE_ASSERT_CORE_MSG(false, "Could not convert TEXTURE2D::Filter!"); return 0;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(const Texture2D::Properties &inProperties)
        : mProperties(inProperties)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &mRendererId);
        glTextureStorage2D(mRendererId, 1, Texture2DFormatToGLInternalFormat(mProperties.Format), mProperties.Width, mProperties.Height);

        glTextureParameteri(mRendererId, GL_TEXTURE_MIN_FILTER, Texture2DFilterToGLFilter(mProperties.MinFilter));
        glTextureParameteri(mRendererId, GL_TEXTURE_MAG_FILTER, Texture2DFilterToGLFilter(mProperties.MagFilter));

        // TODO make this a texture property
        glTextureParameteri(mRendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(mRendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &mRendererId);
    }

    void OpenGLTexture2D::SetData(void *inData, uint32_t inSize)
    {
        uint32_t bpp = Texture2DFormatBytes(mProperties.Format);
        ZE_ASSERT_CORE_MSG(inSize == mProperties.Width * mProperties.Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(mRendererId, 0, 0, 0, mProperties.Width, mProperties.Height, Texture2DFormatToGLFormat(mProperties.Format), GL_UNSIGNED_BYTE, inData);
        if (mProperties.GenerateMips) glGenerateTextureMipmap(mRendererId);
    }
    
    void OpenGLTexture2D::Bind(uint32_t inSlot) const
    {
        glActiveTexture(GL_TEXTURE0 + inSlot);
        glBindTexture(GL_TEXTURE_2D, mRendererId);
    }
}