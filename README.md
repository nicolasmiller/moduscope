moduscope
----------
A small C++ library of signal measurement functions as commonly found
on digital oscilloscopes.

Usage
-------------
```c++
#include "moduscope.h"

double sample_rate_hz = 44100;
size_t signal_length = 1024;
double* my_cool_signal = acquireCoolSignal(signal_length, sample_rate_hz);

moduscope::freq(my_cool_signal, signal_length, sample_rate_hz);
moduscope::ac_rms(my_cool_signal, signal_length);
moduscope::avg_pos_duty(test_signal_a, signal_size);
```

Dependencies
-------------
- g++ (or some other reasonable c++ compiler)
- make (to build and run tests)

Run the tests
-------------
The tests are built with the Catch framework (https://github.com/philsquared/Catch).

They're located in in test_moduscope.cpp.

Run them as follows:
```bash
make test
```

Functions
-------------
All public functions expect at least a buffer of samples and a buffer length.
Some also expect a sample rate and/or voltage range when that makes sense.

```c++
// Integer cycle count based on midpoint crossing count.
int cycles(const double* samples, size_t count);

// Frequency in hertz based on midpoint crossing count.
double freq(const double* samples, size_t count);

// Period in seconds based on midpoint crossing count.
double period(const double* samples, size_t count, double sample_rate_hz);

// Average positive duty cycle in % 0-100.
double avg_pos_duty(const double* samples, size_t count);

// Average negative duty cycle in % 0-100.
double avg_neg_duty(const double* samples, size_t count);

// Average positive duty cycle width in seconds.
double avg_pos_width(const double* samples, size_t count, double sample_rate_hz);

// Average negative duty cycle width in seconds.
double avg_neg_duty(const double* samples, size_t count, double sample_rate_hz);

// Average rise time in seconds from 10% above settled low to 10% below settled high.
double avg_rise_time(const double* samples, size_t count, double voltage_range, double sample_rate_hz);

// Average fall time in seconds from 10% below settled high to 10% above settled low.
double avg_fall_time(const double* samples, size_t count, double voltage_range, double sample_rate_hz);

// Absolute maximum value.
double max(const double* samples, size_t count);

// Absolute minimum value.
double min(const double* samples, size_t count);

// Midpoint between max and min.
double midpoint(const double* samples, size_t count);

// Simple average.
double average(const double* samples, size_t count);

// Difference between max and min.
double peak_to_peak(const double* samples, size_t count);

// Most common high value based on histogram assuming 14-bit vertical binning resolution.
double settled_high(const double* samples, size_t count, double voltage_range);

// Most common low value based on histogram assuming 14-bit vertical binning resolution.
double settled_low(const double* samples, size_t count, double voltage_range);

// Midpoint between settled high and settled low.
double middle(const double* samples, size_t count, double voltage_range);

// Half the difference between settled high and settled low.
double amplitude(const double* samples, size_t count, double voltage_range);

// AC root mean square.
double ac_rms(const double* samples, size_t count);

// DC root mean square.
double dc_rms(const double* samples, size_t count);

// Overshoot % 0-100 defined as `(peak_to_peak() / 2 - amplitude()) / amplitude()`.
double overshoot(const double* samples, size_t count);

// Rise overshoot % 0-100 defined as `(max() - settled_high()) / amplitude()`.
double rise_overshoot(const double* samples, size_t count);

// Fall overshoot % 0-100 defined as `(settled_low() - min()) / amplitude()`.
double fall_overshoot(const double* samples, size_t count);
```

License
-------
MIT. See ./LICENSE for complete text.
