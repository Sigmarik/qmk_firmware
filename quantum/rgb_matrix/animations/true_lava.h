#ifdef ENABLE_RGB_MATRIX_TRUE_LAVA
RGB_MATRIX_EFFECT(TRUE_LAVA)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static float timer;

bool TRUE_LAVA(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    const float sine_one_kx = 0.02;
    const float sine_one_ky = 0.02;
    const float sine_one_w = 0.02;
    const float sine_one_phi0 = 0;
    const float sine_one_amp = 4;
    const float sine_two_kx = 0.06;
    const float sine_two_ky = 0.09;
    const float sine_two_w = 0.021;
    const float sine_two_phi0 = 1;
    const float sine_two_amp = 3;
    const float sine_three_kx = 0.17;
    const float sine_three_ky = 0.10;
    const float sine_three_w = 0.013;
    const float sine_three_phi0 = 2;
    const float sine_three_amp = 2;

    const float timelcm = acos(-1) * 10000000.0;

    float variety = rgb_matrix_config.variety / 256.0;

    if (params->init) {
        timer = 0;
    }
    if (timer > timelcm) {
        timer -= timelcm;
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        int dx = g_led_config.point[i].x - k_rgb_matrix_center.x;
        int dy = g_led_config.point[i].y - k_rgb_matrix_center.y;

        float val = (
            pow(fabs(sin(timer * sine_one_w - dx * sine_one_kx - dy * sine_one_ky * dy + sine_one_phi0)), 2.0) * sine_one_amp
            + pow(fabs(sin(timer * sine_two_w - dx * sine_two_kx - dy * sine_two_ky * dy + sine_two_phi0)), 2.0) * sine_two_amp
            + pow(fabs(sin(timer * sine_three_w - dx * sine_three_kx - dy * sine_three_ky * dy + sine_three_phi0)), 2.0) * sine_three_amp)
            / (sine_one_amp + sine_two_amp + sine_three_amp) * variety;

        HSV hsv = rgb_matrix_config.hsv;

        hsv.h = (hsv.h - (int)(50 * val)) % 256;
        hsv.s += (255 - hsv.s) * val;
        hsv.v -= hsv.v * val;

        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);

        timer += rgb_matrix_config.speed / 2048.0;

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif
