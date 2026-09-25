#pragma once

#include "engine/components/BoxCollider2D.h"
#include "engine/components/PhysicsBody2D.h"
#include "engine/components/Transform.h"
#include "engine/components/Script.h"

#include "engine/components/rendering/meshes/Mesh2D.h"
#include "engine/components/rendering/meshes/Line.h"
#include "engine/components/rendering/meshes/SpriteMesh.h"

#include "engine/components/rendering/renderers/Mesh2DRenderer.h"
#include "engine/components/rendering/renderers/LineRenderer.h"
#include "engine/components/rendering/renderers/SpriteRenderer.h"

using BoxCollider2D = EisEngine::components::BoxCollider2D;
using PhysicsBody2D = EisEngine::components::PhysicsBody2D;
using Transform = EisEngine::components::Transform;
using Script = EisEngine::components::Script;

using Mesh2D = EisEngine::components::Mesh2D;
using SpriteMesh = EisEngine::components::SpriteMesh;
using Line = EisEngine::components::Line;

using Mesh2DRenderer = EisEngine::components::Mesh2DRenderer;
using SpriteRenderer = EisEngine::components::SpriteRenderer;
using LineRenderer = EisEngine::components::LineRenderer;
