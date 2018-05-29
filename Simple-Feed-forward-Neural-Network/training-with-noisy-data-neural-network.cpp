/**************************************************
* YOU MAY USE THIS CODE AT YOUR OWN RISK          *
***************************************************
This is a neural network with one hidden layer.

You should set these parameters apporopriately

NUM_INPUT : number of inputs
NUM_HIDDEN : number of nodes in the hidden layer
NUM_OUTPUT : number of outputs
NUM_TRAINING_DATA : number of data for training
NUM_TEST_DATA : number of data for test
MAX_EPOCH : number of iterations for learning
LEARNING_RATE : learning rate (Eta)

These variables are for training data:
double training_point[NUM_TRAINING_DATA][NUM_INPUT] : inputs of training data
double training_target[NUM_TRAINING_DATA][NUM_OUTPUT] : outputs of training data

These variables are for test data:
double test_point[NUM_TEST_DATA][NUM_INPUT]	: inputs for test
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define SIGMOID(x) (1./(1+exp(-(x))))

#define NUM_INPUT	1
#define	NUM_HIDDEN	10
#define	NUM_OUTPUT	1
#define	NUM_TRAINING_DATA	11
#define	NUM_TEST_DATA	51

#define	MAX_EPOCH	5000000

#define	LEARNING_RATE	0.5

double training_point[NUM_TRAINING_DATA][NUM_INPUT]
= { { 0.0 },{ 0.1 },{ .2 },{ .3 },{ .4 },{ .5 },{ .6 },{ .7 },{ .8 },{ .9 },{ 1.0 } };

double training_target[NUM_TRAINING_DATA][NUM_OUTPUT]
= { { 0.05 },{ 0.33 },{ 0.70 },{ 0.80 },{ 0.99 },{ 1.00 },{ 0.96 },{ 0.87 },{ 0.60 },{ 0.40 },{ 0.05 } }; // 문제에서 주어진 정보로 수정.

double test_point[NUM_TEST_DATA][NUM_INPUT]
= { { 0 },{ 0.02 },{ 0.04 },{ 0.06 },{ 0.08 },{ 0.1 },{ 0.12 },{ 0.14 },{ 0.16 },{ 0.18 },
{ 0.2 },{ 0.22 },{ 0.24 },{ 0.26 },{ 0.28 },{ 0.3 },{ 0.32 },{ 0.34 },{ 0.36 },{ 0.38 },
{ 0.4 },{ 0.42 },{ 0.44 },{ 0.46 },{ 0.48 },{ 0.5 },{ 0.52 },{ 0.54 },{ 0.56 },{ 0.58 },
{ 0.6 },{ 0.62 },{ 0.64 },{ 0.66 },{ 0.68 },{ 0.7 },{ 0.72 },{ 0.74 },{ 0.76 },{ 0.78 },
{ 0.8 },{ 0.82 },{ 0.84 },{ 0.86 },{ 0.88 },{ 0.9 },{ 0.92 },{ 0.94 },{ 0.96 },{ 0.98 },{ 1 } };


int InitWeight(double weight_kj[NUM_OUTPUT][NUM_HIDDEN], double weight_ji[NUM_HIDDEN][NUM_INPUT],
	double bias_k[NUM_OUTPUT], double bias_j[NUM_HIDDEN])
{
	int i, j, k;

	//weight initialization
	for (k = 0; k < NUM_OUTPUT; k++)
		for (j = 0; j < NUM_HIDDEN; j++)
			weight_kj[k][j] = 1.0 * (rand() % 1000 - 500) / 5000;

	for (j = 0; j < NUM_HIDDEN; j++)
		for (i = 0; i < NUM_INPUT; i++)
			weight_ji[j][i] = 1.0 * (rand() % 1000 - 500) / 5000;

	for (k = 0; k < NUM_OUTPUT; k++)
		bias_k[k] = 1.0 * (rand() % 1000 - 500) / 5000;

	for (j = 0; j < NUM_HIDDEN; j++)
		bias_j[j] = 1.0 * (rand() % 1000 - 500) / 5000;

	return 0;
}

int ResetDelta(double delta_kj[NUM_OUTPUT][NUM_HIDDEN], double delta_ji[NUM_HIDDEN][NUM_INPUT],
	double delta_bias_k[NUM_OUTPUT], double delta_bias_j[NUM_HIDDEN])
{
	int i, j, k;

	//weight initialization
	for (k = 0; k < NUM_OUTPUT; k++)
		for (j = 0; j < NUM_HIDDEN; j++)
			delta_kj[k][j] = 0;

	for (j = 0; j < NUM_HIDDEN; j++)
		for (i = 0; i < NUM_INPUT; i++)
			delta_ji[j][i] = 0;

	for (k = 0; k < NUM_OUTPUT; k++)
		delta_bias_k[k] = 0;

	for (j = 0; j < NUM_HIDDEN; j++)
		delta_bias_j[j] = 0;

	return 0;
}

// generate outputs on the output nodes
int Forward(double training_point[NUM_INPUT],
	double weight_kj[NUM_OUTPUT][NUM_HIDDEN], double weight_ji[NUM_HIDDEN][NUM_INPUT],
	double bias_k[NUM_OUTPUT], double bias_j[NUM_HIDDEN],
	double hidden[NUM_HIDDEN], double output[NUM_OUTPUT])
{
	int i, j, k;
	double net_j, net_k;

	//evaluate the output of hidden nodes
	for (j = 0; j < NUM_HIDDEN; j++)
	{
		net_j = 0;
		for (i = 0; i < NUM_INPUT; i++)
			net_j += weight_ji[j][i] * training_point[i];
		net_j += bias_j[j];
		hidden[j] = SIGMOID(net_j);
	}

	//evaluate the output of output nodes
	for (k = 0; k < NUM_OUTPUT; k++)
	{
		net_k = 0;
		for (j = 0; j < NUM_HIDDEN; j++)
			net_k += weight_kj[k][j] * hidden[j];
		net_k += bias_k[k];

		output[k] = SIGMOID(net_k);
	}

	return 0;
}

int Backward(double training_point[NUM_INPUT], double training_target[NUM_OUTPUT],
	double hidden[NUM_HIDDEN], double output[NUM_OUTPUT],
	double weight_kj[NUM_OUTPUT][NUM_HIDDEN],
	double delta_kj[NUM_OUTPUT][NUM_HIDDEN], double delta_ji[NUM_HIDDEN][NUM_INPUT],
	double delta_bias_k[NUM_OUTPUT], double delta_bias_j[NUM_HIDDEN])

{
	int i, j, k;

	//evaluate delta_kj
	for (k = 0; k < NUM_OUTPUT; k++)
		for (j = 0; j < NUM_HIDDEN; j++)
			delta_kj[k][j] += -output[k] * (1 - output[k])*(training_target[k] - output[k])*hidden[j];

	for (k = 0; k < NUM_OUTPUT; k++)
		delta_bias_k[k] += -output[k] * (1 - output[k])*(training_target[k] - output[k]);

	//evaluate delta_ji
	for (j = 0; j < NUM_HIDDEN; j++)
		for (i = 0; i < NUM_INPUT; i++)
		{
			double delta_k = 0;
			for (k = 0; k < NUM_OUTPUT; k++)
				delta_k += -output[k] * (1 - output[k])*(training_target[k] - output[k])*weight_kj[k][j];
			delta_ji[j][i] += delta_k*hidden[j] * (1 - hidden[j])*training_point[i];
		}

	for (j = 0; j < NUM_HIDDEN; j++)
	{
		double delta_k = 0;
		for (k = 0; k < NUM_OUTPUT; k++)
			delta_k += -output[k] * (1 - output[k])*(training_target[k] - output[k])*weight_kj[k][j];
		delta_bias_j[j] += delta_k*hidden[j] * (1 - hidden[j]);
	}

	return 0;
}

int UpdateWeights(double delta_kj[NUM_OUTPUT][NUM_HIDDEN], double delta_ji[NUM_HIDDEN][NUM_INPUT],
	double delta_bias_k[NUM_OUTPUT], double delta_bias_j[NUM_HIDDEN],
	double weight_kj[NUM_OUTPUT][NUM_HIDDEN], double weight_ji[NUM_HIDDEN][NUM_INPUT],
	double bias_k[NUM_OUTPUT], double bias_j[NUM_HIDDEN])
{
	int i, j, k;

	//update weights
	for (k = 0; k < NUM_OUTPUT; k++)
		for (j = 0; j < NUM_HIDDEN; j++)
			weight_kj[k][j] -= LEARNING_RATE*delta_kj[k][j];

	for (k = 0; k < NUM_OUTPUT; k++)
		bias_k[k] -= LEARNING_RATE*delta_bias_k[k];

	for (j = 0; j < NUM_HIDDEN; j++)
		for (i = 0; i < NUM_INPUT; i++)
			weight_ji[j][i] -= LEARNING_RATE*delta_ji[j][i];

	for (j = 0; j < NUM_HIDDEN; j++)
		bias_j[j] -= LEARNING_RATE*delta_bias_j[j];

	return 0;
}

int PrintWeight(double weight_kj[NUM_OUTPUT][NUM_HIDDEN], double weight_ji[NUM_HIDDEN][NUM_INPUT],
	double bias_k[NUM_OUTPUT], double bias_j[NUM_HIDDEN])
{
	int i, j, k;

	//print weights
	for (j = 0; j < NUM_HIDDEN; j++)
		for (i = 0; i < NUM_INPUT; i++)
			printf("%f ", weight_ji[j][i]);

	for (j = 0; j < NUM_HIDDEN; j++)
		printf("%f ", bias_j[j]);

	for (k = 0; k < NUM_OUTPUT; k++)
		for (j = 0; j < NUM_HIDDEN; j++)
			printf("%f ", weight_kj[k][j]);

	for (k = 0; k < NUM_OUTPUT; k++)
		printf("%f ", bias_k[k]);

	printf("\n");

	return 0;
}

/*
hidden[j] : output of node j at hidden layer
output[k] : output of node k at output layer
weight_kj[k][j] : weight between node j at hidden layer and node k at output layer
bias_k[k] : weight between bias (the default input, 1) and node k at output layer
weight_ji[j][i] : weight between input i and node j at hidden layer
bias_j[j] : weight between (the default input, 1) and node j at hidden layer

delta_kj[k][j] : delta for weight_kj[k][j]
delta_ji[j][i] : delta for weight_ji[j][i]
delta_bias_k[k] : delta for bias_k[k]
delta_bias_j[j] : delta for bias_j[j]

error : the summation of error
*/

int main()
{
	double hidden[NUM_HIDDEN], output[NUM_OUTPUT];
	double weight_kj[NUM_OUTPUT][NUM_HIDDEN], weight_ji[NUM_HIDDEN][NUM_INPUT];
	double bias_k[NUM_OUTPUT], bias_j[NUM_HIDDEN];
	double delta_kj[NUM_OUTPUT][NUM_HIDDEN], delta_ji[NUM_HIDDEN][NUM_INPUT];
	double delta_bias_k[NUM_OUTPUT], delta_bias_j[NUM_HIDDEN];
	double training_error, testing_error;
	double buff = 0;

	int k, p;

	srand((unsigned)time(NULL));

	InitWeight(weight_kj, weight_ji, bias_k, bias_j);

	// loop for learning
	printf("******* Training and Testing of NN (Iteration : Error) *******\n");

	for (int epoch = 0; epoch <= MAX_EPOCH; epoch++)
	{
		training_error = 0;
		testing_error = 0;

		ResetDelta(delta_kj, delta_ji, delta_bias_k, delta_bias_j);

		for (p = 0; p < NUM_TRAINING_DATA; p++)
		{
			Forward(training_point[p], weight_kj, weight_ji, bias_k, bias_j,
				hidden, output);

			for (k = 0; k < NUM_OUTPUT; k++) {
				training_error += (output[k] - training_target[p][k])*(output[k] - training_target[p][k])*0.5;
			}

			Backward(training_point[p], training_target[p], hidden, output, weight_kj,
				delta_kj, delta_ji, delta_bias_k, delta_bias_j);
		} // Training 및 error 계산

		for (p = 0; p < NUM_TEST_DATA; p++)
		{
			Forward(test_point[p], weight_kj, weight_ji, bias_k, bias_j,
				hidden, output);

			for (k = 0; k < NUM_OUTPUT; k++) {
				buff = 4 * test_point[p][k] * (1 - test_point[p][k]);

				testing_error += (output[k] - buff)*(output[k] - buff)*0.5;
			}
		} // Testing 및 error 계산

		UpdateWeights(delta_kj, delta_ji, delta_bias_k, delta_bias_j,
			weight_kj, weight_ji, bias_k, bias_j);

		if (epoch == 1000 || epoch == 10000 || epoch == 50000 || epoch == 100000 ||
			epoch == 500000 || epoch == 1000000 || epoch == 2000000 || epoch == 5000000)
			printf("[%d] training error : %f , testing error : %f\n", epoch, training_error, testing_error); // error 값 출력
	}
}
