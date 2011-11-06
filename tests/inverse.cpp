/* 
 * File:   inverse.cpp
 * Author: Jan Vlnas
 *
 * Created on 6.11.2011, 18:01:49
 */

#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <limits>

#include "Matrix4.h"

bool AreSame(double a, double b)
{
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

/*
 * Simple C++ Test Suite
 */

void testInverse()
{
    float input_ar[16] = {
	10.f, -9.f, -12.f, 1.f,
	7.f, -12.f, 11.f, 2.f,
	-10.f, 10.f, 3.f, 1.f,
	-3.f, 1.f, 2.f, 4.f
    };

    float example_ar[16] = {
	-1.661290322580630f, -1.467741935483860f, -0.422580645161288f, -0.667741935483866f,
	-1.274193548387090f, -1.145161290322570f, -0.248387096774192f, -0.545161290322577f,
	-3.209677419354820f, -2.758064516129010f, -0.719354838709673f, -1.358064516129020f,
	1.854838709677410f, 1.629032258064510f, 0.409677419354836f, 1.029032258064510f
    };
    //Inverse[{{10, -9, -12, 1}, {7, -12, 11, 2}, {-10, 10, 3, 1}, {-3, 1, 2, 4}}]
    Matrix4f matrix4 = Matrix4f::FromFlatArray(input_ar);
    matrix4.dump();
    Matrix4f result = matrix4.inverse();
    result.dump();
    Matrix4f example = Matrix4f::FromFlatArray(example_ar);
    example.dump();

//    float * result_mat = result.mat();
//    float * example_mat = example.mat();

    for(int i = 0; i < 4; i++)
    {
	for(int j = 0; i < 4; i++)
	{
	    if(!AreSame(result[i][j], example[i][j]))
	    {
		std::cout << "%TEST_FAILED% time=0 testname=testInverse (inverse) message=expected" << result[i][j] << " to equal " << example[i][j] << std::endl;
	    }
	    else
	    {
		std::cout << result[i][j] << " equals " << example[i][j] << std::endl;
	    }
	}
    }


    //    if(true /*check result*/)
    //    {
    //	std::cout << "%TEST_FAILED% time=0 testname=testInverse (inverse) message=error message sample" << std::endl;
    //    }
}

int main(int argc, char** argv)
{
    std::cout << "%SUITE_STARTING% inverse" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testInverse (inverse)" << std::endl;
    testInverse();
    std::cout << "%TEST_FINISHED% time=0 testInverse (inverse)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return(EXIT_SUCCESS);
}

