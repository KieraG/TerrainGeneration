#include "Terrain.h"

#include <SDL_image.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <time.h>

#include <glm/gtx/normal.hpp>

#include "Engine/Engine.hpp"
#include "Engine/OpenGL.hpp"

Terrain::Terrain() {
    scaleX    = 1.0f;
    scaleY    = 1.0f;
    scaleZ    = 1.0f;
    TextureID = 0;
}

void Terrain::createTriangles() {

    SDL_Surface *Surface = IMG_Load("terrain.png");
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    int Mode = GL_RGB;
    if (Surface->format->BitsPerPixel == 4) {
        Mode = GL_RGBA;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode,
                 GL_UNSIGNED_BYTE, Surface->pixels);

    for (size_t z = 0; z < terrainData.size() - 1; z++) {
        for (size_t x = 0; x < terrainData.size() - 1; x++) {

            triangle left;
            triangle right;

            left.first =
                glm::vec3((x * scaleX), terrainData[x][z] * scaleY, (z)*scaleZ);
            left.second =
                glm::vec3((x + 1) * scaleX,
                          terrainData.at(x + 1).at(z) * scaleY, (z)*scaleZ);
            left.third = glm::vec3((x + 1) * scaleX,
                                   terrainData.at(x + 1).at(z + 1) * scaleY,
                                   (z + 1) * scaleZ);

            right.first = glm::vec3((x)*scaleX, terrainData.at(x).at(z) * scaleY,
                                    (z)*scaleZ);
            right.second = glm::vec3((x + 1) * scaleX,
                                     terrainData.at(x + 1).at(z + 1) * scaleY,
                                     (z + 1) * scaleZ);
            right.third = glm::vec3((x)*scaleX, terrainData.at(x).at(z + 1) * scaleY,
                                    (z + 1) * scaleZ);

            terrainTri.push_back(left);
            terrainTri.push_back(right);
        }
    }
}

void Terrain::render(bool wireframe) {

    int count = 0;

    for (auto tri : terrainTri) {
        glBindTexture(GL_TEXTURE_2D, TextureID);
        if (!wireframe) {
            glBegin(GL_TRIANGLES);
        } else {
            glBegin(GL_LINE_LOOP);
        }

        auto normal = -glm::triangleNormal(tri.first, tri.second, tri.third);
        glNormal3f(normal.x, normal.y, normal.z);

        // calculate the texture coordinates
        if (count % 2 != 0) {
            glTexCoord2f(0, 0);
            glVertex3f(tri.first.x, tri.first.y, tri.first.z);
            glTexCoord2f(1, 0);
            glVertex3f(tri.second.x, tri.second.y, tri.second.z);
            glTexCoord2f(0, 1);
            glVertex3f(tri.third.x, tri.third.y, tri.third.z);
        } else {
            glTexCoord2f(1, 0);
            glVertex3f(tri.first.x, tri.first.y, tri.first.z);
            glTexCoord2f(0, 1);
            glVertex3f(tri.second.x, tri.second.y, tri.second.z);
            glTexCoord2f(0, 1);
            glVertex3f(tri.third.x, tri.third.y, tri.third.z);
        }

        count++;
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // for (size_t z = 0; z < terrainData.size() - 1; z++) {
    //    for (size_t x = 0; x < terrainData.size() - 1; x++) {

    //        if (!wireframe) {
    //            glBegin(GL_TRIANGLES);
    //        } else {
    //            glBegin(GL_LINE_LOOP);
    //        }
    //        glVertex3f((x * scaleX), terrainData[x][z] * scaleY, (z)*scaleZ);
    //        glVertex3f((x + 1) * scaleX, terrainData.at(x + 1).at(z) * scaleY,
    //                   (z)*scaleZ);
    //        glVertex3f((x + 1) * scaleX, terrainData.at(x + 1).at(z + 1) * scaleY,
    //                   (z + 1) * scaleZ);

    //        glEnd();

    //        if (!wireframe) {
    //            glBegin(GL_TRIANGLES);
    //        } else {
    //            glBegin(GL_LINE_LOOP);
    //        }
    //        glVertex3f((x)*scaleX, terrainData.at(x).at(z) * scaleY, (z)*scaleZ);
    //        glVertex3f((x + 1) * scaleX, terrainData.at(x + 1).at(z + 1) * scaleY,
    //                   (z + 1) * scaleZ);
    //        glVertex3f((x)*scaleX, terrainData.at(x).at(z + 1) * scaleY,
    //                   (z + 1) * scaleZ);

    //        glEnd();
    //    }
    //}
}

bool Terrain::loadHeightfield(const std::string filename, const int size) {
    std::ifstream infile(filename, std::ios::binary);

    if (!infile) {
        std::cerr << "Cannot open file :" << filename << std::endl;
        return false;
    }

    if (!terrainData.empty()) {
        terrainData.clear();
    }
    terrainData.resize(size);

    if (size > 0) {
        infile.seekg(0, std::ios::end);
        auto length = infile.tellg();
        if (length % size != 0) {
            std::cout << length << " % " << size << " = " << length % size
                      << std::endl;
            return false;
        }
        infile.seekg(0, std::ios::beg);

        char c;
        for (auto y = 0; y < size; y++) {
            for (auto x = 0; x < size; x++) {
                infile.get(c);
                unsigned char vert = c;
                terrainData.at(y).push_back(vert - 128);
            }
        }
    }
    imageSize = size;
    return true;
}

void Terrain::readTerrainData() {
    for (auto y = 0; y < imageSize; y++) {
        for (auto x = 0; x < imageSize; x++) {
            std::cout << terrainData.at(x).at(y) << ',';
        }
        std::cout << std::endl;
    }
}



void Terrain::filterPass(float *dataP, int increment, float weight) {
    float yprev = *dataP;    // the starting point in the terrain array
    int j       = increment; // +1, -1, +size, -size
    float k     = weight;
    // loop through either
    // one row from left to right (increment = +1), or
    // one row from right to left (increment = -1), or
    // one column from top to bottom (increment = +size), or
    // one column from bottom to top (increment = -size)
    for (int i = 1; i < imageSize; i++) {
        // yi           = k yi - 1 + (1 - k) xi;
        *(dataP + j) = k * yprev + (1 - k) * (*(dataP + j)); //
        yprev        = *(dataP + j);
        j += increment;
    }
}

void Terrain::addFilter(float *heights, float weight) {
    int i;
    // erode left to right, starting at the beginning of each row
    for (i = 0; i < imageSize; i++)
        filterPass(&heights[imageSize * i], 1, weight);
    // erode right to left, starting at the end of each row
    for (i = 0; i < imageSize; i++)
        filterPass(&heights[imageSize * i + imageSize - 1], -1, weight);
    // erode top to bottom, starting at the beginning of each column
    for (i = 0; i < imageSize; i++)
        filterPass(&heights[i], imageSize, weight);
    // erode from bottom to top, starting from the end of each column
    for (i = 0; i < imageSize; i++)
        filterPass(&heights[imageSize * (imageSize - 1) + i], -imageSize, weight);
}

void Terrain::normaliseTerrain(float *heights) {
    float fMin, fMax;
    float fHeight;
    int i;
    fMin = heights[0];
    fMax = heights[0];
    // find the min/max values of the height terrainData
    for (i = 1; i < imageSize * imageSize; i++) {
        if (heights[i] > fMax)
            fMax = heights[i];
        else if (heights[i] < fMin)
            fMin = heights[i];
    }
    // find the range of the altitude
    if (fMax <= fMin)
        return;
    fHeight = fMax - fMin;
    // scale the values to a range of 0-255
    for (i = 0; i < imageSize * imageSize; i++) {
        heights[i] = ((heights[i] - fMin) / fHeight) * 255.0f;
    }
}

bool Terrain::genFaultFormation(int iterations, int hSize, int minHeight,
                                int maxHeight, float weight,
                                int postSmoothingIterations, bool random) {
    int x1, x2, z1, z2;
    float *heights = nullptr;
    int displacement;
    if (hSize <= 0)
        return false;
    if (random) // create truly random map
        srand(time(NULL));
    // allocate memory for heightfield array
    imageSize      = hSize;
    size_t size    = imageSize;
    auto arraySize = size * size;
    heights        = new float[arraySize]; // allocate memory

    if (heights == nullptr)
        return false;
    // initialise the heightfield array to all zeros
    for (int i = 0; i < arraySize; i++) {
        heights[i] = 0.0;
    }

    // generate heightfield
    for (int j = 0; j < iterations; j++) {
        // calculate reducing displacement value - how much to alter height
        displacement = maxHeight - ((maxHeight - minHeight) * j) / iterations;
        // pick the first point P1(x1, z1) at random from the height map
        x1 = (rand() % size);
        z1 = (rand() % size);
        // pick up the second random point P2(x2, z2) and make sure it is
        // different from the first point
        do {
            x2 = (rand() % size);
            z2 = (rand() % size);
        } while (x2 == x1 && z2 == z1);
        // for each point P(x, z) in the field, calculate the new height values
        for (int z = 0; z < size; z++) {
            for (int x = 0; x < size; x++) {
                // determine which side of the line P1P2 the point P lies in
                if (((x - x1) * (z2 - z1) - (x2 - x1) * (z - z1)) > 0) {
                    heights[(z * size) + x] += (float)displacement;
                }
            }
        }
        addFilter(heights, weight);
    }
    for (auto i = 0; i < postSmoothingIterations; i++) {
        addFilter(heights, weight);
    }

    // normalise the heightfield
    normaliseTerrain(heights);
    // copy the float heightfield to terrainData (in unsign char)

    // initialise the heightfield array to all zeros
    for (auto y = 0; terrainData.size() < size; y++) {
        std::vector<float> axis;
        for (int i = 0; i < size; i++) {
            axis.push_back(0.0);
        }
        terrainData.push_back(axis);
    }

    for (size_t z = 0; z < size; z++) {
        for (size_t x = 0; x < size; x++) {
            size_t place            = (z * size) + x;
            terrainData.at(z).at(x) = ((heights[place]));
        }
    }

    // dispose of the temporary array heights
    delete[] heights;
    return true;
}

void Terrain::flatTerrain(int size) {
    terrainData.clear();
    terrainData.resize(size);
    for (auto n : terrainData) {
        for (auto i = 0; i < terrainData.size(); i++) {
            terrainData.at(i).push_back(0.f);
        }
    }
}
