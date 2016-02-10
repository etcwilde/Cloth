#ifndef __SPLINE_HPP
#define __SPLINE_HPP

#define _SPLINE_STEP 0.05f

#include <atlas/math/Math.hpp>
#include <atlas/core/Log.hpp>

#include <glm/gtx/simd_vec4.hpp>
#include <glm/gtx/simd_mat4.hpp>

#include <vector>
#include <string>

#include "PointGeometry.hpp"
#include "ShaderPaths.hpp"

class Spline : public PointGeometry
{
        public:
                Spline();
                ~Spline();

                void renderGeometry(atlas::math::Matrix4 proj,
                                atlas::math::Matrix4 view) override;

                void resetGeometry() override;

                glm::vec3 sampleCurve(size_t segment, float t) const;
        private:
                GLuint mVao;
                GLuint mVbo[2]; // 0 is for the vertex points 1 is for spline

                void genSpline(); // Uses control points to generate the spline


                glm::detail::fmat4x4SIMD mSpline; // The spline matrix
                std::vector<glm::vec3> mSplinePoints;
                std::vector<glm::vec3> mCtrlsPts;
};

#endif//__SPLINE_HPP
