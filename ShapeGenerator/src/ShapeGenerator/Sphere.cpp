#include "Sphere.h"

namespace Shapes
{
	Sphere::Sphere(const float radius, const glm::vec3 color, const int sectorCount, const int stackCount)
		: m_radius(radius)
		, m_color(color)
		, m_sectorCount(sectorCount)
        , m_stackCount(stackCount)
	{
        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / m_radius;    // vertex normal

        const float PI = acos(-1);

        float sectorStep = 2 * PI / m_sectorCount;
        float stackStep = PI / m_stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= m_stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = m_radius * cosf(stackAngle);             // r * cos(u)
            z = m_radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= m_sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                Vertex tmp;
                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                
                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                tmp.position.x = x;
                tmp.position.y = y;
                tmp.position.z = z;

                tmp.color = m_color;
                tmp.normal = glm::vec3(nx, ny, nz);

                m_vertices.push_back(tmp);
                m_positions.push_back(glm::vec3(x, y, z));
                m_colors.push_back(m_color);
                m_normals.push_back(glm::vec3(nx, ny, nz));
            }
        }

        // generate CCW index list of sphere triangles
        // k1--k1+1
        // |  / |
        // | /  |
        // k2--k2+1

        int k1, k2;
        for (int i = 0; i < m_stackCount; ++i)
        {
            k1 = i * (m_sectorCount + 1);     // beginning of current stack
            k2 = k1 + m_sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < m_sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    m_indices.push_back(k1);
                    m_indices.push_back(k2);
                    m_indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    m_indices.push_back(k1 + 1);
                    m_indices.push_back(k2);
                    m_indices.push_back(k2 + 1);
                }
            }
        }

        std::cout << "V: " << m_vertices.size() << std::endl;
        std::cout << "I: " << m_indices.size() << std::endl;

	}
}