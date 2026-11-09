#include "engine/utilities/rendering/RenderingPass.h"
#include "engine/systems/RenderingSystem.h"

namespace EisEngine::rendering {
	RenderingPass::RenderingPass() {
		glGenVertexArrays(1, &VAO);
	}
}
