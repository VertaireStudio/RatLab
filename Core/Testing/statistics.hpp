/************************************/
/*         statistics.hpp           */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/essentials.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

// The Statistics type: The estimation machinery behind a benchmark report, modeled after
// Criterion's statistics module.
//
// Nothing here measures anything. A benchmark hands over the samples it collected and gets
// estimates back. Every estimate is a point plus an interval around it, because a single
// number out of a hundred samples says nothing about how far the next run would move it.
//
// The intervals are bootstrapped: the samples are resampled with replacement as often as
// asked for, the estimator is evaluated on every resample, and the bounds are the
// percentiles of the resulting distribution. The generator is seeded from the data itself,
// so the same samples always produce the same interval and two reports stay comparable.
//
// A comparison against a baseline works the same way, with one difference: both sets of
// samples are resampled first, and the change is then taken between every pair of estimates.
// Counting how many of those changes land on either side of zero is what turns a wall of
// numbers into a p-value.
class Statistics {
    public:
    // A single estimate: the value itself, together with the interval the samples place
    // around it.
    struct Estimate {
        // The value estimated from the samples themselves.
        double point = 0.0;
        // The lower bound: below it, the value would be surprising.
        double lower = 0.0;
        // The upper bound: above it, the value would be surprising.
        double upper = 0.0;
    };

    // The outcome of comparing a measurement against the samples of a baseline.
    struct Comparison {
        // The change as a fraction of the baseline: 0.05 is five percent more. A negative
        // change is a faster benchmark, since a benchmark is faster when it takes less time.
        Estimate change;
        // How far the change is from being noise: near one is noise, near zero is not. It is
        // read off the bootstrapped changes by counting how many of them land on either side of
        // zero, which is why a change of a size this routine has not seen before comes out as
        // a probability at the bottom of the range rather than as an exact zero.
        double p_value = 1.0;
        // Whether the change can be told apart from noise at all.
        bool significant = false;
    };

    // What a comparison amounts to once its noise has been accounted for. Benchmarks are
    // faster when they take less time, so a negative change is an improvement.
    enum class Verdict {
        no_change,
        improved,
        regressed,
    };
    /*-------------------------------------------------------------------------------*/

    // ── Estimates ───────────────────────────────────────────────────────────────────────────

    // Returns the average of the given samples. An empty set averages zero.
    func static double mean(const std::vector<double> &p_data) {
        if (p_data.empty()) {
            return 0.0;
        }
        double total = 0.0;
        for (const double value : p_data) {
            total += value;
        }
        return total / static_cast<double>(p_data.size());
    }

    // Returns how far the given samples are spread around their average, corrected for the
    // sample size ('Bessel's correction'), so that a spread is never read off a single
    // sample. Less than two samples have none.
    func static double deviation(const std::vector<double> &p_data) {
        if (p_data.size() < 2) {
            return 0.0;
        }
        const double average = mean(p_data);
        double total = 0.0;
        for (const double value : p_data) {
            const double difference = value - average;
            total += difference * difference;
        }
        return std::sqrt(total / static_cast<double>(p_data.size() - 1));
    }

    // Returns the middle of the given samples, which is the value half of them are below.
    // An empty set has none.
    static double median(const std::vector<double> &p_data) {
        if (p_data.empty()) {
            return 0.0;
        }
        std::vector<double> sorted(p_data);
        std::sort(sorted.begin(), sorted.end());
        const std::size_t middle = sorted.size() / 2;
        if (sorted.size() % 2 == 1) {
            return sorted[middle];
        }
        return (sorted[middle - 1] + sorted[middle]) * 0.5;
    }

    // Returns the median of how far the given samples are from their median. It says the
    // same about the spread as 'deviation' does, but a single wild sample cannot move it,
    // which is exactly what an interval around a benchmark needs.
    static double absolute_deviation(const std::vector<double> &p_data) {
        if (p_data.empty()) {
            return 0.0;
        }
        const double middle = median(p_data);
        std::vector<double> differences;
        differences.reserve(p_data.size());
        for (const double value : p_data) {
            differences.push_back(std::fabs(value - middle));
        }
        return median(differences);
    }

    // Returns the given samples with the estimated value and the interval around it. The
    // interval is the given share of the bootstrapped distribution of the estimator, which
    // for a confidence level of 0.95 leaves two and a half percent on either side.
    static Estimate estimate(const std::vector<double> &p_data,
                             double (*p_estimator)(const std::vector<double> &),
                             const double p_confidence_level,
                             const unsigned long long p_nresamples) {
        Estimate result;
        if (p_data.empty()) {
            return result;
        }
        result.point = p_estimator(p_data);

        Generator generator(p_data.size(), 0ull);
        std::vector<double> resampled;
        resampled.reserve(p_nresamples);
        for (unsigned long long round = 0; round < p_nresamples; ++round) {
            resampled.push_back(p_estimator(draw(p_data, p_data.size(), generator)));
        }
        std::sort(resampled.begin(), resampled.end());

        const double tail = (1.0 - p_confidence_level) * 0.5;
        result.lower = percentile(resampled, tail);
        result.upper = percentile(resampled, 1.0 - tail);
        return result;
    }

    // Returns the indexes of the samples which sat too far away from the middle to be part of
    // the distribution the rest of them came from. Both ends are kept: a sample which is far
    // too fast is as much a measurement of something else as one which is far too slow.
    //
    // Two conditions have to hold for a sample to be one, and both of them are needed. The
    // first is the distance from the middle measured in units of the median absolute
    // deviation, which is a scale a single wild sample cannot inflate. The second is a
    // relative distance, which is what keeps a benchmark whose samples are quantized by the
    // clock from reporting every sample which happens to land one step above the middle as an
    // outlier of a distribution which has no outliers at all.
    static std::vector<std::size_t> outliers(const std::vector<double> &p_data) {
        std::vector<std::size_t> result;
        if (p_data.size() < MINIMUM_SAMPLES) {
            return result;
        }

        const double middle = median(p_data);
        const double spread = absolute_deviation(p_data) * MAD_SCALE;
        if (spread <= 0.0) {
            // Every sample is the same number, so there is no spread to be far away from.
            return result;
        }

        for (std::size_t index = 0; index < p_data.size(); ++index) {
            const double distance = std::fabs(p_data[index] - middle);
            if (distance / spread > OUTLIER_THRESHOLD &&
                distance > middle * OUTLIER_FRACTION) {
                result.push_back(index);
            }
        }
        return result;
    }

    // Returns the index of the sample which sits the furthest away from the middle, which
    // is the one worth looking at first when a benchmark misbehaves.
    static std::size_t worst_sample(const std::vector<double> &p_data) {
        if (p_data.empty()) {
            return 0;
        }
        const double middle = median(p_data);
        std::size_t worst = 0;
        double furthest = 0.0;
        for (std::size_t index = 0; index < p_data.size(); ++index) {
            const double distance = std::fabs(p_data[index] - middle);
            if (index == 0 || distance > furthest) {
                furthest = distance;
                worst = index;
            }
        }
        return worst;
    }
    /*-------------------------------------------------------------------------------*/

    // ── Comparison ──────────────────────────────────────────────────────────────────────────

    // Compares a measurement against the samples of a baseline and returns how far it moved,
    // how sure that movement is, and whether it is large enough to be believed.
    //
    // The change is positive when the measurement got slower. It is counted as significant
    // when its p-value falls below the significance level and it is larger than the noise
    // threshold, which is the test any difference between two runs has to pass before it
    // is treated as a change rather than as noise.
    static Comparison compare(const std::vector<double> &p_current,
                              const std::vector<double> &p_baseline,
                              const double p_noise_threshold,
                              const double p_significance_level,
                              const unsigned long long p_nresamples,
                              const double p_confidence_level) {
        Comparison result;
        if (p_current.empty() || p_baseline.empty()) {
            return result;
        }

        const double current = mean(p_current);
        const double baseline = mean(p_baseline);
        if (baseline <= 0.0) {
            return result;
        }
        result.change.point = (current - baseline) / baseline;

        // Both sides are bootstrapped first, and the change is then taken between every
        // pair of estimates. How many of those land on either side of zero is the p-value,
        // which needs no closed form to count.
        const std::vector<double> resampled_current = resample_estimates(p_current, p_nresamples, 2ull);
        const std::vector<double> resampled_baseline = resample_estimates(p_baseline, p_nresamples, 3ull);

        std::vector<double> changes;
        changes.reserve(resampled_current.size());
        for (std::size_t index = 0; index < resampled_current.size(); ++index) {
            const double reference = resampled_baseline[index];
            if (reference > 0.0) {
                changes.push_back((resampled_current[index] - reference) / reference);
            }
        }
        if (changes.empty()) {
            return result;
        }

        std::sort(changes.begin(), changes.end());
        const double tail = (1.0 - p_confidence_level) * 0.5;
        result.change.lower = percentile(changes, tail);
        result.change.upper = percentile(changes, 1.0 - tail);

        std::size_t faster = 0, slower = 0;
        for (const double change : changes) {
            if (change > 0.0) {
                ++slower;
            } else if (change < 0.0) {
                ++faster;
            }
        }
        const double count = static_cast<double>(changes.size());
        result.p_value = std::min(1.0, 2.0 * std::min(static_cast<double>(faster),
                                                       static_cast<double>(slower)) / count);
        result.significant = result.p_value < p_significance_level &&
                             std::fabs(result.change.point) >= p_noise_threshold;
        return result;
    }

    // Returns what the given comparison amounts to: nothing at all inside the noise
    // threshold, an improvement or a regression once the change is significant and larger
    // than that threshold.
    static Verdict verdict(const Comparison &p_comparison, const double p_noise_threshold) {
        if (!p_comparison.significant || std::fabs(p_comparison.change.point) < p_noise_threshold) {
            return Verdict::no_change;
        }
        return p_comparison.change.point > 0.0 ? Verdict::regressed : Verdict::improved;
    }
    /*-------------------------------------------------------------------------------*/

    private:
    // How far from the middle, as a multiple of the median absolute deviation, a sample has
    // to sit before it stops being part of the distribution it came from. The scale turns
    // the median absolute deviation of a normal distribution into its standard deviation,
    // and 3.5 is the usual cut-off of the resulting score.
    static constexpr const double OUTLIER_THRESHOLD = 3.5;
    static constexpr const double MAD_SCALE = 1.4826;
    // How far from the middle a sample has to sit, as a share of the middle itself, before the
    // distance is worth the reader's attention even when the spread says it is rare.
    static constexpr const double OUTLIER_FRACTION = 0.05;
    // The fewest samples an outlier may be picked out of: less than this is a measurement,
    // not a distribution, and its furthest sample is not an outlier but the only sample.
    static constexpr const std::size_t MINIMUM_SAMPLES = 4;
    /*-------------------------------------------------------------------------------*/

    // A xorshift generator, which is all the bootstrap needs: it has to hand out every
    // sample of a set about equally often, not to be random in any other sense.
    class Generator {
        public:
        // Constructor. The salt keeps two sets of samples of the same size from being
        // resampled along the same draws, which would compare identical noise to identical
        // noise and find no change at all.
        // NOTE: Not 'func' - the state is carried from one draw to the next.
        Generator(const std::size_t p_size, const unsigned long long p_salt) {
            state = 0x9E3779B97F4A7C15ull ^
                    (static_cast<unsigned long long>(p_size) * 0xBF58476D1CE4E5B9ull) ^
                    (p_salt * 0x94D049BB133111EBull) ^
                    0xD1B54A32D192ED03ull;
            // A xorshift sequence started from zero stays there forever.
            if (state == 0ull) {
                state = 0x9E3779B97F4A7C15ull;
            }
        }

        // Returns the next value of the sequence.
        // NOTE: Not 'func' - the state is carried from one draw to the next.
        unsigned long long next() {
            state ^= state << 13;
            state ^= state >> 7;
            state ^= state << 17;
            return state;
        }

        // Returns a value below the given bound.
        // NOTE: Not 'func' - the state is carried from one draw to the next.
        unsigned long long below(const unsigned long long p_bound) {
            return p_bound > 0 ? next() % p_bound : 0ull;
        }
        /*-------------------------------------------------------------------------------*/

        private:
        // The current state of the sequence.
        unsigned long long state = 0x9E3779B97F4A7C15ull;
    };
    /*-------------------------------------------------------------------------------*/

    // Returns the value at the given fraction of the sorted samples, 0 being the smallest
    // and 1 the largest. A fraction which lands between two samples is interpolated, so the
    // bounds move smoothly with the confidence level instead of jumping by a whole sample.
    static double percentile(const std::vector<double> &p_sorted, const double p_fraction) {
        if (p_sorted.empty()) {
            return 0.0;
        }
        if (p_sorted.size() == 1) {
            return p_sorted[0];
        }
        const double position = std::min(std::max(p_fraction, 0.0), 1.0) *
                                static_cast<double>(p_sorted.size() - 1);
        const std::size_t lower = static_cast<std::size_t>(position);
        const std::size_t upper = lower + 1 < p_sorted.size() ? lower + 1 : lower;
        return p_sorted[lower] +
               (p_sorted[upper] - p_sorted[lower]) * (position - static_cast<double>(lower));
    }

    // Returns one resample of the given data: as many values as asked for, drawn from the
    // data itself with replacement.
    static std::vector<double> draw(const std::vector<double> &p_data, const std::size_t p_amount,
                                    Generator &r_generator) {
        std::vector<double> resampled;
        resampled.reserve(p_amount);
        for (std::size_t index = 0; index < p_amount; ++index) {
            resampled.push_back(p_data[r_generator.below(p_data.size())]);
        }
        return resampled;
    }

    // Returns the indexes of one resample of as many samples of a set of the given size.
    static std::vector<std::size_t> draw(const std::size_t p_data_size, Generator &r_generator) {
        std::vector<std::size_t> indexes;
        indexes.reserve(p_data_size);
        for (std::size_t index = 0; index < p_data_size; ++index) {
            indexes.push_back(static_cast<std::size_t>(r_generator.below(p_data_size)));
        }
        return indexes;
    }

    // Returns the average of as many resamples of the given data, which is the bootstrapped
    // distribution every change is then read off.
    // NOTE: Not 'func' - every resample is drawn and evaluated.
    static std::vector<double> resample_estimates(const std::vector<double> &p_data,
                                                  const unsigned long long p_nresamples,
                                                  const unsigned long long p_salt) {
        Generator generator(p_data.size(), p_salt);
        std::vector<double> estimates;
        estimates.reserve(p_nresamples);
        for (unsigned long long round = 0; round < p_nresamples; ++round) {
            estimates.push_back(mean(draw(p_data, p_data.size(), generator)));
        }
        return estimates;
    }
};