#pragma once

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <vector>
#include <ofVec2f.h>
#include <ofVec3f.h>
#include <iostream>
#include <string>

using namespace Eigen;
using namespace std;

typedef Matrix<float, Dynamic, Dynamic> matrixf;


class simplicialComplex
{

	// Rows are observations, columns are parameters. 
	// So this matrix should be tall and skinny
private: 
	matrixf vertices;
	matrixf stdVertices;
	matrixf distances;
	Matrix<float, Dynamic, 2> projection2;
	Matrix<float, Dynamic, 3> projection3;
	int numSamples;
	int numParameters;
	float curProxRadius;
	float curSimpRadius;
	bool distancesCalculated = false;

	vector<vector<vector<int>>> simplices; 
	//a vector is a simplex 
	//a vector of vectors is a bunch of simplices with the same size
	//a vector of vectors is all simplices ordered by size

	Matrix<bool, Dynamic, Dynamic> proximityGraph;

	void calcDistances();
	void calcSimplices(float radius);
	void calcProximityGraph(float radius);

	void printSimplices();


public:
	simplicialComplex();
	simplicialComplex(int rows, int cols);
	~simplicialComplex();
	vector<ofVec2f> get2d();
	vector<ofVec3f> get3d();
	Matrix<bool, Dynamic, Dynamic> getProximityGraph(float radius);
	vector<vector<vector<int>>> getSimplices(float radius);

};

