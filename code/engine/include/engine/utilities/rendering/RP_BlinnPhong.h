#pragma once

#include "engine/utilities/rendering/RenderingPass.h"

namespace EisEngine{
	namespace rendering {
		class RP_BlinnPhong : public RenderingPass {
		public:
			void Execute() override;
		};
	}
}