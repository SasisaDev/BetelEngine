#pragma once

#include "Vertex.h"
#include <cstdint>
#include <vector>

struct Model {
    std::vector<Vertex> Vertices;
    std::vector<uint16_t> Indices;

    Model() {}
    Model(std::vector<Vertex> verts, std::vector<uint16_t> inds) : Vertices(verts), Indices(inds) {}

    size_t VerticesSize() const {return Vertices.size() * sizeof(Vertex);}
    Vertex* VerticesData() {return Vertices.data();}
    size_t IndicesSize() const {return Indices.size() * sizeof(uint16_t);}
    uint16_t* IndicesData() {return Indices.data();}
};

namespace BetelModel {
    static Model Quad = {{{{0.0f, 0.0f},  {0.0f, 0.0f}},
                          {{1.0f, 0.0f},   {1.0f, 0.0f}},
                          {{1.0f, 1.0f},    {1.0f, 1.0f}},
                          {{0.0f, 1.0f},   {0.0f, 1.0f}}},
                          {0, 1, 2, 2, 3, 0}};
};