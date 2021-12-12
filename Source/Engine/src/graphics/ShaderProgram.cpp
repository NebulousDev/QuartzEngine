#include "ShaderProgram.h"

namespace Quartz
{
    Bool8 ShaderProgram::IsValidTraditional()
    {
        return vertex && fragment &&
            !compute &&
            !mesh &&
            !rayGeneration;
    }

    Bool8 ShaderProgram::IsValidMesh()
    {
        return task && mesh &&
            !compute &&
            !vertex &&
            !fragment &&
            !rayGeneration;
    }

    Bool8 ShaderProgram::IsValidCompute()
    {
        return compute &&
            !mesh &&
            !vertex &&
            !fragment &&
            !rayGeneration;
    }
}

