#ifndef OJ_SCORECALCULATOR_H
#define OJ_SCORECALCULATOR_H

namespace OJ {

class ScoreCalculator {
public:
    // Computes minutes used from start and end timestamps.
    // Uses ceil(seconds/60) so partial minutes still count.
    static int minutesUsed(long long startEpoch, long long endEpoch) {
        if (startEpoch <= 0 || endEpoch <= 0) return 0;
        long long elapsed = endEpoch - startEpoch;
        if (elapsed < 0) elapsed = 0;
        int minutes = (int)((elapsed + 59) / 60);
        if (minutes < 0) minutes = 0;
        return minutes;
    }

    // Example formula (per Steps.md):
    // score = difficulty * 100 - wrongAttempts * 10 - minutesUsed
    // Here, basePoints is typically sum(difficulty*100 for solved problems).
    static int computeScore(int basePoints, int wrongAttempts, int minutesUsed) {
        int score = basePoints - wrongAttempts * 10 - minutesUsed;
        if (score < 0) score = 0;
        return score;
    }

    // Wrong attempts: if solved, attempts beyond the first; else all attempts.
    static int computeWrongAttempts(int attempts, bool solved) {
        if (attempts <= 0) return 0;
        if (solved) {
            return (attempts > 1) ? (attempts - 1) : 0;
        }
        return attempts;
    }
};

}

#endif
