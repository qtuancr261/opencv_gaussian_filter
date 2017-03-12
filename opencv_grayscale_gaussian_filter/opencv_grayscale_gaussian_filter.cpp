#define _USE_MATH_DEFINES 
#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void generateGaussKernel(vector<vector<double>>& kernel)
{
	int kernelSize{};
	cout << " Nhap vao kich thuoc m cua ma tran Kernel: ";
	cin >> kernelSize;
	kernel = vector<vector<double>>(kernelSize, vector<double>(kernelSize));
	int halfSize = kernelSize / 2;
	double sigma{ 1.0 }; // change sigma value here
	double s2{ sigma*sigma };
	for (int i{ -halfSize }; i <= halfSize; i++)
		for (int j{ -halfSize }; j <= halfSize; j++)
			kernel[i + halfSize][j + halfSize] = exp(-(i*i + j*j) / (2.0 * s2)) / (2.0*M_PI*s2);
}
void gaussFilter(Mat& input, Mat& output, vector<vector<double>>& kernel)
{
	double newPixelValue{};
	int halfSize = kernel.size() / 2;
	for (int i{ halfSize }; i < input.rows - halfSize; i++)
		for (int j{ halfSize }; j < input.cols - halfSize; j++)
		{
			newPixelValue = 0;
			for (int x = { -halfSize }; x <= halfSize; x++)
				for (int y = { -halfSize }; y <= halfSize; y++)
				{
					newPixelValue += input.at<uchar>(i + x, j + y)*kernel[x + halfSize][y + halfSize];
				}
			output.at<uchar>(i, j) = newPixelValue;
		}
}
int main(int argc, char *argv[])
{
	double start{ (double)getTickCount() };
	Mat input_img;
	input_img = imread(argv[1], IMREAD_GRAYSCALE);
	if (input_img.empty())
	{
		cerr << "Couldn't open image . Exit program...................." << endl;
		return 1;
	}
	Mat output_img(input_img.clone());
	//-------------------------------------------------------------------------------------------
	vector<vector<double>> kernel;
	generateGaussKernel(kernel);
	gaussFilter(input_img, output_img, kernel);
	namedWindow(argv[1], WINDOW_AUTOSIZE);
	imshow(argv[1], input_img);
	namedWindow("Gaussian Filter", WINDOW_AUTOSIZE);
	imshow("Gaussian Filter", output_img);
	double end{ (double)(getTickCount() - start) / getTickFrequency() };
	cout << " Timing : " << end << endl;
	waitKey();
	return 0;
}
