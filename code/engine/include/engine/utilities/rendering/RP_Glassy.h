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
			void Execute(Camera* camera) override;
		private:
			/// Initializes the framebuffer object for depth mapping.
			void InitFBO(const int& index, const Vector2& screenDims);
			/// Resizes buffers on window size shift.
			void ResizeFBOItems(const Vector2& newScreenDims);
			void DepthMapping(std::vector<Renderer*>& assignedRenderers, Shader* activeShader, Camera* camera);
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
