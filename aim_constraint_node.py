# -*- coding: utf-8 -*-
from __future__ import print_function
from __future__ import unicode_literals
from __future__ import absolute_import
from __future__ import generators
from __future__ import division

import math
import sys

import maya.api.OpenMaya as om
import maya.api.OpenMayaUI as omui


def maya_useNewAPI():
    pass


class AimConstaraintNode(om.MPxNode):
    id = om.MTypeId(0x7f010)

    aim_axis = om.MObject()

    input_aim_matrix = om.MObject()

    input_target_position = om.MObject()
    input_target_position_x = om.MObject()
    input_target_position_y = om.MObject()
    input_target_position_z = om.MObject()

    input_target_parent_matrix = om.MObject()

    output_rotate = om.MObject()
    koAngles = om.MObject()

    @staticmethod
    def initialize():
        e_attr = om.MFnEnumAttribute()
        c_attr = om.MFnCompoundAttribute()
        n_attr = om.MFnNumericAttribute()

        AimConstaraintNode.aim_axis = e_attr.create("aim_axis", "axa", 0)
        # e_attr.setChannelBox(True)
        e_attr.keyable = False
        e_attr.addField("X", 0)
        e_attr.addField("Y", 1)
        e_attr.addField("Z", 2)

        mat_attr = om.MFnMatrixAttribute()
        AimConstaraintNode.input_aim_matrix = mat_attr.create("input_aim_matrix", "iam")
        mat_attr.writable = True
        mat_attr.readable = False
        mat_attr.keyable = True

        AimConstaraintNode.input_target_parent_matrix = mat_attr.create("input_target_parent_matrix", "itm")
        mat_attr.writable = True
        mat_attr.readable = False
        mat_attr.keyable = True

        AimConstaraintNode.input_target_position = c_attr.create("input_target_position", "iapos")
        c_attr.writable = True
        c_attr.readable = False
        c_attr.keyable = True

        AimConstaraintNode.input_target_position_x = n_attr.create("input_target_pos_x", "itposx", om.MFnNumericData.kFloat, 0.0)
        n_attr.writable = True
        c_attr.addChild(AimConstaraintNode.input_target_position_x)

        AimConstaraintNode.input_target_position_y = n_attr.create("input_target_pos_y", "itposy", om.MFnNumericData.kFloat, 0.0)
        n_attr.writable = True
        c_attr.addChild(AimConstaraintNode.input_target_position_y)

        AimConstaraintNode.input_target_position_z = n_attr.create("input_target_pos_z", "itposz", om.MFnNumericData.kFloat, 0.0)
        c_attr.addChild(AimConstaraintNode.input_target_position_z)
        n_attr.writable = True

        rotate_attr = om.MFnCompoundAttribute()
        AimConstaraintNode.output_rotate = rotate_attr.create("output_rotate", "or")
        rotate_attr.writable = True

        rotate_unit_attr = om.MFnUnitAttribute()
        AimConstaraintNode.koAngles = [0, 0, 0]

        out_rotate_name = ["output_rotateX", "output_rotateY", "output_rotateZ"]
        out_rotate_shortname = ["orx", "ory", "orz"]
        for _ in range(3):
            AimConstaraintNode.koAngles[_] = rotate_unit_attr.create(out_rotate_name[_], out_rotate_shortname[_], om.MFnUnitAttribute.kAngle, 0.0)
            rotate_unit_attr.writable = True
            rotate_attr.addChild(AimConstaraintNode.koAngles[_])

        AimConstaraintNode.addAttribute(AimConstaraintNode.aim_axis)
        AimConstaraintNode.addAttribute(AimConstaraintNode.input_aim_matrix)
        AimConstaraintNode.addAttribute(AimConstaraintNode.input_target_position)

        AimConstaraintNode.addAttribute(AimConstaraintNode.input_target_parent_matrix)
        AimConstaraintNode.addAttribute(AimConstaraintNode.output_rotate)

        AimConstaraintNode.attributeAffects(AimConstaraintNode.aim_axis, AimConstaraintNode.output_rotate)
        AimConstaraintNode.attributeAffects(AimConstaraintNode.input_aim_matrix, AimConstaraintNode.output_rotate)
        AimConstaraintNode.attributeAffects(AimConstaraintNode.input_target_parent_matrix, AimConstaraintNode.output_rotate)
        AimConstaraintNode.attributeAffects(AimConstaraintNode.input_target_position, AimConstaraintNode.output_rotate)

    def __init__(self):
        super(AimConstaraintNode, self).__init__()

    def compute(self, plug, data_block):
        eval_plug = plug.parent() if plug.isChild else plug
        if eval_plug == AimConstaraintNode.output_rotate:

            # 計算
            input_aim_matrix_value = data_block.inputValue(AimConstaraintNode.input_aim_matrix).asMatrix()
            input_target_parent_matrix_value = data_block.inputValue(AimConstaraintNode.input_target_parent_matrix).asMatrix()

            input_aim_position = decompose_matrix(input_aim_matrix_value)[0]
            input_aim_position = om.MVector(input_aim_position[0], input_aim_position[1], input_aim_position[2])

            input_parent_target_position = decompose_matrix(input_target_parent_matrix_value)[0]
            input_parent_target_position = om.MVector(input_parent_target_position[0], input_parent_target_position[1], input_parent_target_position[2])

            input_target_position = om.MVector(data_block.inputValue(AimConstaraintNode.input_target_position_x).asFloat(),
                                               data_block.inputValue(AimConstaraintNode.input_target_position_y).asFloat(),
                                               data_block.inputValue(AimConstaraintNode.input_target_position_z).asFloat()
                                               )

            target_aim_axis = data_block.inputValue(AimConstaraintNode.aim_axis).asInt()
            target_vector = [om.MVector().kXaxisVector, om.MVector().kYaxisVector, om.MVector().kZaxisVector]

            eye_aim = target_vector[target_aim_axis]

            eye_u = (input_aim_position - (input_target_position + input_parent_target_position)).normal()

            result_rotatesion = om.MQuaternion()
            result_rotatesion = om.MQuaternion(eye_aim, eye_u).asEulerRotation()

            data_block.outputValue(AimConstaraintNode.koAngles[0]).setDouble(result_rotatesion[0])
            data_block.outputValue(AimConstaraintNode.koAngles[1]).setDouble(result_rotatesion[1])
            data_block.outputValue(AimConstaraintNode.koAngles[2]).setDouble(result_rotatesion[2])

            data_block.setClean(plug)


def initializePlugin(obj):
    mplugin = om.MFnPlugin(obj)

    try:
        mplugin.registerNode('AimConstaraintNode', AimConstaraintNode.id, AimConstaraintNode, AimConstaraintNode.initialize, om.MPxNode.kDependNode)

    except Exception:
        sys.stderr.write('Faled to register node: {}'.format('AimConstaraintNode'))
        raise Exception()


def uninitializePlugin(obj):
    mplugin = om.MFnPlugin(obj)
    try:
        mplugin.deregisterNode(AimConstaraintNode.id)

    except Exception:
        sys.stderr.write('Faled to uninitialize node: {}'.format('AimConstaraintNode'))
        raise Exception()


def decompose_matrix(matrix, order=om.MTransformationMatrix.kXYZ):

    mt_mat = om.MTransformationMatrix(matrix)

    trans = mt_mat.translation(om.MSpace.kWorld)

    euler_rotation = mt_mat.rotation()
    euler_rotation.reorderIt(order)

    angles = [math.degrees(euler_rotation.x), math.degrees(euler_rotation.y), math.degrees(euler_rotation.z)]

    scale = mt_mat.scale(om.MSpace.kWorld)

    return [trans.x, trans.y, trans.z], angles, scale
