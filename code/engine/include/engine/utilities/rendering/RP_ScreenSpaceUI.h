# pragma once

#include "engine/utilities/rendering/RenderingPass.h"
#include "engine/components/Renderer.h"

namespace EisEngine {
	namespace rendering {
		using EisEngine::components::Renderer;

		class RP_ScreenSpaceUI : public RenderingPass {
		public:
			void Execute(Camera* camera) override;
		private:
			bool CompareZValues(Renderer* a, Renderer* b);
		};
	}
}
