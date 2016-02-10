#ifndef __POINT_GEOMETRY_HPP
#define __POINT_GEOMETRY_HPP

#include <atlas/utils/Geometry.hpp>
#include <atlas/math/Math.hpp>
#include <atlas/gl/Shader.hpp>
#include <atlas/core/Macros.hpp>

#include <vector>

/**
 * PointGeometry
 *
 * Extends the atlas::utils::Geometry class for the use of geometry with points in
 * 3D space.
 */
class PointGeometry : public atlas::utils::Geometry
{
        public:
                PointGeometry() {}
                PointGeometry(PointGeometry const& geom) :
                        Geometry(geom),
                        mPoints(geom.mPoints) { }
                virtual ~PointGeometry() {}

                inline const std::vector<atlas::math::Vector>& getPoints() const { return mPoints; }
                inline const atlas::math::Vector& getPoint(size_t index) const { return mPoints[index]; }

        protected:
                std::vector<atlas::math::Vector> mPoints;
};

#endif//__POINT_GEOMETRY_HPP
