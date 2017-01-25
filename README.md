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

API Reference
-------------
All public functions expect at least a buffer of samples and a length.
Some expect a sample rate and voltage range when that makes sense.

```c++
int cycles(const double* samples, size_t count);
```
Integer cycle count based on midpoint crossing count.

```c++
double freq(const double* samples, size_t count);
```
Frequency in hertz based on midpoint crossing count.

```c++
double period(const double* samples, size_t count, double sample_rate_hz);
```
Period in seconds based on midpoint crossing count.

```c++_
double avg_pos_duty(const double* samples, size_t count);
```
Average positive duty cycle in % 0-100.

```c++_
double avg_neg_duty(const double* samples, size_t count);
```
Average negative duty cycle in % 0-100.

```c++_
double avg_pos_width(const double* samples, size_t count, double sample_rate_hz);
```
Average positive duty cycle width in seconds.

```c++_
double :avg_neg_duty(const double* samples, size_t count, double sample_rate_hz);
```
Average negative duty cycle width in seconds.

```c++_
double avg_rise_time(const double* samples, size_t count, double voltage_range, double sample_rate_hz);
```
Average rise time in seconds from 10% above settled low to 10% below settled high.

```c++_
double avg_fall_time(const double* samples, size_t count, double voltage_range, double sample_rate_hz);
```
Average fall time in seconds from 10% below settled high to 10% above settled low.

```c++_
double max(const double* samples, size_t count);
```
Absolute maximum value.

```c++_
double min(const double* samples, size_t count);
```
Absolute minimum value.

```c++_
double midpoint(const double* samples, size_t count);
```
Midpoint between max and min.

```c++_
double average(const double* samples, size_t count);
```
Simple average.

```c++_
double peak_to_peak(const double* samples, size_t count);
```
Difference between max and min.

```c++_
double settled_high(const double* samples, size_t count, double voltage_range);
```
Most common high value based on histogram assuming 14-bit vertical binning resolution.

```c++_
double settled_low(const double* samples, size_t count, double voltage_range);
```
Most common low value based on histogram assuming 14-bit vertical binning resolution.

```c++_
double middle(const double* samples, size_t count, double voltage_range);
```
Midpoint between settled high and settled low.

```c++_
double amplitude(const double* samples, size_t count, double voltage_range);
```
Half the difference between settled high and settled low.

```c++_
double ac_rms(const double* samples, size_t count);
```
AC root mean square.

```c++_
double dc_rms(const double* samples, size_t count);
```
DC root mean square.

```c++_
double overshoot(const double* samples, size_t count);
```
Overshoot % 0-100 defined as `(peak_to_peak() / 2 - amplitude()) / amplitude()`.

```c++_
double rise_overshoot(const double* samples, size_t count);
```
Rise overshoot % 0-100 defined as `(max() - settled_high()) / amplitude()`.

```c++_
double fall_overshoot(const double* samples, size_t count);
```
Fall overshoot % 0-100 defined as `(settled_low() - min()) / amplitude()`.

License
-------
MIT. See ./LICENSE for complete text.
