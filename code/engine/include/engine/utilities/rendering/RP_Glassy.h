# pragma once

#include "engine/utilities/rendering/RenderingPass.h"

#include <vector>

namespace EisEngine {
	class Game;

	namespace components {
		class Renderer;
		class Mesh;
	}

	using EisEngine::components::Renderer;
	using EisEngine::components::Mesh;

	namespace rendering {
		class RP_Glassy : public RenderingPass {
		public:
			RP_Glassy(Game& engine);
			/// Draws all renderers associated with this rendering pass object.
			void Execute(Camera* camera) override;
		protected:
			/// Selects the camera-related properties needed for the render pass and activates them in the shader.
			void AssignCameraValues(Shader* activeShader, Camera* camera) const override;
			/// Selects the material properties needed for the render pass and activates them in the shader.
			void AssignRendererValues(Shader* activeShader, Renderer* renderer) const override;
			/// Selects the transform properties needed for the render pass and activates them in the shader.
			void AssignTransformValues(Shader* activeShader, Transform* transform) const override;
		private:
			/// Initializes the framebuffer object for depth mapping.
			void InitFBO(const int& index, const Vector2& screenDims);
			/// Resizes buffers on window size shift.
			void ResizeFBOItems(const Vector2& newScreenDims);
			void DepthMapping(std::vector<Renderer*>& assignedRenderers, Camera* camera);
			void ApplyGlassyRenderingSettings(Shader* activeShader, Entity* skybox);
			void Prepare3DDraw(Mesh& mesh, Shader* activeShader);

			/// FBO array storing a depth-mapping RBO.
			std::array<GLuint, 2> RBO;
			/// FBO array storing a depth-mapping FBO.
			std::array<GLuint, 2> FBO;
			/// Texture index storing depth data.
			std::array<GLuint, 2> depthTex;
			Game& engine;
		};
	}
}
