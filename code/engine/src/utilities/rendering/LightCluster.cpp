#include "engine/utilities/rendering/LightCluster.h"

namespace EisEngine::systems {
    float LightCluster::estimateError(const Vector3& pos) const {
        // get closest point from bounding box to pos
        Vector3 closest = bounding_box.GetClosestPointTo(pos);
        float d_min = max(Vector3::Distance(closest, pos), Math::EPSILON);

        // square attenuation 1 / d^2 (estimation of light effect on drawn mesh):
        auto geom = 1.0f / (d_min * d_min);

        // maximum value of specular lobe (upper bound of light effect at perfect alignment)
        auto brdf = RenderingSystem::GetMaxBRDF();

        return total_intensity * geom * brdf;
    }
}
