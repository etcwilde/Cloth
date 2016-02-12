#include "Spline.hpp"

#include <chrono>
#include <iostream>

#define CUBE(x) x * x * x
#define SQUARE(x) x * x


#include <glm/gtx/simd_mat4.hpp>
#include <glm/detail/intrinsic_matrix.hpp>
#include <glm/gtx/spline.hpp>

Spline::Spline()
{

        mCtrlsPts.push_back(glm::vec3(0, 2, -10));
        mCtrlsPts.push_back(glm::vec3(10, 2, 0));
        mCtrlsPts.push_back(glm::vec3(7, 2, 7));
        mCtrlsPts.push_back(glm::vec3(0, 2, 10));
        mCtrlsPts.push_back(glm::vec3(-7, 2, 7));
        mCtrlsPts.push_back(glm::vec3(-10, 2, 0));
        mCtrlsPts.push_back(glm::vec3(-7, 2, -7));
        mCtrlsPts.push_back(glm::vec3(0, 2, -10));
        mCtrlsPts.push_back(glm::vec3(7, 2, -7));
        mCtrlsPts.push_back(glm::vec3(10, 2, 0));
        mCtrlsPts.push_back(glm::vec3(0, 2, 10));
        USING_ATLAS_GL_NS;

        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);
        glGenBuffers(1, mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
        mSpline = glm::transpose(glm::detail::fmat4x4SIMD(
                                2, -2, 1, 1,
                                -3, 3, -2, -1,
                                0, 0, 1, 0,
                                1, 0, 0, 0));
        genSpline();

        USING_ATLAS_CORE_NS;
        size_t points = sizeof(glm::vec3) * mSplinePoints.size();
        glBufferData(GL_ARRAY_BUFFER, points, mSplinePoints.data(), GL_STATIC_DRAW);

        const std::string shaderDir = generated::ShaderPaths::getShaderDirectory();
        std::vector<ShaderInfo> shaders =
        {
                { GL_VERTEX_SHADER, shaderDir + "grid.vs.glsl"},
                { GL_FRAGMENT_SHADER, shaderDir + "grid.fs.glsl"}
        };

        mShaders.push_back(ShaderPointer(new Shader));
        mShaders[0]->compileShaders(shaders);
        mShaders[0]->linkShaders();

        GLuint varID;
        varID = mShaders[0]->getUniformVariable("MVP");
        mUniforms.insert(UniformKey("MVP", varID));
        varID = mShaders[0]->getUniformVariable("color");
        mUniforms.insert(UniformKey("color", varID));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Spline::~Spline()
{
        glDeleteVertexArrays(1, &mVao);
        glDeleteBuffers(1, mVbo);
}

void Spline::renderGeometry(atlas::math::Matrix4 proj, atlas::math::Matrix4 view)
{
        USING_ATLAS_MATH_NS;
        mShaders[0]->enableShaders();
        glBindVertexArray(mVao);
        Matrix4 mvp = proj * view * mModel;
        glUniformMatrix4fv(mUniforms["MVP"], 1, GL_FALSE, &mvp[0][0]);
        GLfloat color[] = { 1.f, 1.f, 1.f};
        glUniform3fv(mUniforms["color"], 1, color);
        glPointSize(2.5f);
        glDrawArrays(GL_LINE_STRIP, 0, mSplinePoints.size());
        glBindVertexArray(0);
        mShaders[0]->disableShaders();
}

glm::vec3 Spline::sampleCurve(size_t segment, float t) const
{
        const glm::vec3 p0 = mCtrlsPts[(segment) % mCtrlsPts.size()];
        const glm::vec3 p1 = mCtrlsPts[(segment + 1) % mCtrlsPts.size()];
        const glm::vec3 p2 = mCtrlsPts[(segment + 2) % mCtrlsPts.size()];
        const glm::vec3 p3 = mCtrlsPts[(segment + 3) % mCtrlsPts.size()];
        const glm::vec3 d0 = (p2 - p0) * 0.5f;
        const glm::vec3 d1 = (p3 - p1) * 0.5f;
        const glm::detail::fvec4SIMD a = mSpline * glm::detail::fvec4SIMD(p1.x, p2.x, d0.x, d1.x);
        const glm::detail::fvec4SIMD b = mSpline * glm::detail::fvec4SIMD(p1.y, p2.y, d0.y, d1.y);
        const glm::detail::fvec4SIMD c = mSpline * glm::detail::fvec4SIMD(p1.z, p2.z, d0.z, d1.z);
        const glm::detail::fvec4SIMD u(CUBE(t), SQUARE(t), t, 1.f);
        return glm::vec3(
                        glm::vec4_cast(glm::dot4(a * u, glm::detail::fvec4SIMD(1.f))).x,
                        glm::vec4_cast(glm::dot4(b * u, glm::detail::fvec4SIMD(1.f))).y,
                        glm::vec4_cast(glm::dot4(c * u, glm::detail::fvec4SIMD(1.f))).z
                        );
}

void Spline::genSpline()
{
        USING_ATLAS_CORE_NS;
        size_t sections = mCtrlsPts.size() > 3 ? mCtrlsPts.size() - 3 : 0;
        size_t steps = static_cast<size_t>(1.f / _SPLINE_STEP);

        size_t total_size = sections * steps;
        mSplinePoints = std::vector<glm::vec3>();
        mSplinePoints.reserve(total_size);

#define PRINT_VEC4(v) Log::log(Log::SeverityLevel::DEBUG,\
                "[" + std::to_string(v.x) + ", " +\
                std::to_string(v.y) + ", " +\
                std::to_string(v.z) + ", " +\
                std::to_string(v.w) + "]");
#define PRINT_VEC3(v) Log::log(Log::SeverityLevel::DEBUG,\
                "[" + std::to_string(v.x) + ", " +\
                std::to_string(v.y) + ", " +\
                std::to_string(v.z) + "]");

        auto start  = std::chrono::high_resolution_clock::now();
#ifdef READABLE
        // This is slower, but more readable
        for (size_t section = 0; section < sections; ++section)
                for (size_t step = 0; step <= steps; ++step)
                        mSplinePoints.push_back(sampleCurve(section, (1.f / steps) * step));

#else
        for (size_t section = 0; section < sections; ++section)
        {
                // Control Points:
                const glm::vec3 p0 = mCtrlsPts[(section) % mCtrlsPts.size()];
                const glm::vec3 p1 = mCtrlsPts[(section + 1) % mCtrlsPts.size()];
                const glm::vec3 p2 = mCtrlsPts[(section + 2) % mCtrlsPts.size()];
                const glm::vec3 p3 = mCtrlsPts[(section + 3) % mCtrlsPts.size()];
                const glm::vec3 d0 = (p2 - p0) * 0.5f;
                const glm::vec3 d1 = (p3 - p1) * 0.5f;

                glm::detail::fvec4SIMD a = mSpline * glm::detail::fvec4SIMD(p1.x, p2.x, d0.x, d1.x);
                glm::detail::fvec4SIMD b = mSpline * glm::detail::fvec4SIMD(p1.y, p2.y, d0.y, d1.y);
                glm::detail::fvec4SIMD c = mSpline * glm::detail::fvec4SIMD(p1.z, p2.z, d0.z, d1.z);

                for (size_t step = 0; step <= steps; ++step)
                {
                        const glm::detail::fvec4SIMD u(CUBE(1.f/steps * step), SQUARE(1.f/steps * step), (1.f/ steps) * step, 1.f);
                        mSplinePoints.push_back(
                                        glm::vec3(
                                                glm::vec4_cast(glm::dot4(a * u, glm::detail::fvec4SIMD(1.f))).x,
                                                glm::vec4_cast(glm::dot4(b * u, glm::detail::fvec4SIMD(1.f))).y,
                                                glm::vec4_cast(glm::dot4(c * u, glm::detail::fvec4SIMD(1.f))).z));
                }
        }
#endif
        auto end = std::chrono::high_resolution_clock::now();
        Log::log(Log::SeverityLevel::DEBUG, "Populate Duration: " +
                        std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()));
}

void Spline::resetGeometry() { }
