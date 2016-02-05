#include "Grid.hpp"
#include "ShaderPaths.hpp"

#include <atlas/gl/Shader.hpp>
#include <atlas/gl/ErrorCheck.hpp>

Grid::Grid() :
        mVertexCount(100)
{
        USING_ATLAS_GL_NS;
        USING_ATLAS_MATH_NS;

        mModel = Matrix4(1.f);
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);
        glGenBuffers(1, &mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        Vector4* vertices = new Vector4[mVertexCount];
        size_t n = mVertexCount / 4;
        for(size_t i = 0; i < n; ++i)
        {
                vertices[4 * i]         = Vector4(-12.f + i, 0.f, -12.f    , 1.f);
                vertices[4 * i + 1]     = Vector4(-12.f + i, 0.f,  12.f    , 1.f);
                vertices[4 * i + 2]     = Vector4(-12.f    , 0.f, -12.f + i, 1.f);
                vertices[4 * i + 3]     = Vector4( 12.f    , 0.f, -12.f + i, 1.f);
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * mVertexCount, vertices, GL_STATIC_DRAW);
        const std::string ShaderDir = generated::ShaderPaths::getShaderDirectory();
        std::vector<ShaderInfo> shaders {
                { GL_VERTEX_SHADER, ShaderDir + "grid.vs.glsl"},
                { GL_FRAGMENT_SHADER, ShaderDir + "grid.fs.glsl"}
        };

        mShaders.push_back(ShaderPointer(new Shader));
        mShaders[0]->compileShaders(shaders);
        mShaders[0]->linkShaders();

        GLuint varID;
        varID = mShaders[0]->getUniformVariable("MVP");
        mUniforms.insert(UniformKey("MVP", varID));
        varID = mShaders[0]->getUniformVariable("color");
        mUniforms.insert(UniformKey("color", varID));
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        mShaders[0]->disableShaders();
        delete[] vertices;
}

Grid::~Grid()
{
        glDeleteVertexArrays(1, &mVao);
        glDeleteBuffers(1, &mVbo);
}

void Grid::renderGeometry(atlas::math::Matrix4 proj,
                atlas::math::Matrix4 view)
{
        USING_ATLAS_MATH_NS;
        mShaders[0]->enableShaders();
        glBindVertexArray(mVao);
        Matrix4 mvp = proj * view * mModel;
        glUniformMatrix4fv(mUniforms["MVP"], 1, GL_FALSE, &mvp[0][0]);
        GLfloat color[] = {0.489, 0.489, 0.489};
        glUniform3fv(mUniforms["color"], 1, color);
        glDrawArrays(GL_LINES, 0, mVertexCount);
        glBindVertexArray(0);
        mShaders[0]->disableShaders();
}


