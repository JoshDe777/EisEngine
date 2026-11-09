#pragma once

#include "engine/utilities/rendering/RenderingPass.h"

namespace EisEngine {
	namespace rendering {
		class RP_CookTorrance : public RenderingPass {
		public:
			void Execute(Camera* camera) override;
		};
	}
	
}