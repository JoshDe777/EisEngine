#pragma once

#include "engine/utilities/rendering/RenderingPass.h"

namespace EisEngine {
	namespace rendering {
		class RP_BlinnPhong : public RenderingPass {
		public:
			/// Draws all renderers associated with this rendering pass object.
			void Execute(Camera* camera) override;
		protected:
			/// Selects the camera-related properties needed for the render pass and activates them in the shader.
			void AssignCameraValues(Shader* activeShader, Camera* camera) const override;
			/// Selects the material properties needed for the render pass and activates them in the shader.
			void AssignRendererValues(Shader* activeShader, Renderer* renderer) const override;
			/// Selects the transform properties needed for the render pass and activates them in the shader.
			void AssignTransformValues(Shader* activeShader, Transform* transform) const override;
		};
	}
}