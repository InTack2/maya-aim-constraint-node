#include <maya/MFnPlugin.h>

#include "aimConstraintNode.h"

MStatus initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj, "Tack2", "1.0", "any");

    status = plugin.registerNode("AimConstraintNode",
                                 aimConstraintNode::id,
                                 aimConstraintNode::creator,
                                 aimConstraintNode::initialize);

    if (!status)
    {
        status.perror("registerNode");
        return status;
    }

    return status;
}

MStatus uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj);

    status = plugin.deregisterNode(aimConstraintNode::id);
    if (!status)
    {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}