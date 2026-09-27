#pragma once

// this file exists because of some issues including opengl libs with the input system.
// locks problematic includes behind a #ifndef clause, to avoid redefining things.

#ifndef OPENGL_INCLUDED
#define OPENGL_INCLUDED

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#endif
