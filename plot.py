import sys
import pandas as pd
import matplotlib.pyplot as plt


def main():
    print("Loading data")
    df = pd.read_csv(sys.stdin)

    for col in df.columns:
        df[col] = pd.to_numeric(df[col], errors="coerce")

    df["exp_fast_float_err"] = df["exp_fast_float"] - df["exp_float"]
    df["exp_fast_float_rerr"] = df["exp_fast_float_err"] / df["exp_float"]
    df["exp_very_fast_float_err"] = df["exp_very_fast_float"] - df["exp_float"]
    df["exp_very_fast_float_rerr"] = df["exp_very_fast_float_err"] / df["exp_float"]

    df["exp_float_err"] = df["exp_float"] - df["exp_double"]
    df["exp_float_rerr"] = df["exp_float_err"] / df["exp_double"]
    df["exp_fast_double_err"] = df["exp_fast_double"] - df["exp_double"]
    df["exp_fast_double_rerr"] = df["exp_fast_double_err"] / df["exp_double"]
    df["exp_very_fast_double_err"] = df["exp_very_fast_double"] - df["exp_double"]
    df["exp_very_fast_double_rerr"] = df["exp_very_fast_double_err"] / df["exp_double"]

    print("Plotting data")

    plt.style.use("seaborn-v0_8-darkgrid")
    fig, axes = plt.subplots(2, 1, figsize=(10, 10))
    fig.suptitle("Analysis of Exponential Function Results", fontsize=16)

    axes[0].plot(
        df["x_float"],
        df["exp_fast_float_rerr"],
        marker=".",
        markersize=2,
        linestyle=":",
        label="Fast Float Relative Error",
    )
    axes[0].plot(
        df["x_float"],
        df["exp_very_fast_float_rerr"],
        marker=".",
        markersize=2,
        linestyle="-.",
        label="Very Fast Float Relative Error",
    )
    axes[0].legend()

    axes[1].plot(
        df["x_double"],
        df["exp_float_rerr"],
        marker=".",
        markersize=2,
        linestyle=":",
        label="Float Relative Error",
    )
    axes[1].plot(
        df["x_double"],
        df["exp_fast_double_rerr"],
        marker=".",
        markersize=2,
        linestyle=":",
        label="Fast Double Relative Error",
    )
    axes[1].plot(
        df["x_double"],
        df["exp_very_fast_double_rerr"],
        marker=".",
        markersize=2,
        linestyle="-.",
        label="Very Fast Double Relative Error",
    )
    axes[1].legend()

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.show()


if __name__ == "__main__":
    main()
