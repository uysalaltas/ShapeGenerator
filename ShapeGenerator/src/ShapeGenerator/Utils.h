#pragma once

#include "Shapes.h"

namespace Shapes
{
	class Utils
	{
	public:
		static glm::vec3 ComputeFaceNormals(Vertex& v1, Vertex& v2, Vertex& v3);

	private:
		static Utils* s_Instance;
    };
}