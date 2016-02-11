#ifndef __CLOTH_HPP
#define __CLOTH_HPP

#define CLOTH_DEFAULT_MASS      0.2f // Mass of masses (grams)
#define CLOTH_DEFAULT_SPACE     0.5f // Spacing between masses
#define CLOTH_DEFAULT_K 6.0f // Spring constant
#define CLOTH_DEFAULT_D 0.4f // Dampening Constant

#include <vector>

#include <stddef.h> // Offsetof

#include <atlas/utils/Geometry.hpp>
#include <atlas/utils/Time.hpp>
#include <atlas/math/Matrix.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/gl/Shader.hpp>

#include <glm/gtx/simd_vec4.hpp>
#include <glm/gtx/simd_mat4.hpp>

#include "ShaderPaths.hpp"


class Cloth : public atlas::utils::Geometry
{
        public:
                Cloth(unsigned int width, unsigned int height);
                ~Cloth();

                void updateGeometry(atlas::utils::Time const& t) override;
                void renderGeometry(atlas::math::Matrix4 proj, atlas::math::Matrix4 view) override;
                void resetGeometry() override;
        private:
                struct neighbor_t
                {
                        GLuint index; // 0 index means no neighbor Index is index - 1
                        struct{
                                float k;
                                float d;
                                float lr;
                        };
                };
                struct mass_t
                {
                        glm::vec3 position;
                        float mass;
                        struct neighbor_t neighbors[12];
                };

                inline GLuint getIndex(unsigned int w, unsigned int h) const
                { return w + mHeight * h; }
        private:
                GLuint mVao;
                GLuint mVertexBuffer;
                GLuint mIndexBuffer;

                unsigned int mWidth;
                unsigned int mHeight;

                bool mFan;

                GLuint mIndexSize;

                // Mass Data
                std::vector<mass_t> mMasses; // xyz coord, w: mass
                std::vector<GLuint> mPinned; // Masses that are pinned
                std::vector<glm::vec4> mVelocities; // Velocity of each mass
                bool mPaused;
};

#endif//__CLOTH_HPP
