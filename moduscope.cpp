#include "moduscope.h"
#include <cmath>
#include <map>
#include <array>
#include <limits>
#include <iostream>

namespace moduscope {

    static double histogram_bin_size(double range) {
	return range / (2 << 13);
    }

    int cycles(const double* samples, size_t count) {
	double mp = midpoint(samples, count);
	bool above_midpoint = samples[0] > mp;
	int midpoint_count = 0;
	for(size_t j = 0; j < count; j++) {
	    if(above_midpoint && samples[j] <= mp) {
		midpoint_count++;
		above_midpoint = false;
	    }
	    else if(!above_midpoint && samples[j] > mp) {
		midpoint_count++;
		above_midpoint = true;
	    }
	}
	return std::ceil(midpoint_count / 3.0);
    }

    double freq(const double* samples, size_t count,
		double sample_rate_hz) {
	return 1 / period(samples, count, sample_rate_hz);
    }

    double period(const double* samples, size_t count,
		  double sample_rate_hz) {
	double mp = midpoint(samples, count);
	bool above_midpoint = samples[0] > mp;
	size_t midpoint_indices[] = {0, 0, 0};
	size_t midpoint_index = 0;
	for(size_t j = 0; j < count; j++) {
	    if(above_midpoint && samples[j] <= mp) {
		midpoint_indices[midpoint_index++] = j;
		above_midpoint = false;
	    }
	    else if(!above_midpoint && samples[j] > mp) {
		midpoint_indices[midpoint_index++] = j;
		above_midpoint = true;
	    }
	    if(midpoint_index > 2) {
		break;
	    }
	}
	return (midpoint_indices[2] - midpoint_indices[0]) * (1 / sample_rate_hz);
    }

    static double duty(const double* samples, size_t count, bool positive = true) {
	double mp = midpoint(samples, count);
	bool above_midpoint = samples[0] > mp;
	int midpoint_count = 0;
	int cycle_count = -1;
	int samples_in_cycle = 0;
	int target_samples_in_cycle = 0;
	double cycle_sum = 0.0;

	for(size_t j = 0; j < count; j++) {
	    samples_in_cycle++;
	    if(positive ?
	       samples[j] >= 0 :
	       samples[j] < 0) {
		target_samples_in_cycle++;
	    }
	    if((above_midpoint && samples[j] < mp) ||
	       (!above_midpoint && samples[j] >= mp)) {
		    midpoint_count++;
		    above_midpoint = !above_midpoint;
		    if(midpoint_count % 2 == 1) {
			cycle_count += 1;
			if(cycle_count > 0) {
			    cycle_sum += ((double) target_samples_in_cycle) / samples_in_cycle;
			}
			target_samples_in_cycle = above_midpoint ? 1 : 0;
			samples_in_cycle = 1;
		    }
	    }
	}
	return 100 * (cycle_sum / cycle_count);
    }

    double avg_pos_duty(const double* samples, size_t count) {
	return duty(samples, count);
    }

    double avg_neg_duty(const double* samples, size_t count) {
	return duty(samples, count, false);
    }

    double avg_pos_width(const double* samples, size_t count,
		     double sample_rate_hz) {
	return (avg_pos_duty(samples, count) *
		period(samples, count, sample_rate_hz)) / 100;
    }

    double avg_neg_width(const double* samples, size_t count,
		     double sample_rate_hz) {
	return (avg_neg_duty(samples, count) *
		period(samples, count, sample_rate_hz)) / 100;
    }


    double avg_rise_time(const double* samples, size_t count,
		     double voltage_range, double sample_rate_hz) {
	double high = settled_high(samples, count,
				   voltage_range);
	double low = settled_low(samples, count,
				 voltage_range);
	double delta = (high - low) * 0.1;

	high -= delta;
	low += delta;

	int j = 0;
	int rise_cycles = 0;
	int rise_length_sum = 0;

	while(j < count) {
	    while(j < count && !(samples[j] < low)) {
		j++;
	    }

	    while(j < count && samples[j] < low) {
		j++;
	    }

	    int rise_length = 0;
	    while(j < count && samples[j] < high) {
		rise_length++;
		j++;
	    }
	    rise_cycles++;
	    rise_length_sum += rise_length;
	    j++;
	}

	return (((double) rise_length_sum) / rise_cycles) * (1 / sample_rate_hz);
    }

    double avg_fall_time(const double* samples, size_t count,
		     double voltage_range, double sample_rate_hz) {
	double high = settled_high(samples, count,
				   voltage_range);
	double low = settled_low(samples, count,
				 voltage_range);
	double delta = (high - low) * 0.1;

	high -= delta;
	low += delta;

	int j = 0;
	int fall_cycles = 0;
	int fall_length_sum = 0;

	while(j < count) {
	    while(j < count && !(samples[j] > high)) {
		j++;
	    }

	    while(j < count && samples[j] > high) {
		j++;
	    }

	    int fall_length = 0;
	    while(j < count && samples[j] > low) {
		fall_length++;
		j++;
	    }
	    fall_cycles++;
	    fall_length_sum += fall_length;
	    j++;
	}

	return (((double) fall_length_sum) / fall_cycles) * (1 / sample_rate_hz);
    }

    double max(const double* samples, size_t count) {
	double current_max = std::numeric_limits<double>::min();
	for(size_t i = 0; i < count; i++) {
	    current_max = (samples[i] > current_max) ? samples[i] : current_max;
	}
	return current_max;
    }

    double min(const double* samples, size_t count) {
	double current_min = std::numeric_limits<double>::max();
	for(size_t i = 0; i < count; i++) {
	    current_min = (samples[i] < current_min) ? samples[i] : current_min;
	}
	return current_min;
    }

    double midpoint(const double* samples, size_t count) {
	return (max(samples, count) + min(samples, count)) / 2.0;
    }

    double average(const double* samples, size_t count) {
	double sum = 0;
	for(size_t i = 0; i < count; i++) {
	    sum += samples[i];
	}
	return sum / count;
    }

    double peak_to_peak(const double* samples, size_t count) {
	return (max(samples, count) -
		min(samples, count));
    }

    double settled_high(const double* samples, size_t count,
			double voltage_range) {
	double maximum = max(samples, count);
	double minimum = min(samples, count);
	double midpoint = (maximum + minimum) / 2.0;

	std::map<size_t, int> hist;
	double bin_size = histogram_bin_size(voltage_range);
	int bin_count = std::ceil(voltage_range / bin_size / 2);
	for(size_t i = 0; i < bin_count; i++) {
	    hist[i] = 0;
	}
	for(size_t i = 0; i < count; i++) {
	    if(samples[i] >= midpoint) {
		hist[samples[i] / bin_size] += 1;
	    }
	}

	double m = std::numeric_limits<double>::min();
	size_t most_frequent = 0;
	for(auto const& pair : hist) {
	    if(pair.second > m) {
		most_frequent = pair.first;
		m = pair.second;
	    }
	}

	return bin_size * most_frequent;
    }

    double settled_low(const double* samples, size_t count,
		       double voltage_range) {
	double maximum = max(samples, count);
	double minimum = min(samples, count);
	double midpoint = (maximum + minimum) / 2.0;

	std::map<size_t, int> hist;
	double bin_size = histogram_bin_size(voltage_range);
	int bin_count = std::ceil(voltage_range / bin_size / 2);
	for(size_t i = 0; i < bin_count; i++) {
	    hist[i] = 0;
	}
	for(size_t i = 0; i < count; i++) {
	    if(samples[i] < midpoint) {
		hist[std::abs(samples[i]) / bin_size] += 1;
	    }
	}

	double m = std::numeric_limits<double>::min();
	size_t most_frequent = 0;
	for(auto const& pair : hist) {
	    if(pair.second > m) {
		most_frequent = pair.first;
		m = pair.second;
	    }
	}

	return -bin_size * most_frequent;
    }

    double middle(const double* samples, size_t count,
		  double voltage_range) {
	return (settled_high(samples, count, voltage_range) +
		settled_low(samples, count, voltage_range)) / 2;
    }

    double amplitude(const double* samples, size_t count,
		     double voltage_range) {
	return (settled_high(samples, count, voltage_range) -
		settled_low(samples, count, voltage_range)) / 2;
    }

    double ac_rms(const double* samples, size_t count) {
	double v = 0;
	double avg = average(samples, count);
	for(size_t i = 0; i < count; i++) {
	    v += pow(samples[i] - avg, 2);
	}
	v /= count;
	v = sqrt(v);
	return v;
    }

    double dc_rms(const double* samples, size_t count) {
	double v = 0;
	for(size_t i = 0; i < count; i++) {
	    v += pow(samples[i], 2);
	}
	v /= count;
	v = sqrt(v);
	return v;
    }

    double overshoot(const double* samples, size_t count,
		     double voltage_range) {
	double v = amplitude(samples, count, voltage_range);
	if(std::isnan(v) || v == 0)
	    return 0;
	return 100 * (peak_to_peak(samples, count) / v / 2 - 1);
    }

    double rise_overshoot(const double* samples, size_t count,
			  double voltage_range) {
	double v = amplitude(samples, count, voltage_range);
	if(std::isnan(v) || v == 0)
	    return 0;
	return 100 * (max(samples, count) -
		      settled_high(samples, count, voltage_range)) / v;
    }

    double fall_overshoot(const double* samples, size_t count,
			  double voltage_range) {
	double v = amplitude(samples, count, voltage_range);
	if(std::isnan(v) || v == 0)
	    return 0;
	return 100 * (settled_low(samples, count, voltage_range) -
		      min(samples, count)) / v;
    }
}
