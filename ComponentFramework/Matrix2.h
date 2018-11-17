#ifndef MATRIX2_H
#define MATRIX2_H

#include "Vector.h"
#include <iostream>

namespace MATH {
	class Mat3 {
	private:
		float m[3][3];
	public:
		//Constructor
		inline Mat3(float r1c1, float r1c2, float r1c3,
			float r2c1, float r2c2, float r2c3,
			float r3c1, float r3c2, float r3c3) {
			m[0][0] = r1c1; m[0][1] = r1c2; m[0][2] = r1c3;
			m[1][0] = r2c1; m[1][1] = r2c2; m[1][2] = r2c3;
			m[2][0] = r3c1; m[2][1] = r3c2; m[2][2] = r3c3;
		}

		inline Mat3(float n = 1.0f) {
			if (n == 1.0f) {
				LoadIdentity();
			}
			else {
				m[0][0] = n; m[0][1] = n; m[0][2] = n;
				m[1][0] = n; m[1][1] = n; m[1][2] = n;
				m[2][0] = n; m[2][1] = n; m[2][2] = n;
			}
		}

		inline Mat3(const Mat3& n) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					m[i][j] = n(i, j);
				}
			}
		}

		//Operator Overloads

		inline Mat3 operator *(const Mat3& n) {
			Mat3 result;
			float sum = 0.0f;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 3; k++) {
						sum += m[i][k] * n.m[k][j];
					}
					result.m[i][j] = sum;
					sum = 0.0f;
				}
			}
			return result;
		}

		inline Mat3 operator+(const Mat3& n) {
			Mat3 result;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					result.m[i][j] = m[i][j] + n.m[i][j];
				}
			}
			return result;
		}

		inline Mat3 operator-(const Mat3& n) {
			Mat3 result;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					result.m[i][j] = m[i][j] - n.m[i][j];
				}
			}
			return result;
		}

		inline Vec3 operator*(const Vec3& v) {
			return Vec3(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
			);
		}

		//Get Matrix component
		inline float operator () (int r, int c) const {
			return m[r][c];
		}

		//Sets Matrix component
		inline float& operator() (int r, int c) {
			return m[r][c];
		}
		
		//Static Member Methods
		inline static Mat3 Translate(float x, float y, float z) {
			Mat3 mat;
			mat(0, 2) = x;
			mat(1, 2) = y;
			mat(2, 2) = z;
			return mat;
		}
		
		inline static Mat3 Translate(Vec3 translation) {
			Mat3 mat; 
			mat(0, 2) = translation.x;
			mat(1, 2) = translation.y;
			mat(2, 2) = translation.z;
		}

		inline static Mat3 Scale(float x, float y, float z) {
			Mat3 mat;
			mat(0, 0) = x;
			mat(1, 1) = y;
			mat(2, 2) = z;
			return mat;
		}

		inline static Mat3 Inverse(const Mat3& translation) {
			float determinant = 0;
			Mat3 inverse;
			for (int i = 0; i < 3; i++)
				determinant += translation(0, i) * (translation(1, (i + 1) % 3) * translation(2, (i + 2) % 3) - translation(1, (i + 2) % 3) * translation(2, (i + 1) % 3));

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					inverse(i, j) = ((translation((j + 1) % 3, (i + 1) % 3) * translation((j + 2) % 3, (i + 2) % 3)) - (translation((j + 1) % 3, (i + 2) % 3) * translation((j + 2) % 3, (i + 1) % 3))) / determinant;
				}
			}
			return inverse;
		}

		inline void LoadIdentity() {
			m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
			m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
			m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
		}

		inline Vec3 ToVec3() {
			return Vec3(m[0][0], m[1][0], m[2][0]);
		}

		inline void Print() {
			std::cout <<
				m[0][0] << "\t" << m[0][1] << "\t" << m[0][2] << "\n" <<
				m[1][0] << "\t" << m[1][1] << "\t" << m[1][2] << "\n" <<
				m[2][0] << "\t" << m[2][1] << "\t" << m[2][2] << "\n" <<
				std::endl;
		}
	};
}

#endif // !MATRIX2_H