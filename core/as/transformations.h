#pragma once
#include "../ew/ewMath/mat4.h"

namespace as {


	//Identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	//Scale on x,y,z axes
	inline ew::Mat4 Scale(ew::Vec3 s) {
		return ew::Mat4(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};
    //Rotation around X axis (pitch) in radians
    inline ew::Mat4 RotateX(float rad) {
        float c = cos(rad);
        float s = sin(rad);
        return ew::Mat4(
            1, 0, 0, 0,
            0, c, -s, 0,
            0, s, c, 0,
            0, 0, 0, 1
        );
    }

    //Rotation around Y axis (yaw) in radians
    inline ew::Mat4 RotateY(float rad) {
        float c = cos(rad);
        float s = sin(rad);
        return ew::Mat4(
            c, 0, s, 0,
            0, 1, 0, 0,
            -s, 0, c, 0,
            0, 0, 0, 1
        );
    }

    //Rotation around Z axis (roll) in radians
    inline ew::Mat4 RotateZ(float rad) {
        float c = cos(rad);
        float s = sin(rad);
        return ew::Mat4(
            c, -s, 0, 0,
            s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }

    //Translate x,y,z
    inline ew::Mat4 Translate(ew::Vec3 t) {
        return ew::Mat4(
            1, 0, 0, t.x,
            0, 1, 0, t.y,
            0, 0, 1, t.z,
            0, 0, 0, 1
        );
    }
    struct Transform {
        ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
        ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); // Euler angles (degrees)
        ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
        const double PI = 3.14159265358979323846;

        ew::Mat4 getModelMatrix() const {
            ew::Mat4 scaleMatrix = Scale(scale);

            ew::Mat4 rotationMatrixX = RotateX(rotation.x * (PI / 180.0f));  // Convert to radians
            ew::Mat4 rotationMatrixY = RotateY(rotation.y * (PI / 180.0f));  // Convert to radians
            ew::Mat4 rotationMatrixZ = RotateZ(rotation.z * (PI / 180.0f));  // Convert to radians

            ew::Mat4 translationMatrix = Translate(position);

            return translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * scaleMatrix;
        }
    };

}
