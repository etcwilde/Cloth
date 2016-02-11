#include "Cloth.hpp"


Cloth::Cloth(unsigned int width , unsigned int height):
        mWidth(width),
        mHeight(height),
        mMasses(width * height),
        mPinned({0,1, 6,7}),
        mVelocities(width * height),
        mPaused(false) // TODO: Change this in the future
{
#ifdef PROG_DEBUG
        USING_ATLAS_CORE_NS;
        Log::log(Log::SeverityLevel::DEBUG, "Size of int: " + std::to_string(sizeof(unsigned int)) +", Float: " + std::to_string(sizeof(float)));
#endif
        mModel = atlas::math::Matrix4(1.f);
        assert(sizeof(unsigned int) == sizeof(float));
        std::vector<unsigned int> indices;
        glm::vec3 origin(-3.5f, 8.f, 0.f);
        const float diagnonalWidth = std::sqrt(CLOTH_DEFAULT_SPACE * CLOTH_DEFAULT_SPACE + CLOTH_DEFAULT_SPACE * CLOTH_DEFAULT_SPACE);
        for (unsigned int h = 0; h < mHeight; ++h)
                for (unsigned int w = 0; w < mWidth; ++w)
                {
                        const unsigned int index = getIndex(w, h);
                        // Initialize mass position and mass
                        mMasses[index].position =
                                origin + glm::vec3( w * CLOTH_DEFAULT_SPACE,
                                                -static_cast<float>(h*CLOTH_DEFAULT_SPACE),
                                                0.f);
                        mMasses[index].mass = CLOTH_DEFAULT_MASS;

/*
 * NEIGHBORS Data layout
 *                        0
 *                        |
 *                     3--+--1
 *                        |
 *                        2
 *
 *
 *                      4   5
 *                       \ /
 *                        X
 *                       / \
 *                      7   6
 *
 *                        8
 *                        |
 *                        0
 *                        |
 *                 11--3--+--1--9
 *                        |
 *                        2
 *                        |
 *                        10
 *
 */
                        // Straight Neighbors
                        if(h != 0)
                                mMasses[index].neighbors[0]=
                                {getIndex(w, h-1) + 1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, CLOTH_DEFAULT_SPACE}};
                        else
                                mMasses[index].neighbors[0].index = 0;
                        if(w != mWidth - 1)
                                mMasses[index].neighbors[1]=
                                {getIndex(w+1, h)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, CLOTH_DEFAULT_SPACE}};
                        else mMasses[index].neighbors[1].index = 0;
                        if(h != mHeight - 1)
                                mMasses[index].neighbors[2]=
                                {getIndex(w, h+1)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, CLOTH_DEFAULT_SPACE}};
                        else mMasses[index].neighbors[2].index = 0;
                        if(w != 0)
                                mMasses[index].neighbors[3]=
                                {getIndex(w-1, h)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, CLOTH_DEFAULT_SPACE}};
                        else mMasses[index].neighbors[3].index = 0;
                        // Diagonal Neighbors
                        if(h != 0 && w != 0)
                                mMasses[index].neighbors[4]=
                                {getIndex(w-1,h-1)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, diagnonalWidth}};
                        else mMasses[index].neighbors[4].index = 0;
                        if(h != 0 && w != mWidth - 1)
                                mMasses[index].neighbors[5]=
                                {getIndex(w+1,h-1)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, diagnonalWidth}};
                        else mMasses[index].neighbors[5].index = 0;
                        if(h != mHeight - 1 && w != mWidth - 1)
                                mMasses[index].neighbors[6]=
                                {getIndex(w+1, h+1)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, diagnonalWidth}};
                        else mMasses[index].neighbors[6].index = 0;
                        if(h != mHeight - 1 && w != 0)
                                mMasses[index].neighbors[7]=
                                {getIndex(w-1, h+1)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, diagnonalWidth}};
                        else mMasses[index].neighbors[7].index = 0;
                        // Two away -- Could use straight neighbors to help
                        if(h != 1 && h != 0)
                                mMasses[index].neighbors[8]=
                                {getIndex(w, h-2)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, CLOTH_DEFAULT_SPACE * 2}};
                        else mMasses[index].neighbors[8].index = 0;
                        if(w + 1 != mWidth - 1 && w != mWidth - 1)
                                mMasses[index].neighbors[9]=
                                {getIndex(w+2, h)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, CLOTH_DEFAULT_SPACE * 2}};
                        else mMasses[index].neighbors[9].index = 0;
                        if(h + 1 != mHeight - 1 && h != mHeight - 1)
                                mMasses[index].neighbors[10]=
                                {getIndex(w, h+2)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, CLOTH_DEFAULT_SPACE * 2}};
                        else mMasses[index].neighbors[10].index = 0;
                        if(w != 1 && w != 0)
                                mMasses[index].neighbors[11]=
                                {getIndex(w-2, h)+1, {CLOTH_DEFAULT_K, CLOTH_DEFAULT_D, CLOTH_DEFAULT_SPACE * 2}};
                        else mMasses[index].neighbors[11].index = 0;
                }
        unsigned int current_index = 0;
        for (auto m: mMasses)
        {
                for (unsigned int i = 0; i < 12; ++i)
                {
                        const struct neighbor_t n = m.neighbors[i];
                        if (n.index != 0) // So, not an empty one
                        {
                                indices.push_back(current_index);
                                indices.push_back(n.index - 1);
                        }
                }
                current_index++;
        }

        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        glGenBuffers(1, &mVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(struct mass_t) * mMasses.size(), mMasses.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mass_t), (void*)0);
        glGenBuffers(1, &mIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
        mIndexSize = indices.size();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndexSize, indices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        // shaders
        USING_ATLAS_GL_NS;
        const std::string ShaderDir = generated::ShaderPaths::getShaderDirectory();
        std::vector<ShaderInfo> shaders
        {
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
        glBindVertexArray(0);
        mShaders[0]->disableShaders();
}

Cloth::~Cloth()
{
        glDeleteBuffers(1, &mVertexBuffer);
        glDeleteBuffers(1, &mIndexBuffer);
        glDeleteVertexArrays(1, &mVao);
}

void Cloth::updateGeometry(atlas::utils::Time const& t)
{
#ifdef PROG_DEBUG
        USING_ATLAS_CORE_NS;
        Log::log(Log::SeverityLevel::DEBUG, "updating Cloth");
#endif
        if(mPaused) return;
        // Each mass will have the same force applied
        // gravitay
        const glm::vec4 g(0, -9.087, 0, 0);
        glm::vec4 fan(0.f);
        if (mFan) fan = glm::vec4(0, 0, std::sin(t.currentTime), 0) / 3.f;
        // TODO: We can use a sin wave to generate some fluttering

        // Copy masses
        std::vector<mass_t> masses(mMasses); // Update these, then overwrite the original
        auto sum_dampen = [=](struct mass_t m){
                float accum = 0.f;
                for (unsigned int i = 0; i < 12; ++i)
                        accum += m.neighbors[i].index == 0 ? 0.f : m.neighbors[i].d;
                return accum;
        };
        for (auto mIt = masses.begin(); mIt != masses.end(); mIt++)
        {
                const unsigned int index = mIt - masses.begin();
                bool pinned = false;
                // Yes, I could use a boolean and a continue, but I don't want to.
                // Check that this isn't a fixed mass
                for (auto i: mPinned)
                        if (index == i)
                        { pinned = true; break; }
                if (pinned) continue;
                glm::vec4 F(0.f);       // Force
                glm::vec4 s(0.f);       // Displacement
                const float d = sum_dampen(*mIt);
                F = g + fan - d * mVelocities[index];
                for (unsigned int i = 0; i < 12; ++i)
                {
                        // Ignore baddies
                        if (mIt->neighbors[i].index == 0) continue;
#ifdef PROG_DEBUG
                        const unsigned int other_index = mIt->neighbors[i].index - 1;
#endif
                        // Okay, for each of our neighbors, calculate force
                        const struct mass_t mother = mMasses[mIt->neighbors[i].index - 1];
                        const glm::vec3 displacement = mother.position - mIt->position;
                        const float x = glm::length(displacement);
                        F += glm::vec4(-mIt->neighbors[i].k * (mIt->neighbors[i].lr - x ) * glm::normalize(displacement), 0.f);
                        //F = -mIt->neighbors[i].k * (mIt->neighbors[i].lr - x) * glm::vec4(0, 1, 0, 0);
#ifdef PROG_DEBUG
                        glm::vec3 t = mIt->neighbors[i].k * (x-mIt->neighbors[i].lr) *glm::normalize(displacement);
                        Log::log(Log::SeverityLevel::DEBUG,
                                        "Distance: " + std::to_string(x) + ", " +
                                        std::to_string(mIt->neighbors[i].lr) + "=" +
                                        std::to_string(mIt->neighbors[i].lr - x));
                        Log::log(Log::SeverityLevel::DEBUG, "Displacement: " +
                                        std::to_string(displacement.x) + ", " +
                                        std::to_string(displacement.y) + ", " +
                                        std::to_string(displacement.z));
                        /*Log::log(Log::SeverityLevel::DEBUG, "Spring Force[" + std::to_string(index) + "-" + std::to_string(other_index)+"][" + std::to_string(i)+"]: " +
                                        std::to_string(t.x) + ", " +
                                        std::to_string(t.y) + ", " +
                                        std::to_string(t.z)); */
#endif
                }

                // Euler Integrator
                const glm::vec4 a = F / mIt->mass;
                s = mVelocities[index] * t.deltaTime + 0.5f * a * t.deltaTime * t.deltaTime;
                mVelocities[index] = mVelocities[index] + a * t.deltaTime;
#ifdef PROG_DEBUG
                /*
                USING_ATLAS_CORE_NS;
                Log::log(Log::SeverityLevel::DEBUG, "F: " +
                                std::to_string(F.x) + ", "+
                                std::to_string(F.y) + ", "+
                                std::to_string(F.z));

                Log::log(Log::SeverityLevel::DEBUG, "Acceleration: " +
                                std::to_string(a.x) + ", "+
                                std::to_string(a.y) + ", "+
                                std::to_string(a.z));
                Log::log(Log::SeverityLevel::DEBUG, "S: " +
                                std::to_string(s.x) + ", "+
                                std::to_string(s.y) + ", "+
                                std::to_string(s.z));

                Log::log(Log::SeverityLevel::DEBUG, "Original Position: " +
                                std::to_string(mIt->position.x) + ", "+
                                std::to_string(mIt->position.y) + ", "+
                                std::to_string(mIt->position.z));
                                */
#endif
                mIt->position = mIt->position + glm::vec3(s.x, s.y, s.z);

#ifdef PROG_DEBUG
                /*
                Log::log(Log::SeverityLevel::DEBUG, "New Position: " +
                                std::to_string(mIt->position.x) + ", "+
                                std::to_string(mIt->position.y) + ", "+
                                std::to_string(mIt->position.z));
                                */
#endif
        }
        mMasses = masses;
        // Update position
        glBindVertexArray(mVao);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mass_t) * mMasses.size(), mMasses.data());
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cloth::renderGeometry(atlas::math::Matrix4 proj, atlas::math::Matrix4 view)
{
        USING_ATLAS_MATH_NS;
        mShaders[0]->enableShaders();
        glBindVertexArray(mVao);
        atlas::math::Matrix4 mvp = proj * view * mModel;
        glUniformMatrix4fv(mUniforms["MVP"], 1, GL_FALSE, &mvp[0][0]);
        GLfloat color[] = {0.000, 0.5, 0.998};
        glUniform3fv(mUniforms["color"], 1, color);
        glDrawElements(GL_LINES, mIndexSize, GL_UNSIGNED_INT, (void*)1);
        glBindVertexArray(0);
        mShaders[0]->disableShaders();
}

void Cloth::resetGeometry()
{
}
