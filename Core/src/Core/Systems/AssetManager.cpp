#include "AssetManager.h"
#include <stb_image.h>
#include <glad/glad.h>

static Core::AssetManager* s_instance = nullptr;
Core::AssetManager& Core::AssetManager::Get() { return *s_instance; }

void Core::AssetManager::Init() {
	if (!s_instance) s_instance = new AssetManager;
}

void Core::AssetManager::Shutdown() {
	if (s_instance) delete s_instance;
}

//Helper Function
bool LoadTextureFromPath(Core::sPtr<Core::Texture> texture, const std::string& path);

void Core::AssetManager::LoadTexturesInPath_Impl(const std::string& path, bool async) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (!entry.is_regular_file()) continue; // Skip folders, symlinks, etc.

		const std::string filePath = entry.path().string();
		LoadTexture_Impl(filePath, async);
	}
}

void Core::AssetManager::LoadTexture_Impl(const std::string& path, bool async) {
	std::string key = static_cast<std::filesystem::path>(path).filename().string();
	sPtr<Texture> tex = SetSPtr<Texture>(path);

	if (m_loadedTextures.contains(key)) return;
		
	if (async) {
		// Queue async load and insert once uploaded
		LoadAsync_Impl(path, [this, key](sPtr<Texture> tex) {
			std::lock_guard<std::mutex> lock(m_callbackMutex);
			m_loadedTextures.insert({key, tex});
		});
	}
	else {
		if (LoadTextureFromPath(tex, path)) {
			m_loadedTextures.insert({key, tex});
		}
	}
}

void Core::AssetManager::UnloadTexture_Impl(const std::string& name) {
	auto it = m_loadedTextures.find(name);
	if (it != m_loadedTextures.end()) {
		m_loadedTextures.erase(it);
	}
}

Core::sPtr<Core::Texture> Core::AssetManager::GetTexture_Impl(const std::string& name) {
	auto it = m_loadedTextures.find(name);
	if (it != m_loadedTextures.end() && it->second->uploaded) {
		return it->second;
	}
	return nullptr;
}

void Core::AssetManager::LoadAsync_Impl(const std::string& path, Callback onComplete) {
	auto tex = SetSPtr<Texture>(path);
	std::thread([tex, onComplete, path, this]() {
		int w, h, ch;
		unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, STBI_rgb_alpha);
		if (!data) return;

		std::lock_guard<std::mutex> lock(m_callbackMutex);
		m_pendingTextureCallbacks.push_back([=]() {
			GLuint texID;
			glGenTextures(1, &texID);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);

			tex->id = texID;
			tex->width = w;
			tex->height = h;
			tex->uploaded = true;
			onComplete(tex);
		});
	}).detach();
}

void Core::AssetManager::Dispatch_Impl() {
	std::lock_guard<std::mutex> lock(m_callbackMutex);
	for (auto& cb : m_pendingTextureCallbacks) cb();
	m_pendingTextureCallbacks.clear();
}

bool LoadTextureFromPath(Core::sPtr<Core::Texture> texture, const std::string& path) {
	int w, h, ch;
	unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, STBI_rgb_alpha);
	if (!data) return false;

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	texture->id = texID;
	texture->width = w;
	texture->height = h;
	texture->uploaded = true;
	return true;
}
