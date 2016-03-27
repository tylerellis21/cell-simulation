#ifndef NEURALNETWORK_H_INCLUDE
#define NEURALNETWORK_H_INCLUDE

// Standard includes.
#include <cmath>
#include <memory>

#include <scl/types.h>

/**
 * @brief This class is used to calculate the output data of the cell.
 */
class NeuralNetwork
{
public:

    /**
     * @brief Deafult NeuralNetwork Constructor.
     * @param input = The number of inputs.
     * @param hidden = The number of hidden layers.
     * @param output = The number of output layers.
     */
    NeuralNetwork(u32 input, u32 hidden, u32 output);

    /**
     * @brief Default NeuralNetwork Destructor.
     */
    ~NeuralNetwork();

    /**
     * @brief Get the number of inputs in the network.
     * @return The number of inputs.
     */
    const u32 getInputCount() const { return m_inputCount; }

    /**
     * @brief Get the number of hidden layers in the network.
     * @return The number of hidden layers.
     */
    const u32 getHiddenCount() const { return m_hiddenCount; }

    /**
     * @brief Get the output count of the network.
     * @return The number of outputs.
     */
    const u32 getOutputCount() const { return m_outputCount; }

    /**
     * @brief Get the number of weights needed for this network.
     * @return The number of weights required for this network.
     */
    const u32 getWeightCount() const {
        return (m_inputCount * m_hiddenCount) + (m_hiddenCount * m_outputCount) + m_hiddenCount + m_outputCount;
    }

    /**
     * @brief Get the number of outputs needed for this network.
     * @return The number of network outputs.
     */
    const r32* getOutputs() const { return m_outputs; }

    /**
     * @brief Set the weight data used for processing the input values.
     * @param weights = A pointer to the weight data.
     */
    void setWeights(const r32 *weights);

    /**
     * @brief Compute the output values of the network based on the input values.
     * @param xValues = The input values.
     */
    void computeOutputs(r32* xValues);

    /**
     * @brief One of the functions that can be used to process data in the nerual network.
     * @param x = The input value.
     * @return = The calculated output value.
     */
    inline r32 StepFunction(r32 x)
    {
        if (x > 0.0f)
            return 1.0f;
        else
            return 0.0f;
    }

    /**
     * @brief One of the functions that can be used to process data in the nerual network.
     * @param x = The input value.
     * @return = The calculated output value.
     */
    inline r32 SigmoidFunction(r32 x)
    {
        if (x < -45.0f)
            return 0.0f;
        else if (x > 45.0f)
            return 1.0f;
        else
            return 1.0f / (1.0f + exp(-x));
    }

    /**
     * @brief One of the functions that can be used to process data in the nerual network.
     * @param x = The input value.
     * @return = The calculated output value.
     */
    inline r32 HyperTanFunction(r32 x)
    {
        if (x < -10.0f)
            return -1.0f;
        else if (x > 10.0f)
            return 1.0f;
        else
            return tanh(x);
    }

private:

    /**
     * @brief The number of inputs for this network.
     */
    const u32 m_inputCount;

    /**
     * @brief The number of hidden layers for this network.
     */
    const u32 m_hiddenCount;

    /**
     * @brief The number of outputs for this network.
     */
    const u32 m_outputCount;

    /**
     * @brief Used to processes the input data.
     */
    r32* m_inputs;

    /**
     * @brief Used for network processing.
     */
    r32** m_inputWeights;

    /**
     * @brief Used to sum the input values.
     */
    r32* m_inputSums;

    /**
     * @brief The input biases caculated from the weight data.
     */
    r32* m_inputBiases;

    /**
     * @brief The output buffer for this network.
     */
    r32* m_inputOutputs;

    /**
     * @brief The output weights caculated from the weight data.
     */
    r32** m_outputWeights;

    /**
     * @brief The caculated output sums of the network.
     */
    r32* m_outputSums;

    /**
     * @brief The output biases calculated from the weight data.
     */
    r32* m_outputBiases;

    /**
     * @brief The output buffer used to store the network output during processing.
     */
    r32* m_outputs;

};

#endif // NEURALNETWORK_H_INCLUDE
