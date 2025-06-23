#pragma once

#if !defined(EI_CLASSIFIER_PROJECT_ID)
#error "You must include the Edge Impulse library before <esptoolkit.h>"
#endif

#if defined(EI_CLASSIFIER_OBJECT_DETECTION)
#include "./camera.h"
#include "../../includes/edgeimpulse/FOMO.h"
#endif