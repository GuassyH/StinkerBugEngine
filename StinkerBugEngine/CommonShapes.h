#ifndef COMMON_SHAPES_H
#define COMMON_SHAPES_H

#include <iostream>
#include <array>
#include <vector>

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "Vertex.h"

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

    struct Triangle : public Shape {
        static inline const std::vector<Vertex> vertices = {
            // POSITION     NORMAL     TEXCOORD
            { {-1, -1, 0}, {-1, -1, 0}, {0, 0} },
            { {1, -1, 0},  {1, -1, 0},  {1, 0} },
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

}

#endif