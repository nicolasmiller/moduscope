#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "moduscope.h"
#include <iostream>
#include <fstream>

static double* test_signal_a;
static const double signal_a_expected_freq = 1000.0;
static double* test_signal_b;
static const double signal_b_expected_freq = 666.3822525597;
static size_t signal_size;
const static double sample_rate_hz = 1562000.0; // 1.562 MHz

TEST_CASE( "computes cycle count", "moduscape::cycles" ) {
    REQUIRE(moduscope::cycles(test_signal_a, signal_size) == 4);
    REQUIRE(moduscope::cycles(test_signal_b, signal_size) == 2);
}

TEST_CASE( "computes frequency (hz)",
	   "moduscape::freq" ) {
    REQUIRE(moduscope::freq(test_signal_a,
			    signal_size,
			    sample_rate_hz) == Approx(signal_a_expected_freq));
    REQUIRE(moduscope::freq(test_signal_b,
			    signal_size,
			    sample_rate_hz) == Approx(signal_b_expected_freq));
}

TEST_CASE( "computes period (s)",
	   "moduscape::period" ) {
    REQUIRE(moduscope::period(test_signal_a,
			      signal_size,
			      sample_rate_hz) == Approx(1 / signal_a_expected_freq));
    REQUIRE(moduscope::period(test_signal_b,
			      signal_size,
			      sample_rate_hz) == Approx(1 / signal_b_expected_freq));
}

TEST_CASE( "computes average positive duty cycle (%)",
	   "moduscope::pos_duty" ) {
    REQUIRE(moduscope::avg_pos_duty(test_signal_a,
				    signal_size) == Approx(50.0).epsilon(0.1));
    REQUIRE(moduscope::avg_pos_duty(test_signal_b,
				    signal_size) == Approx(50.0).epsilon(0.1));
}

TEST_CASE( "computes average negative duty cycle (%) ",
	   "moduscape::neg_duty" ) {
    REQUIRE(moduscope::avg_neg_duty(test_signal_a,
				signal_size) == Approx(50.0).epsilon(0.1));
    REQUIRE(moduscope::avg_neg_duty(test_signal_b,
				signal_size) == Approx(50.0).epsilon(0.1));
}

TEST_CASE( "computes average positive duty cycle width (s)",
	   "moduscape::pos_width" ) {
    REQUIRE(moduscope::avg_pos_width(test_signal_a,
				     signal_size, sample_rate_hz) == Approx(500e-6));
    REQUIRE(moduscope::avg_pos_width(test_signal_b,
				     signal_size, sample_rate_hz) == Approx(750e-6));
}

TEST_CASE( "computes average negative duty cycle width (s)",
	   "moduscape::neg_width" ) {
    REQUIRE(moduscope::avg_neg_width(test_signal_a,
				signal_size, sample_rate_hz) == Approx(500e-6));
    REQUIRE(moduscope::avg_neg_width(test_signal_b,
				signal_size, sample_rate_hz) == Approx(750e-6));
}

TEST_CASE( "computes average rise_time (s)",
	   "moduscape::rise_time" ) {
    REQUIRE(moduscope::avg_rise_time(test_signal_a,
				 signal_size, 5, sample_rate_hz) == Approx(245e-6));
    REQUIRE(moduscope::avg_rise_time(test_signal_b,
				 signal_size, 5, sample_rate_hz) == Approx(1e-6));
}

TEST_CASE( "computes average fall_time (s)",
	   "moduscape::fall_time" ) {
    REQUIRE(moduscope::avg_fall_time(test_signal_a,
				     signal_size, 5, sample_rate_hz) == Approx(245e-6));
    REQUIRE(moduscope::avg_fall_time(test_signal_b,
				     signal_size, 5, sample_rate_hz) == Approx(1e-6));
}

TEST_CASE( "computes max (v)",
	   "moduscape::max" ) {
    REQUIRE(moduscope::max(test_signal_a,
				signal_size) == Approx(2).epsilon(0.001));
    REQUIRE(moduscope::max(test_signal_b,
				signal_size) == Approx(1.75).epsilon(0.001));
}

TEST_CASE( "computes min (v)",
	   "moduscape::min" ) {
    REQUIRE(moduscope::min(test_signal_a,
			   signal_size) == Approx(-2).epsilon(0.001));
    REQUIRE(moduscope::min(test_signal_b,
				signal_size) == Approx(-1.75).epsilon(0.001));
}

TEST_CASE( "computes midpoint (v)",
	   "moduscape::midpoint" ) {
    REQUIRE(moduscope::midpoint(test_signal_a,
				signal_size) == Approx(0).epsilon(0.001));
    REQUIRE(moduscope::midpoint(test_signal_b,
				signal_size) == Approx(0).epsilon(0.001));
}

TEST_CASE( "computes average (v)",
	   "moduscape::average" ) {
    REQUIRE(moduscope::midpoint(test_signal_a,
				signal_size) == Approx(0).epsilon(0.001));
    REQUIRE(moduscope::midpoint(test_signal_b,
				signal_size) == Approx(0).epsilon(0.001));
}

TEST_CASE( "computes peak to peak (v)",
	   "moduscape::peak_to_peak" ) {
    REQUIRE(moduscope::peak_to_peak(test_signal_a,
				    signal_size) == Approx(4).epsilon(0.001));
    REQUIRE(moduscope::peak_to_peak(test_signal_b,
				    signal_size) == Approx(1.75 * 2).epsilon(0.001));
}

TEST_CASE( "computes settled high voltage (v)",
	   "moduscape::settled_high" ) {
    REQUIRE(moduscope::settled_high(test_signal_a,
				    signal_size, 5) == Approx(2).epsilon(0.001));
    REQUIRE(moduscope::settled_high(test_signal_b,
			    signal_size, 5) == Approx(1.75).epsilon(0.001));
}

TEST_CASE( "computes settled low voltage (v)",
	   "moduscape::low" ) {
    REQUIRE(moduscope::settled_low(test_signal_a,
				   signal_size, 5) == Approx(-2).epsilon(0.001));
    REQUIRE(moduscope::settled_low(test_signal_b,
				   signal_size, 5) == Approx(-1.75).epsilon(0.001));
}

TEST_CASE( "computes average of settled high and settled low (v)",
	   "moduscape::middle" ) {
    REQUIRE(moduscope::middle(test_signal_a,
			      signal_size, 5) == Approx(0.0007).epsilon(0.001));
    REQUIRE(moduscope::middle(test_signal_b,
			      signal_size, 5) == Approx(0.001).epsilon(0.0001));
}

TEST_CASE( "computes half difference between settled high and settled low (v)",
	   "moduscape::amplitude" ) {
    REQUIRE(moduscope::amplitude(test_signal_a,
				 signal_size, 5) == Approx(2).epsilon(0.001));
    REQUIRE(moduscope::amplitude(test_signal_b,
				 signal_size, 5) == Approx(1.75).epsilon(0.001));
}

TEST_CASE( "computes AC rms (v)",
	   "moduscape::ac_rms" ) {
    REQUIRE(moduscope::ac_rms(test_signal_a,
			      signal_size) == Approx(1.413).epsilon(0.001));
    REQUIRE(moduscope::ac_rms(test_signal_b,
			      signal_size) == Approx(1.732).epsilon(0.001));
}

TEST_CASE( "computes DC rms (v)",
	   "moduscape::dc_rms" ) {
    REQUIRE(moduscope::dc_rms(test_signal_a,
			      signal_size) == Approx(1.412).epsilon(0.001));
    REQUIRE(moduscope::dc_rms(test_signal_b,
			      signal_size) == Approx(1.75).epsilon(0.001));
}

TEST_CASE( "computes overshoot (%)",
	   "moduscape::overshoot" ) {
    REQUIRE(moduscope::overshoot(test_signal_a,
				 signal_size, 5) == Approx(0.1).epsilon(0.1));
    REQUIRE(moduscope::overshoot(test_signal_b,
				 signal_size, 5) == Approx(0.1).epsilon(0.1));
}

TEST_CASE( "computes rise overshoot (%)",
	   "moduscape::rise_overshoot" ) {
    REQUIRE(moduscope::rise_overshoot(test_signal_a,
				      signal_size, 5) == Approx(0.1).epsilon(0.1));
    REQUIRE(moduscope::rise_overshoot(test_signal_b,
				      signal_size, 5) == Approx(0.1).epsilon(0.1));
}

TEST_CASE( "computes fall overshoot (%)",
	   "moduscape::fall_overshoot" ) {
    REQUIRE(moduscope::fall_overshoot(test_signal_a,
				      signal_size, 5) == Approx(0.1).epsilon(0.1));
    REQUIRE(moduscope::fall_overshoot(test_signal_b,
				      signal_size, 5) == Approx(0.1).epsilon(0.1));
}

int main(int argc, char* argv[])
{
    std::ifstream test_data("test_data.csv");
    std::vector<double> test_signal_a_vec;
    std::vector<double> test_signal_b_vec;
    int i = 0;
    double sample;
    while(test_data >> sample) {
	if(i % 3 == 1) {
	    test_signal_a_vec.push_back(sample);
	}
	if(i % 3 == 2) {
	    test_signal_b_vec.push_back(sample);
	}
	i++;
    }
    test_signal_a = &test_signal_a_vec[0];
    test_signal_b = &test_signal_b_vec[0];
    signal_size = test_signal_a_vec.size();

    return Catch::Session().run(argc, argv);
}
