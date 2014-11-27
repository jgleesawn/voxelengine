#include "renderable.h"

Renderable::Renderable(glm::vec4 pos_in, int inst_id, glm::quat or_in, float p_in, float y_in, float m_in, btCollisionShape * shape_in ) : instance_id(inst_id), Object(pos_in, or_in, p_in, y_in, m_in, shape_in) { }
