#pragma once

#include "engine/systems/Camera.h"
#include "engine/components/Transform.h"

namespace EisEngine {
	// fwd declarations 
	namespace systems { 
		class RenderingSystem; 
	}

	using EisEngine::systems::Camera;
	using EisEngine::components::Transform;

	namespace rendering {
		class RenderingPass {
			friend EisEngine::systems::RenderingSystem;
		public:
			/// Constructor for abstract rendering pass objects to generate a VAO.
			RenderingPass();
			/// Draws all renderers associated with this rendering pass object.
			virtual void Execute(Camera* camera) = 0;
		protected:
			/// Selects the camera-related properties needed for the render pass and activates them in the shader.
			virtual void AssignCameraValues(Shader* activeShader, Camera* camera) const = 0;
			/// Selects the material properties needed for the render pass and activates them in the shader.
			virtual void AssignRendererValues(Shader* activeShader, Renderer* renderer) const = 0;
			/// Selects the transform properties needed for the render pass and activates them in the shader.
			virtual void AssignTransformValues(Shader* activeShader, Transform* transform) const = 0;
			/// Vertex Array Object assigned to this RenderPass object.
			unsigned int VAO;
		};
	}
}
