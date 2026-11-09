#pragma once

#include "engine/systems/Camera.h"

namespace EisEngine {
	// fwd declarations 
	namespace systems { 
		class RenderingSystem; 
	}

	using EisEngine::systems::Camera;

	namespace rendering {
		class RenderingPass {
			friend EisEngine::systems::RenderingSystem;
		public:
			RenderingPass();
			virtual void Execute(Camera* camera) = 0;
		protected:
			unsigned int VAO;
		};
	}
}
