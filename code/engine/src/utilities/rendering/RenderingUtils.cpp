#include "engine/utilities/rendering/RenderingUtils.h"
#include "engine/systems/RenderingSystem.h"

#include <filesystem>

#include <engine/ResourceManager.h>

namespace EisEngine::rendering {
	const std::string RenderingConfig::configPath = "config/config.toml";
	
	RenderingConfig::RenderingConfig() {
		Load();
	}

	RenderingConfig& RenderingConfig::GetInstance() {
		// Meyers singleton.
		
		static RenderingConfig instance;
		return instance;
	}

	void RenderingConfig::Load() {
		auto path = resolveAssetPath(RenderingConfig::configPath);
		// if the given config path is invalid, throw an error message and exit with default hard-coded values.
		if (path == fs::path("Invalid")) {
			DEBUG_ERROR("Couldn't find config file at " + configPath)
			return;
		}

		toml::table tbl;
		try {
			tbl = toml::parse_file(path.string());
			
			nToonLevels = tbl.at_path("rendering.nToonLevels").value_or(nToonLevels);
			ambientFactor = tbl.at_path("rendering.ambientFactor").value_or(ambientFactor);
			maxShininess = tbl.at_path("rendering.maxShininess").value_or(maxShininess);
		}
		catch (const toml::parse_error& err) {
			// throw an error if the parsing didn't load and exit with default values.
			DEBUG_ERROR("Failed to parse Config file " + path.string() + ": " + err.what())
			return;
		}
	}
}
