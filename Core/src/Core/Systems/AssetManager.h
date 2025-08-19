#pragma once
#include "Core/Resources/Texture.h"
#include "Core/Core.h"
#include <functional>
#include <mutex>
#include <filesystem>
#include <unordered_map>

namespace Core {

	class AssetManager {
	public:
		using Callback = std::function<void(sPtr<Texture>)>;
		
		static void Init();
		static void Shutdown();

		inline static void LoadTexturesInPath(const std::string& path, bool async = false) { Get().LoadTexturesInPath_Impl(path, async); }
		inline static void LoadTexture(const std::string& path, bool async = false) { Get().LoadTexture_Impl(path, async); }
		inline static void UnloadTexture(const std::string& name) { Get().UnloadTexture_Impl(name); }
		inline static sPtr<Texture> GetTexture(const std::string& name) { return Get().GetTexture_Impl(name); }

		inline static void LoadAsync(const std::string& path, Callback onComplete) { Get().LoadAsync_Impl(path, onComplete); }
		inline static void Dispatch() { Get().Dispatch_Impl(); }

	private:
		AssetManager() {}
		~AssetManager() {}

		static AssetManager& Get();

		void LoadTexturesInPath_Impl(const std::string& path, bool async);
		void LoadTexture_Impl(const std::string& path, bool async);
		void UnloadTexture_Impl(const std::string& name);
		sPtr<Texture> GetTexture_Impl(const std::string& name);

		void LoadAsync_Impl(const std::string& path, Callback onComplete);
		void Dispatch_Impl();

	private:
		std::mutex m_callbackMutex;
		std::vector<std::function<void()>> m_pendingTextureCallbacks;
		std::unordered_map<std::string, sPtr<Texture>> m_loadedTextures;
	};

}


