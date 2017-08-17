//
//  autil_parameter.hpp
//  libblockdsp
//
//  Created by Luke on 8/26/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_parameter_hpp
#define autil_parameter_hpp

#include "autil_number.hpp"
#include "autil_array.hpp"
#include "autil_str.hpp"
#include "autil_mod.hpp"

class APUParameter;
class AudioProcessingUnit;

/** Interface for a callback object that handles parameter changes */
class APUParameterCallback
{
public:
    /** This method will be called when the parameters current value is changed.
      * @param parameter a pointer to the parameter that was changed 
      */
    virtual void onParameterChanged(APUParameter *parameter) = 0;
};

/** Manages a value that can be manipulated during processing.
  * All getters and setters for internal numeric values are atomic
  */
class APUParameter
: public APUObject
{
    friend class AudioProcessingUnit;

public:
    APUOBJ_FWDDECL

    /** Construct a parameter object
      * @param identifier the unique identifier of the parameter
      * @param minValue the minimum value allowed for the parameter
      * @param maxValue the maximum value allowed for the parameter
      * @param defaultValue the initial value for the parameter
      * @param cb optional callback object to handle parameter changes
      */
    APUParameter(APUString *identifier,
                 APUNumber minValue,
                 APUNumber maxValue,
                 APUNumber defaultValue,
                 APUParameterCallback *cb=NULL);

    virtual ~APUParameter();

    /** Get the unique identifier for the parameter */
    APUObjRet<APUString> getIdentifier();

    /** Get the name of the parameter to display in the UI */
    APUObjRet<APUString> getName();

    /** Get the target value of the parameter (value after smoothing) (copy) */
    APUNumber getTarget();
    /** Get the current value of the parameter (copy) */
    APUNumber getCurrentValue();

    /** Get the value type of the parameter. This is determined by the value of the number passed to setTarget() and/or setValue() */
    APUNumberType type();

    /** Set the string representing the units of the parameter value */
    void setUnits(APUString *units);
    /** Get the string representing the units of the parameter value */
    APUObjRet<APUString> getUnits();

    /** Set the callback object for the parameter */
    void setCallback(APUParameterCallback *cb);

    /** Set the minimum permissible value */
    virtual void setMinValue(APUNumber minVal);
    /** Set the maximum permissible value */
    virtual void setMaxValue(APUNumber maxVal);

    /** Get the minimum allowed value for the parameter */
    APUNumber getMinValue();
    /** Get the maximum allowed value for the parameter */
    APUNumber getMaxValue();

    /** Set the value (no smoothing) of the parameter. If the value is outside of the min/max range, it will be clamped.
      * @return false if the value was clamped. Otherwise, true
      */
    virtual bool setValue(APUNumber value);
    /** Set the value of the parameter to be smoothed if smoothing is enabled.
      * @return false if the value was clamped. Otherwise, true
      */
    virtual bool setTarget(APUNumber target);

    /** @return true if the parameter's value can be ramped over time */
    virtual bool supportsSmoothing();
    /** Set whether or not parameters will ramp to the target value over time. */
    virtual void setSmoothingEnabled(bool enabled);
    /** Set the interval (in millseconds) over which the parameter value will ramp if smoothing is enabled */
    virtual void setSmoothingInterval(double millisec);
    /** Get the number of milliseconds over which the parameter value will ramp if smoothing is enabled */
    virtual double getSmoothingInterval();
    /** Determine if smoothing is enabled for the parameter */
    bool isSmoothingEnabled();

    /** Set the modulation signal source */
    void setModulationSource(APUModSource *source);

    /** Set the modulation depth (0.0 -- 1.0) */
    void setModulationDepth(float depth);

    /** Set the sample rate internally to calculate smoothing time */
    virtual void setSampleRate(size_t sampleRate);

protected:
    /** Called by the APU to update the current parameter value for smoothing */
    virtual void update();
    /** Clamp the value to the min/max if needed */
    bool normalizeValue(APUNumber &value);

private:
    class Pimpl;
    Pimpl *m_pimpl;
};

/** Parameter representing a set of discrete values */
class APUEnumParameter : public APUParameter
{
public:
    /** Constructor
      * @param name name of the parameter
      * @param strings list of strings for the enum values
      * @param cb optional callback object
      */
    APUEnumParameter(APUString *name, APUArray<APUString> *strings, APUParameterCallback *cb=NULL);
    virtual ~APUEnumParameter();

    /** NO-OP: Cannot set min value for enums. Minimum is always 0 */
    void setMinValue(APUNumber minVal);
    /** NO-OP: Cannot set max value for enums. Maximum is always numItems - 1 */
    void setMaxValue(APUNumber maxVal);

    bool supportsSmoothing();

    /** Get the string representation of the given value */
    APUObjRet<APUString> stringForValue(uint32_t value);

private:
    class EnumParamPimpl;
    EnumParamPimpl *m_enumParamPimpl;
};

#endif /* autil_parameter_hpp */
