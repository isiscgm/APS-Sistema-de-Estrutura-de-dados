
import os
import re
import numpy as np
import matplotlib.pyplot as plt


# ===========================================================================
# MODEL CATALOG
# ===========================================================================
COMPLEXITY_ORDER = [
    'O(1)', 'O(log n)', 'O(n)', 'O(n log n)',
    'O(n^1.5)', 'O(n^2)', 'O(n^3)'
]


def model_functions(n):
    n = np.asarray(n, dtype=float)

    return {
        'O(log n)': np.log(n),
        'O(n)': n,
        'O(n log n)': n * np.log(n),
        'O(n^1.5)': n ** 1.5,
        'O(n^2)': n ** 2,
        'O(n^3)': n ** 3
    }


def model_function(name, n):
    n = np.asarray(n, dtype=float)

    if name == 'O(1)':
        return np.ones_like(n)

    return model_functions(n)[name]


# ===========================================================================
# LOAD DATA
# ===========================================================================

def load_data(path):

    if not os.path.exists(path):
        raise FileNotFoundError(f"Path {path} not found.")

    if os.path.isdir(path):
        files = sorted(
            f for f in os.listdir(path)
            if f.endswith(('.txt', '.csv'))
            and f != 'estatisticas.csv'
        )

        paths = [os.path.join(path, f) for f in files]

    else:
        paths = [path]

    data_list = []

    for full_path in paths:

        try:
            raw = np.loadtxt(
                full_path,
                delimiter=',',
                comments='#',
                ndmin=2,
                skiprows=1
            )

            if raw.size == 0 or raw.shape[1] < 2:
                print(f"IGNORADO: {os.path.basename(full_path)}")
                continue

            n = raw[:, 0].astype(float)
            y = raw[:, 1].astype(float)

            mask = (
                (n > 0) &
                (y > 0) &
                np.isfinite(n) &
                np.isfinite(y)
            )

            n = n[mask]
            y = y[mask]

            if len(n) < 3:
                print(
                    f"IGNORADO: {os.path.basename(full_path)} "
                    f"- apenas {len(n)} pontos validos"
                )
                continue

            order = np.argsort(n)

            data_list.append(
                (os.path.basename(full_path), n[order], y[order])
            )

        except (ValueError, OSError, IndexError) as error:
            print(
                f"ERRO ao ler {os.path.basename(full_path)}: {error}"
            )

    return data_list


# ===========================================================================
# POWER LAW
# ===========================================================================

def fit_power_law(n, y):

    mask = (
        (n > 0) &
        (y > 0) &
        np.isfinite(n) &
        np.isfinite(y)
    )

    if mask.sum() < 2 or np.unique(n[mask]).size < 2:
        return np.nan, np.nan

    coef = np.polyfit(
        np.log(n[mask]),
        np.log(y[mask]),
        deg=1
    )

    return coef[0], np.exp(coef[1])


# ===========================================================================
# MODEL FITTING
# ===========================================================================

def fit_model(n, y, f_n, with_fixed_cost=True):

    n = np.asarray(n, dtype=float)
    y = np.asarray(y, dtype=float)
    f_n = np.asarray(f_n, dtype=float)

    mask = (
        (y > 0) &
        np.isfinite(y) &
        np.isfinite(f_n)
    )

    predicted = np.full(n.shape, np.nan)

    if mask.sum() < 2:
        return {
            'a': np.nan,
            'b': 0.0,
            'r2': 0.0,
            'relative_error': np.inf,
            'predicted': predicted
        }

    yy = y[mask]
    ff = f_n[mask]

    scale_y = yy.mean()
    scale_f = ff.mean()

    if scale_y <= 0 or not np.isfinite(scale_y):
        return {
            'a': np.nan,
            'b': 0.0,
            'r2': 0.0,
            'relative_error': np.inf,
            'predicted': predicted
        }

    if not np.isfinite(scale_f) or abs(scale_f) < 1e-15:
        scale_f = 1.0

    ys = yy / scale_y
    fs = ff / scale_f

    w = 1.0 / ys ** 2

    S1 = w.sum()
    Sf = np.sum(w * fs)
    Sff = np.sum(w * fs * fs)
    Sy = np.sum(w * ys)
    Sfy = np.sum(w * fs * ys)

    use_b = (
        with_fixed_cost and
        (np.max(fs) - np.min(fs)) > 1e-12
    )

    if use_b:

        det = Sff * S1 - Sf * Sf

        if det > 1e-9 * Sff * S1 and Sff > 0:
            a_s = (Sfy * S1 - Sy * Sf) / det
            b_s = (Sff * Sy - Sf * Sfy) / det

        else:
            a_s = Sfy / Sff if Sff > 0 else np.nan
            b_s = 0.0

        if (
            not np.isfinite(a_s) or
            not np.isfinite(b_s) or
            b_s < 0
        ):
            a_s = Sfy / Sff if Sff > 0 else np.nan
            b_s = 0.0

    else:
        a_s = Sfy / Sff if Sff > 0 else np.nan
        b_s = 0.0

    a = a_s * scale_y / scale_f
    b = b_s * scale_y

    predicted[mask] = a * f_n[mask] + b

    rel_resid = (yy - (a * ff + b)) / yy

    error = float(
        np.sqrt(np.mean(rel_resid ** 2)) * 100.0
    )

    ss_res = float(np.sum((yy - (a * ff + b)) ** 2))
    ss_tot = float(np.sum((yy - yy.mean()) ** 2))

    r2 = 1.0 - ss_res / ss_tot if ss_tot > 0 else 0.0

    return {
        'a': float(a),
        'b': float(b),
        'r2': float(r2),
        'relative_error': error,
        'predicted': predicted
    }


# ===========================================================================
# COMPLEXITY ANALYSIS
# ===========================================================================

def analyze_complexity(n, y, parsimony_margin=0.15):

    n = np.asarray(n, dtype=float)
    y = np.asarray(y, dtype=float)

    mask = (
        (n > 0) &
        (y > 0) &
        np.isfinite(n) &
        np.isfinite(y)
    )

    n = n[mask]
    y = y[mask]

    if n.size < 3:
        raise ValueError(
            "At least 3 points with n > 0 and y > 0 are required."
        )

    results = {}

    for name in COMPLEXITY_ORDER:

        f_n = model_function(name, n)

        with_b = fit_model(
            n, y, f_n,
            with_fixed_cost=True
        )

        without_b = fit_model(
            n, y, f_n,
            with_fixed_cost=False
        )

        with_b['error_without_fixed_cost'] = (
            without_b['relative_error']
        )

        results[name] = with_b

    errors = {
        m: results[m]['relative_error']
        for m in COMPLEXITY_ORDER
    }

    min_error = min(errors.values())

    if not np.isfinite(min_error):
        raise ValueError("Nao foi possivel ajustar os modelos.")

    threshold = min_error * (1.0 + parsimony_margin)

    candidates = [
        m for m in COMPLEXITY_ORDER
        if errors[m] <= threshold
    ]

    best = min(candidates, key=COMPLEXITY_ORDER.index)

    return results, best


def next_more_complex(best):

    i = COMPLEXITY_ORDER.index(best)
    rest = COMPLEXITY_ORDER[i + 1:]

    return rest[0] if rest else None


# ===========================================================================
# PLOT HELPERS
# ===========================================================================

def safe_filename(name):

    return re.sub(
        r'[^A-Za-z0-9_-]+',
        '_',
        name
    ).strip('_')


# ===========================================================================
# COMPARISON GRAPH
# ===========================================================================

def plot_comparison(results_list, title, scale='linear-linear'):

    if not results_list:
        return

    fig, ax = plt.subplots(figsize=(12, 7))

    x_scale, y_scale = scale.split('-')

    for file_name, n, y, results, best in results_list:

        r = results[best]

        ax.scatter(
            n,
            y,
            s=45,
            label=f'Dados: {file_name}'
        )

        if n.max() > n.min():
            n_line = np.logspace(
                np.log10(n.min()),
                np.log10(n.max()),
                600
            )
        else:
            n_line = np.full(600, n.min())

        fit = (
            r['a'] * model_function(best, n_line) +
            r['b']
        )

        if y_scale == 'log':
            valid = fit > 0
            ax.plot(
                n_line[valid],
                fit[valid],
                '--',
                label=(
                    f"{file_name}: {best} "
                    f"(RMS {r['relative_error']:.2f}%)"
                )
            )
        else:
            ax.plot(
                n_line,
                fit,
                '--',
                label=(
                    f"{file_name}: {best} "
                    f"(RMS {r['relative_error']:.2f}%)"
                )
            )

    ax.set_xscale(x_scale)
    ax.set_yscale(y_scale)

    ax.set_title(f"{title} - escala {scale}")
    ax.set_xlabel(f"Tamanho da entrada (n), escala {x_scale}")
    ax.set_ylabel(f"Ciclos de CPU, escala {y_scale}")

    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3, which='both')

    fig.tight_layout()

    os.makedirs("graficos", exist_ok=True)

    filename = (
        safe_filename(title) +
        "_" +
        safe_filename(scale) +
        ".png"
    )

    fig.savefig(
        os.path.join("graficos", filename),
        dpi=300,
        bbox_inches="tight"
    )

    plt.show()
    plt.close(fig)


# ===========================================================================
# RESIDUAL GRAPH
# ===========================================================================

def plot_residuals(results_list, title):

    if not results_list:
        return

    n_series = len(results_list)

    ncols = 2 if n_series > 1 else 1
    nrows = int(np.ceil(n_series / ncols))

    fig, axes = plt.subplots(
        nrows,
        ncols,
        figsize=(13, 3.9 * nrows),
        squeeze=False
    )

    for ax, (file_name, n, y, results, best) in zip(
        axes.ravel(),
        results_list
    ):

        ax.axhline(0, color='black', lw=1)

        r = results[best]

        ax.plot(
            n,
            (y - r['predicted']) / y * 100,
            'o-',
            label=f"{best}, RMS {r['relative_error']:.2f}%"
        )

        rival = next_more_complex(best)

        if rival:

            rr = results[rival]

            ax.plot(
                n,
                (y - rr['predicted']) / y * 100,
                's--',
                alpha=0.75,
                label=(
                    f"{rival}, RMS "
                    f"{rr['relative_error']:.2f}%"
                )
            )

        ax.set_xscale('log')
        ax.set_title(file_name, fontsize=10)
        ax.set_xlabel("n, escala log")
        ax.set_ylabel("Residuo relativo (%)")

        ax.grid(True, alpha=0.3, which='both')
        ax.legend(fontsize=8)

    for ax in axes.ravel()[n_series:]:
        ax.axis('off')

    fig.suptitle(title)
    fig.tight_layout()

    os.makedirs("graficos", exist_ok=True)

    fig.savefig(
        "graficos/residuos.png",
        dpi=300,
        bbox_inches="tight"
    )

    plt.show()
    plt.close(fig)


# ===========================================================================
# MAIN
# ===========================================================================

if __name__ == "__main__":

    os.makedirs("graficos", exist_ok=True)

    PATH = "experiment_data/"

    try:

        raw_list = load_data(PATH)

        if not raw_list:
            raise RuntimeError(
                "Nenhum CSV com pelo menos 3 pontos validos encontrado."
            )

        final_list = []

        for name, n, y in raw_list:

            print(f"\nAnalisando: {name}")
            print(f"Quantidade de pontos validos: {len(n)}")

            try:

                results, best = analyze_complexity(n, y)

                r = results[best]

                k_exp, _ = fit_power_law(n, y)

                print(
                    f"{name} | Complexidade detectada: {best} "
                    f"| erro relativo {r['relative_error']:.2f}% "
                    f"| R2 {r['r2']:.5f}"
                )

                print(
                    f"  y ~ {r['a']:.6g} * {best} + {r['b']:.4g}"
                )

                if best != 'O(1)':

                    print(
                        f"  custo fixo b = {r['b']:.4g} ciclos"
                    )

                    print(
                        f"  erro sem custo fixo: "
                        f"{r['error_without_fixed_cost']:.2f}% "
                        f"-> com custo fixo: "
                        f"{r['relative_error']:.2f}%"
                    )

                print(f"  expoente global k = {k_exp:.3f}")

                ranking = sorted(
                    COMPLEXITY_ORDER,
                    key=lambda m: results[m]['relative_error']
                )

                print(
                    "  Ranking: " +
                    "  ".join(
                        f"{m}={results[m]['relative_error']:.2f}%"
                        for m in ranking
                    )
                )

                final_list.append(
                    (name, n, y, results, best)
                )

            except (ValueError, FloatingPointError) as error:

                print(f"Falha na analise de {name}: {error}")
                continue

        if not final_list:
            raise RuntimeError(
                "Nenhum arquivo pode ser analisado."
            )

        print("\nGerando graficos...")

        plot_comparison(
            final_list,
            "APS - Analise de Complexidade"
        )

        plot_comparison(
            final_list,
            "APS - Analise de Complexidade",
            scale='log-linear'
        )

        plot_comparison(
            final_list,
            "APS - Analise de Complexidade",
            scale='log-log'
        )

        plot_residuals(
            final_list,
            "Residuos relativos: modelo escolhido vs concorrente"
        )

        print("\nTodos os graficos foram gerados!")
        print("Pasta de saida: graficos/")

    except Exception as error:

        print(f"Erro: {error}")
