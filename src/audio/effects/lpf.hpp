#ifndef LPF_HPP_
#define LPF_HPP_

namespace Audio
{

template <typename T> class LPF
{
private:
    T cutoffFrequency;
    T oldValue;
    T smoothingFactor;
    T samplePeriod;

public:
    LPF(T cutoffFrequency, T samplePeriod) : cutoffFrequency(cutoffFrequency), samplePeriod(samplePeriod)
    {
        cutoffFrequency = cutoffFrequency;
        smoothingFactor = (samplePeriod / (((T)1.0f / cutoffFrequency) + samplePeriod));
    }

    T update(T newValue)
    {
        T retValue = (((T)1.0f - smoothingFactor) * oldValue + (newValue * smoothingFactor));
        oldValue   = newValue;
        return retValue;
    }

    void reset(void)
    {
        oldValue = T(0);
    }
};

}

#endif  // LPF_HPP_
