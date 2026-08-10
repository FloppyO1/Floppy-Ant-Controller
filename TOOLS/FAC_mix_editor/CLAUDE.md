# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

`FAC_mix_editor.html` is a **single self-contained HTML file** (~7700 lines: inline `<style>`, inline vanilla ES2020, no build step, no npm, no CDN, no network) in which a user assembles a F.A.C. V2 **mix** (8 inputs / 10 outputs) or **special function** (1 input / 1 output) out of a closed set of integer blocks, simulates it in the browser **bit-identically to the MCU**, and exports the `.c`/`.h` pair plus a hand-registration guide for the firmware.

The whole premise is that a mix is a composition of ~30 known integer operations, not arbitrary C — integer means the browser and the Cortex-M0 agree exactly. Any change that introduces a float into the graph math, or a second copy of a primitive, destroys the tool's reason to exist.

`PROMPT_FAC_MIX_EDITOR.md.initial` is the original build brief: the full requirement spec, decision log and acceptance criteria. Read it before any non-trivial change — but see *Precedence* below.

### Work order V2 — DONE, both phases

`PROMPT_FAC_MIX_EDITOR_V2.md.old` is the second work order and **all ten items are shipped**. Phase 1: §1 symmetric channel domain, §3 collapsed-group drag, §4 marquee selection, §5 grid snap, §6 right-button pan, §2 resizable panes, §7 auto-layout, §9 the *Blocks* tab. Phase 2: §8, the live 2D robot view and the closed loop that drives the simulated IMU from the robot's motion. Everything those sections describe is in the file and is documented below — read this file, not the work order, for what the tool does today.

Where §8 landed differently from the brief, and why:

- The brief's skid-steer preset is `M1+M3 / M2+M4`. **There is no M4**: `fac_settings.c:103-107` has five mapper rows, three of them motors. The pair selectors are therefore over the rows that exist, the second device of a side may be *none*, and the panel says a real four-motor skid needs a fourth row in firmware.
- Max yaw rate is displayed as the brief's `maxSpeed / track`, which is one side at full stick. Spinning on the spot is twice that; it is in the field's tooltip rather than being a second number to keep in step.

Two corrections the work order itself records, so they do not get re-proposed: scaling the motor mapper by `MOTOR_SPEED_RESOLUTION - 1` recovers nothing and truncates the whole scale, and the `FAC_servo_SET_position` clamp is load-bearing (it bounds the pulse formula), not defensive.

### Work order V3 — DONE, all three sections

`PROMPT_FAC_MIX_EDITOR_V3.md` turned the robot from two numbers into a machine you lay out, in three commits: **§1 the model and the migration**, **§2 the kinematics, the IMU at a position and the renderer**, **§3 the layout editors**.

§1 replaced `P.sim.robot` with a **chassis polygon, a wheel list, a device-marker list and an IMU position** — see the block comment on `blankProject()`. `lenCm`, `widCm`, `trackCm`, `kin`, `left`, `right`, `leftB` and `rightB` are gone, and `SCHEMA_VERSION` is **3**. §2 replaced §1's placeholder unicycle with the real solver. §3 made the geometry editable, numerically and by dragging.

Four things about the migration worth not rediscovering:

- **A wheel whose mapper row is not linked is *absent*, not a zero.** That is what `sideDrive()` did — its own comment said the opposite — and it is now the difference between a wheel having a rolling row and not having one.
- **A side whose second device was `none` migrates to no wheel at all.** The file said there was nothing there; an invented undriven wheel would invent drag the solver would then charge the user for.
- **A schema-2 `kin: 'diff'` project drives as it always did**, and its **IMU counts are bit-identical to what HEAD produced**. The pose agrees to 9 decimals and diverges in the last ulp or two, because a least-squares solve reaches the same number by a different arithmetic path. That is the honest statement of "unchanged" and it is what the assertions pin.
- **A migrated skid turns more slowly than it used to, and that is the correction**, not a regression: four wheels on a rigid frame really do resist yaw in proportion to the wheelbase. Do not "fix" it by migrating skid wheels to `x = 0`.

### The solver — `solveTwist()`

Every wheel makes two statements about the body twist `ξ = (vx, vy, ω)`: it **rolls** at the speed its motor was told (`(u·d − s)²`, weight 1, driven wheels only) and it **does not slide sideways** (`(u·n)²`, weight `grip`, every wheel), with `u = (vx − ωy, vy + ωx)`. Both are linear in `ξ`, so it is a weighted least squares: `M ξ = c`, `M = Σ w·a·aᵀ`, solved by explicit Cramer on the 3×3. The per-wheel residuals come back with the answer — **they are the slip, and nothing on screen is a number that is not one of them**.

- **The ridge is conditional, and that is a deliberate departure from the work order**, which asks for it unconditionally. An always-on Tikhonov ridge turns `vx = (sL+sR)/2` into `(sL+sR)/(2+λ)`: it taxes every well-posed machine to rescue the degenerate ones. Measured, that cost an omni machine 119.998 cm/s instead of 120 and put the four-wheel skid 3e-7 off its own closed form at `grip 0`. So it fires in two steps: **a zero on the diagonal** (a whole component nobody constrained — `grip 0` everywhere leaves `M[1][1]` at nothing) is ridged *alone*, and only a rank deficiency invisible on the diagonal (one wheel, every wheel at the same point) takes the full ridge. `c` is zero in every null direction of `M`, so the ridge only ever pulls an unconstrained direction to zero — it decides nothing it was not asked to.
- **The three closed forms hold, and are asserted against the algebra, not against the solver.** Two parallel wheels at `x=0, y=±b` give `vx=(sL+sR)/2, vy=0, ω=(sR−sL)/2b` **exactly, to the bit**. Four wheels at `x=±a, y=±b, grip g` give `ω = (sR−sL)/(2b + 2·g·a²/b)` — exact except for one layout where the algebra factors differently and it lands 1 ulp out. An **omni wheel is `grip = 0` and nothing else**; mecanum needs no special case anywhere.
- **A wheel's steering angle is live**: `deg` plus a `steer` mapper row scaled by `steerDeg`. `wheelAngle()` is called by the solver *and* by the renderer, so a wheel is never drawn at an angle the physics did not use.
- **The *max yaw* readout calls the solver** (`maxYawRate()`), never a formula. `maxSpeed/track` is only the answer for two parallel wheels.

### The IMU sits somewhere

`a_x = v̇x − ω·vy − α·y − ω²·x`, `a_y = v̇y + ω·vx + α·x − ω²·y`. At the origin with `vy = 0` this is `(v̇x, ω·vx)` — the general form **contains** the pre-§2 model. All three of `v̇x`, `v̇y`, `α` go through the existing `ROBOT_ACC_TAU` lag (`S.dvx/dvy/dw` are the lagged derivatives; `S.ax/ay` are what the sensor reads, which is a different thing). Spinning on the spot with the IMU at radius `r` reads `ω²r` **pointing at the centre** — asserted as a vector, since the sign is the half that can be wrong. Where the IMU sits **never moves the chassis**, and that is asserted too.

### The layout editor

- **One mapping, both directions.** `robotViewMap(cv)` hands out `sc`/`cX`/`cY` *and* `toScreen`/`toBody`; the renderer draws with the first and the hit test grabs with the second. A handle that does not stay under the pointer that grabbed it comes from exactly those two disagreeing, so the self-test pins that they are inverses to 1e-9 across poses, headings, `follow` and both view modes. It takes a bare `{clientWidth, clientHeight}`, which is what lets all of this run headless.
- **The tables build their shell once and re-render only the rows.** A field handler writes the model and updates the one derived span beside it; nothing else moves. Only add, remove and the *end* of a canvas drag rebuild rows — a drag calls `syncRobotGeom()`, which pushes model→inputs and skips the focused element.
- **A press on empty ground is not a drag.** `robotHitTest()` returns the nearest handle within `RB_GRAB_PX`, or null; the IMU is first in `robotHandles()` so it wins a tie against a wheel sitting on it. The vertices are *drawn* because they are draggable — an outline with nothing to grab is one you cannot edit.
- **The panel drag needs no extra guard**, and the comment says why rather than guarding twice: `rbDrag()` is bound to `#rbHead`/`#rbGrip` and the canvas is inside `#rbBody`, a sibling of both. The DOM has already decided.
- **There is no grid snap here, deliberately.** `UI.grid` is a spacing in *editor pixels* for the graph canvas and means nothing in cm; reusing it would snap a wheel to a grid set for something else. Positions land on 0.1 cm, which is only the resolution the fields show. The tab says so.
- **The layout is not undoable**, because `applySnap()` restores nodes, wires, mode and instances only. That is true of every other `P.sim` field; making this one list the exception would be the odd thing.

## Running and testing

There is no build, no package manager, no test runner.

- **Run**: open `FAC_mix_editor.html` in Chrome/Edge/Firefox (`file://` works, offline).
- **Tests**: the `SELFTEST` section is the test suite — **380 assertions** covering the `fac_math.h` port, the guarded returns, unsigned `sqrt`, the receiver-deadzone port swept over `value × deadzone × channel`, the mapper mirrors at the device end, a `simple_tank` parity grid against a hand transcription of `fac_simple_tank_mix.c`, the robot→IMU int16 boundary, the sensor model (superposition, the ODR hold, the per-run offset, and that it never moves the chassis), the fixed view's scale, the geometry helpers, the schema 2→3 reconstruction, a 300-step run pinning a migrated machine against a new one, the solver against its three closed forms, every degenerate layout staying finite, the live steering, the slip residuals, the accelerometer at a position, and the layout editor's screen↔body mapping, hit test and typed coercions. `runSelfTest()` runs **automatically at boot**; the *Self-test* bottom tab shows pass/fail and re-runs on demand. **Export is blocked while the self-test has not run or has any failure** — keep it that way.
- Nothing in the self-test may mutate the live project or the live `SIM`. Where it has to (the scripted robot run), it saves and restores `P`, `SIM.robot`, `SIM.outputs` and `SIM.tick` in a `finally`. That is why the schema assertions call **`migrateProject()`**, which is pure, instead of `loadProject()`, which resets the simulator and would wipe the user's scope traces on every manual re-run.
- Presets (`PRESETS`) double as regression fixtures; `simple_tank` is the acceptance test for the group-4 (plain-C) blocks, `headlessMix()` evaluates a project without the UI and is what the parity grid uses.
- Only if a browser refuses the Gamepad API on `file://`: `python -m http.server` in this folder and open `localhost`. That is a convenience, never a requirement — the tool must stay fully usable with sliders and no pad.

### Verifying a change without a browser

There is no test runner in the repo and there must not be one. When working from an agent that has no browser, build a **throwaway node harness in the scratchpad** and delete it afterwards — nothing test-shaped gets committed. What worked, and is worth rebuilding rather than reinventing:

- **The pure sections evaluate under `node`.** Slice the file from the `SECTION: FAC_MATH` banner to `SECTION: UI - palette`, run it in a `vm` context, and you get `NODES`, `PRESETS`, `analyse()`, `generate()`, `runSelfTest()` and the whole `FAC_MATH` port. Four gotchas: `const`/`class` bindings are script-scoped and never land on the context, so append an epilogue that copies the names you want onto a global; `P` is a `let`, so export a `setP`/`getP` pair to swap the active project (and *seed* it — `runSelfTest()` reaches `projectForSave()`, which in the browser runs after boot has loaded a project, so an unseeded `P` is `null` and it throws); get the slice boundaries off by one and it ends mid-statement, which reads as a baffling `SyntaxError` in whatever you appended next; and `runSelfTest()` ends by calling `uiSelfTestBanner()` and `uiTest()`, which need stubs. The suite also now calls **`robotScale()`**, which lives in the UI half next to its renderer — append it to the slice or the robot-view assertions throw rather than run.
- **UI functions can still be tested** by extracting a single function's text by brace matching and evaluating it with stubs — `marqueeHits`, `snapDelta`, `collectMoveTargets`, `autoLayout`, `blockDocList`, `loadUiPrefs` and `drawRobotView` were all checked that way. Extracting the *real* function instead of restating its logic is the point: a copied assertion proves nothing. Two traps that cost time: an extracted function whose free variables are missing (`loadUiPrefs` without `LAYOUT_KEY`/`GRID_SIZES`) throws into its own `catch (e) {}` and **silently does nothing**, so every "rejected a bad value" case passes and every "kept a good value" case fails — if a whole class of assertions passes trivially, suspect the harness before the code. And a `vm` context whose `globalThis` you overwrite stops sharing bindings between scripts.
- **A canvas renderer is worth smoke-testing** against a `Proxy` standing in for the 2D context that counts calls and throws on any non-finite coordinate. Run it over the awkward shapes — 0×0, tiny, huge, nothing mapped, no drive side selected, a 1 cm chassis, a long trail — which is how a renderer's real bugs (a division by a zero dimension, an undefined field) surface without a browser.
- **A whole-script parse check** (`new vm.Script(...)` over the `<script>` body) catches what the pure-section harness cannot, since most of the UI lives outside it.
- **Two standing regression gates**: dump the generated `.c`/`.h` of every preset before and after a change and diff them (against `git show HEAD:...`, stripping the `FAC_MIX_EDITOR_PROJECT` trailer, which carries the project and is *supposed* to move), and check that every `$('#id')` in the JS matches an id that exists in the markup — `selbox` is a known false positive there, since it is `createElement`d on demand.
- **A new assertion is worth mutation-testing.** Break the thing it covers on purpose, confirm it goes red, put it back. That is what caught `T('…', m2.sim.robot.viewCm, d.robot.viewCm)`: it compared a migrated value against *the default itself*, so both sides moved together and changing the default kept it green. The assertion that bites is the property — the default view is `> 0`, i.e. a new project does not open in the auto-fit.

A caveat that cost a real bug: the harness does **not** exercise the pointer interaction, so a change to the drag, the marquee, the splitters or the panning is not verified until it is opened in a browser. `Array.prototype.slice.call(aSet)` returning `[]` — a `Set` is not array-like — shipped past a clean parse check because nothing executed that line.

## Architecture

One file, sections marked with banner comments — grep `SECTION:` to jump. Order of declaration matters (plain script, no modules).

| Section | Owns |
|---|---|
| `FAC_MATH` | line-by-line port of `fac_math.h`. `idiv()`, `m_*()` primitives, `FacOverflow`, `chk32`, `mulChecked` |
| `RECEIVER` | `map_int32`, `rxDeadzone`, `rxStoreClamp`, `rxStage1/2` — the four-stage channel chain |
| `INTERVALS` | `IV` interval algebra; a proven `[lo,hi]` per wire |
| `NODE_CATALOG` | `NODES{}` via `def({...})` — the single source of truth for every block |
| `GRAPH` | project model `P`, `topoOrder()`, undo/redo |
| `VALIDATE` | `analyse()` → range analysis + findings (`error` blocks export, `warn` does not) |
| `CODEGEN` | `makeCtx()`, `genBody()`, `buildMixC/H`, `buildFunctionC/H` |
| `REGISTRATION` | `registrationGuide()` — text only |
| `PROJECT FILES` | `.facmix.json` save/load, autosave, round-trip import from a generated `.c`, `migrateProject()` / `mergeSim()` / `migrateRobotGeometry()` |
| `ROBOT MODEL` | the machine's geometry and the kinematics that reads it, pure and on its own: `chassisBounds()`, `wheelTrack()`, `wheelAngle()`, `wheelGrip()`, `solveTwist()`, `maxYawRate()` |
| `SIM` | fixed 1 ms loop driving a simulated `HAL_GetTick()`, plus `servoPosition()` / `motorSpeed()` mirroring `fac_mapper.c`, and the robot loop: `integrateRobot()`, `sensorOffsets()`, `robotImuCounts()`, `writeRobotImu()` |
| `GAMEPAD` | Gamepad API → the same stage-0 values the sliders write |
| `SELFTEST`, `PRESETS` | |
| `UI - palette, canvas editor, …` | also the UI prefs (`UI`, `LAYOUT_KEY`, `applyLayout()`), `snapPos()`/`snapDelta()`, the drag and marquee machinery |
| `AUTO LAYOUT` | `layoutUnits()`, `autoLayout()` — left to right by rank, over layout units |
| `UI - bottom tabs`, `UI - export, help, keyboard, autosave, boot` | also the robot view panel: `rbHost()`, `applyRobotPanel()`, `robotViewChanged()`, `rbDrag()`, `robotViewMap()`, `drawRobotView()`, and the layout editor: `renderRobotGeom()`, `syncRobotGeom()`, `robotHitTest()`, `initRobotGeomDrag()` |
| `UI - block documentation` | `blockEmitDemo()`, `blockRange()`, `blockDocList()`, `uiBlocks()` — the *Blocks* tab, derived entirely from `NODES` |

### The one invariant: no second implementation

A catalogue entry declares `sim(ctx, args, params, state)` **and** `emit(ctx, args, params, node)` side by side, plus `range()`, `divs`, `out` kind, `ins` ports, `params` and a `help` string that states the division count and range constraint. The simulator calls `sim`; the generator calls `emit` and the emitted C maps one-to-one onto the same primitive. When adding a block, add all of these in one `def({...})` — never a simulation path that the generator does not mirror, or vice versa.

### Integer exactness rules (non-negotiable)

- `idiv(a,b) = Math.trunc(a/b)` — C truncates toward zero. **Never `Math.floor`, never bare `/`** on graph values.
- Group 1 clamps arguments *then* result, in the C's order (`m_mul(2000,2000) === 1000`). Group 2 does not clamp — every product goes through `mulChecked` (BigInt) and raises `FacOverflow`, surfaced on the node. **Never wrap silently**: `int32_t` overflow is UB in C.
- `m_sqrt` is the unsigned binary restoring method — `>>>` and `>>> 0`, not signed shifts.
- **There are exactly two floats in the program, and both die at a named boundary.** (1) a gamepad axis, in `PAD.padToStage0()`'s `Math.round(((axis+1)/2)*RECEIVER_CHANNEL_RESOLUTION)`. (2) the simulated robot's physical state — cm, cm/s, rad/s are floating by nature — in **`SIM.robotImuCounts()`**, which rounds *and* clamps to `int16_t` because that is what the driver hands over (`LSM6DS3.h:137-138`). Nothing downstream of either may see a non-integer, and the self-test sweeps both. `robotImuCounts()` is kept **pure and deterministic** for exactly that reason: the sensor error arrives as an *argument*, floats in counts from `SIM.sensorOffsets()` — the one impure place, where the random draws live — and is superimposed *before* the rounding so it cannot slip a fraction or an out-of-range value past the boundary.
- No shift blocks are offered: `-3 >> 1` is `-2` while `-3 / 2` is `-1`, and a tool premised on bit-identical arithmetic must not hand the user two operations that look interchangeable.

### The channel chain — four stages, and the travel is symmetric

```
0  backend capture        0 .. 1000     fac_pwm_receiver.c:82, fac_ppm_receiver.c:62
1a deadzone               0 .. 1000     FAC_std_receiver_calculate_dead_zone()
1b store clamp       clamp to 1000      FAC_std_receiver_SET_channel()
2  mix input          -1000 .. +1000    FAC_math_from_range(ch, 0, 1000)
```

The channel domain is `0 … RECEIVER_CHANNEL_RESOLUTION` **inclusive — 1001 values**, so it has an exact centre: `rx = 500` normalises to exactly `0`, and `mix_input = 2*rx - 1000` holds across the whole chain with no rounding. Sliders and the gamepad write stage 0 only; the deadzone (on by default at 2 %, ported verbatim including the channel-3 no-centre-deadzone case, 1-based channel number) is firmware behaviour and is modelled, not optional. Keep the stages apart — they are separate functions with separate domains in the source, and collapsing them either double-applies the deadzone or invents travel the hardware does not have. Stage 1b is a no-op in practice now, and stays as the guard the source has.

This used to be asymmetric — a mix input reached `-1000` but only `+998`, because the capture and the store clamped to `RECEIVER_CHANNEL_RESOLUTION - 1` while the deadzone and `FAC_math_from_range` already spoke `0…1000`. That was a firmware bug and it has been fixed on the firmware side; `SECTION: RECEIVER` keeps the history because it is why the stages are modelled separately at all.

The device end, mirrored by `SIM.servoPosition` / `SIM.motorSpeed` from `fac_mapper.c`:

```
servo   m_to_range(v, 0, MAX_SERVO_VALUE)      0 .. 999     fac_mapper.c:102
motor   abs(v), sign carried as direction      0 .. 1000    fac_mapper.c:77
```

The servo targets `MAX_SERVO_VALUE` (999) because that is the divisor of the pulse formula at `fac_servo.c:98` — that makes the map uniform over the travel, at the price of the centre landing on 499. The motor conversion is exact (both resolutions are 1000) and `±1000` is a true 100 % duty.

### The loop is closed — the robot drives the IMU

The mix drives the mapper, the mapper drives a kinematic robot, and the robot drives the simulated IMU the graph reads back. The twist comes from `solveTwist()` in `SECTION: ROBOT MODEL` — see *The solver* above — and is integrated at the existing 1 ms step.

- **`integrateRobot()` runs AFTER `runGraph()`**, so a pass reads the counts the previous millisecond produced. A sensor measures the past; integrating first would let a mix react to its own command with no delay at all.
- **It is a kinematic sketch, not a dynamics simulation** — no mass, no inertia, no traction limit, no motor curve, so a step on the stick is a step in ground speed. **The lateral grip is a kinematic weight, not a friction coefficient**, and the Robot tab must not let anyone read it as newtons. The panel says so and that caveat stays visible. The one concession: each twist component's derivative is a difference quotient over a 1 ms step, i.e. hundreds of g on any stick movement, so all three go through a first-order lag of `ROBOT_ACC_TAU` (20 ms). That leaves a ramp exactly right and only rounds the corners of a step.
- **Axes: X forward, Y left, Z up**, right-handed, so a left turn is a positive gyro Z. That is a statement about how the board is *mounted* — the firmware only passes the sensor's own X/Y/Z through (`enum AXIS`, `LSM6DS3.h:143-147`).
- Gyro Z from `ω` at `GYRO_MDPS_PER_COUNT` (70, `LSM6DS3.h:130`); gyro X/Y are zero *by construction*, the sketch being planar. Accel X and Y from the two expressions in *The IMU sits somewhere* above, Z holds 1 g, all at `ACCEL_UG_PER_COUNT` (488, `LSM6DS3.h:129`).
- **The view is a fixed window on the ground, not a fit.** `P.sim.robot.viewCm` is how much ground the view shows across its **short** side, and pinning it is what makes the screen speed *be* the ground speed. The auto-fit it replaced grew with the trail, so driving twice as fast zoomed out twice as fast and exactly cancelled: measured on a 420×300 canvas, a **10× change in top speed moved the robot on screen 1.00× faster**, and the whole picture crawled more the longer it ran (98 → 28 px/s over 6 s). `viewCm = 0` keeps that behaviour as an explicit choice, for reading back a finished run where the *shape* of the trail is the subject and the speed is not. It is pixels only — `robotScale()` is downstream of everything, and the self-test asserts that the same run writes the same pose and the same IMU counts at any view size.
- **The sensor is modelled too, and it is opt in.** A real LSM6DS3 hands over the motion *plus an error*, so `SIM.sensorOffsets()` superimposes one on the counts — four project fields, all in raw counts, all defaulting to 0 so nothing built before it exists starts behaving differently. Two effects, because one number cannot stand for both: `gyroNoise`/`accelNoise` are **one sigma of a gaussian** (a datasheet's noise density is an RMS, and a sum of small physical effects is gaussian, not uniform), redrawn at the **sensor's own 416 Hz output data rate** and held in between (`IMU_SAMPLE_MS`, `LSM6DS3.c:138,147` — a fresh draw every 1 ms pass would let a filter average away 2.4× more than the real part gives it the chance to); `gyroBias`/`accelBias` are the **peak of one constant per axis for the whole run**, which does *not* average out and is what makes an integrated gyro walk. The bias is stored as a **unit draw** in `SIM.robot.bias` and scaled by the field on every use, so turning the peak up scales the machine already drawn instead of rolling a different one — a *reset* is what draws a new one. The gyro numbers are the smaller ones because the driver measures and subtracts the gyro zero rate at boot over 128 samples (`LSM6DS3_calculate_offset`, `LSM6DS3.c:194-222`), leaving only the drift after it, while the accelerometer is never offset-calibrated (`X/Y/Z_OFS_USR` are not written). **It moves counts and nothing else**: the chassis integration is untouched (asserted on an open-loop run), the generated `.c`/`.h` of every preset is byte-identical, and on the manual source the whole model is inert — the Sensors tab's `noise` generator is the answer there. A pre-split file carrying the old single `noise` **peak** migrates to `peak/√3`, the sigma with the same noise power.
- The user can put the IMU back **on the sliders** (`P.sim.robot.imuSource`), and that path is bit-identical to the pre-§8 behaviour — including the resting `accel:[0,0,1000]` default, which was deliberately *not* "corrected" to 1 g. While the robot is driving, `applyGenerators()` returns early (every generator target is a gyro or accel axis, so writing and losing it one line later would be dishonest) and the Sensors tab shows the sliders **disabled but live**, as gauges.

### Precedence

Firmware source > firmware docs > the build brief > this file. The brief's § 3.4 claimed the channel domain is `0…1000` while the source's store clamp said `0…999`; the brief was describing the intent and the source has now caught up with it, so the two agree. When the brief and the source disagree, follow the source and leave a comment saying which line of which file settles it.

Firmware sources of truth live in the sibling submodule `../../FIRMWARE V2/FAC-Firmware-V2/`:
`docs/README_MIX_EDITOR_API.md` (the contract), `docs/README_API.md`, its own `CLAUDE.md`, `Core/Inc/FAC_Code/mixes_functions/fac_math.h` (exact semantics of every primitive), `fac_mixes.h` / `fac_functions.h`, `Core/Src/FAC_Code/fac_std_receiver.c` (the deadzone), `fac_settings.c` (mapper rows, defaults), and the two conformance examples `fac_simple_tank_mix.c` / `fac_direct_link_function.c`.

## Code generation rules

The generator owns exactly four regions; **everything else is byte-identical to the firmware template**, including the 9-step recipe header:

1. the `/* CUSTOM INCLUDE */` block (only headers the graph actually uses; `fac_imu.h` already pulls `LSM6DS3.h`, so no second include for the axis macros),
2. file-scope `static`s under `/* PRIVATE FUNCTIONS AND VARIABLES */`,
3. the `DESCRIPTION` comment and the `#define INPUT_*` / `OUTPUT_*` lines,
4. the body between `/* INSERT YOUR CODE HERE -START- */` and `-END-`.

Four bounded exceptions the committed examples already make: the file name, `Created on:` / `Author:`, the ID + update-function name, and the `(void)` parameter list (the template's empty `()` is a C23 error).

Emission constraints worth not rediscovering:

- `ctx.stat()` declares statics and, for a multi-instance special function, **always** makes them an array indexed by `instanceSlot` — a shared scalar across instances is unrepresentable by construction.
- `ctx.resetIf()` collects the disarm-gap resets; every stateful block must expose an explicit *reset / hold* policy (`onResume`), and the graph emits the `(uint32_t)(now - lastRun) > 100` gap guard when any state exists.
- `ctx.rxLocal()` reads each RX channel once into a local — `FAC_std_receiver_GET_channel()` is not a free field read.
- Any IMU block forces the `FAC_IMU_update()` + `FAC_IMU_GET_status()` guard preamble with a user-chosen fallback; the main loop never calls `FAC_IMU_update()` itself.
- Battery reads are always behind a tick-gated cache — the accessor is float-based and re-reads the ADC five times.
- Forbidden in generated code: `float`/`double`, `math.h`, `stdlib.h` (`abs()`), `main.h` (so `1`/`0`, never `TRUE`/`FALSE`), `malloc`, VLA, recursion, `HAL_Delay` or any wait loop, any device/settings/EEPROM write, and `map_int32()` as a block (it clamps only one end and costs a 64-bit helper call on an M0).
- Tabs, K&R braces, English identifiers, `-Wall` clean, no unused locals, keep `__attribute__((unused))` on the ID marker.

## Editor behaviour worth not rediscovering

- **One `snapPos()`.** Every path that produces a node position goes through it — palette drop, double-click insert, paste, arrow nudge, drag, auto-layout. A drag snaps its **delta** against the block you grabbed (`snapDelta()`), never each node's absolute position: snapping absolutely makes a multi-selection drift apart. Paste snaps the offset once for the same reason. The snap bypass reads `e.ctrlKey` from the live mouse event — `keyState` goes stale, because its `keydown` handler returns early inside an input.
- **A move drag resolves its DOM targets once**, at mousedown, into `dragState.doms` and `dragState.boxes`. A collapsed group's members are not rendered at all, so a per-frame `[data-id=…]` lookup finds nothing; the box tracks `min(x), min(y)` of its members and updates `GROUP_BOX` so the wires follow. Visual updates are coalesced into one `requestAnimationFrame`.
- **The marquee previews live** and commits on mouseup; there is deliberately **no `click` listener** clearing the selection, because it fired after the mouseup and wiped what the marquee had just built. `refreshGeometry()` re-measures before a marquee so the hit test never guesses a width.
- **Pan is decided first** in the mousedown handler — right, middle, Alt or Space — so a pan that starts on a block pans instead of dragging it. Bare letter shortcuts (`F`, `L`) are gated on no modifier, or `Ctrl+F` steals the browser's find.
- **UI preferences live in `localStorage` under `facmix.layout.v1`**, never in the project file and never in `SCHEMA_VERSION`: pane sizes, collapse state, compact density, grid size and the robot panel's geometry/dock/opacity are a property of the person, not of the mix. `AUTOSAVE_KEY` is a separate key and the two must not read each other. `applyLayout()` is the single place a preference becomes CSS, and it calls `applyRobotPanel()` so nothing else has to remember to.
- **The line between the two stores runs through the robot.** Where the view *sits* is the person's (`UI.rb*`); *the machine* — the chassis polygon, where each wheel sits and what drives it, the device markers, where the IMU is bolted, the top speed, the IMU source, how noisy its sensor is — describes what the mix is for and is the project's (`P.sim.robot`, and the reason `SCHEMA_VERSION` is **3**). How much *ground* the view shows (`viewCm`) is the project's too, and deliberately: an arena size is context of the machine, a mix is tuned for one, and it is where `cmPerPx` already lived. It is the one view setting on that side of the line.
- **A saved project is completed by `migrateProject()`**, which is pure and builds onto a `blankProject()` it never writes into. The bug it fixes is worth not reintroducing: the old code did `P = Object.assign(b, o)` and *then* used `b.sim` as the base of the merge — by which point `b.sim` **was** `o.sim`, so a file carrying a `sim` block was merged onto itself and nothing it omitted was ever defaulted. `chAssign`, `disarmMs` and `reg.fnCount` came back `undefined` and had done so all along.
- **The robot view is one panel, one renderer, three homes** — floating over `#editor`, docked in the right column, or docked in the Robot tab — and it draws whenever the simulator runs, whatever bottom tab is open. `drawRobotView(canvas)` takes its canvas as an argument and sizes it from `clientWidth/Height`, so a hidden host (collapsed pane, unopened tab) is a zero-size box and an early return rather than a special case. The Robot tab both *hosts* the panel and *describes where it is*, which is why `uiRobot()` parks the panel in `#editor` before overwriting its own `innerHTML`, and why every change of home or visibility goes through `robotViewChanged()`.
- **A collapsed group is visual only**, and blocks marked `noGroup` in the catalogue — the mix/function inputs and outputs — are never grouped: they are where the graph begins and ends. `loadProject()` strips a stale group off them, so the rule holds for older projects too.
- **Auto-layout arranges layout units, not nodes**: a collapsed group is one unit at its box size and its members move rigidly inside it. Cycles are broken on the edge *into* a stateful block — the same edge that makes them legal. A freshly loaded preset is laid out once, with `undoSuspend` set, because the presets' hand-written coordinates overlap; a restored session keeps the user's positions.
- **Positions are not semantics.** The generated C is identical before and after a layout. The `.c` is *not* byte-identical, because it carries the project as JSON for round-trip import and that JSON holds the coordinates — which is the point of it. When a work order asks for a byte-identical `.c` across a layout, that is the clause it did not account for.
- **`openTab()` is the one way a bottom tab opens**, so a link from the palette or the inspector behaves exactly like clicking it (and un-collapses the bottom panel if needed). `TAB_RENDER` maps a tab name to its render function.
- **A list with a filter must not rebuild its own filter controls.** The *Blocks* tab builds its shell once and re-renders only `#bdList`; rebuilding the whole panel on every keystroke destroyed the input being typed into and took the focus and caret with it. The same shape applies to any filtered list added later — the robot panel's control strip is in the markup and is only ever *synced* (`syncRobotCtl()`), because `applyRobotPanel()` runs on every frame of a splitter or panel drag.
- There is **no UI scale control**: it was built and then removed on the user's call, because the browser's own zoom covers the case. `--ui-dense` survives for the *compact* switch and multiplies chrome paddings only — fonts are plain px, and nothing in `.node`/`.port`/`.prow` uses it, which is what keeps node metrics fixed in world coordinates.

## Validation

`analyse()` propagates an interval per wire and reports at graph-build time. A **group-4 (plain C) node whose interval cannot be *proven* to fit `int32_t` is an error, not a warning** — that group has no clamp and no guard, and the interval analysis is the only thing between the user and UB. Other errors: cycles without a stateful break, unconnected required ports, a duplicated output index, `raw_div_lit` with `k = 0`, a violated documented range constraint, IMU without preamble, unindexed multi-instance state.

## Block documentation

The *Blocks* bottom tab is **generated from `NODES` and nothing else**. Ports, parameters, division count, proven range and the `help` string are read out of the catalogue entry, and the C shown is obtained by calling the block's **own `emit()`** with placeholder arguments on a throwaway `makeCtx()` — which is already isolated, since every `makeCtx()` call returns a fresh context. That is what makes the documentation unable to drift: a hand-written sentence about a block that already says it in its `help` is a bug, and adding a `def({...})` must make a card appear with no other edit.

The theory — the receiver chain, the group 1/2/3/4 clamp and overflow semantics, the cost model — lives **only** in that tab. The *simulator's* own theory — the kinematic model, the axis convention and its firmware citation, the two sensitivities, the one-pass sensor lag, the three-motor-row limit, the fixed view, and the sensor model (noise vs offset, the ODR hold, why the gyro offset is the smaller number) — lives **only** in the Robot tab, next to the controls it explains. The help modal keeps the shortcuts and the workflow and links to both. One copy of each explanation.

## Registration guide

The tool emits **no patch and no diff** and must never claim to have edited firmware — it cannot know the line numbers of a tree it has never seen. The guide is text only, states the two ID counts it *asked the user for* (`FAC_MIXES_ID` and `FAC_SPECIAL_FUNCTIONS_ID`, pre-filled 2 and 11), and every index it derives is labelled as derived from them. It must keep warning about: append-only enum edits (the active mix is stored in EEPROM as a number), refreshing the CubeIDE project or the new `.c` is silently not compiled, `FAC_SPECIAL_FUNCTION_LAST ≤ 20`, and the five mapper rows' hardcoded `200+10` ceiling — index 10 is already taken, so a special function added today registers and runs but cannot be linked to any device.

## Conventions

- Single file, offline, no external font/script/style, no telemetry, no network call. Any dependency is a regression.
- Keep the section banners and the section boundaries; do not let the file degenerate into one blob.
- Firmware API misspellings are API — `SPECIAL_FUNCITONS_NUMBER`, `special_functions_outouts`, `FAC_std_reciever_init`. Match them, do not "fix" them.
- Tabs for indentation; comments and UI text in English.
- Do not invent primitives. If it is not in `fac_math.h` or plain C already allowed by the contract, it is a firmware change, not something the generator may emit.
- Do not "simplify" the `simple_tank` preset's `diff` term into a saturated sum — a gimbal moves in a square gate; the explanation ships with the preset and with the firmware source.
