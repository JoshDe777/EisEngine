#pragma once

namespace EisEngine {
	// fwd declarations 
	namespace systems { class RenderingSystem; }

	namespace rendering {
		class RenderingPass {
			friend EisEngine::systems::RenderingSystem;
		public:
			virtual void Execute() = 0;
		};
	}
}
