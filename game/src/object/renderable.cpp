#include "renderable.h"

Renderable::Renderable(glm::vec4 pos_in, int inst_id, glm::quat or_in ) : instance_id(inst_id), Object(pos_in, or_in) { }
