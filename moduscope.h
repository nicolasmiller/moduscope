#ifndef MODUSCAPE_H
#define MODUSCAPE_H

#include <cstddef>

namespace moduscope {
    static double histogram_bin_size(double range);

    int cycles(const double* samples, size_t count);
    double freq(const double* samples, size_t count,
		double sample_rate_hz);
    double period(const double* samples, size_t count,
		  double sample_rate_hz);

    double avg_pos_duty(const double* samples, size_t count);
    double avg_neg_duty(const double* samples, size_t count);
    double avg_pos_width(const double* samples, size_t count,
		     double sample_rate_hz);
    double avg_neg_width(const double* samples, size_t count,
		     double sample_rate_hz);

    double avg_rise_time(const double* samples, size_t count,
			     double voltage_range, double sample_rate_hz);
    double avg_fall_time(const double* samples, size_t count,
			     double voltage_range, double sample_rate_hz);

    double max(const double* samples, size_t count);
    double min(const double* samples, size_t count);
    double midpoint(const double* samples, size_t count);
    double average(const double* samples, size_t count);
    double peak_to_peak(const double* samples, size_t count);

    double settled_high(const double* samples, size_t count, double voltage_range);
    double settled_low(const double* samples, size_t count, double voltage_range);
    double middle(const double* samples, size_t count, double voltage_range);
    double amplitude(const double* samples, size_t count, double voltage_range);

    double ac_rms(const double* samples, size_t count);
    double dc_rms(const double* samples, size_t count);

    double overshoot(const double* samples, size_t count, double voltage_range);
    double rise_overshoot(const double* samples, size_t count, double voltage_range);
    double fall_overshoot(const double* samples, size_t count, double voltage_range);
}

#endif
