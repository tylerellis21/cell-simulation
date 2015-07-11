#include "neuralnetwork.h"
#include "../util/mathutils.h"
#include <iostream>

NeuralNetwork::NeuralNetwork(uint32 input, uint32 hidden, uint32 output) :
    m_inputCount(input),
    m_hiddenCount(hidden),
    m_outputCount(output)
{

    m_inputs = new real32[m_inputCount];

    m_inputWeights = createMatrix(m_inputCount, m_hiddenCount);

    m_inputSums = new real32[m_hiddenCount];
    m_inputBiases = new real32[m_hiddenCount];
    m_inputOutputs = new real32[m_hiddenCount];

    m_outputWeights = createMatrix(m_hiddenCount, m_outputCount);

    m_outputSums = new real32[m_outputCount];
    m_outputBiases = new real32[m_outputCount];
    m_outputs = new real32[m_outputCount];
}

NeuralNetwork::~NeuralNetwork()
{
    delete [] m_inputs;

    deleteMatrix(m_inputWeights, m_inputCount);

    delete [] m_inputSums;
    delete [] m_inputBiases;
    delete [] m_inputOutputs;

    deleteMatrix(m_outputWeights, m_outputCount);

    delete [] m_outputSums;
    delete [] m_outputBiases;
    delete [] m_outputs;
}

void NeuralNetwork::setWeights(const real32* weights)
{
    //TODO: Memcopy this shit.

    int k = 0;

    for (uint32 i = 0; i < m_inputCount; ++i)
        for (uint32 j = 0; j < m_hiddenCount; ++j)
            m_inputWeights[i][j] = weights[k++];

    for (uint32 i = 0; i < m_hiddenCount; ++i)
        m_inputBiases[i] = weights[k++];

    for (uint32 i = 0; i < m_hiddenCount; ++i)
        for (int j = 0; j < m_outputCount; ++j)
            m_outputWeights[i][j] = weights[k++];

    for (uint32 i = 0; i < m_outputCount; ++i)
        m_outputBiases[i] = weights[k++];
}

void NeuralNetwork::computeOutputs(real32* xValues)
{
    // zero
    for (uint32 i = 0; i < m_hiddenCount; ++i)
        m_inputSums[i] = 0.0;

    for (uint32 i = 0; i < m_outputCount; ++i)
        m_outputSums[i] = 0.0;

    // Compute input-to-hidden weighted sums.

    for (uint32 j = 0; j < m_hiddenCount; ++j)
        for (uint32 i = 0; i < m_inputCount; ++i)
            m_inputSums[j] += xValues[i] * m_inputWeights[i][j];

    // Add biases to input-to-hidden sums.
    for (uint32 i = 0; i < m_hiddenCount; ++i)
        m_inputSums[i] += m_inputBiases[i];

    // Determine input-to-hidden output.
    // Step function

    for (uint32 i = 0; i < m_hiddenCount; ++i)
        m_inputOutputs[i] = HyperTanFunction(m_inputSums[i]);

    // Compute hidden-to-output weighted sums
    for (uint32 j = 0; j < m_outputCount; ++j)
        for (uint32 i = 0; i < m_hiddenCount; ++i)
            m_outputSums[j] += m_inputOutputs[i] * m_outputWeights[i][j];

    // Add biases to input-to-hidden sums
    for (uint32 i = 0; i < m_outputCount; ++i)
        m_outputSums[i] += m_outputBiases[i];

    // Determine hidden-to-output result HyperTanFunction
    for (uint32 i = 0; i < m_outputCount; ++i)
        m_outputs[i] = HyperTanFunction(m_outputSums[i]);
}
