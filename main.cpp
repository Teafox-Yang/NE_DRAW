#include <Eigen/Core>
#include <ostream>

#include<iostream>
#include<Eigen/Dense>
using namespace std;
using namespace Eigen;
int main()
{
	Matrix2d a;
	a << 1, 2,
		3, 4;
	MatrixXd b(2, 2);
	b << 2, 3,
		1, 4;
	cout << a + b << endl;
	return 1;
}