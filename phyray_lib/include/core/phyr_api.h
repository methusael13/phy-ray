#ifndef PHYRAY_CORE_PHYRAPI_H
#define PHYRAY_CORE_PHYRAPI_H

#include <core/phyr.h>
#include <core/scene.h>
#include <core/phyr_mem.h>

#include <core/color/spectrum.h>
#include <core/light/light.h>
#include <core/material/texture.h>
#include <core/material/material.h>
#include <core/integrator/integrator.h>

#include <modules/integrators/path.h>
#include <modules/cameras/perspective.h>

#include <modules/shapes/disk.h>
#include <modules/shapes/sphere.h>

#include <modules/textures/consttex.h>
#include <modules/textures/scaled.h>
#include <modules/textures/mix.h>

#include <modules/materials/glass.h>
#include <modules/materials/matte.h>
#include <modules/materials/metal.h>
#include <modules/materials/mirror.h>
#include <modules/materials/plastic.h>

#include <modules/lights/point.h>
#include <modules/lights/distant.h>
#include <modules/lights/diffuse.h>

#include <modules/filters/box.h>
#include <modules/filters/triangle.h>
#include <modules/filters/gaussian.h>
#include <modules/filters/lanczos.h>
#include <modules/filters/mitchell.h>

#include <modules/samplers/stratified.h>

#include <core/accel/bvh.h>

namespace phyr {

enum class PhyrState { UNINITIALIZED, OPTIONS_BLOCK, WORLD_BLOCK };
static PhyrState currentPhyrState = PhyrState::UNINITIALIZED;

}  // namespace phyr

#endif
