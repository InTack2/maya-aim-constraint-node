#pragma once

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class aimConstraintNode : public MPxNode
{
public:
    aimConstraintNode(){};
    virtual ~aimConstraintNode(){};
    static void *creator();
    static MStatus initialize();
    MPxNode::SchedulingType schedulingType() const override;
    MStatus compute(const MPlug &plug, MDataBlock &data) override;

public:
    static MTypeId id;
    static MObject aim_axis;
    static MObject input_aim_matrix;
    static MObject input_target_position;
    static MObject input_target_position_x;
    static MObject input_target_position_y;
    static MObject input_target_position_z;
    static MObject input_target_parent_matrix;

    static MObject output_rotate;

    static MObject output_rotate_x;
    static MObject output_rotate_y;
    static MObject output_rotate_z;
};