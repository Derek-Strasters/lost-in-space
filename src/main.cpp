#include <Arduino.h>

#define BLU_LED 11
#define GRN_LED 10
#define RED_LED 9
#define PHOTO_SENSE A0

//#define MAX_U_INT 0xFFFFu
#define DOUBLE_MAX_U_INT 0x1FFFEul
#define MONITOR_INTERVAL 500u


/**
 * Return a time delta (in ms) from when the object was created or the last
 * time the getDelta() method was called.
 */
class MSDeltaTimer {
    uint32_t lastTime = millis();
public:
    MSDeltaTimer() = default;

    uint16_t getDelta() {
        uint32_t now = millis();
        uint16_t elapsed = now - lastTime;
        lastTime = now;

        return elapsed;
    }
};


/**
 * Translate a desired LED brightness value (as perceived by humans) to a corresponding PWM duty cycle.
 *
 * Human eyes do not perceive brightness linearly, rather they perceive it logarithmically.
 * This function maps (approximately) the range 0 - 2^16-1 ( 0x0 - 0xFFFF ) of
 * desired brightness levels to an output range of 256 - 2^16-1 ( 0xFF - 0xFFFF ).
 * This is suitable to use with a PWM to control an LED's perceived brightness linearly.
 *
 * More information about light perception: https://jared.geek.nz/2013/feb/linear-led-pwm
 *
 * @param brightness The desired brightness.
 * @return The adjusted value for power or duty cycle for an LED (256 - 2^16-1).
 */
uint16_t cieLightness(const uint16_t brightness) {
    const uint32_t adjustedBrightness = uint32_t(brightness) + 2313ul;  // Adjusted so LED never fully turns off.

    if (brightness <= 5243u)  // if below 8% of max.
        return uint16_t(double(adjustedBrightness) / 9.033);

    double intensity = (double(adjustedBrightness) + 10826.890762237848) / 1951.3943684720543;
    intensity = intensity * intensity * intensity;
    return uint16_t(intensity);
}


/**
 * Return one sample's amplitude from a continuous, normalized, positive only valued, sine wave.
 *
 * The signal is translated and normalized to values between 0 and MAX_U_INT_16.
 *
 * @param period The duration (in ms) of one complete cycle.
 * @param phase Negative phase shift, or time domain location of a sample to be returned.
 * @return The amplitude value of a the sample at the current time indicated by the phase parameter.
 */
uint16_t sineWave(
        const uint16_t period,
        const uint16_t phase
) {
    const double theta = (double(phase) / period) * TWO_PI;
    return (uint16_t) ((1.0 - cos(theta)) * ((double) 0xFFFF / 2));
}


/**
 * Return one sample's amplitude from a continuous, normalized, positive only valued, triangle wave.
 *
 * The signal is translated and normalized to values between 0 and 1 multiplied by the amplitude (max - min).
 *
 * @param period The duration (in ms) of one complete cycle.
 * @param phase Negative phase shift, or time domain location of a sample to be returned.
 * @return The amplitude value of a the sample at the current time indicated by the phase parameter.
 */
uint16_t triangle(const uint16_t period, const uint16_t phase) {
    const uint32_t displacement = (phase * DOUBLE_MAX_U_INT) / period;
    if (2 * phase <= period) {
        return uint16_t(displacement);
    }
    return uint16_t(DOUBLE_MAX_U_INT - displacement);
}


/**
 * SignalGenerator objects can return the amplitude (as an unsigned 8 bit int
 * [or char]) at a point on a waveform.
 * Objects maintain state like the last location sampled (cursor).
 * SignalGenerator's constructor will accept functions pointers to functions
 * that can provide a signal.
 */
class SignalGenerator {
    uint16_t (*waveFunc)(
            const uint16_t,
            const uint16_t
    );

    uint16_t period;
    uint16_t cursor;

    uint8_t remainder_on;
    uint8_t remainder_off;

public:
    SignalGenerator(
            uint16_t (*waveFunc)(
                    const uint16_t,
                    const uint16_t
            ),
            const uint16_t period
    ) :
            waveFunc(waveFunc),
            period(period),
            cursor(0ul),
            remainder_on(0),
            remainder_off(0) {};

    void setPeriod(const uint16_t newPeriod) {
        if (newPeriod == period) return;

        cursor = (uint32_t(cursor) * newPeriod) / period;
        period = newPeriod;
    }

    uint8_t getAmplitude(uint16_t delta) {
        cursor = cursor + delta;
        if (cursor >= period) cursor -= period;

        const uint16_t sample = cieLightness(waveFunc(period, cursor));
        const uint8_t truncated = sample >> 8;

        if (delta > 2u || truncated == 0xFF) {
            return truncated;
        }

        // Both remainder_on is empty and remainder_off is empty, so get new values.
        if (!remainder_on && !remainder_off) {
            remainder_on = (sample & 0xFF) >> 4;
            remainder_off = 16 - remainder_on;
        }

        if (remainder_on) {
            remainder_on = delta <= remainder_on ? remainder_on - delta : 0;
            return truncated + 1;
        }

        // if (remainder_off)
        remainder_off = delta <= remainder_off ? remainder_off - delta : 0;
        return truncated;
    }
};


/**
 * When the Arduino is first powered on or rebooted, the instructions in this
 * function are executed once before continuing to the main loop.
 */
__attribute__((unused)) void setup() {
    Serial.begin(9600);
    pinMode(BLU_LED, OUTPUT);
    pinMode(GRN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
}


/**
 * The main loop.  After setup() the Arduino will execute the instructions in
 * this function repeatedly, in succession, until interrupted or powered off.
 */
__attribute__((unused)) void loop() {
    // **************** STATIC OBJECTS ****************

    // Main loop timer.  Reports time passed since last loop started in ms.
    static MSDeltaTimer deltaTimer = MSDeltaTimer();

    // LED modulators that track state.
    static SignalGenerator bluWave = SignalGenerator(triangle, 30000u);
    static SignalGenerator grnWave = SignalGenerator(sineWave, 30500u);
    static SignalGenerator redWave = SignalGenerator(triangle, 31000u);

    // A timer and counter to trigger a report on loop frequency among other things every 1/2 second.
    static uint16_t loopTimer = 0;
    static uint16_t loopCount = 0;

    // **************** MAIN LOOP ACTIONS ****************

    // Get delta time and read analogue pins.
    const uint16_t delta = deltaTimer.getDelta();
    const uint16_t ambient_light = max(600 - analogRead(PHOTO_SENSE), 0);

    // Adjust the cycle period for LED light patterns.
    bluWave.setPeriod(300u + ambient_light * 35);
    grnWave.setPeriod(310u + ambient_light * 35);
    redWave.setPeriod(320u + ambient_light * 35);

    // Get the computed LED light intensity values.
    const uint8_t bluIntensity = bluWave.getAmplitude(delta);
    const uint8_t grnIntensity = grnWave.getAmplitude(delta);
    const uint8_t redIntensity = redWave.getAmplitude(delta);

    // Set LED intensity values to their respective pins.
    analogWrite(BLU_LED, bluIntensity);
    analogWrite(GRN_LED, grnIntensity);
    analogWrite(RED_LED, redIntensity);

    // **************** REPORTING ****************

    // Report status of pins and main loop frequency to the serial port.
    loopTimer += delta;
    loopCount++;
    if (loopTimer >= MONITOR_INTERVAL) {
        char buffer[79];
        sprintf(
                buffer,
                "Blu: %3u  Grn: %3u  Red: %3u  LightLVL:%3u LoopFRQ:%4uHz (%4dms) (%2d ms)",
                bluIntensity,
                grnIntensity,
                redIntensity,
                ambient_light,
                loopCount * 2,
                loopTimer,
                delta
        );
        Serial.println(buffer);
        loopCount = 0;
        loopTimer -= MONITOR_INTERVAL;
    }
}
