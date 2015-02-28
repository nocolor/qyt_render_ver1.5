//
//  QYTSampler.cpp
//  QYTRayTracer
//
//  Created by nocolor on 14-4-11.
//  Copyright (c) 2014年 QinYuTong. All rights reserved.
//

#include "QYTSampler.h"
#include <algorithm>

void QYT::QYTSampler::setupShuffledIndices()
{
    if(!shuffledIndices.empty())
        shuffledIndices.clear();
    
    shuffledIndices.reserve(numOfSamples*numOfSets);
    std::vector<int> indices;
    
    for (int i = 0; i < numOfSamples; i++)
        indices.push_back(i);
    
    for (int i = 0; i < numOfSets; i++)
    {
        std::random_shuffle(indices.begin(), indices.end());
        
        for (int j = 0; j < numOfSamples; j++)
            shuffledIndices.push_back(indices[j]);
    }
}

void QYT::QYTSampler::mapSamplesToUnitDisk()
{
    if(!diskSamples.empty())
        diskSamples.clear();
    
    size_t size = squareSamples.size();
    
    float r, phi;
    
    QYTPoint2d sp;
    
    diskSamples.resize(size);
    
    for (int i = 0; i < size; i++)
    {
        sp.x = 2.0*squareSamples[i].x - 1.0;
        sp.y = 2.0*squareSamples[i].y - 1.0;
        
		if (sp.x > -sp.y) {			// sectors 1 and 2
			if (sp.x > sp.y) {		// sector 1
				r = sp.x;
				phi = sp.y / sp.x;
			}
			else {					// sector 2
				r = sp.y;
				phi = 2 - sp.x / sp.y;
			}
		}
		else {						// sectors 3 and 4
			if (sp.x < sp.y) {		// sector 3
				r = -sp.x;
				phi = 4 + sp.y / sp.x;
			}
			else {					// sector 4
				r = -sp.y;
				if (sp.y != 0.0)	// avoid division by zero at origin
					phi = 6 - sp.x / sp.y;
				else
					phi  = 0.0;
			}
		}
        phi *= M_PI / 4.0;
        
		diskSamples[i].x = r * cos(phi);
		diskSamples[i].y = r * sin(phi);
    }
}

void QYT::QYTSampler::mapSamplesToHemisphere(float e)
{
    if(!hemisphereSamples.empty())
        hemisphereSamples.clear();
    
    
    size_t size = squareSamples.size();
    hemisphereSamples.reserve(numOfSamples*numOfSets);
    
    for (int j = 0; j < size; j++) 
    {
		float cos_phi = cos(2.0 * M_PI * squareSamples[j].x);
		float sin_phi = sin(2.0 * M_PI * squareSamples[j].x);
		float cos_theta = pow((1.0 - squareSamples[j].y), 1.0 / (e + 1.0));
		float sin_theta = sqrt (1.0 - cos_theta * cos_theta);
		float pu = sin_theta * cos_phi;
		float pv = sin_theta * sin_phi;
		float pw = cos_theta;
		hemisphereSamples.push_back(QYTPoint3(pu, pv, pw));
	}
}

void QYT::QYTSampler::mapSamplesToSphere()
{
    if(!sphereSamples.empty())
        sphereSamples.clear();
    
    float r1, r2;
	float x, y, z;
	float r, phi;
    
	sphereSamples.reserve(numOfSamples * numOfSets);   
    
	for (int j = 0; j < numOfSamples * numOfSets; j++) {
		r1 	= squareSamples[j].x;
    	r2 	= squareSamples[j].y;
    	z 	= 1.0 - 2.0 * r1;
    	r 	= sqrt(1.0 - z * z);
    	phi = M_PI*2  * r2;
    	x 	= r * cos(phi);
    	y 	= r * sin(phi);
		sphereSamples.push_back(QYTPoint3(x, y, z));
	}
}

QYT::QYTSampler::QYTPoint2d QYT::QYTSampler::nextSampleInUnitSquare()
{
    if (countOfSamplesUsed % numOfSamples == 0) 
		jump = (rand() % numOfSets) * numOfSamples;
    
	return (squareSamples[jump + shuffledIndices[jump + countOfSamplesUsed++ % numOfSamples]]);  
}

QYT::QYTSampler::QYTPoint2d QYT::QYTSampler::nextSampleInUnitDisk()
{
    if(countOfSamplesUsed % numOfSamples == 0)
        jump = (rand() % numOfSets)*numOfSamples;
    return (diskSamples[jump + shuffledIndices[jump + countOfSamplesUsed++ % numOfSamples]]);
}

QYT::QYTPoint3 QYT::QYTSampler::nextSampleInHemishpere()
{
    if(countOfSamplesUsed % numOfSamples == 0)
        jump = (rand() % numOfSets)*numOfSamples;
    return (hemisphereSamples[jump + shuffledIndices[jump + countOfSamplesUsed++ % numOfSamples]]);

}

QYT::QYTPoint3 QYT::QYTSampler::nextSampleInSphere()
{
    if(countOfSamplesUsed % numOfSamples == 0)
        jump = (rand() % numOfSets)*numOfSamples;
    return (sphereSamples[jump + shuffledIndices[jump + countOfSamplesUsed++ % numOfSamples]]);

}