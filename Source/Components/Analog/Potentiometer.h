#pragma once

#include "Resistor.h"

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Potentiometer : public Resistor {
public:
    Potentiometer(double totalResistance = 10000.0, double initialPosition = 0.5);  // Default 10kΩ, centered
    ~Potentiometer() = default;

    // Set the wiper position (0.0 to 1.0)
    void setPosition(double position);
    
    // Get the current wiper position
    double getPosition() const { return position; }
    
    // Get resistance between wiper and terminal 1 (0.0 to 1.0 of total resistance)
    double getResistance1() const;
    
    // Get resistance between wiper and terminal 2 (0.0 to 1.0 of total resistance)
    double getResistance2() const;
    
    // Process a single sample with the current wiper position
    double process(double voltage1, double voltage2);
    
    // Reset state
    void reset() override;

private:
    double position;  // Wiper position (0.0 to 1.0)
    
    // Constants
    static constexpr double MIN_POSITION = 0.0;
    static constexpr double MAX_POSITION = 1.0;
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 