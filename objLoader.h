#pragma once
#include <gl/glew/glew.h>
#include <vector>
#include <string>
#include<gl\glm\glm.hpp>


bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices)
{
    std::vector< unsigned int > vertexIndices;
    std::vector< glm::vec3 > temp_vertices;
    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Impossible to open the file !\n");
        return false;
    }
    while (1)
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            // Face
            int x;
            unsigned int vertexIndex[3];
            int matches = fscanf(file, "%d %d %d\n",
                &vertexIndex[0],
                &vertexIndex[1],
                &vertexIndex[2]);
            if (matches != 3) {
                printf("File can't be read by our simple parser: Try exporting with other options.\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
        }
    }
    printf("\n file parsed");
    printf("\n %d %d %d", vertexIndices[vertexIndices.size() - 3], vertexIndices[vertexIndices.size() - 2], vertexIndices[vertexIndices.size() - 1]);
    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    return true;
}
#pragma once
