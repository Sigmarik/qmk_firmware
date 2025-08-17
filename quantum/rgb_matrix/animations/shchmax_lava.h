#ifdef ENABLE_RGB_MATRIX_SHCHMAX_LAVA
RGB_MATRIX_EFFECT(SHCHMAX_LAVA)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static void shchmax_lava_set_color(int i, effect_params_t* params) {
    if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) return;
    uint8_t dhue = (uint8_t)((random8() - 128) * rgb_matrix_config.hsv.s / 256. / 256. * 128);
    uint8_t val = 255 - (uint8_t)(random8() * rgb_matrix_config.hsv.v / 256.);
    HSV hsv = {(rgb_matrix_config.hsv.h + dhue) % 256, 255, val};
    RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
}

bool SHCHMAX_LAVA(effect_params_t* params) {
    if (!params->init) {
        // Change one LED every tick, make sure speed is not 0
        if (scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 16)) % 5 == 0) {
            shchmax_lava_set_color(random8_max(RGB_MATRIX_LED_COUNT), params);
        }
        return false;
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (int i = led_min; i < led_max; i++) {
        shchmax_lava_set_color(i, params);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_SHCHMAX_LAVA
