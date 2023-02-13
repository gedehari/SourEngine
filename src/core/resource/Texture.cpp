#include "Texture.h"

#include <stb_image.h>

#include "../LogManager.h"

Texture::Texture() : m_width(0), m_height(0), m_textureID(0) {}

Texture::~Texture()
{
    if (m_initialized)
        glDeleteTextures(1, &m_textureID);
}

Error Texture::LoadFromFile(const std::string &path)
{
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data == nullptr)
    {
        LOG_ERROR("Failed to load image from file: {}", path);
        return FAILED;
    }

    if (channels == 1)
        m_format = BW;
    else if (channels == 3)
        m_format = RGB;
    else if (channels == 4)
        m_format = RGBA;
    else
    {
        LOG_ERROR("Unsupported image format: {}", channels);
        stbi_image_free(data);
        return FAILED;
    }

    m_width = width;
    m_height = height;

    m_path = path;

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLint format;
    if (m_format == RGB)
        format = GL_RGB;
    else if (m_format == RGBA)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    m_initialized = true;

    LOG_DEBUG("Successfully loaded image: {}", path);

    return OK;
}