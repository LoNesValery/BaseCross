/*!
@file BaseMathInc.h
@brief XMMath���g�����߂̔������C���[(include�t�@k��)
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

#include "stdafx.h"
#include "BaseMath.h"

namespace basecross {
namespace bsm {

	//--------------------------------------------------------------------------------------
	///	Flt2�C�����C������
	//--------------------------------------------------------------------------------------
	inline Flt2::Flt2(const Flt2 & vec) :
		XMFLOAT2()
	{
		x = vec.x;
		y = vec.y;
	}

	inline Flt2::Flt2(float _x, float _y) :
		XMFLOAT2()
	{
		x = _x;
		y = _y;
	}

	inline Flt2::Flt2(float val) :
		XMFLOAT2()
	{
		x = val;
		y = val;
	}

	inline Flt2::Flt2(const XMVECTOR& vec) :
		XMFLOAT2()
	{
		XMVECTOR temp = vec;
		XMStoreFloat2(this, temp);
	}

	inline Flt2::operator XMVECTOR() const {
		XMFLOAT2 temp = *this;
		XMVECTOR Vec = XMLoadFloat2(&temp);
		return Vec;
	}

	inline Flt2 & Flt2::operator =(const Flt2 & other)
	{
		if (this != &other) {
			x = other.x;
			y = other.y;
		}
		return *this;
	}

	inline Flt2&  Flt2::operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat2(this, temp);
		return *this;
	}



	inline Flt2 & Flt2::setX(float _x)
	{
		x = _x;
		return *this;
	}


	inline Flt2 & Flt2::setY(float _y)
	{
		y = _y;
		return *this;
	}


	inline Flt2 & Flt2::setElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	inline float Flt2::getElem(int idx) const
	{
		return *(&x + idx);
	}

	inline float & Flt2::operator [](int idx)
	{
		return *(&x + idx);
	}

	inline float Flt2::operator [](int idx) const
	{
		return *(&x + idx);
	}

	inline const Flt2 Flt2::operator +(const Flt2 & vec) const
	{
		return Flt2(
			(x + vec.x),
			(y + vec.y)
		);
	}

	inline const Flt2 Flt2::operator -(const Flt2 & vec) const
	{
		return Flt2(
			(x - vec.x),
			(y - vec.y)
		);
	}

	inline const Flt2 Flt2::operator *(float val) const
	{
		return Flt2(
			(x * val),
			(y * val)
		);
	}

	inline Flt2 & Flt2::operator +=(const Flt2 & vec)
	{
		*this = *this + vec;
		return *this;
	}

	inline Flt2 & Flt2::operator -=(const Flt2 & vec)
	{
		*this = *this - vec;
		return *this;
	}

	inline Flt2 & Flt2::operator *=(float val)
	{
		*this = *this * val;
		return *this;
	}

	inline const Flt2 Flt2::operator /(float val) const
	{
		return Flt2(
			(x / val),
			(y / val)
		);
	}

	inline Flt2 & Flt2::operator /=(float val)
	{
		*this = *this / val;
		return *this;
	}

	inline const Flt2 Flt2::operator -() const
	{
		return Flt2(
			-x,
			-y
		);
	}


	//--------------------------------------------------------------------------------------
	///	Flt3�C�����C������
	//--------------------------------------------------------------------------------------
	inline Flt3::Flt3(const Flt3 & vec) :
		XMFLOAT3()
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	inline Flt3::Flt3(float _x, float _y, float _z):
		XMFLOAT3()
	{
		x = _x;
		y = _y;
		z = _z;
	}

	inline Flt3::Flt3(float val):
		XMFLOAT3()
	{
		x = val;
		y = val;
		z = val;
	}

	inline Flt3::Flt3(const XMVECTOR& vec) :
		XMFLOAT3()
	{
		XMVECTOR temp = vec;
		XMStoreFloat3(this, temp);
	}

	inline Flt3::operator XMVECTOR() const {
		XMFLOAT3 temp = *this;
		XMVECTOR Vec = XMLoadFloat3(&temp);
		return Vec;
	}

	inline Flt3 & Flt3::operator =(const Flt3 & other)
	{
		if (this != &other) {
			x = other.x;
			y = other.y;
			z = other.z;
		}
		return *this;
	}

	inline Flt3&  Flt3::operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
		return *this;
	}



	inline Flt3 & Flt3::setX(float _x)
	{
		x = _x;
		return *this;
	}


	inline Flt3 & Flt3::setY(float _y)
	{
		y = _y;
		return *this;
	}


	inline Flt3 & Flt3::setZ(float _z)
	{
		z = _z;
		return *this;
	}


	inline Flt3 & Flt3::setElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	inline float Flt3::getElem(int idx) const
	{
		return *(&x + idx);
	}

	inline float & Flt3::operator [](int idx)
	{
		return *(&x + idx);
	}

	inline float Flt3::operator [](int idx) const
	{
		return *(&x + idx);
	}

	inline const Flt3 Flt3::operator +(const Flt3 & vec) const
	{
		return Flt3(
			(x + vec.x),
			(y + vec.y),
			(z + vec.z)
		);
	}

	inline const Flt3 Flt3::operator -(const Flt3 & vec) const
	{
		return Flt3(
			(x - vec.x),
			(y - vec.y),
			(z - vec.z)
		);
	}

	inline const Flt3 Flt3::operator *(float val) const
	{
		return Flt3(
			(x * val),
			(y * val),
			(z * val)
		);
	}

	inline Flt3 & Flt3::operator +=(const Flt3 & vec)
	{
		*this = *this + vec;
		return *this;
	}

	inline Flt3 & Flt3::operator -=(const Flt3 & vec)
	{
		*this = *this - vec;
		return *this;
	}

	inline Flt3 & Flt3::operator *=(float val)
	{
		*this = *this * val;
		return *this;
	}

	inline const Flt3 Flt3::operator /(float val) const
	{
		return Flt3(
			(x / val),
			(y / val),
			(z / val)
		);
	}

	inline Flt3 & Flt3::operator /=(float val)
	{
		*this = *this / val;
		return *this;
	}

	inline const Flt3 Flt3::operator -() const
	{
		return Flt3(
			-x,
			-y,
			-z
		);
	}

	//--------------------------------------------------------------------------------------
	///	Flt4�C�����C������
	//--------------------------------------------------------------------------------------
	inline Flt4::Flt4(const Flt4 & vec) :
		XMFLOAT4()
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	inline Flt4::Flt4(float _x, float _y, float _z, float _w) :
		XMFLOAT4()
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	inline Flt4::Flt4(float val) :
		XMFLOAT4()
	{
		x = val;
		y = val;
		z = val;
		w = val;
	}

	inline Flt4::Flt4(const XMVECTOR& vec) :
		XMFLOAT4()
	{
		XMVECTOR temp = vec;
		XMStoreFloat4(this, temp);
	}

	inline Flt4::operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}

	inline Flt4::operator Flt3() const {
		return Flt3(x, y, z);
	}


	inline Flt4 & Flt4::operator =(const Flt4 & other)
	{
		if (this != &other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}

	inline Flt4&  Flt4::operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat4(this, temp);
		return *this;
	}



	inline Flt4 & Flt4::setX(float _x)
	{
		x = _x;
		return *this;
	}


	inline Flt4 & Flt4::setY(float _y)
	{
		y = _y;
		return *this;
	}


	inline Flt4 & Flt4::setZ(float _z)
	{
		z = _z;
		return *this;
	}

	inline Flt4 & Flt4::setW(float _w)
	{
		w = _w;
		return *this;
	}

	inline Flt4 & Flt4::setElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	inline float Flt4::getElem(int idx) const
	{
		return *(&x + idx);
	}

	inline float & Flt4::operator [](int idx)
	{
		return *(&x + idx);
	}

	inline float Flt4::operator [](int idx) const
	{
		return *(&x + idx);
	}

	inline const Flt4 Flt4::operator +(const Flt4 & vec) const
	{
		return Flt4(
			(x + vec.x),
			(y + vec.y),
			(z + vec.z),
			(w + vec.w)
		);
	}

	inline const Flt4 Flt4::operator -(const Flt4 & vec) const
	{
		return Flt4(
			(x - vec.x),
			(y - vec.y),
			(z - vec.z),
			(w - vec.w)
		);
	}

	inline const Flt4 Flt4::operator *(float val) const
	{
		return Flt4(
			(x * val),
			(y * val),
			(z * val),
			(w * val)
		);
	}

	inline Flt4 & Flt4::operator +=(const Flt4 & vec)
	{
		*this = *this + vec;
		return *this;
	}

	inline Flt4 & Flt4::operator -=(const Flt4 & vec)
	{
		*this = *this - vec;
		return *this;
	}

	inline Flt4 & Flt4::operator *=(float val)
	{
		*this = *this * val;
		return *this;
	}

	inline const Flt4 Flt4::operator /(float val) const
	{
		return Flt4(
			(x / val),
			(y / val),
			(z / val),
			(w / val)
		);
	}

	inline Flt4 & Flt4::operator /=(float val)
	{
		*this = *this / val;
		return *this;
	}

	inline const Flt4 Flt4::operator -() const
	{
		return Flt4(
			-x,
			-y,
			-z,
			-w
		);
	}


	//--------------------------------------------------------------------------------------
	///	Quat�C�����C���֐�
	//--------------------------------------------------------------------------------------
	inline Quat::Quat(const Quat & quat):
		XMFLOAT4()
	{
		x = quat.x;
		y = quat.y;
		z = quat.z;
		w = quat.w;
	}

	inline Quat::Quat(float _x, float _y, float _z, float _w):
		XMFLOAT4()
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	inline Quat::Quat(const Flt3 & xyz, float _w):
		XMFLOAT4()
	{
		this->setXYZ(xyz);
		this->setW(_w);
	}

	inline Quat::Quat(float val):
		XMFLOAT4()
	{
		x = val;
		y = val;
		z = val;
		w = val;
	}

	inline Quat::Quat(const XMVECTOR& other) :
		XMFLOAT4() 
	{
		XMVECTOR temp = other;
		XMStoreFloat4(this, temp);
	}

	inline Quat::operator XMVECTOR() const {
		XMFLOAT4 temp = *this;
		XMVECTOR Vec = XMLoadFloat4(&temp);
		return Vec;
	}

	inline const Quat Quat::identity()
	{
		return (Quat)XMQuaternionIdentity();
	}

	inline Quat& Quat::operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat4(this, temp);
		return *this;
	}

	inline Quat & Quat::operator =(const Quat & quat)
	{
		if (this != &quat) {
			x = quat.x;
			y = quat.y;
			z = quat.z;
			w = quat.w;
		}
		return *this;
	}

	inline Quat & Quat::setXYZ(const Flt3 & vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	inline const Flt3 Quat::getXYZ() const
	{
		return Flt3(x, y, z);
	}

	inline Quat & Quat::setX(float _x)
	{
		x = _x;
		return *this;
	}

	inline Quat & Quat::setY(float _y)
	{
		y = _y;
		return *this;
	}

	inline Quat & Quat::setZ(float _z)
	{
		z = _z;
		return *this;
	}

	inline Quat & Quat::setW(float _w)
	{
		w = _w;
		return *this;
	}

	inline Quat & Quat::setElem(int idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	inline float Quat::getElem(int idx) const
	{
		return *(&x + idx);
	}

	inline float & Quat::operator [](int idx)
	{
		return *(&x + idx);
	}

	inline float Quat::operator [](int idx) const
	{
		return *(&x + idx);
	}

	inline const Quat Quat::operator +(const Quat & quat) const
	{
		return Quat(
			(x + quat.x),
			(y + quat.y),
			(z + quat.z),
			(w + quat.w)
		);
	}

	inline const Quat Quat::operator -(const Quat & quat) const
	{
		return Quat(
			(x - quat.x),
			(y - quat.y),
			(z - quat.z),
			(w - quat.w)
		);
	}

	inline const Quat Quat::operator *(const Quat & quat) const
	{
		return (Quat)XMQuaternionMultiply(quat, *this);
	}

	inline const Quat Quat::operator *(float val) const
	{
		return Quat(
			(x * val),
			(y * val),
			(z * val),
			(w * val)
		);
	}


	inline const Quat Quat::rotationX(float radians)
	{
		return (Quat)XMQuaternionRotationAxis(XMVECTOR(Flt3(1,0,0)), radians);
	}

	inline const Quat Quat::rotationY(float radians)
	{
		return (Quat)XMQuaternionRotationAxis(Flt3(0, 1, 0), radians);
	}

	inline const Quat Quat::rotationZ(float radians)
	{
		return (Quat)XMQuaternionRotationAxis(Flt3(0, 0, 1), radians);
	}

	inline const Quat Quat::rotation(float radians, const Flt3 & unitVec) {
		return (Quat)XMQuaternionRotationAxis(unitVec, radians);
	}


	//--------------------------------------------------------------------------------------
	///	Mat3x3�C�����C���֐�
	//--------------------------------------------------------------------------------------
	inline Mat3x3::Mat3x3(const Mat3x3 & mat):
		XMFLOAT3X3(mat)
	{
	}

	inline Mat3x3::Mat3x3(float val):
		XMFLOAT3X3()
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m[i][j] = val;
			}
		}
	}

	inline Mat3x3::Mat3x3(const XMMATRIX& other):
		XMFLOAT3X3()
	{
		XMMATRIX temp = other;
		XMStoreFloat3x3(this, temp);
	}

	inline  Mat3x3::operator XMMATRIX() const {
		Mat3x3 temp = *this;
		XMMATRIX m = XMLoadFloat3x3(&temp);
		return m;
	}

	inline Mat3x3& Mat3x3::operator=(const XMMATRIX& other) {
		XMMATRIX temp = other;
		XMStoreFloat3x3(this, temp);
		return *this;
	}


	inline Mat3x3::Mat3x3(const Quat & unitQuat):
		XMFLOAT3X3()
	{
		*this = (Mat3x3)XMMatrixRotationQuaternion(unitQuat);
	}

	inline Mat3x3::Mat3x3(const Flt3 & _major0, const Flt3 & _major1, const Flt3 & _major2):
		XMFLOAT3X3()
	{
		setMajor(0, _major0);
		setMajor(1, _major1);
		setMajor(2, _major2);
	}

	inline Mat3x3 & Mat3x3::setMajor0(const Flt3 & _major0)
	{
		setMajor(0, _major0);
		return *this;
	}

	inline Mat3x3 & Mat3x3::setMajor1(const Flt3 & _major1)
	{
		setMajor(1, _major1);
		return *this;
	}

	inline Mat3x3 & Mat3x3::setMajor2(const Flt3 & _major2)
	{
		setMajor(2, _major2);
		return *this;
	}

	inline Mat3x3 & Mat3x3::setMajor(int major, const Flt3 & vec)
	{
		m[major][0] = vec.x;
		m[major][1] = vec.y;
		m[major][2] = vec.z;
		return *this;
	}

	inline Mat3x3 & Mat3x3::setMinor(int minor, const Flt3 & vec)
	{
		setElem(0, minor, vec.getElem(0));
		setElem(1, minor, vec.getElem(1));
		setElem(2, minor, vec.getElem(2));
		return *this;
	}

	inline Mat3x3 & Mat3x3::setElem(int major, int minor, float val)
	{
		m[major][minor] = val;
		return *this;
	}

	inline float Mat3x3::getElem(int major, int minor) const
	{
		return this->getMajor(major).getElem(minor);
	}

	inline const Flt3 Mat3x3::getMajor0() const
	{
		return getMajor(0);
	}

	inline const Flt3 Mat3x3::getMajor1() const
	{
		return getMajor(1);
	}

	inline const Flt3 Mat3x3::getMajor2() const
	{
		return getMajor(2);
	}

	inline const Flt3 Mat3x3::getMajor(int major) const
	{
		Flt3 temp(m[major][0], m[major][1], m[major][2]);
		return temp;
	}

	inline const Flt3 Mat3x3::getMinor(int minor) const
	{
		return Flt3(m[0][minor], m[1][minor], m[2][minor]);
	}

	inline Flt3 & Mat3x3::operator [](int major)
	{
		return (Flt3&)(m[major][0]);
	}

	inline const Flt3 Mat3x3::operator [](int major) const
	{
		return (Flt3&)(m[major][0]);
	}

	inline Mat3x3 & Mat3x3::operator =(const Mat3x3& mat)
	{
		if (this != &mat) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					m[i][j] = mat[i][j];
				}
			}
		}
		return *this;
	}

	inline const Mat3x3 Mat3x3::operator +(const Mat3x3 & mat) const
	{
		Mat3x3 temp(*this);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				temp.m[i][j] += mat[i][j];
			}
		}
		return temp;
	}

	inline const Mat3x3 Mat3x3::operator -(const Mat3x3 & mat) const
	{
		Mat3x3 temp(*this);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				temp.m[i][j] -= mat[i][j];
			}
		}
		return temp;
	}

	inline const Mat3x3 Mat3x3::operator -() const
	{
		Mat3x3 temp;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				temp.m[i][j] = -m[i][j];
			}
		}
		return temp;
	}

	inline const Mat3x3 Mat3x3::operator *(float val) const
	{
		Mat3x3 temp(*this);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				temp.m[i][j] *= val;
			}
		}
		return temp;
	}

	inline const Flt3 Mat3x3::operator *(const Flt3 & vec) const
	{
		return (Flt3)XMVector3Transform(vec, *this);
	}


	inline const Mat3x3 Mat3x3::operator *(const Mat3x3 & mat) const
	{
		return  (Mat3x3)XMMatrixMultiply(mat,*this);
	}



	inline Mat3x3 & Mat3x3::operator +=(const Mat3x3 & mat)
	{
		*this = *this + mat;
		return *this;
	}

	inline Mat3x3 & Mat3x3::operator -=(const Mat3x3 & mat)
	{
		*this = *this - mat;
		return *this;
	}


	inline const Mat3x3 Mat3x3::identity()
	{
		return (Mat3x3)XMMatrixIdentity();
	}

	inline const Mat3x3 Mat3x3::scale(const Flt3 & scaleVec)
	{
		return  (Mat3x3)XMMatrixScalingFromVector(scaleVec);
	}

	//--------------------------------------------------------------------------------------
	///	Mat4x4�C�����C���֐�
	//--------------------------------------------------------------------------------------
	inline Mat4x4::Mat4x4(const Mat4x4 & mat) :
		XMFLOAT4X4(mat)
	{
	}

	inline Mat4x4::Mat4x4(const Mat3x3 & mat) :
		XMFLOAT4X4()
	{
		*this = Mat4x4::identity();
		this->setUpper3x3(mat);
	}


	inline Mat4x4::Mat4x4(const Flt3 & _major0, const Flt3 & _major1, const Flt3 & _major2, const Flt3& _major3) :
		XMFLOAT4X4()
	{
		*this = Mat4x4::identity();
		setMajor(0, _major0);
		setMajor(1, _major1);
		setMajor(2, _major2);
		setMajor(3, _major3);
	}

	inline Mat4x4::Mat4x4(const Flt4 & _major0, const Flt4 & _major1, const Flt4 & _major2, const Flt4& _major3) :
		XMFLOAT4X4()
	{
		*this = Mat4x4::identity();
		setMajor(0, _major0);
		setMajor(1, _major1);
		setMajor(2, _major2);
		setMajor(3, _major3);
	}

	inline Mat4x4::Mat4x4(const Mat3x3 & tfrm, const Flt3 & translateVec)
	{
		*this = Mat4x4::identity();
		this->setUpper3x3(tfrm);
		this->setTranslation(translateVec);
	}

	inline Mat4x4::Mat4x4(const Quat & unitQuat, const Flt3 & translateVec)
	{
		*this = Mat4x4::identity();
		this->setUpper3x3(Mat3x3(unitQuat));
		this->setTranslation(translateVec);
	}


	inline Mat4x4::Mat4x4(float val) :
		XMFLOAT4X4()
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[i][j] = val;
			}
		}
	}

	inline Mat4x4::Mat4x4(const XMMATRIX& other) :
		XMFLOAT4X4()
	{
		XMMATRIX temp = other;
		XMStoreFloat4x4(this, temp);
	}

	inline  Mat4x4::operator XMMATRIX() const {
		Mat4x4 temp = *this;
		XMMATRIX m = XMLoadFloat4x4(&temp);
		return m;
	}

	inline Mat4x4& Mat4x4::operator=(const XMMATRIX& other) {
		XMMATRIX temp = other;
		XMStoreFloat4x4(this, temp);
		return *this;
	}


	inline Mat4x4::Mat4x4(const Quat & unitQuat) :
		XMFLOAT4X4()
	{
		*this = (Mat4x4)XMMatrixRotationQuaternion(XMVECTOR(unitQuat));
	}

	inline Mat4x4 & Mat4x4::setUpper3x3(const Mat3x3 & tfrm)
	{
		this->operator[](0) = tfrm.getMajor0();
		this->operator[](1) = tfrm.getMajor1();
		this->operator[](2) = tfrm.getMajor2();
		return *this;
	}

	inline const Mat3x3 Mat4x4::getUpper3x3() const
	{
		return Mat3x3(getMajor0(), getMajor1(), getMajor2());
	}

	inline Mat4x4 & Mat4x4::setTranslation(const Flt3 & translateVec)
	{
		this->operator[](3) = translateVec;
		m[3][3] = 1.0f;
		return *this;
	}

	inline const Flt3 Mat4x4::getTranslation() const
	{
		return (Flt3)getMajor3();
	}
	inline Mat4x4 & Mat4x4::setMajor0(const Flt3 & _major0)
	{
		setMajor(0, _major0);
		m[0][3] = 0.0f;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor1(const Flt3 & _major1)
	{
		setMajor(1, _major1);
		m[1][3] = 0.0f;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor2(const Flt3 & _major2)
	{
		setMajor(2, _major2);
		m[2][3] = 0.0f;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor3(const Flt3 & _major3)
	{
		setMajor(3, _major3);
		m[3][3] = 1.0f;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor0(const Flt4 & _major0)
	{
		setMajor(0, _major0);
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor1(const Flt4 & _major1)
	{
		setMajor(1, _major1);
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor2(const Flt4 & _major2)
	{
		setMajor(2, _major2);
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor3(const Flt4 & _major3)
	{
		setMajor(3, _major3);
		return *this;
	}


	inline Mat4x4 & Mat4x4::setMajor(int major, const Flt3 & vec)
	{
		m[major][0] = vec.x;
		m[major][1] = vec.y;
		m[major][2] = vec.z;
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMajor(int major, const Flt4 & vec)
	{
		m[major][0] = vec.x;
		m[major][1] = vec.y;
		m[major][2] = vec.z;
		m[major][3] = vec.w;
		return *this;
	}


	inline Mat4x4 & Mat4x4::setMinor(int minor, const Flt3 & vec)
	{
		setElem(0, minor, vec.getElem(0));
		setElem(1, minor, vec.getElem(1));
		setElem(2, minor, vec.getElem(2));
		return *this;
	}

	inline Mat4x4 & Mat4x4::setMinor(int minor, const Flt4 & vec)
	{
		setElem(0, minor, vec.getElem(0));
		setElem(1, minor, vec.getElem(1));
		setElem(2, minor, vec.getElem(2));
		setElem(3, minor, vec.getElem(3));
		return *this;
	}
	inline Mat4x4 & Mat4x4::setElem(int major, int minor, float val)
	{
		m[major][minor] = val;
		return *this;
	}

	inline float Mat4x4::getElem(int major, int minor) const
	{
		return this->getMajor(major).getElem(minor);
	}

	inline const Flt4 Mat4x4::getMajor0() const
	{
		return getMajor(0);
	}

	inline const Flt4 Mat4x4::getMajor1() const
	{
		return getMajor(1);
	}

	inline const Flt4 Mat4x4::getMajor2() const
	{
		return getMajor(2);
	}

	inline const Flt4 Mat4x4::getMajor3() const
	{
		return getMajor(3);
	}


	inline const Flt4 Mat4x4::getMajor(int major) const
	{
		Flt4 temp(m[major][0], m[major][1], m[major][2], m[major][3]);
		return temp;
	}

	inline const Flt4 Mat4x4::getMinor(int minor) const
	{
		return Flt4(m[0][minor], m[1][minor], m[2][minor], m[3][minor]);
	}

	inline Flt4 & Mat4x4::operator [](int major)
	{
		return (Flt4&)(m[major][0]);
	}

	inline const Flt4 Mat4x4::operator [](int major) const
	{
		return (Flt4&)(m[major][0]);
	}

	inline Mat4x4 & Mat4x4::operator =(const Mat4x4& mat)
	{
		if (this != &mat) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m[i][j] = mat[i][j];
				}
			}
		}
		return *this;
	}


	inline const Mat4x4 Mat4x4::operator +(const Mat4x4 & mat) const
	{
		Mat4x4 temp(*this);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				temp.m[i][j] += mat[i][j];
			}
		}
		return temp;
	}

	inline const Mat4x4 Mat4x4::operator -(const Mat4x4 & mat) const
	{
		Mat4x4 temp(*this);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				temp.m[i][j] -= mat[i][j];
			}
		}
		return temp;
	}

	inline const Mat4x4 Mat4x4::operator -() const
	{
		Mat4x4 temp;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				temp.m[i][j] = -m[i][j];
			}
		}
		return temp;
	}

	inline const Mat4x4 Mat4x4::operator *(float val) const
	{
		Mat4x4 temp(*this);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				temp.m[i][j] *= val;
			}
		}
		return temp;
	}

	inline const Flt3 Mat4x4::operator *(const Flt3 & vec) const
	{
		return (Flt3)XMVector4Transform(vec, *this);
	}

	inline const Flt4 Mat4x4::operator *(const Flt4 & vec) const
	{
		return (Flt4)XMVector4Transform(vec, *this);
	}



	inline const Mat4x4 Mat4x4::operator *(const Mat4x4 & mat) const
	{
		return  (Mat4x4)XMMatrixMultiply(mat, *this);
	}

	inline const Mat4x4 Mat4x4::operator *(const Mat3x3 & mat) const
	{
		return  (Mat4x4)XMMatrixMultiply(mat,*this);
	}


	inline Mat4x4 & Mat4x4::operator +=(const Mat4x4 & mat)
	{
		*this = *this + mat;
		return *this;
	}

	inline Mat4x4 & Mat4x4::operator -=(const Mat4x4 & mat)
	{
		*this = *this - mat;
		return *this;
	}

	inline Mat4x4 & Mat4x4::operator *=(const Mat4x4 & tfrm)
	{
		*this = *this * tfrm;
		return *this;
	}

	inline Mat4x4 & Mat4x4::operator *=(const Mat3x3 & tfrm)
	{
		*this = *this * tfrm;
		return *this;
	}

	inline const Mat4x4 Mat4x4::identity()
	{
		return (Mat4x4)XMMatrixIdentity();
	}

	inline const Mat4x4 Mat4x4::scale(const Flt3 & scaleVec)
	{
		return  (Mat4x4)XMMatrixScalingFromVector(scaleVec);
	}

	inline const Mat4x4 Mat4x4::rotation(const Quat & unitQuat)
	{
		return Mat4x4(Mat3x3(unitQuat), Flt3(0.0f));
	}

	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QFlt2
	//--------------------------------------------------------------------------------------
	inline const Flt2 operator *(float val, const Flt2 & vec)
	{
		return vec * val;
	}
	inline const Flt2 operator /(float val, const Flt2 & vec)
	{
		return vec / val;
	}


	inline const Flt2 mulPerElem(const Flt2 & vec0, const Flt2 & vec1)
	{
		return Flt2(
			(vec0.x * vec1.x),
			(vec0.y * vec1.y)
		);
	}

	inline const Flt2 divPerElem(const Flt2 & vec0, const Flt2 & vec1)
	{
		return Flt2(
			(vec0.x / vec1.x),
			(vec0.y / vec1.y)
		);
	}


	inline const Flt2 absPerElem(const Flt2 & vec)
	{
		return Flt2(
			fabsf(vec.x),
			fabsf(vec.y)
		);
	}

	inline const Flt2 maxPerElem(const Flt2 & vec0, const Flt2 & vec1)
	{
		return Flt2(
			(vec0.x > vec1.x) ? vec0.x : vec1.x,
			(vec0.y > vec1.y) ? vec0.y : vec1.y
		);
	}

	inline const Flt2 minPerElem(const Flt2 & vec0, const Flt2 & vec1)
	{
		return Flt2(
			(vec0.x < vec1.x) ? vec0.x : vec1.x,
			(vec0.y < vec1.y) ? vec0.y : vec1.y
		);
	}

	inline float dot(const Flt2 & vec0, const Flt2 & vec1)
	{
		return ((Flt2)XMVector2Dot(vec0, vec1)).x;
	}

	inline float lengthSqr(const Flt2 & vec)
	{
		return ((Flt2)XMVector2LengthSq(vec)).x;
	}

	inline float length(const Flt2 & vec)
	{
		return ((Flt2)XMVector2Length(vec)).x;
	}

	inline const Flt2 normalize(const Flt2 & vec)
	{
		return (Flt2)XMVector2Normalize(vec);
	}


	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QFlt3
	//--------------------------------------------------------------------------------------
	inline const Flt3 operator *(float val, const Flt3 & vec)
	{
		return vec * val;
	}
	inline const Flt3 operator /(float val, const Flt3 & vec)
	{
		return vec / val;
	}


	inline const Flt3 mulPerElem(const Flt3 & vec0, const Flt3 & vec1)
	{
		return Flt3(
			(vec0.x * vec1.x),
			(vec0.y * vec1.y),
			(vec0.z * vec1.z)
		);
	}

	inline const Flt3 divPerElem(const Flt3 & vec0, const Flt3 & vec1)
	{
		return Flt3(
			(vec0.x / vec1.x),
			(vec0.y / vec1.y),
			(vec0.z / vec1.z)
		);
	}


	inline const Flt3 absPerElem(const Flt3 & vec)
	{
		return Flt3(
			fabsf(vec.x),
			fabsf(vec.y),
			fabsf(vec.z)
		);
	}

	inline const Flt3 maxPerElem(const Flt3 & vec0, const Flt3 & vec1)
	{
		return Flt3(
			(vec0.x > vec1.x) ? vec0.x : vec1.x,
			(vec0.y > vec1.y) ? vec0.y : vec1.y,
			(vec0.z > vec1.z) ? vec0.z : vec1.z
		);
	}

	inline const Flt3 minPerElem(const Flt3 & vec0, const Flt3 & vec1)
	{
		return Flt3(
			(vec0.x < vec1.x) ? vec0.x : vec1.x,
			(vec0.y < vec1.y) ? vec0.y : vec1.y,
			(vec0.z < vec1.z) ? vec0.z : vec1.z
		);
	}

	inline float dot(const Flt3 & vec0, const Flt3 & vec1)
	{
		return ((Flt3)XMVector3Dot(vec0, vec1)).x;
	}

	inline float lengthSqr(const Flt3 & vec)
	{
		return ((Flt3)XMVector3LengthSq(vec)).x;
	}

	inline float length(const Flt3 & vec)
	{
		return ((Flt3)XMVector3Length(vec)).x;
	}

	inline const Flt3 normalize(const Flt3 & vec)
	{
		return (Flt3)XMVector3Normalize(vec);
	}

	inline const Flt3 cross(const Flt3 & vec0, const Flt3 & vec1)
	{
		return (Flt3)XMVector3Cross(vec0, vec1);
	}

	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QFlt4
	//--------------------------------------------------------------------------------------
	inline const Flt4 operator *(float val, const Flt4 & vec)
	{
		return vec * val;
	}
	inline const Flt4 operator /(float val, const Flt4 & vec)
	{
		return vec / val;
	}


	inline const Flt4 mulPerElem(const Flt4 & vec0, const Flt4 & vec1)
	{
		return Flt4(
			(vec0.x * vec1.x),
			(vec0.y * vec1.y),
			(vec0.z * vec1.z),
			(vec0.w * vec1.w)
		);
	}

	inline const Flt4 divPerElem(const Flt4 & vec0, const Flt4 & vec1)
	{
		return Flt4(
			(vec0.x / vec1.x),
			(vec0.y / vec1.y),
			(vec0.z / vec1.z),
			(vec0.w / vec1.w)
		);
	}


	inline const Flt4 absPerElem(const Flt4 & vec)
	{
		return Flt4(
			fabsf(vec.x),
			fabsf(vec.y),
			fabsf(vec.z),
			fabsf(vec.w)
		);
	}

	inline const Flt4 maxPerElem(const Flt4 & vec0, const Flt4 & vec1)
	{
		return Flt4(
			(vec0.x > vec1.x) ? vec0.x : vec1.x,
			(vec0.y > vec1.y) ? vec0.y : vec1.y,
			(vec0.z > vec1.z) ? vec0.z : vec1.z,
			(vec0.w > vec1.w) ? vec0.w : vec1.w
		);
	}

	inline const Flt4 minPerElem(const Flt4 & vec0, const Flt4 & vec1)
	{
		return Flt4(
			(vec0.x < vec1.x) ? vec0.x : vec1.x,
			(vec0.y < vec1.y) ? vec0.y : vec1.y,
			(vec0.z < vec1.z) ? vec0.z : vec1.z,
			(vec0.w < vec1.w) ? vec0.w : vec1.w
		);
	}

	inline float dot(const Flt4 & vec0, const Flt4 & vec1)
	{
		return ((Flt4)XMVector4Dot(vec0, vec1)).x;
	}

	inline float lengthSqr(const Flt4 & vec)
	{
		return ((Flt4)XMVector4LengthSq(vec)).x;
	}

	inline float length(const Flt4 & vec)
	{
		return ((Flt4)XMVector4Length(vec)).x;
	}

	inline const Flt4 normalize(const Flt4 & vec)
	{
		return (Flt4)XMVector4Normalize(vec);
	}
	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QQuat�֘A
	//--------------------------------------------------------------------------------------
	inline const Quat normalize(const Quat & quat)
	{
		return (Quat)XMQuaternionNormalize(quat);
	}
	inline const Flt3 rotate(const Quat & quat, const Flt3 & vec)
	{
		return (Flt3)XMVector3Rotate(vec, quat);
	}

	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QMat3x3�֘A
	//--------------------------------------------------------------------------------------
	inline const Mat3x3 absPerElem(const Mat3x3 & mat)
	{
		return Mat3x3(
			absPerElem(mat.getMajor0()),
			absPerElem(mat.getMajor1()),
			absPerElem(mat.getMajor2())
		);
	}

	inline const Mat3x3 operator *(float val, const Mat3x3 & mat)
	{
		return mat * val;
	}

	inline const Mat3x3 transpose(const Mat3x3 & mat)
	{
		return  (Mat3x3)XMMatrixTranspose(mat);
	}

	inline const Mat3x3 inverse(const Mat3x3 & mat)
	{
		XMVECTOR Vec;
		return (Mat3x3)XMMatrixInverse(&Vec, mat);
	}

	inline const Mat3x3 crossMatrix(const Flt3 & vec)
	{
		return Mat3x3(
			Flt3(0.0f, vec.z, -vec.y),
			Flt3(-vec.z, 0.0f, vec.x),
			Flt3(vec.y, -vec.x, 0.0f)
		);
	}

	//--------------------------------------------------------------------------------------
	///	���[�e�B���e�B�֐��QMat4x4�֘A
	//--------------------------------------------------------------------------------------
	inline const Mat4x4 absPerElem(const Mat4x4 & mat)
	{
		return Mat4x4(
			absPerElem(mat.getMajor0()),
			absPerElem(mat.getMajor1()),
			absPerElem(mat.getMajor2()),
			absPerElem(mat.getMajor3())
		);
	}

	inline const Mat4x4 operator *(float val, const Mat4x4 & mat)
	{
		return mat * val;
	}

	inline const Mat4x4 transpose(const Mat4x4 & mat)
	{
		return  (Mat4x4)XMMatrixTranspose(mat);
	}

	inline const Mat4x4 inverse(const Mat4x4 & mat)
	{
		XMVECTOR Vec;
		return (Mat4x4)XMMatrixInverse(&Vec, mat);
	}


	typedef Flt2 Vec2;
	typedef Flt2 Pt2;
	typedef Flt3 Vec3;
	typedef Flt3 Pt3;
	typedef Flt4 Vec4;
	typedef Flt4 Col4;

}
// end bsm
}
//end basecross