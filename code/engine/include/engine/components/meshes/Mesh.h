#pragma once

#include "engine/ecs/Component.h"

namespace EisEngine {
	using namespace ecs;
	namespace components {
		/// This abstract component encompasses all geometry for EisEngine.
		class Mesh : public Component {
		public:
			explicit Mesh(Game& engine, guid_t owner);

			/// Draws the mesh onto the screen once per frame.
			virtual void draw(const unsigned int& shaderProgram) const = 0;
		};
	}
}
