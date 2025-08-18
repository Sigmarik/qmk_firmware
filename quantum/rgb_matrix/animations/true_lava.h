#ifdef ENABLE_RGB_MATRIX_TRUE_LAVA
RGB_MATRIX_EFFECT(TRUE_LAVA)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static uint64_t timer = 0;

static float gen_ripples(uint8_t index)
{
    uint8_t count = g_last_hit_tracker.count;

    float ripple = 0.0f;

    for (uint8_t j = 0; j < count; j++) {
        int16_t  dx   = g_led_config.point[index].x - g_last_hit_tracker.x[j];
        int16_t  dy   = g_led_config.point[index].y - g_last_hit_tracker.y[j];
        uint8_t  dist = sqrt16(dx * dx + dy * dy);
        float distance = (float)dist / 256.0;
        float tick = (float)g_last_hit_tracker.tick[j] / 256.0 / 5.0;

        const float maxDistance = 0.3125;
        float currentRipple = expf(-409.6 * (distance - tick) * (distance - tick)) * (1 - distance / maxDistance);

        if (distance <= maxDistance) {
            ripple = fmax(ripple, currentRipple);
        }
    }

    return ripple;
}

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

    float variety = rgb_matrix_config.variety / 256.0;

    if (params->init) {
        timer = 0;
    }

    static uint32_t s_last_timer_value = 0;

    if (s_last_timer_value != 0)
        timer += g_rgb_timer - s_last_timer_value;
    s_last_timer_value = g_rgb_timer;

    double scaled_timer = timer / 1000.0f * rgb_matrix_config.speed;

    // The prophecy foretold that this number will result in minimal jitters
    timer %= 5716620ll * 1000ll;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        int dx = g_led_config.point[i].x - k_rgb_matrix_center.x;
        int dy = g_led_config.point[i].y - k_rgb_matrix_center.y;

        float val = (
            pow(fabs(sin(scaled_timer * sine_one_w - dx * sine_one_kx - dy * sine_one_ky * dy + sine_one_phi0)), 2.0) * sine_one_amp
            + pow(fabs(sin(scaled_timer * sine_two_w - dx * sine_two_kx - dy * sine_two_ky * dy + sine_two_phi0)), 2.0) * sine_two_amp
            + pow(fabs(sin(scaled_timer * sine_three_w - dx * sine_three_kx - dy * sine_three_ky * dy + sine_three_phi0)), 2.0) * sine_three_amp)
            / (sine_one_amp + sine_two_amp + sine_three_amp) * variety;

        float ripples = gen_ripples(i);
        float ripple_peak = fmin(fmax(variety * 2.0, 0.2), 1.0);
        val = val * (1.0 - ripples) + (ripple_peak - val) * ripples;

        HSV hsv = rgb_matrix_config.hsv;

        hsv.h = (hsv.h - (int)(50 * val)) % 256;
        hsv.s += (255 - hsv.s) * val;
        hsv.v -= hsv.v * val;

        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);

        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif
