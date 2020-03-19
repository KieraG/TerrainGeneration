#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "Engine/OpenGL.hpp"
class Terrain {

  public:
    typedef struct triangle {
        glm::vec3 first;
        glm::vec3 second;
        glm::vec3 third;
    } triangle;

    Terrain();
    std::vector<std::vector<float>> terrainData;
    std::vector<triangle> terrainTri;

    void createTriangles();
    bool loadHeightfield(const std::string filename, const int size);
    void readTerrainData();
    void render(bool wireframe);
    bool genFaultFormation(int iterations, int hSize, int minHeight,
                           int maxHeight, float weight,int postSmoothingIterations, bool random);
    void flatTerrain(int size);
    void filterPass(float *dataP, int increment, float weight);
    void addFilter(float *terrainData, float weight);
    void normaliseTerrain(float *terrainData);

    GLuint TextureID;

  private:
    int imageSize = 0;
    float scaleX  = 1;
    float scaleY  = 1;
    float scaleZ  = 1;
};