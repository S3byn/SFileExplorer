#include "Texture.h"
#include <glad/glad.h>

Core::Texture::~Texture() {
	glDeleteTextures(1, &id);
}