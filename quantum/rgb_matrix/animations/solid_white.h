RGB_MATRIX_EFFECT(SOLID_WHITE)
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool SOLID_WHITE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        rgb_matrix_set_color(i, rgb_matrix_config.variety, rgb_matrix_config.variety, 255);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
