#include "engine/components/meshes/Mesh.h"

namespace EisEngine::components {
	using EisEngine::ecs::Component;
	Mesh::Mesh(Game& engine, guid_t owner) : Component(engine, owner) { }
}
