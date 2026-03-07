

namespace Utils {

    inline float normalize_value(float value, float current_min, float current_max, float target_min, float target_max) {
        if (current_max == current_min) return target_min; // Avoid division by zero

        float normalized = target_min + ((value - current_min) * (target_max - target_min)) / (current_max - current_min);
        return normalized;
    }

}