//
//  QYTMultiJitteredSampler.cpp
//  QYTRayTracer
//
//  Created by nocolor on 14-4-11.
//  Copyright (c) 2014年 QinYuTong. All rights reserved.
//

#include "QYTMultiJitteredSampler.h"
//#include "QYTGeometry.h"



void QYT::QYTMultiJitteredSampler::generateSamples()
{
    
    if(!squareSamples.empty())
        squareSamples.clear();
        
	int n = (int)sqrt((float)numOfSamples);
	float subcell_width = 1.0 / ((float) numOfSamples);
    
    static std::default_random_engine randomEngine;
	static std::uniform_real_distribution<float> u(0, subcell_width);
    
    QYT::QYTSampler::QYTPoint2d fill_point;
	for (int j = 0; j < numOfSamples * numOfSets; j++)
		squareSamples.push_back(fill_point);
    
	
	for (int p = 0; p < numOfSets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				squareSamples[i * n + j + p * numOfSamples].x = (i * n + j) * subcell_width + u(randomEngine);
				squareSamples[i * n + j + p * numOfSamples].y = (j * n + i) * subcell_width + u(randomEngine);
			}
	
	for (int p = 0; p < numOfSets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = QYT::QYTRandInt(j, n - 1);
				float t = squareSamples[i * n + j + p * numOfSamples].x;
				squareSamples[i * n + j + p * numOfSamples].x = squareSamples[i * n + k + p * numOfSamples].x;
				squareSamples[i * n + k + p * numOfSamples].y = t;
			}
    
	for (int p = 0; p < numOfSets; p++)
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = QYTRandInt(j, n - 1);
				float t = squareSamples[j * n + i + p * numOfSamples].y;
				squareSamples[j * n + i + p * numOfSamples].y = squareSamples[k * n + i + p * numOfSamples].y;
				squareSamples[k * n + i + p * numOfSamples].y = t;
            }
}