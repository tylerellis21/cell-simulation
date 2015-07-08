#ifndef NEURALNETWORK_H_INCLUDE
#define NEURALNETWORK_H_INCLUDE

// Standard includes.
#include <cmath>

// Project includes.
#include "../typedefs.h"

class NeuralNetwork
{
public:

    /**
     * @brief Deafult NeuralNetwork Constructor.
     * @param input = The number of inputs.
     * @param hidden = The number of hidden layers.
     * @param output = The number of output layers.
     */
    NeuralNetwork(uint32 input, uint32 hidden, uint32 output);

    /**
     * @brief Default NeuralNetwork Destructor.
     */
    ~NeuralNetwork();

    const uint32 getInputCount() const { return m_inputCount; }
    const uint32 getHiddenCount() const { return m_hiddenCount; }
    const uint32 getOutputCount() const { return m_outputCount; }

    const uint32 getWeightCount() const {
        return (m_inputCount * m_hiddenCount) + (m_hiddenCount * m_outputCount) + m_hiddenCount + m_outputCount;
    }

    const real32* getOutputs() const { return m_outputs; }

    void setWeights(const real32* weights);

    void computeOutputs(real32* xValues);

    inline real32 StepFunction(real32 x)
    {
        if (x > 0.0f)
            return 1.0f;
        else
            return 0.0f;
    }

    inline real32 SigmoidFunction(real32 x)
    {
        if (x < -45.0f)
            return 0.0f;
        else if (x > 45.0f)
            return 1.0f;
        else
            return 1.0f / (1.0f + exp(-x));
    }

    inline real32 HyperTanFunction(real32 x)
    {
        if (x < -10.0f)
            return -1.0f;
        else if (x > 10.0f)
            return 1.0f;
        else
            return tanh(x);
    }

private:

    const uint32 m_inputCount;
    const uint32 m_hiddenCount;
    const uint32 m_outputCount;

    real32* m_inputs;

    real32** m_inputWeights;

    real32* m_inputSums;
    real32* m_inputBiases;
    real32* m_inputOutputs;

    real32** m_outputWeights;
    real32* m_outputSums;
    real32* m_outputBiases;

    real32* m_outputs;

};

#endif // NEURALNETWORK_H_INCLUDE
