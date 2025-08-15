#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <array>
#include <vector>

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "Vertex.h"

namespace Constants {
    namespace Shapes {
    struct Shape {
        virtual const std::vector<Vertex>& getVertices() const = 0;
        virtual const std::vector<GLuint>& getIndices() const = 0;
        virtual ~Shape() = default;
    };


    struct Plane : public Shape {
        static inline const std::vector<Vertex> vertices = {
            // POSITION     NORMAL     TEXCOORD
            { {0, 0, 0}, {0, 1, 0}, {0, 0} },
            { {1, 0, 0}, {0, 1, 0}, {1, 0} },
            { {0, 0, 1}, {0, 1, 0}, {0, 1} },
            { {1, 0, 1}, {0, 1, 0}, {1, 1} },
        };
        static inline const std::vector<GLuint> indices = {
            0, 2, 1,
            1, 2, 3
        };

        const std::vector<Vertex>& getVertices() const override { return vertices; }
        const std::vector<GLuint>& getIndices() const override { return indices; }
    };


    struct UVSphere : public Shape {
        static inline const std::vector<Vertex> vertices = [] {
            std::vector<Vertex> verts;
            const unsigned int sectorCount = 64; // longitude
            const unsigned int stackCount = 32; // latitude
            const float radius = 0.5f;

            const float PI = 3.14159265359f;
            const float sectorStep = 2 * PI / sectorCount;
            const float stackStep = PI / stackCount;

            for (unsigned int i = 0; i <= stackCount; ++i) {
                float stackAngle = PI / 2 - i * stackStep; // from pi/2 to -pi/2
                float xy = radius * cosf(stackAngle);
                float z = radius * sinf(stackAngle);

                for (unsigned int j = 0; j <= sectorCount; ++j) {
                    float sectorAngle = j * sectorStep; // 0 to 2pi

                    float x = xy * cosf(sectorAngle);
                    float y = xy * sinf(sectorAngle);

                    glm::vec3 pos(x, y, z);
                    glm::vec3 normal = glm::normalize(pos);
                    pos.x += radius;
                    pos.z += radius;
                    glm::vec2 texCoord(
                        (float)j / sectorCount,
                        (float)i / stackCount
                    );

                    verts.push_back({ pos, normal, texCoord });
                }
            }
            return verts;
            }();
        static inline const std::vector<GLuint> indices = [] {
            std::vector<GLuint> inds;
            const unsigned int sectorCount = 64;
            const unsigned int stackCount = 32;

            for (unsigned int i = 0; i < stackCount; ++i) {
                unsigned int k1 = i * (sectorCount + 1); // start of current stack
                unsigned int k2 = k1 + sectorCount + 1;  // start of next stack

                for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                    // first triangle
                    inds.push_back(k1);
                    inds.push_back(k2);
                    inds.push_back(k1 + 1);

                    // second triangle
                    inds.push_back(k1 + 1);
                    inds.push_back(k2);
                    inds.push_back(k2 + 1);
                }
            }
            return inds;
            }();

        const std::vector<Vertex>& getVertices() const override { return vertices; }
        const std::vector<GLuint>& getIndices() const override { return indices; }
    };


    struct Triangle : public Shape {
        static inline const std::vector<Vertex> vertices = {
            // POSITION     NORMAL     TEXCOORD
            { {0, 0, 0}, {-1, -1, 0}, {0, 0} },
            { {1, 0, 0},  {1, -1, 0},  {1, 0} },
            { {0, 1, 0},   {0, 1, 0},   {0.5f, 1} },
        };
        static inline const std::vector<GLuint> indices = {
            0, 1, 2
        };

        const std::vector<Vertex>& getVertices() const override { return vertices; }
        const std::vector<GLuint>& getIndices() const override { return indices; }
    };


    struct Cube : public Shape {
        static inline const std::vector<Vertex> vertices = {
            // POSITION     NORMAL     TEXCOORD
            { {0, 0, 0}, {0, 0, -1}, {1.0f, 0.0f} }, // Left Bottom Back
            { {0, 1, 0}, {0, 0, -1}, {1.0f, 1.0f} }, // Left Top Back
            { {1, 0, 0}, {0, 0, -1}, {0.0f, 0.0f} }, // Right Bottom Back
            { {1, 1, 0}, {0, 0, -1}, {0.0f, 1.0f} }, // Right Top Back

            { {0, 0, 1}, {0, 0, 1}, {0.0f, 0.0f} },  // Left Bottom Front
            { {1, 0, 1}, {0, 0, 1}, {1.0f, 0.0f} },  // Right Bottom Front
            { {0, 1, 1}, {0, 0, 1}, {0.0f, 1.0f} },  // Left Top Front
            { {1, 1, 1}, {0, 0, 1}, {1.0f, 1.0f} },  // Right Top Front

            { {0, 1, 0}, {0, 1, 0}, {0.0f, 1.0f} },  // Left Top Back
            { {0, 1, 1}, {0, 1, 0}, {0.0f, 0.0f} },  // Left Top Front
            { {1, 1, 0}, {0, 1, 0}, {1.0f, 1.0f} },  // Right Top Back
            { {1, 1, 1}, {0, 1, 0}, {1.0f, 0.0f} },  // Right Top Front

            { {0, 0, 0}, {0, -1, 0}, {0.0f, 0.0f} }, // Left Bottom Back
            { {1, 0, 0}, {0, -1, 0}, {1.0f, 0.0f} }, // Right Bottom Back
            { {0, 0, 1}, {0, -1, 0}, {0.0f, 1.0f} }, // Left Bottom Front
            { {1, 0, 1}, {0, -1, 0}, {1.0f, 1.0f} }, // Right Bottom Front

            { {0, 0, 0}, {-1, 0, 0}, {0.0f, 0.0f} }, // Left Bottom Back
            { {0, 0, 1}, {-1, 0, 0}, {1.0f, 0.0f} }, // Left Bottom Front
            { {0, 1, 0}, {-1, 0, 0}, {0.0f, 1.0f} }, // Left Top Back
            { {0, 1, 1}, {-1, 0, 0}, {1.0f, 1.0f} }, // Left Top Front

            { {1, 0, 0}, {1, 0, 0}, {1.0f, 0.0f} },  // Right Bottom Back
            { {1, 1, 0}, {1, 0, 0}, {1.0f, 1.0f} },  // Right Top Back
            { {1, 0, 1}, {1, 0, 0}, {0.0f, 0.0f} },  // Right Bottom Front
            { {1, 1, 1}, {1, 0, 0}, {0.0f, 1.0f} },  // Right Top Front
        };
        static inline const std::vector<GLuint> indices = {
            // Back face
            0, 1, 2,
            2, 1, 3,

            // Front face
            4, 5, 6,
            6, 5, 7,

            // Top face
            8,  9, 10,
            10, 9, 11,

            // Bottom face
            12, 13, 14,
            14, 13, 15,

            // Left face
            16, 17, 18,
            18, 17, 19,

            // Right face
            20, 21, 22,
            22, 21, 23
        };

        const std::vector<Vertex>& getVertices() const override { return vertices; }
        const std::vector<GLuint>& getIndices() const override { return indices; }
    };

    struct Quad : public Shape {
        static inline const std::vector<Vertex> vertices = {
            // POSITION     NORMAL     TEXCOORD
            { {-1, -1, 0},    {0, 1, 0}, {0, 0} },
            { {1, -1, 0},    {0, 1, 0}, {1, 0} },
            { {-1, 1, 0},    {0, 1, 0}, {0, 1} },
            { {1, 1, 0},    {0, 1, 0}, {1, 1} },
        };
        static inline const std::vector<GLuint> indices = {
            0, 1, 2,
            2, 1, 3,
        };

        const std::vector<Vertex>& getVertices() const override { return vertices; }
        const std::vector<GLuint>& getIndices() const override { return indices; }
    };
}
    namespace Colors {
        static inline glm::vec4 Red = glm::vec4(1.0, 0.0, 0.0, 1.0);
        static inline glm::vec4 Green = glm::vec4(0.0, 1.0, 0.0, 1.0);
        static inline glm::vec4 Blue = glm::vec4(0.0, 0.0, 1.0, 1.0);

        static inline glm::vec4 Purple = glm::vec4(0.6, 0.0, 0.7, 1.0);

        static inline glm::vec4 White = glm::vec4(1.0);
        static inline glm::vec4 Black = glm::vec4(0.0, 0.0, 0.0, 1.0);
    }
    namespace Dirs {
        static inline glm::vec3 Up = glm::vec3(0.0, 1.0, 0.0);
        static inline glm::vec3 Right = glm::vec3(1.0, 0.0, 0.0);
        static inline glm::vec3 Forward = glm::vec3(1.0, 0.0, -1.0);
    }
    
}

#endif