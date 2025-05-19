#pragma once

#include <algorithm>
#include <cmath>

namespace ArchitextureStudiosAnalogCore::Analog {

class Potentiometer {
public:
    enum class TaperType {
        Linear,
        Logarithmic
    };

    static constexpr double MIN_POSITION = 0.0;
    static constexpr double MAX_POSITION = 1.0;

    Potentiometer(double totalResistance, double initialPosition = 0.5, TaperType taper = TaperType::Linear);
    
    void setPosition(double newPosition);
    double getPosition() const { return position; }
    

    void setTaperType(TaperType newTaper) { taperType = newTaper; }
    TaperType getTaperType() const { return taperType; }
    // Get resistance between wiper and terminal 1
    double getResistance1() const;
    
    // Get resistance between wiper and terminal 2
    double getResistance2() const;
    
    // Process voltage through the potentiometer
    double process(double voltage1, double voltage2);

private:
    double totalResistance;  // Total resistance between terminals
    double position;         // Position of wiper (0.0 to 1.0)
    TaperType taperType;     // Type of potentiometer taper
    
    // Helper function to convert position based on taper type
    double getEffectivePosition() const;
};

} // namespace ArchitextureStudiosAnalogCore::Analog 