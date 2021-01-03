#include "aimConstraintNode.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>

MTypeId aimConstraintNode::id(0x00121BD3);

MObject aimConstraintNode::aim_axis;
MObject aimConstraintNode::input_aim_matrix;
MObject aimConstraintNode::input_target_position;
MObject aimConstraintNode::input_target_position_x;
MObject aimConstraintNode::input_target_position_y;
MObject aimConstraintNode::input_target_position_z;
MObject aimConstraintNode::input_target_parent_matrix;

MObject aimConstraintNode::output_rotate;

MObject aimConstraintNode::output_rotate_x;
MObject aimConstraintNode::output_rotate_y;
MObject aimConstraintNode::output_rotate_z;

void *aimConstraintNode::creator()
{
    return new aimConstraintNode();
}

MStatus aimConstraintNode::initialize()
{
    MStatus status;
    MFnEnumAttribute enum_attr;
    MFnCompoundAttribute compo_attr;
    MFnNumericAttribute num_attr;
    MFnMatrixAttribute matrix_attr;
    MFnUnitAttribute unit_attr;

    aim_axis = enum_attr.create("aim_axis", "axa", 2);
    enum_attr.addField("X", 0);
    enum_attr.addField("Y", 1);
    enum_attr.addField("Z", 2);
    enum_attr.setKeyable(false);
    aimConstraintNode::addAttribute(aim_axis);

    input_aim_matrix = matrix_attr.create("input_aim_matrix", "iam");
    matrix_attr.setWritable(true);
    matrix_attr.setReadable(false);
    matrix_attr.setKeyable(true);
    aimConstraintNode::addAttribute(input_aim_matrix);

    input_target_parent_matrix = matrix_attr.create("input_target_parent_matrix", "itm");
    matrix_attr.setWritable(true);
    matrix_attr.setReadable(false);
    matrix_attr.setKeyable(true);
    aimConstraintNode::addAttribute(input_target_parent_matrix);

    input_target_position = compo_attr.create("input_target_position", "iapos");
    compo_attr.setWritable(true);
    compo_attr.setReadable(false);
    compo_attr.setKeyable(true);

    input_target_position_x = num_attr.create("input_target_pos_x", "itposx", MFnNumericData::kFloat, 0.0);
    num_attr.setWritable(true);

    input_target_position_y = num_attr.create("input_target_pos_y", "itposy", MFnNumericData::kFloat, 0.0);
    num_attr.setWritable(true);

    input_target_position_z = num_attr.create("input_target_pos_z", "itposz", MFnNumericData::kFloat, 0.0);
    num_attr.setWritable(true);

    compo_attr.addChild(input_target_position_x);
    compo_attr.addChild(input_target_position_y);
    compo_attr.addChild(input_target_position_z);
    aimConstraintNode::addAttribute(input_target_position);

    output_rotate = compo_attr.create("output_rotate", "or");
    compo_attr.setWritable(true);

    output_rotate_x = unit_attr.create("output_rotateX", "orx", MFnUnitAttribute::kAngle, 0.0);
    unit_attr.setWritable(true);
    compo_attr.addChild(output_rotate_x);
    output_rotate_y = unit_attr.create("output_rotateY", "ory", MFnUnitAttribute::kAngle, 0.0);
    unit_attr.setWritable(true);
    compo_attr.addChild(output_rotate_y);
    output_rotate_z = unit_attr.create("output_rotateZ", "orz", MFnUnitAttribute::kAngle, 0.0);
    unit_attr.setWritable(true);
    compo_attr.addChild(output_rotate_z);
    aimConstraintNode::addAttribute(output_rotate);

    aimConstraintNode::attributeAffects(aim_axis, output_rotate);
    aimConstraintNode::attributeAffects(input_aim_matrix, output_rotate);
    aimConstraintNode::attributeAffects(input_target_parent_matrix, output_rotate);
    aimConstraintNode::attributeAffects(input_target_position, output_rotate);

    return MS::kSuccess;
}
MPxNode::SchedulingType aimConstraintNode::schedulingType() const
{
    return MPxNode::kParallel;
}

MStatus aimConstraintNode::compute(const MPlug &plug, MDataBlock &data)
{
    const MPlug eval_plug = plug.isChild() ? plug.parent() : plug;
    if (eval_plug != output_rotate)
    {
        return MS::kUnknownParameter;
    }
    MMatrix input_aim_matrix_value = data.inputValue(input_aim_matrix).asMatrix();
    MMatrix input_target_parent_matrix_value = data.inputValue(input_target_parent_matrix).asMatrix();

    MVector input_aim_position = MPxTransformationMatrix(input_aim_matrix_value).translation();
    MVector input_parent_target_position = MPxTransformationMatrix(input_target_parent_matrix_value).translation();

    MVector input_target_position_vector = MVector(data.inputValue(input_target_position_x).asFloat(),
                                                   data.inputValue(input_target_position_y).asFloat(),
                                                   data.inputValue(input_target_position_z).asFloat());

    int target_aim_axis = data.inputValue(aim_axis).asInt();

    MVector target_vector[] = {MVector::xAxis, MVector::yAxis, MVector::zAxis};
    MVector eye_aim = target_vector[target_aim_axis];

    MVector eye_u = input_aim_position - (input_target_position_vector + input_parent_target_position);
    eye_u = eye_u.normal();

    MEulerRotation result_rotatesion = MQuaternion(eye_aim, eye_u).asEulerRotation();

    data.outputValue(output_rotate_x).setDouble(result_rotatesion.x);
    data.outputValue(output_rotate_y).setDouble(result_rotatesion.y);
    data.outputValue(output_rotate_z).setDouble(result_rotatesion.z);

    data.setClean(plug);

    return MS::kSuccess;
}