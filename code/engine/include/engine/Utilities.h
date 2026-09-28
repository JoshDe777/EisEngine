#pragma once

#include <random>

// easy import file for all utility classes + some operators and utility functions.

#include "engine/utilities/Math.h"
#include "engine/utilities/Debug.h"
#include "engine/utilities/Color.h"
#include "engine/Events.h"

#include "engine/utilities/Vector2.h"
#include "engine/utilities/Vector3.h"

#include "engine/utilities/DebugLine.h"
#include "engine/utilities/Bounds2D.h"

#include "engine/utilities/rendering/PrimitiveMesh2D.h"
#include "engine/utilities/rendering/PrimitiveMesh3D.h"
#include "engine/utilities/rendering/PrimitiveSpriteMesh.h"
#include "engine/utilities/rendering/Texture2D.h"

#include "engine/utilities/ToStringExtensions.h"
#include "engine/utilities/ListUtils.h"
#include "engine/utilities/Random.h"

using Vector2 = EisEngine::Vector2;
using Vector3 = EisEngine::Vector3;
using Color = EisEngine::Color;
using DebugLine = EisEngine::DebugLine;
using Bounds2D = EisEngine::Bounds2D;
using PrimitiveMesh2D = EisEngine::rendering::PrimitiveMesh2D;
using PrimitiveMesh3D = EisEngine::rendering::PrimitiveMesh3D;
using PrimitiveSpriteMesh = EisEngine::rendering::PrimitiveSpriteMesh;
using Texture2D = EisEngine::Texture2D;
