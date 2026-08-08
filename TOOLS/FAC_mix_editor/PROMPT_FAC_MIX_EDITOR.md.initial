# Build prompt — F.A.C. V2 graphical mix & special-function editor

This file is the **brief** for building `TOOLS/FAC_mix_editor/FAC_mix_editor.html`. Hand it to a coding agent (or follow it yourself). It fixes what the tool must do, what it must emit, and what it is not allowed to get wrong. It is written in English to match the rest of the repository; the tool's UI, code and comments are English too.

---

## 0. Mission

A **single self-contained HTML file** in which a user assembles a F.A.C. V2 **mix** (8 inputs / 10 outputs) or **special function** (1 input / 1 output) out of a closed set of integer blocks, **simulates it in the browser bit-identically to the MCU**, and exports the `.c` / `.h` pair that drops into the firmware and compiles unchanged — plus a **registration guide** telling the user exactly which lines to add to `fac_mixes.h` / `fac_mixes.c` (or `fac_functions.h` / `.c`) by hand.

The whole reason this is feasible is that a mix is a composition of **~30 known integer operations**, not arbitrary C. Arbitrary C cannot be simulated in a browser; these can — and being integer, browser and MCU agree **exactly**.

---

## 1. Sources of truth

Read these before writing any code. Paths are relative to the repository root `Floppy-Ant-Controller/`.

| File | What it fixes |
|---|---|
| `FIRMWARE V2/FAC-Firmware-V2/docs/README_MIX_EDITOR_API.md` | **The contract.** Execution model, value contract, boilerplate, every primitive with its cost and range limits, sensor API, state/time, hard rules, registration, conformance checklist |
| `FIRMWARE V2/FAC-Firmware-V2/docs/README_API.md` | Firmware architecture, the signal chain, the mapper, settings, known issues |
| `FIRMWARE V2/FAC-Firmware-V2/CLAUDE.md` | Conventions, `§ Planned: graphical mix/function editor` |
| `Core/Inc/FAC_Code/mixes_functions/fac_math.h` | **The exact integer semantics of every primitive** — transcribe from here, not from prose |
| `Core/Inc/FAC_Code/mixes_functions/fac_mixes.h` / `fac_functions.h` | Framework API, the two ID enums, `MIXES_MAX_*`, `SPECIAL_FUNCITONS_NUMBER` |
| `Core/Src/.../mixes/fac_template_mix.c.template`, `.../functions/fac_template_function.c.template` | The boilerplate to reproduce **verbatim** |
| `Core/Src/.../mixes/fac_simple_tank_mix.c`, `.../functions/fac_direct_link_function.c` | The two conformance examples — a generated file must be indistinguishable in shape |
| `Core/Inc/FAC_Code/config.h` | `RECEIVER_CHANNEL_RESOLUTION`, `RECEIVER_CHANNELS_NUMBER`, resolutions |
| `Core/Src/FAC_Code/fac_std_receiver.c` | `FAC_std_receiver_calculate_dead_zone()` — the exact deadzone the simulator must port, § 8 |
| `Core/Src/FAC_Code/fac_settings.c` | The five mapper rows (defaults and the `200+10` ceiling), the deadzone default |
| `Core/Inc/FAC_Code/fac_imu.h` | It already `#include`s `Libraries/LSM6DS3.h`, so `X_AXIS`/`Y_AXIS`/`Z_AXIS` come with it — one include, not two |

**Precedence: the source wins over the documents.** The claim that used to sit here — that `README_MIX_EDITOR_API.md § 8.1` denies the existence of an integer square root — is **wrong and has been removed**: the doc documents `FAC_math_sqrt` fully in `§ 7.4` and `§ 8.1` points at it by name as the primitive a melty graph needs. `FAC_math_sqrt` is at `fac_math.h:314`, exact, 0 divisions, fixed 16 steps, and the tool must offer it.

What the source *does* contradict, verified line by line — the tool follows the **right** column:

| Where | The document / template says | The source does | The tool must |
|---|---|---|---|
| `fac_template_mix.c.template:40`, `fac_template_function.c.template` | commented `#include "FAC_Code/fac_fac_imu.h"` | the header is `Core/Inc/FAC_Code/fac_imu.h` — `fac_fac_imu.h` does not exist | emit `#include "FAC_Code/fac_imu.h"`. **Do not uncomment the template line verbatim** — it does not compile |
| `README_MIX_EDITOR_API § 8.4` | `FAC_std_receiver_GET_channel()` returns `0 … 999` | the backend produces `0 … 999` (`fac_pwm_receiver.c:82`), but the deadzone stage snaps the extremes to `RECEIVER_CHANNEL_RESOLUTION` = **1000** (`fac_std_receiver.c:105-108`) | model the channel domain as **`0 … 1000`**, § 3.4 |
| `fac_template_mix.c.template:94`, `.h.template:14` | `void FAC_name_mix_update();` — empty parentheses | the committed example declares `(void)` (`fac_simple_tank_mix.c:93`) | emit `(void)`. This is the **one** documented exception to "byte-identical to the template" |
| `fac_simple_tank_mix.c:41,131` | includes `stdlib.h` and calls `abs()` | legal, and `README_MIX_EDITOR_API § 13` allows it if the include is there | emit the plain-C ternary of § 5.5 instead, so no extra include is needed. Behaviour is identical on the domain involved |
| `README_MIX_EDITOR_API § 9.2` (latch recipe) | uses `TRUE` / `FALSE` | neither is defined without `main.h`, which the boilerplate does not include | emit `1` / `0`, § 5.5 |

---

## 2. Non-negotiable constraints on the tool itself

1. **One file**: `TOOLS/FAC_mix_editor/FAC_mix_editor.html`. No build step, no `npm`, no CDN, no external font/script/style. Double-click and it works, offline, forever. Same spirit as `TOOLS/FAC_jingle_composer.html`.
2. Vanilla JS (ES2020), inline `<style>`/`<script>`. Canvas or SVG for the graph, Canvas for the scope and the robot preview.
3. No telemetry, no network calls, no analytics.
4. Must run in current Chrome/Edge/Firefox. Keyboard + mouse; touch is a bonus, not a requirement. The **Gamepad API** (§ 8.1) is browser-native and allowed — it is not an external dependency and makes no network call.
5. Dark UI by default, readable at 1366×768, usable at 1920×1080. A single accent colour, no icon fonts (draw icons as inline SVG paths or Unicode).
6. Code organised in clearly-marked sections inside the one file: `FAC_MATH`, `RECEIVER` (the deadzone port and the three-stage channel chain of § 8), `NODE_CATALOG`, `GRAPH`, `VALIDATE`, `SIM`, `CODEGEN`, `REGISTRATION`, `UI`. Do not let it degenerate into one unstructured blob.

---

## 3. Domain model the tool must encode

### 3.1 Values

`fac_value_t` is `int32_t`. `FAC_VALUE_MAX = 1000`, `FAC_VALUE_MIN = -1000`, `FAC_VALUE_ZERO = 0`. The `±1000` scale equals the receiver, motor and servo resolutions, so both ends convert exactly.

Track a **kind** on every wire, for validation and for choosing the right block:

| Kind | Range | Meaning |
|---|---|---|
| `NORM` | `[-1000, +1000]` | a normalized value — what a mix input is and an output must be |
| `RAW` | full `int32_t` | unscaled sensor counts, accumulators, µs, mV, ticks |
| `ANGLE` | `[0, 4095]` | binary angle, 4096 units = one turn |
| `BOOL` | `0` / `1` | a condition |

Kind mismatches are **warnings** (the C compiles either way), except where a documented range constraint is actually violated — that is an **error** (see § 6).

### 3.2 Execution model (this drives the whole stateful-block design)

- Bare-metal superloop, ~1 ms per pass, **no preemption**.
- The chain runs **only in `FAC_STATE_NORMAL`**. Disarm/cutoff → the mix and every function simply **do not execute**; every `static` freezes.
- A mix runs **at most once per pass**, only if a device links to it. Same for each special function slot.
- **Never block**: no `HAL_Delay`, no wait loop. IWDG ~400 ms, refreshed after the mapper returns.
- Cortex-M0 @48 MHz, **no FPU, no hardware divider, no long multiply**. Even `/1000` is an `__aeabi_idiv` call. **Count the divisions** — the tool must show a running division count per graph.

### 3.3 Device meaning of an output

| Device | Conversion by the mapper | Meaning |
|---|---|---|
| M1–M3 (DC motor) | sign → direction, `FAC_math_abs(v)` → speed | `0` stopped, `±1000` full speed either way |
| S1–S2 (servo/ESC) | `FAC_math_to_range(v, 0, 1000)` | `-1000` → one end of travel, `+1000` → the other |

Link values in the mapper settings: `0` unused, `100+i` mix output *i* (0…9), `200+i` special function *i*.

### 3.4 Inputs

Mix input *i* = receiver channel chosen by a setting, normalized with `FAC_math_from_range(rx, 0, RECEIVER_CHANNEL_RESOLUTION)`, then negated if the per-input **reverse** flag is set; a slot whose channel setting is `0` is **disabled and forced to `FAC_VALUE_ZERO`**. A channel value of `0` is **not** "no signal" — it is full negative travel. Special function: exactly one input, same rules.

**The channel domain is `0 … 1000`, not `0 … 999`.** With both resolutions at 1000 the normalization is the exact `2·rx − 1000`, so:

- `rx = 0` → `-1000`, `rx = 500` → `0`, `rx = 1000` → `+1000`;
- the receiver *backend* only ever produces `0 … 999` (`fac_pwm_receiver.c:82` maps into `RECEIVER_CHANNEL_RESOLUTION - 1`), which alone would top out at **`+998`**;
- it is the **deadzone stage** that makes the ends reachable: it snaps anything within twice the deadzone of an extreme to exactly `0` or `1000`. That stage is **not optional** — see § 8 item 1.

So the **range analysis** must treat a mix input as the full `[-1000, +1000]`, and the simulator must keep the two stages apart: the sliders and the gamepad write the backend's `0 … 999`, the deadzone port turns that into the stored `0 … 1000`, and only then does `from_range` run. The three-stage model is spelled out in § 8 item 1. A tool that collapses the two stages either loses full positive travel (`+998` ceiling) or applies the deadzone twice.

---

## 4. The `FAC_MATH` JS port — bit-identical or the tool is worthless

Transcribe every primitive from `fac_math.h` line by line. Rules that make it exact:

1. **Integer division truncates toward zero.** Use a single helper `idiv(a,b) = Math.trunc(a/b)` and **never** `Math.floor`, never bare `/`. Same for `%`.
2. **Group 1 saturates, group 2 does not.** Reproduce the clamp order exactly: `FAC_math_mul(a,b)` is `clamp( idiv( clamp(a)*clamp(b), 1000 ) )` — arguments clamped **before** the product.
3. **Reproduce the guarded returns and the argument pre-conditioning.** The full list, from the source:

   | Primitive | Guard / pre-conditioning |
   |---|---|
   | `mul_scaled`, `div_scaled` | `scale == 0 → 0`; `b == 0 → 0` |
   | `from_range` | `in_max == in_min → 0`; **`x` is clamped into `[in_min, in_max]` first** |
   | `to_range` | `v` is clamped to `[-1000, +1000]` first |
   | `deadzone` | `size <= 0 → clamp(v)`; `size >= 1000 → 0`; `|v| <= size → 0` |
   | `expo` | `amount <= 0 → clamp(v)`; **`amount > 1000` is clipped to `1000`**, not rejected |
   | `scale` | `percent` is **clipped to `±10000`** before the product, not rejected |
   | `blend` | `w <= 0 → clamp(a)`; `w >= 1000 → clamp(b)` |
   | `atan2` | `x == 0 && y == 0 → 0` |
   | `sqrt` | `v <= 0 → 0` |
4. **Precision.** Group-1 intermediates never exceed ~10⁶ — plain doubles are exact there. Group-2 products can exceed 2⁵³, where doubles stop being exact **and** where the C would be undefined behaviour. So in group 2 (`mul_scaled`, `div_scaled`, `sqrt` pre-multiplication, any user-parameterised raw multiply) compute the product with **BigInt**, check it against the `int32_t` range, and if it does not fit raise a **runtime overflow error** that the UI surfaces on the offending node. Never silently wrap: `int32_t` wrap is not a feature.
5. `FAC_math_sqrt` is the binary restoring method on **unsigned** 32-bit. Port it with `>>>` and `>>> 0`, not signed shifts.
6. `FAC_math_angle_wrap` is `angle & 4095` — in JS `angle & 4095` already gives `[0,4095]` for negatives too, matching C.
7. `FAC_math_atan_ratio` is a helper: implement it (atan2 needs it) but **do not expose it as a block**.

**Self-test panel** (a hidden dev tab, always shipped): asserts at minimum

- `sin(0)=0`, `sin(1024)=1000`, `sin(2048)=0`, `sin(3072)=-1000`, `cos(0)=1000`
- `atan2(0,0)=0`
- `idiv(-7,2) === -3` and `-7 % 2 === -1`
- every guarded return of the table above, including the two *clipping* ones (`expo(v, 5000)` behaves as `expo(v, 1000)`, `scale(v, 99999)` as `scale(v, 10000)`)
- `mul(2000, 2000) === 1000` (arguments clamp first, then the product saturates)
- `sqrt(n*n) === n` and `sqrt(n*n - 1) === n-1` for a spread of `n`
- **channel domain**: `from_range(0, 0, 1000) === -1000`, `from_range(1000, 0, 1000) === +1000`, `from_range(999, 0, 1000) === 998`, and `from_range(x,0,1000) === 2x - 1000` over the whole `0…1000` range
- **group 4 (raw)**: `raw_abs(-3) === 3`, `raw_div_lit(-3, 2) === -1` (truncation, *not* `-2` as `>>1` would give), and every group-4 node's declared range interval contains the value actually produced over a swept domain
- **receiver deadzone**: the JS port of `FAC_std_receiver_calculate_dead_zone()` agrees with a transcription of `fac_std_receiver.c:86-126` for every `value` in `0…1000` × every `deadzonePerc` in `0…50` × `chNumber ∈ {1, 3}` — the channel-3 special case included
- **integer typing after a gamepad update**: every one of the 8 channel values satisfies `Number.isInteger()` (§ 8.1)
- the **simple tank** preset reproduces `fac_simple_tank_mix.c` for a grid of (throttle, steering) pairs including the four corners of the square gate

The panel prints pass/fail counts. Any failure is a red banner — the tool must not let a user export from a build whose math does not agree with itself.

---

## 5. Block catalogue

Every block declares: `id`, `label`, `category`, input ports (with expected kind), parameters (with min/max/step and whether they are compile-time literals), output kind, **division count**, C emission template, JS simulation function, validation hooks, and a one-line help string shown on hover.

### 5.1 Sources
| Block | Emits | Notes |
|---|---|---|
| **Mix input** *i* (0…7) | `inputs[INPUT_<NAME>]` | Only in mix mode. User names it; the **index is positional and must never be renumbered** |
| **Function input** | `input` | Only in function mode |
| **Constant** | integer literal | Kind selectable NORM/RAW/ANGLE |
| **RX channel (direct)** | `FAC_math_from_range(FAC_std_receiver_GET_channel(n), 0, RECEIVER_CHANNEL_RESOLUTION)` | `n` is **1-based**, 1…8. Not a free read — emit it once into a local. Offer it only under an "advanced" flag, with the warning that it bypasses the user-configurable channel setting |
| **RX connected** | `FAC_std_receiver_GET_is_connected()` | BOOL. Warn in the UI: latches TRUE forever, **not** a failsafe |
| **ADC aux** | `FAC_math_from_range(FAC_adc_get_raw_channel_value(1), 0, FAC_adc_GET_resolution())` | Index literal restricted to `0`,`1`,`2` — the accessor has **no bounds check** |
| **IMU gyro raw** / **accel raw** | `FAC_IMU_GET_gyro_raw(Z_AXIS)` etc. | RAW ±32768. Forces the IMU preamble, § 7.4 |
| **IMU gyro mdps** / **accel mg** | `FAC_IMU_GET_gyro_X_mdps()` etc. | `_mdps` costs 0 divisions, `_mg` costs 1. Prefer raw + `from_range` and say so in the help |
| **Battery mV** / **cell mV** | cached read, § 7.5 | ⚠ expensive **and float-based** internally — must be emitted behind a tick-gated cache, never called per pass |
| **Tick** | `HAL_GetTick()` | RAW ms |
| **Setting** | `FAC_settings_GET_value(<code>)` | Advanced. Read-only. All 64 codes are assigned; a graph's own tuning constants must be **literals baked in by the tool**, not new settings |

### 5.2 Group 1 — normalized, saturating
`clamp(v)` · `abs(v)` · `add(a,b)` · `sub(a,b)` · `mul(a,b)` · `scale(v, percent)` · `min(a,b)` · `max(a,b)` · `blend(a,b,w)` · `deadzone(v,size)` · `expo(v,amount)` · `to_range(v,out_min,out_max)` · `from_range(x,in_min,in_max)`

Division counts: 0 for `clamp/abs/add/sub/min/max`, 1 for `mul/scale/blend/deadzone/to_range/from_range`, **3 for `expo`**.
Parameter limits to enforce: `|percent| ≤ 10000` (clipped by the primitive, but tell the user), `(out_max-out_min) < 10⁶`, `(in_max-in_min) < 10⁶`.

### 5.3 Group 2 — raw fixed point, non-saturating
`mul_scaled(a,b,scale)` · `div_scaled(a,b,scale)` · `clamp_to(v,min,max)` · `sqrt(v)`

Constraints to validate: `a*b` must fit `int32_t` (`mul_scaled`), `a*scale` must fit (`div_scaled`), `sqrt`'s argument must stay under ~2×10⁹ **after** any pre-multiplication. `sqrt` costs no division but ~16 steps — budget it as two divisions in the cost display, and explain the halved-scale rule (`sqrt(x*scale)` comes back on `sqrt(scale)`; to keep the input's scale, pre-multiply by the scale once more).

### 5.4 Group 3 — angles
`angle_wrap(a)` (0 div) · `sin(a)` (4) · `cos(a)` (4) · `atan2(y,x)` (6). Helper `DEG(d)` as a parameter widget so the user types degrees and the tool emits `FAC_MATH_DEG(90)`. `atan2` requires `|x|,|y| < 200000`.

### 5.5 Group 4 — plain C: raw arithmetic, logic and selection

None of these is a `fac_math` primitive. They are ordinary C, explicitly allowed by the contract, and the tool emits them with **explicit parentheses** around every operand.

**Raw arithmetic — the group that makes the tank mix expressible.** Group 1 saturates and group 2 only offers scaled multiply/divide, so without this group the tool cannot express `fac_simple_tank_mix.c`, which deliberately lets its sums reach twice full scale before halving them (`fac_simple_tank_mix.c:113-141`). Collapsing that into saturating adds is exactly the "simplification" the source file warns against in capitals.

| Block | Emits | Div | Notes |
|---|---|---|---|
| `raw_add(a,b)` | `(a) + (b)` | 0 | **Does not saturate.** The whole point |
| `raw_sub(a,b)` | `(a) - (b)` | 0 | |
| `raw_neg(v)` | `-(v)` | 0 | |
| `raw_abs(v)` | `((v) < 0 ? -(v) : (v))` | 0 | The plain-C absolute value. **Not** `FAC_math_abs` (which clamps to ±1000 first) and **not** `abs()` (which would need `stdlib.h`) |
| `raw_mul_lit(v, k)` | `(v) * k` | 0 | `k` a compile-time integer literal |
| `raw_div_lit(v, k)` | `(v) / k` | 1 — discounted only under rule 2 below | `k` a non-zero compile-time integer literal. **`k = 0` is a validation error**, not a runtime guard |

Three rules on this group, all enforced by the validator:

1. **Range analysis is mandatory on every group-4 node.** These blocks have no clamp and no guard; the interval propagation of § 6 is the only thing standing between the user and undefined behaviour. A node whose interval cannot be proven to fit `int32_t` is an **error**, not a warning.
2. **No shift blocks.** `>>` and `<<` are deliberately absent even though they are the cheap form: on a negative value `-3 >> 1` is `-2` while `-3 / 2` is `-1`, so a shift is *not* a division and swapping them silently changes behaviour around zero — precisely the class of divergence this tool exists to prevent. The user writes `/ 2`; whether GCC folds it is GCC's business. For a power-of-two `k` the readout may say the division costs **no `__aeabi_idiv` call** — the compiler emits a shift, plus a sign correction when the analysis cannot prove the value non-negative — and must not present it as free otherwise.
3. **Kind rules.** `NORM → RAW` is free (same `int32_t`, only the promise about the range is dropped). `RAW → NORM` requires an explicit `clamp`, `clamp_to` + `from_range`, or `from_range` — never an implicit conversion. A group-4 result wired straight into a mix output is a **warning**, since the boilerplate's final `FAC_math_clamp` will silently saturate it.

**Logic and selection** — 0 divisions: `compare` (`>`, `>=`, `<`, `<=`, `==`, `!=`) → BOOL · `and` / `or` / `not` · `select(cond, a, b)` → `cond ? a : b` · `sign(v)`.
Use `1` / `0`, never `TRUE` / `FALSE`: the boilerplate does not include `main.h`, and the tool must not add it just for two constants.

### 5.6 Stateful blocks — the part most easily got wrong
Each of these owns one or more file-scope `static`s and **must** expose a per-block **"on resume after disarm: reset / hold"** choice (§ 7.6). Every one must be correct when its state is zero at boot.

| Block | State | Emission (follow `README_MIX_EDITOR_API § 9.2` exactly) |
|---|---|---|
| **Previous value (unit delay)** | 1 | read, then assign last |
| **Edge detect** | 1 + threshold param | rising / falling / both |
| **Toggle latch** | 2 (`latched`, `armed`) | **two thresholds, never one** — a single threshold chatters at 1 kHz |
| **Slew limiter (ramp)** | 1 + step | `if (t > c+step) c += step; else if (t < c-step) c -= step; else c = t;` — exact, always reaches the target. Offer *step per pass* **and** *ramp time in ms* (tick-gated), because the pass rate is only approximately 1 kHz |
| **Low-pass filter** | 1 (×1000 accumulator) | `acc += idiv(x*1000 - acc, N); out = idiv(acc,1000);` — **never** the naive `v += (x-v)/N`, which stalls with a permanent dead band. Warn when `N` is not a power of two. Count it as **2 divisions, one of which never folds**: `README_MIX_EDITOR_API § 9.2` says a power-of-two `N` takes "both divisions" to zero, but `acc / 1000` is a division by 1000 and stays an `__aeabi_idiv` call whatever `N` is |
| **Integrator** | 1 | mandatory anti-windup `FAC_math_clamp_to` bounds + a reset input. Refuse to emit an unbounded one |
| **Timer / periodic gate** | 1 (`t0`) | `if ((uint32_t)(now - t0) >= period)` — **never** `now > t0 + period` |
| **Sample & hold** | 1 + trigger | |
| **Cached read** | 2 (`lastRead`, `cached`) | wraps an expensive source, tick-gated; the battery blocks use it implicitly |

### 5.7 Sinks
**Mix output** *j* (0…9) — user-named, index positional. **Function output** — the single `output`.
Unconnected outputs stay at `FAC_VALUE_ZERO` (the boilerplate's zero-init already does this; do not emit redundant assignments).

---

## 6. Graph model and validation

- Directed acyclic graph. **Cycles are an error** — the only legal feedback is through a stateful block, which reads its `static` from the *previous* pass, so a stateful block breaks the cycle by construction. Model that explicitly: a stateful block's state output is a graph *source*, its input a graph *sink*.
- Nodes have positions, collapsible groups, and free-text notes that flow into the generated `DESCRIPTION` block.
- Multiple inputs may drive one node port only through an explicit block; **no implicit fan-in**. Fan-out is unlimited.
- Validation runs continuously and lists findings as **error** (blocks export) or **warning** (does not):
  - **error**: cycle without a stateful break · unconnected required port · output index used twice · a documented range constraint provably violated over the full input domain · an `int32_t` intermediate that can overflow · **a group-4 node whose interval cannot be *proven* to fit `int32_t`** (unproven is an error here, not a warning — the group has no clamp) · `raw_div_lit` with `k = 0` · division by a value that can be zero outside the guarded primitives · IMU used without the mandatory preamble · a multi-instance function whose state is not indexed by `sFunctionID` · special-function ID count pushing `FAC_SPECIAL_FUNCTION_LAST` past 20
  - **warning**: kind mismatch on a wire · a group-4 (RAW) result wired straight into an output, where the boilerplate's final clamp will saturate it silently · dead node (computes nothing that reaches an output) · division count above ~10 · `expo`/`sin`/`cos`/`atan2` used more than once where a shared subexpression would do · a filter `N` that is not a power of two · a stateful block with no explicit reset/hold decision · direct RX channel read bypassing the configurable setting
- **Range analysis**: propagate an interval `[lo,hi]` per wire (sources have known ranges, each primitive has a known transfer) and use it to decide the overflow/constraint errors **at graph-build time, not at run time**. This is exactly what the contract asks for.
- **Cost readout**, always visible: number of divisions, number of `sqrt` steps, number of statics (RAM), estimated ~µs per pass with the caveat that only `FAC_debug_utils` on target gives the real number.

---

## 7. Code generation

### 7.1 Rules
- The tool owns **only** the region between `/* INSERT YOUR CODE HERE -START- */` and `-END-`, plus file-scope `static`s in the `/* PRIVATE FUNCTIONS AND VARIABLES */` region, plus the `/* CUSTOM INCLUDE */` block, plus the `#define INPUT_*` / `OUTPUT_*` and the `DESCRIPTION` comment. **Everything else is byte-identical to the template**, including the 9-step recipe header comment. Four bounded exceptions, all of which the committed examples already make:
  - the file name in the first comment line and in the `#include` of the own header;
  - `Created on:` = the export date, `Author:` = a free-text field in the tool (default: the project title's author, falling back to `F.A.C. mix editor`);
  - `mix_id` / `first_special_function_id` set to the chosen ID, and the update function renamed;
  - the update function declared **`(void)`** and not `()` — the template's empty parentheses are a C23 error, and `fac_simple_tank_mix.c:93` already writes `(void)`.
- **Delete the template's `if (0) { }` placeholder.** It is scaffolding, not boilerplate; leaving it in produces an unreachable block and a `-Wall` complaint waiting to happen.
- The generated `.c` **does not compile on its own**: `mix_id = FAC_MIX_<NAME>` refers to an enum entry that only exists after registration step 2. That is expected, and the registration guide must say so in that order — enum first, then the file builds.
- Tabs for indentation. K&R braces. Comments and identifiers **in English**. Match the existing non-native spellings where they are API (`SPECIAL_FUNCITONS_NUMBER`, `special_functions_outouts`, `FAC_std_reciever_init`) — do not "fix" them.
- `-Wall` clean: no unused locals; keep `__attribute__((unused))` on the `mix_id` / `first_special_function_id` marker.
- One `int32_t` / `fac_value_t` local per node, named from the node's label sanitised to a C identifier, deduplicated with a numeric suffix. Emit in topological order. Inline a node only when it feeds exactly one consumer and the result stays readable.
- Emit a short comment above each stateful block saying what it is and what its reset policy is.
- Forbidden absolutely: `float`, `double`, `math.h`, `malloc`, VLA, recursion, `HAL_Delay`, any wait/poll loop, any device setter (motor/servo/LED/buzzer), any settings write, any EEPROM/USB/I²C access — with the single exception of `FAC_IMU_update()`.

### 7.2 File naming
`fac_<name>_mix.c` / `.h`, `fac_<name>_function.c` / `.h`, `<name>` lower_snake_case from the user's title. Declare the update function `(void)` for a mix and `(uint8_t sFunctionID)` for a function — **never empty parentheses** (C23 error).

**The two header guards do not follow the same rule** — match each committed example rather than deriving one form for both:

| | Guard |
|---|---|
| Mix | `INC_FAC_CODE_MIXES_FUNCTIONS_FAC_<NAME>_MIX_H_` — **no `MIXES_` path segment**, as in `fac_simple_tank_mix.h` |
| Special function | `INC_FAC_CODE_MIXES_FUNCTIONS_FUNCTIONS_FAC_<NAME>_FUNCTION_H_` — **with** the `FUNCTIONS_` segment, as in `fac_direct_link_function.h` |

### 7.3 Includes
Emit into `/* CUSTOM INCLUDE */` only the headers the graph actually uses, and only in the corrected spelling of § 1: `FAC_Code/fac_imu.h`, `FAC_Code/fac_adc.h`, `FAC_Code/fac_battery.h`, `FAC_Code/fac_std_receiver.h`, `FAC_Code/fac_app.h`. Notes:

- `fac_imu.h` already pulls `Libraries/LSM6DS3.h`, so `X_AXIS` / `Y_AXIS` / `Z_AXIS` are available from that single include — **do not emit a second one**.
- The boilerplate does **not** include `main.h` (so no `TRUE`/`FALSE`, § 5.5) nor `stdlib.h` (so no `abs()` — use `raw_abs`'s ternary for a raw value, `FAC_math_abs` for a normalized one, remembering that the latter clamps its argument first).
- `fac_mixes.h` re-exports `map_int32()`. **Do not offer it as a block**: it clamps `x` to `in_max` but not to `in_min`, and uses 64-bit intermediates that cost a helper call on an M0. `FAC_math_from_range` / `FAC_math_to_range` are the supported conversions.

### 7.4 IMU preamble (mandatory whenever any IMU block is present)
```c
	FAC_IMU_update();                         /* the main loop does NOT do this for us */
	if (FAC_IMU_GET_status() != HAL_ERROR) {
		/* ... the IMU-dependent part of the graph ... */
	} else {
		/* the user's chosen fallback: hold last output, or FAC_VALUE_ZERO */
	}
```
`FAC_IMU_update()` once, at the top of the region, before any accessor. The status guard and an **explicit user-chosen fallback** are not optional: a dead sensor reads exactly like a sensor holding perfectly still, which is what a self-righting graph would trust.

### 7.5 Battery
Never per pass. Always the tick-gated cache (default 200 ms), because `FAC_battery_GET_voltage()` re-reads the ADC five times and converts in **floats** on a core with no FPU.

### 7.6 Disarm-gap guard
Whenever the graph holds any state, emit:
```c
	static uint32_t lastRun = 0;
	uint32_t now = HAL_GetTick();
	if ((uint32_t)(now - lastRun) > 100) {    /* we were not running: disarm gap */
		/* reset here every block whose policy is RESET */
	}
	lastRun = now;
```
Blocks whose policy is HOLD are simply absent from that body. The UI must force the choice per block rather than letting `static` decide by accident.

### 7.7 Multi-instance special functions
If the user asks for *N* instances: emit *N* **consecutive** IDs with `1ST`/`2ND`/`3RD`… suffixes, set `first_special_function_id` to the first, and **key every `static` by `sFunctionID`** — `static int32_t state[SPECIAL_FUNCITONS_NUMBER];` or an array of the instance count indexed by `sFunctionID - FIRST_ID`. A shared scalar across instances is the single most likely defect in generated code; the validator must make it impossible.

*N* is bounded by the free IDs, which the tool computes from the count the user gives it (§ 9): `20 - <existing IDs>` before `FAC_SPECIAL_FUNCTION_LAST` overruns the framework arrays, and `11 - <existing IDs>` before the new instances stop being linkable to a device at all. On the firmware this brief was written against that is **9 free IDs, none of them mappable** — index 10 is the last the mapper's `200+10` ceiling can reach and it is already taken. Show both numbers; they are different limits with different consequences.

### 7.8 Round-trip (recommended, must be toggleable, default on)
Append the project JSON, minified, inside a trailing comment block of the `.c` — so the tool can **re-import a generated file** and keep editing. Off → the file carries no JSON.

---

## 8. Simulator

Fixed step, 1 ms per pass, driving a simulated `HAL_GetTick()`. Run / pause / single-step / reset-state. Speed multiplier ×0.1…×10 for watching ramps.

1. **Virtual sticks + live values.** The input path has **three stages**, and the tool must keep them distinct — this is where a simulator quietly double-applies a deadzone or clips its own full scale:

   | Stage | Range | Who writes it |
   |---|---|---|
   | **0 — backend capture** | `0 … 999` (`RECEIVER_CHANNEL_RESOLUTION - 1`, `fac_pwm_receiver.c:82`) | the 8 sliders, or the gamepad of § 8.1 |
   | **1 — stored channel** | `0 … 1000` | the deadzone port below. This is what `FAC_std_receiver_GET_channel()` returns |
   | **2 — mix input** | `-1000 … +1000` | `FAC_math_from_range(ch, 0, RECEIVER_CHANNEL_RESOLUTION)`, then the reverse flag, then the disabled-channel rule |

   So: 8 sliders in **stage-0** units `0…999`, each with the channel-assignment and reverse-flag settings of the mix, plus a "channel disabled (0)" toggle that forces the input to `FAC_VALUE_ZERO`. Show stage 0, stage 1 and stage 2 side by side per channel, so the deadzone's effect is visible rather than inferred. Live value on every wire, colour-coded by sign, bar meters on all 10 outputs. The sliders are the fallback for whoever has no gamepad — see item 6, which writes the **same stage-0 values**.

   **The receiver deadzone is modelled, exactly, and it is ON by default.** It is not an optional refinement: `FAC_SETTINGS_CODE_CHANNELS_DEADZONE_PERCENTAGE` defaults to **2** (range `0…50`, `fac_settings.c:55`) and `FAC_std_receiver_new_channel_value()` applies it to **every** channel on **every** frame. Port `FAC_std_receiver_calculate_dead_zone()` (`fac_std_receiver.c:86-126`) line by line, including all three of its parts — centre deadzone, extremes deadzone, and the re-linearisation of the travel that remains — and including the **channel-3 special case** (the throttle stick has no return spring, so it gets the extremes deadzone but *not* the centre one; the test is on the **1-based** channel number). Expose it as a percentage slider `0…50` defaulting to 2, so the user can reproduce their own setting, and label it plainly as *firmware behaviour on the robot* — as opposed to the pad deadzone of § 8.1, which is hardware slop on the desk.

   Two consequences the panel should make visible, because they surprise people: with the deadzone at its 2 % default the stick ends snap to exactly `0` and `1000`, i.e. **±1000 normalized**; with the deadzone set to `0` the ends only reach `0` and `999`, i.e. **−1000 … +998**, and full positive travel becomes unreachable. Both are the real firmware, § 3.4.
2. **Scope** — rolling time-series of any selected wires/outputs, ring buffer, adjustable window (0.1 s…30 s), pause and cursor readout. This is the only way to see a ramp, filter, latch or timer behave, so it is not optional.
3. **2D robot preview** — top view with M1/M2/M3 and S1/S2 driven through a **configurable mapper** (the five link values, `0` / `100+i` / `200+i`, defaulting to the firmware defaults: M1←mix out 0, M2←mix out 1, S1←function 0). Motors show direction and speed; servos show arm angle. A simple differential-drive integration for the top view, clearly labelled *illustrative, not physical*.
4. **Simulated sensors** — gyro X/Y/Z and accel X/Y/Z in raw counts (slider, or generator: constant / sine / sweep / noise), pack voltage in mV with a cell-count selector, ADC aux. An **"IMU failure"** toggle that makes `FAC_IMU_GET_status()` return `HAL_ERROR` while the accessors keep returning the last values — so the user can watch their fallback actually work.
5. **Disarm button** — freezes the graph for a user-set duration (default 3 s) without advancing it, then resumes with the tick jumped forward. This exercises the disarm gap and is how the user checks their reset/hold choices.
6. **Real gamepad input** — § 8.1.

The simulator must call **exactly** the same JS functions the codegen maps to. No second implementation, ever — one source, two consumers.

### 8.1 Driving the simulation with a real gamepad

Whoever owns an Xbox / PlayStation / generic USB pad must be able to **fly the mix with real sticks** instead of dragging eight sliders. This is the difference between "the numbers look right" and "the robot drives right", and it costs nothing: the **Gamepad API is built into the browser**, needs no library, no driver and no network, so it does not touch the single-file constraint of § 2.

**Where it plugs in.** The gamepad is *not* a new input path. It writes the **same eight stage-0 values `0…999`** the sliders write, upstream of everything else — the receiver deadzone, the channel assignment, the reverse flag and the disabled-channel rule all still apply afterwards, unchanged. Everything downstream cannot tell the two apart, which is the whole point: what the user tunes with a pad is exactly what the receiver will feed on the robot.

**The float boundary — the one rule that matters.** `navigator.getGamepads()` returns axes as **floats in `[-1, +1]`**. That float must die at the boundary:

```js
/* the ONLY place a float is allowed in this tool */
let ch = Math.round(((axis + 1) / 2) * (RECEIVER_CHANNEL_RESOLUTION - 1));   /* 0 .. 999, stage 0 */
ch = Math.min(RECEIVER_CHANNEL_RESOLUTION - 1, Math.max(0, ch));
```

The multiplier is `RESOLUTION - 1` because this writes **stage 0**, the backend capture, which really does top out at `999` on hardware. Reaching a full `+1000` normalized is the job of the **deadzone stage** downstream (§ 8 item 1), exactly as on the robot — do not compensate for it here by scaling to `1000`, or the deadzone gets applied to a value that already had it and the two models drift apart.

From there on it is an integer and every rule of § 4 applies. **No float may ever reach the graph math**, or the browser stops agreeing with the MCU — which is the tool's entire reason to exist. The self-test of § 4 should assert that the channel state is integer-typed after any gamepad update.

**Polling and hold.** Browsers refresh gamepad state at roughly display rate (~60 Hz), while the simulation runs at 1 kHz. Poll once per animation frame and **hold** the last value across the ~16 simulated passes in between — which is not a compromise but a *better* model of reality, since a real RC link updates at ~50 Hz too. Offer an explicit **"RC frame rate" hold** (default 50 Hz, off = update as fast as the browser polls) so a graph that reacts to input steps can be checked against a realistic frame period. Read the array fresh on every poll: `getGamepads()` returns a snapshot, not a live object.

**Binding UI** — a panel listing the 8 channels, each with:

| Control | Purpose |
|---|---|
| Source | axis *n*, button *n*, or none (falls back to that channel's slider) |
| Invert | mirrors the axis; independent of the mix's own per-input reverse flag, which stays where it is |
| Centre / trim | offset for a stick that does not rest at exactly `0.0` |
| Endpoints | min/max travel, so a worn stick still reaches `0` and `999` |
| Pad deadzone | default **5 %**, applied to the *gamepad* only. Keep it visibly separate from the receiver deadzone modelled in item 1 — one is hardware slop on the desk, the other is firmware behaviour on the robot |
| Button mode | for a button source: *momentary* (held = high), *toggle* (press flips), or *3-position* cycling low/centre/high — this is how an arming or weapon channel gets tested |

Defaults on a standard-layout pad: CH1 ← right stick X, CH2 ← right stick Y (inverted, since the API reports up as negative), CH3 ← left stick Y (inverted), CH4 ← left stick X, and the two triggers offered for CH5/CH6. Show the detected `gamepad.id` and the mapping name so the user knows what was recognised.

**Robustness, all of it required:**

- Handle `gamepadconnected` / `gamepaddisconnected`; show the device name, and on disconnect **fall back to the sliders holding their last value** rather than dropping to zero — a channel dropping to `0` is full negative travel, not neutral, exactly the trap `README_API § 6.4` documents for issue #19.
- Chrome exposes nothing until the user **presses a button on the pad first**; say so in the panel instead of looking broken.
- Triggers differ by pad and by browser: some report as axes in `[-1,+1]`, some as `buttons[n].value` in `[0,1]`. Support both, and let the binding UI show the live raw value so the user can see what moved.
- Support more than one connected pad (pick one from a dropdown), and a "learn" button that binds whatever axis moves next.
- If `navigator.getGamepads` is missing or always empty, say so plainly and keep the sliders. Note in the panel that the Gamepad API needs a secure context: `file://` counts as one in current Chrome and Firefox, but if a browser refuses, serving the folder over `localhost` is the fallback — **that is a convenience, never a requirement**, and the tool must stay fully usable without a pad.
- Keep the binding set in the project JSON (§ 10) so a user's pad layout survives a reload.
- No vibration, no haptics, no gamepad-driven UI navigation. Input only.

---

## 9. Export and the registration guide

Export produces, as individual downloads and as a single `.zip`-less bundle (concatenated preview panel with copy buttons):

1. `fac_<name>_mix.c` / `.h` (or `_function`).
2. A **registration guide** — on screen, copy-able, and downloadable as `REGISTRATION_<name>.md`. Plain textual instructions with the exact snippets and where they go. **The tool does not patch the firmware, does not emit a `.patch` / unified diff, and must not claim to.** A diff would carry line numbers the tool cannot know — the user's `fac_mixes.c` may already have mixes of their own — and a patch that fails to apply is worse than a snippet that a human places correctly. Guide only.

**Where the ID numbers come from.** The guide has to state the index the new ID will take, and for a special function whether that index crosses the mapper's `200+10` ceiling. The tool cannot read the user's firmware tree, so it **asks**: two numeric fields in the export panel, *"how many entries does your `enum FAC_MIXES_ID` have before `FAC_MIX_LAST`?"* and the same for `FAC_SPECIAL_FUNCTIONS_ID`, pre-filled with the values of the firmware this brief was written against (**2 mixes**: `FAC_MIX_NONE`, `FAC_MIX_SIMPLE_TANK`; **11 special functions**: 8 × `DIRECT_LINK_TO_CHANNEL` + 3 × `DC_SERVO`) and with a one-line hint on where to count them. Everything the guide asserts about indices, about `FAC_SPECIAL_FUNCTION_LAST ≤ 20` and about the `200+10` ceiling is computed from those two numbers, and the guide **says which numbers it assumed** so a wrong entry is visible rather than silent.

For a **mix** the guide states, in order:

1. Drop the two files into `Core/Src/FAC_Code/mixes_functions/mixes/` and `Core/Inc/FAC_Code/mixes_functions/mixes/`.
2. In `Core/Inc/FAC_Code/mixes_functions/fac_mixes.h`, add `FAC_MIX_<NAME>,` to `enum FAC_MIXES_ID` **immediately before `FAC_MIX_LAST`** — append only, **never** insert or reorder: the active mix is stored in EEPROM as a number, so reordering silently makes an existing configuration select a different mix.
3. In `Core/Src/FAC_Code/mixes_functions/fac_mixes.c`, add `#include "FAC_Code/mixes_functions/mixes/fac_<name>_mix.h"` in the `/* NEW MEXES */` block.
4. In the same file, add `case FAC_MIX_<NAME>: FAC_<name>_mix_update(); break;` to `FAC_mix_update()`.
5. **Refresh / regenerate the STM32CubeIDE project** so `Debug/**/subdir.mk` picks up the new `.c` — otherwise it is *silently not compiled*, the mix appears in the menu and does nothing. Never hand-edit `Debug/`; it is git-ignored and holds absolute paths.
6. Note that `FAC_SETTINGS_CODE_ACTIVE_MIX`'s `max` follows `FAC_MIX_LAST-1` automatically — nothing to do there.
7. Reminder: bumping `FIRMWARE_VERSION_TAG` rewrites **all** settings to defaults (10 fast boot blinks = defaults written, 3 = normal load).

For a **special function**, the same shape plus:

- IDs go before `FAC_SPECIAL_FUNCTION_LAST`, which **must stay ≤ 20** (`SPECIAL_FUNCITONS_NUMBER`).
- Multi-instance: group the `case` labels with **no `break` except on the last**.
- ⚠ **The five mapper rows in `fac_settings.c:103-107` have a hardcoded `max` of `200+10`.** Index `10` is the highest a device link can name, and on the current firmware it is **already taken** (`FAC_SPECIAL_FUNCTION_DC_SERVO_3RD`). So *any* special function added today registers, compiles and runs but **cannot be linked to any device**, and nothing warns about it. The guide must say this loudly, compute the index the new ID will take from the count the user supplied, and — when it exceeds 10 — spell out the fix: raise the `max` field of all five `FAC_SETTINGS_CODE_MAPPER_*` rows, which is a firmware edit the tool does not perform.

The guide ends with the **conformance checklist** of `README_MIX_EDITOR_API § 13`, pre-ticked for everything the tool verified itself and left open for what only a build can confirm.

---

## 10. Project files, presets, UX

- **Save/Load** a project as `.facmix.json` with a `schemaVersion`. Autosave to `localStorage`, with a "restore last session" prompt. Import back from a generated `.c` when § 7.8 is on.
- **Presets**, loadable as starting points and doubling as regression fixtures:
  - `simple tank` — must reproduce `fac_simple_tank_mix.c`, **including the `diff` term**. That term exists because a gimbal moves in a **square** gate: a plain saturated sum would collapse the corners and stop steering at full throttle. Ship it with that explanation attached, and never "simplify" it. It is built entirely out of **group 4** (§ 5.5): `raw_add` / `raw_sub` for the deliberately unsaturated sums, `raw_abs` for the `diff` magnitudes, `select` for the two sign branches, `raw_div_lit(·, 2)` for the halving — the boilerplate's final `FAC_math_clamp` is what brings it back into range, and the graph must not add its own. This preset is the acceptance test for group 4: if it cannot be expressed node for node, group 4 is wrong.
  - `direct link` — the pass-through function.
  - `tank + expo on steering`, `weapon toggle latch`, `servo soft-start ramp`, `low-battery output scaling` (cached battery read), `IMU self-righting with status guard`.
  - a `melty brain` **skeleton** that demonstrates `atan2` + `sqrt`, with an explicit note that the ±2000 dps gyro (≈333 RPM) saturates well below melty working speed and that the centripetal route (`ω = √(a/r)`) is bounded by the ±16 g accelerometer and the IMU's radius — pick units so the pre-multiplied `sqrt` argument stays under 2×10⁹ (centi-rad/s works, milli-rad/s overflows).
- Undo/redo (at least 50 steps), multi-select, copy/paste of a subgraph, box-select, delete, snap-to-grid, pan/zoom, "frame all".
- Right panel: node inspector (parameters with units and limits), validation list (click to focus the node), cost readout.
- Bottom panel: tabs for Scope / Robot / Sensors / Generated code (live preview, updating as the graph changes) / Registration guide.
- Every block's help text states its **division count** and its **range constraint**. The user is a robot builder, not a compiler engineer — the tool must teach the cost model as it goes.

---

## 11. Acceptance criteria

The tool is done when all of these hold:

- [ ] Single HTML file, opened offline with no network, fully functional.
- [ ] `FAC_MATH` self-test all green, including the `simple tank` parity grid, the deadzone port and the channel-domain assertions.
- [ ] Building the tank mix graphically and exporting it yields a file that a human reading it side by side with `fac_simple_tank_mix.c` judges equivalent — same boilerplate, same structure, same behaviour on every stick pair. The `diff` term survives, the sums are still unsaturated, and the halving is still `/ 2`.
- [ ] The simulated channel chain reproduces the firmware's own: stage 0 `0…999`, the ported deadzone at its 2 % default, stage 1 reaching exactly `0` and `1000` at the stick ends, and `-1000 … +998` when the deadzone is set to `0`.
- [ ] The exported `.c` / `.h` compile in STM32CubeIDE with **no `-Wall` warnings** after following the registration guide (the user verifies this; the tool states it as the final step).
- [ ] A stateful graph (ramp + latch + filter) behaves identically in the simulator before and after a simulated disarm gap, according to the reset/hold policy the user selected.
- [ ] An IMU graph cannot be exported without the `FAC_IMU_update()` preamble and the status guard.
- [ ] A multi-instance function cannot be exported with unindexed state.
- [ ] The validator refuses a graph with a provable `int32_t` overflow, and points at the node.
- [ ] The registration guide names every file to edit, in order, with the append-only and CubeIDE warnings, states the two ID counts it assumed, and flags the `200+10` mapper ceiling when the computed index crosses it. It ships **no patch file** and never claims to have edited anything.
- [ ] A connected gamepad drives the mix through the **same eight stage-0 values** as the sliders, its floats are converted to integers at the boundary and nowhere else, and unplugging it mid-run leaves the channels holding their last value instead of dropping to `0` (which would be full negative travel).
- [ ] The tool is fully usable, with no warning fatigue, on a machine with **no gamepad at all**.
- [ ] No `float`, no `Math.floor` on a division, no second copy of the math anywhere in the source. The only float in the program is the gamepad axis, and it dies in the conversion of § 8.1.

---

## 12. Suggested build order

1. `FAC_MATH` port + the receiver-deadzone port + self-test panel. **Nothing else until this is green.**
2. Graph data model, node catalogue for group 1 and group 4, canvas editing, save/load. Group 4 comes in early because the tank preset — the reference for everything downstream — cannot be built without it.
3. Codegen for a stateless mix + the boilerplate reproduction; validate against the tank preset.
4. Simulator core: 1 ms loop, the three-stage channel chain of § 8 item 1, sticks, live values, output bars. Then the gamepad binding (§ 8.1) on top of the same stage-0 state — it is small, and it is what makes every later stage worth testing.
5. Validation and range analysis, cost readout.
6. Stateful blocks + disarm guard + the reset/hold policy UI.
7. Special-function mode, including multi-instance.
8. Sensors (IMU/battery/ADC) with their mandatory preambles, plus simulated sensors and the failure toggle.
9. Scope, then 2D robot preview.
10. Registration guide generator, presets, round-trip import, polish.

Ship each stage working. A tool that only does stateless mixes but does them exactly is worth more than a feature-complete one whose arithmetic drifts from the MCU by one unit.

---

## 13. What not to do

- Do not invent primitives. If it is not in `fac_math.h` or plain C already allowed by the contract, it does not exist. Anything else is a **firmware** change, not something a generator may emit.
- Do not let generated code write a device, a setting, or the EEPROM. The mapper owns the devices and rewrites them every pass.
- Do not offer "is armed?" as a condition: the chain only ever runs in `FAC_STATE_NORMAL`, so it is always true. What a graph can detect is the *gap*, § 7.6.
- Do not build a failsafe out of `FAC_std_receiver_GET_is_connected()` — it latches TRUE and never returns to FALSE.
- Do not read another special function's output: the mapper's update order is the order of the five link settings, not a dependency graph.
- Do not renumber `INPUT_*` / `OUTPUT_*`, do not reorder either ID enum, do not add a setting.
- Do not silently accept an out-of-range intermediate. Signed overflow is UB in C and JavaScript doubles would not reproduce it — that divergence is exactly what destroys the tool's reason to exist.
- Do not offer `map_int32()` even though `fac_mixes.h` re-exports it: it clamps `x` to `in_max` but not to `in_min`, and its 64-bit intermediates cost a helper call on an M0. `FAC_math_from_range` / `to_range` are the supported conversions.
- Do not offer shift blocks. `-3 >> 1` is `-2` and `-3 / 2` is `-1`; a tool whose whole premise is bit-identical arithmetic does not hand the user two operations that look interchangeable and are not (§ 5.5).
- Do not emit a patch, a diff, or a modified copy of any firmware file. The tool writes two new files and a guide; every edit to an existing file is the user's, made by hand, in the order the guide gives.
- Do not let the tool assume the ID counts. They come from the user (§ 9), and every number derived from them is labelled as such.

---

## 14. Decision log

Choices made after the first review pass, recorded so the reasoning survives the next one.

| # | Question | Decision | Why |
|---|---|---|---|
| 1 | The block catalogue could not express `fac_simple_tank_mix.c` — the reference file uses unsaturated `int32_t` sums, a raw `abs()` and `/ 2`, none of which existed as blocks | **Add group 4** (§ 5.5): plain-C raw arithmetic, with mandatory range analysis and no shift blocks | Without it the tank preset is not reproducible, and § 11's central acceptance criterion is unreachable. Group 4 also unblocks any graph that needs headroom before normalizing |
| 2 | Where the current `FAC_MIXES_ID` / `FAC_SPECIAL_FUNCTIONS_ID` counts come from, given that the tool cannot read the user's tree | **Ask the user**, two numeric fields pre-filled with 2 and 11 | A hardcoded snapshot is wrong the moment the user adds a mix of their own, and silently wrong — which is the worst kind for a number the `200+10` warning depends on. Asking makes the assumption visible, and the guide restates it |
| 3 | Guide only, or guide plus a unified diff as `README_MIX_EDITOR_API § 11.2` suggests | **Guide only** | The tool cannot know the line numbers of a tree it has never seen. A patch that fails to apply costs more than a snippet a human places correctly, and a patch that applies *wrongly* is worse still |
| 4 | The receiver deadzone, which the first draft modelled as optional and off | **Ported exactly, on by default at 2 %** | It is not optional on hardware: the setting defaults to 2 and is applied to every channel of every frame. Off-by-default modelled a configuration that only exists if the user sets it to `0` — and, as § 3.4 shows, it is precisely the stage that makes full positive travel reachable |
