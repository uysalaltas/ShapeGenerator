#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

#include "Shapes.h"

namespace Shapes
{
	class Utils
	{
    protected:
        Utils(){}
        static Utils* utils_;

    public:
        Utils(Utils& other) = delete;
        void operator=(const Utils&) = delete;
        static Utils* GetInstance();
    
	public:
		glm::vec3 ComputeFaceNormals(Vertex& v1, Vertex& v2, Vertex& v3)
		{
			const float EPSILON = 0.000001f;

			glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
			float nx, ny, nz;

			// find 2 edge vectors: v1-v2, v1-v3
			float ex1 = v2.position.x - v1.position.x;
			float ey1 = v2.position.y - v1.position.y;
			float ez1 = v2.position.z - v1.position.z;
			float ex2 = v3.position.x - v1.position.x;
			float ey2 = v3.position.y - v1.position.y;
			float ez2 = v3.position.z - v1.position.z;

			// cross product: e1 x e2
			nx = ey1 * ez2 - ez1 * ey2;
			ny = ez1 * ex2 - ex1 * ez2;
			nz = ex1 * ey2 - ey1 * ex2;

			// normalize only if the length is > 0
			float length = sqrtf(nx * nx + ny * ny + nz * nz);
			if (length > EPSILON)
			{
				// normalize
				float lengthInv = 1.0f / length;
				normal.x = nx * lengthInv;
				normal.y = ny * lengthInv;
				normal.z = nz * lengthInv;
			}

			return normal;
		}
    };

    Utils* Utils::utils_ = nullptr;;

    Utils* Utils::GetInstance()
    {
        if (utils_ == nullptr) {
            utils_ = new Utils();
        }
        return utils_;
    }
}