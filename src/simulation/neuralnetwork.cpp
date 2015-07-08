#include "neuralnetwork.h"
#include "../util/mathutils.h"

NeuralNetwork::NeuralNetwork(uint32 input, uint32 hidden, uint32 output) :
    m_inputCount(input),
    m_hiddenCount(hidden),
    m_outputCount(output)
{
    m_inputs = new real32[input];
    m_inputWeights = createMatrix(input, hidden);
    m_inputSums = new real32[hidden];
    m_inputBiases = new real32[hidden];
    m_inputOutputs = new real32[hidden];
    m_outputWeights = createMatrix(hidden, output);
    m_outputSums = new real32[output];
    m_outputBiases = new real32[output];
    m_outputs = new real32[output];
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

    for (int i = 0; i < m_inputCount; ++i)
        for (int j = 0; j < m_hiddenCount; ++j)
            m_inputWeights[i][j] = weights[k++];

    for (int i = 0; i < m_hiddenCount; ++i)
        m_inputBiases[i] = weights[k++];

    for (int i = 0; i < m_hiddenCount; ++i)
        for (int j = 0; j < m_outputCount; ++j)
            m_outputWeights[i][j] = weights[k++];

    for (int i = 0; i < m_outputCount; ++i)
        m_outputBiases[i] = weights[k++];
}

void NeuralNetwork::computeOutputs(real32* xValues)
{
    // zero
    for (int i = 0; i < m_inputCount; ++i)
        m_inputSums[i] = 0.0;

    for (int i = 0; i < m_outputCount; ++i)
        m_outputSums[i] = 0.0;

    // Compute input-to-hidden weighted sums.

    for (int j = 0; j < m_hiddenCount; ++j)
        for (int i = 0; i < m_inputCount; ++i)
            m_inputSums[j] += xValues[i] * m_inputWeights[i][j];

    // Add biases to input-to-hidden sums.
    for (int i = 0; i < m_hiddenCount; ++i)
        m_inputSums[i] += m_inputBiases[i];

    // Determine input-to-hidden output.
    // Step function

    for (int i = 0; i < m_hiddenCount; ++i)
        m_inputOutputs[i] = HyperTanFunction(m_inputSums[i]);

    // Compute hidden-to-output weighted sums
    for (int j = 0; j < m_outputCount; ++j)
        for (int i = 0; i < m_hiddenCount; ++i)
            m_outputSums[j] += m_inputOutputs[i] * m_outputWeights[i][j];

    // Add biases to input-to-hidden sums
    for (int i = 0; i < m_outputCount; ++i)
        m_outputSums[i] += m_outputBiases[i];

    // Determine hidden-to-output result HyperTanFunction
    for (int i = 0; i < m_outputCount; ++i)
        m_outputs[i] = HyperTanFunction(m_outputSums[i]);
}
