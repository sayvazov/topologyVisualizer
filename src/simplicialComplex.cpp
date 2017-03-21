#include "simplicialComplex.h"



simplicialComplex::simplicialComplex()
{
	int defNum = 350;
	int defDim = 2;
	vertices = matrixf::Random(defNum, defDim);
	numSamples = defNum;
	numParameters = defDim;
	get2d();
	calcDistances();
}

simplicialComplex::simplicialComplex(int rows, int cols)
{
	vertices = matrixf::Random(rows, cols);
	numSamples = rows;
	numParameters = cols;
}

simplicialComplex::~simplicialComplex()
{
}

vector<ofVec2f> simplicialComplex::get2d()
{
	// PCA for n samples of p parameters
	//Calculate mean
	auto means = vertices.colwise().mean();

	matrixf deviationFromMeans = vertices.rowwise() - means;
	auto stdDeviations = deviationFromMeans.colwise().squaredNorm() / numSamples;;
	for (int i = 0; i < deviationFromMeans.cols(); i++)
	{
		deviationFromMeans.col(i) /= sqrt(stdDeviations(i));
	}
	stdVertices = deviationFromMeans;
	matrixf C = deviationFromMeans.adjoint() * deviationFromMeans / double(numSamples); //rows is n

	SelfAdjointEigenSolver<matrixf> solver;
	solver.compute(C);
	//newBasis is a p by 2 matrix
	auto newBasis = solver.eigenvectors().leftCols(2);

	projection2 = vertices* newBasis;
	vector<ofVec2f> ofprojection;
	for (int i = 0; i < projection2.rows(); i++)
	{
		ofVec2f point(projection2(i, 0), projection2(i, 1));
		ofprojection.push_back(point);
	}
	return ofprojection;
}

vector<ofVec3f> simplicialComplex::get3d()
{

	// PCA for n samples of p parameters
	//Calculate mean
	auto means = vertices.colwise().mean();
	
	matrixf deviationFromMeans = vertices.rowwise() - means;
	auto stdDeviations = deviationFromMeans.colwise().squaredNorm() / numSamples;
	//auto stdDeviations = deviationFromMeans.dot(deviationFromMeans) / vertices.rows();


	for (int i = 0; i < deviationFromMeans.cols(); i++)
	{
		deviationFromMeans.col(i) /= sqrt(stdDeviations(i));
	}


	matrixf C = deviationFromMeans.adjoint() * deviationFromMeans / double(numSamples); //rows is n
	//C /= C(0, 0);
	//matrixf C = vertices.adjoint() * vertices / (vertices.rows() - 1);

	//matrixf centered = deviationFromMeans.rowwise() - deviationFromMeans.colwise().mean();
	//matrixf C = (centered.adjoint() * centered) / double(deviationFromMeans.rows() - 1);


	SelfAdjointEigenSolver<matrixf> solver;
	solver.compute(C);
	//newBasis is a p by 2 matrix
	auto newBasis = solver.eigenvectors().leftCols(3);

	projection3 = vertices* newBasis;
	vector<ofVec3f> ofprojection;
	for (int i = 0; i < projection3.rows(); i++)
	{
		ofVec3f point(projection3(i, 0), projection3(i, 1), projection3(i, 2));
		ofprojection.push_back(point);
	}
	return ofprojection;
}

void simplicialComplex::calcDistances()
{
	if (distancesCalculated)
	{
		return;
	}
	distances = matrixf::Zero(numSamples, numSamples);
	for (int i = 0; i < numSamples; i++)
	{
		for (int j = 0; j < i; j++)
		{
			float dist = (stdVertices.row(i) - stdVertices.row(j)).norm();
			distances(i, j) = dist;
			distances(j, i) = dist;
		}
	}
	distancesCalculated = true;
}

void simplicialComplex::calcSimplices(float radius)
{
	simplices.clear();

	//Initialize with points
	vector<vector<int>> points;
	for (int i = 0; i < numSamples; i++)
	{
		int singleton[1] = { i };
		points.push_back(vector<int>(singleton, singleton + 1));
	}
	simplices.push_back(points);

	// Try adding to existing simplices to get new ones
	while (simplices.back().size() > 0)
	{
		vector<vector<int>> nextSizeSimplices;
		vector<vector<int>> curSimplices = simplices.back();
		for (int indexToExpand = 0; indexToExpand < curSimplices.size(); indexToExpand++)
		{
			vector<int> simplexToExpand = curSimplices[indexToExpand];
			for (int newVert = 0; newVert < numSamples; newVert++)
			{
				bool add = true;
				for (int compareIndex = 0; compareIndex < simplexToExpand.size(); compareIndex++)
				{
					if (newVert == simplexToExpand[compareIndex])
					{
						add = false;
					}
					else if (!proximityGraph(newVert, simplexToExpand[compareIndex]))
					{
						add = false;
					}
				}
				if (add)
				{
					vector<int> newSimplex(simplexToExpand);
					newSimplex.push_back(newVert);
					//Check if newSimplex is a permutation of something already counted
					bool perm = false;
					for (auto checkSimplex = nextSizeSimplices.begin(); checkSimplex < nextSizeSimplices.end(); checkSimplex++)
					{
						if (is_permutation(newSimplex.begin(), newSimplex.end(), checkSimplex->begin(), checkSimplex->end()))
						{
							perm = true;
						}
					}
					if (!perm)
					{
						nextSizeSimplices.push_back(newSimplex);
					}
				}
			}
		}
		simplices.push_back(nextSizeSimplices);
		cout << "Calculating simplices of size " << simplices.size()  << endl;
	}

	//printSimplices();
}

void simplicialComplex::printSimplices()
{
	for (int i = 0; i < simplices.size(); i++)
	{
		cout << "Printing " << i << "-simplices" << endl;
		for (int j = 0; j < simplices[i].size(); j++)
		{
			for (int k = 0; k < simplices[i][j].size(); k++)
			{
				cout << simplices[i][j][k] << " ";
			}
			cout << endl;
		}
	}
}


 void simplicialComplex::calcProximityGraph(float radius)
{
	proximityGraph.resize(numSamples, numSamples);
	for (int i = 0; i < numSamples; i++)
	{
		for (int j = 0; j < i; j++)
		{
			bool val = distances(i, j) <= radius;
			proximityGraph(i, j) = val;
			proximityGraph(j, i) = val;
		}
	}
}

 Matrix<bool, Dynamic, Dynamic> simplicialComplex::getProximityGraph(float radius)
 {
	 if (abs(radius - curProxRadius) > 0.0001)
	 {
		 curProxRadius = radius;
		 calcProximityGraph(radius);
	 }
	 return proximityGraph;
 }

 vector<vector<vector<int>>> simplicialComplex::getSimplices(float radius)
 {
	 if (abs(radius - curSimpRadius) > 0.0001)
	 {
		 if (abs(radius - curProxRadius > 0.0001))
		 {
			 curProxRadius = radius;
			 calcProximityGraph(radius);
		 }
		 curSimpRadius = radius;
		 calcSimplices(radius);
	 }
	 return simplices;
 }
	
