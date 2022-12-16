#include <Arduino.h>

#define BLU_LED 11
#define GRN_LED 10
#define RED_LED 9

#define MAX_UINT16_T ( 0x0000U - 0x0001U )

/**
 * Adjust power to an LED to match it's brightness (as perceived by humans) to the input value.
 *
 * Human eyes do not perceive the brightness of a light linearly, rather it is perceived logarithmically.
 * This function will map (approximately) the entire range of 2^16-1 to power (duty cycle) levels that
 * will reflect a linear increase in brightness as the input is increased linearly.
 *
 * @param brightness The desired brightness.
 * @return The adjusted power or duty cycle for an LED (0-255)
 */
static inline unsigned char cie_lightness(const unsigned int brightness) {
    if (brightness <= 0x147Bu)     // if below 8% of max
        return (brightness / 9) >> 8;  // same as dividing by 900%
    unsigned long intensity = (((unsigned long) brightness + 0x28F6u) << 8) / (0x000128F5ul);  // = 0xFFFFu + 0x28F6u
    // Add 16% of max and compare to get a useful result with
    // integer division, we shift left in the expression above
    // and revert what we've done again after squaring.
    intensity = intensity * intensity * intensity >> 16;
    if (intensity < 0xFF) return intensity;
    return 0xFF;
}

/**
 * Return one sample from a continuous, positive value only, sine sineWave; based on the current time (in milliseconds).
 *
 * The signal is translated and normalized to values between 0 and 1 multiplied by the amplitude (max - min).
 *
 * @param period The duration of one complete cycle (in ms).
 * @param min_value The lowest brightness within one cycle.
 * @param max_value The highest brightness within one cycle.
 * @return The value of the sample at the current time (in ms).
 */
unsigned char sineWave(const unsigned int period, const unsigned int min_value, const unsigned int max_value) {
    unsigned int amplitude = max_value - min_value;
    unsigned int msTime = millis() % period;
    double theta = (TWO_PI * msTime) / period;
    unsigned int sample = lround(((sin(theta) + 1) * amplitude) / 2) + min_value;
    return cie_lightness(sample);
}

unsigned char sineWave(const unsigned int period) {
    return sineWave(period, 0, 0xFFFFu);
}

/**
 * Return one sample from a continuous triangle wave; based on the current time (in milliseconds).
 *
 * The signal is translated and normalized to values between 0 and 1 multiplied by the amplitude (max - min).
 *
 * @param period The duration of one complete cycle (in ms).
 * @param min_value The lowest brightness within one cycle.
 * @param max_value The highest brightness within one cycle.
 * @return The value of the sample at the current time (in ms).
 */
unsigned char triangle(const unsigned int period, const unsigned int min_value, const unsigned int max_value) {
    unsigned int amplitude = max_value - min_value;
    unsigned int msTime = millis() % period;
    double em = 2.0 * double(amplitude) / period;
    unsigned long displacement = lround(em * msTime);
    if (displacement <= max_value) {
        return cie_lightness(min_value + displacement);
    }
    return cie_lightness((2ul * max_value) - displacement);
}

unsigned char triangle(const unsigned int period) {
    return triangle(period, 0, 0xFFFFu);
}

__attribute__((unused)) void setup() {
    pinMode(BLU_LED, OUTPUT);
    pinMode(GRN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
}

__attribute__((unused)) void loop() {
    analogWrite(BLU_LED, sineWave(37900));
    analogWrite(GRN_LED, triangle(36700));
    analogWrite(RED_LED, sineWave(37300));
}
