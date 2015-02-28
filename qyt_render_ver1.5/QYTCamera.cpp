//
//  QYTCamera.cpp
//  QYTRayTracer
//
//  Created by nocolor on 14-4-14.
//  Copyright (c) 2014年 QinYuTong. All rights reserved.
//

#include "QYTCamera.h"


QYT::QYTCamera::QYTCamera():
eye(0, 0, 500),
lookAt(0),
ra(0),
up(0, 1, 0),
u(1, 0, 0),
v(0, 1, 0),
w(0, 0, 1)
{
}

QYT::QYTCamera::QYTCamera(const QYT::QYTCamera& rhs):
eye(rhs.eye),
lookAt(rhs.lookAt),
ra(rhs.ra),
up(rhs.up),
u(rhs.u),
v(rhs.v),
w(rhs.w)
{
}

QYT::QYTCamera& QYT::QYTCamera::operator=(const QYT::QYTCamera &rhs)
{
    if (this == &rhs)
		return (*this);
	
	eye				= rhs.eye;
	lookAt			= rhs.lookAt;
	ra				= rhs.ra;
	up				= rhs.up;
	u				= rhs.u;
	v				= rhs.v;
	w				= rhs.w;
    
	return (*this);
}

QYT::QYTCamera::~QYTCamera()
{}

void QYT::QYTCamera::compute_uvw()
{
    w = eye - lookAt;
    w = QYT::QYTVector3::Normalize(w);
    u = QYT::QYTVector3::Cross(up, w);
    u = QYT::QYTVector3::Normalize(u);
    v = QYT::QYTVector3::Cross(w, u);
    
    if(eye.x == lookAt.x && eye.z == lookAt.z && eye.y > lookAt.y)
    {
        u = QYT::QYTVec3(0, 0, 1);
        v = QYT::QYTVec3(1, 0, 0);
        w = QYT::QYTVec3(0, 1, 0);
    }
    
    if(eye.x == lookAt.x && eye.z == lookAt.z && eye.y < lookAt.y)
    {
        u = QYT::QYTVec3(0, 0, 1);
        v = QYT::QYTVec3(1, 0, 0);
        w = QYT::QYTVec3(0, -1, 0);
    }
}