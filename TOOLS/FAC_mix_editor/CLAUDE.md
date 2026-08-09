# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

`FAC_mix_editor.html` is a **single self-contained HTML file** (~5400 lines: inline `<style>`, inline vanilla ES2020, no build step, no npm, no CDN, no network) in which a user assembles a F.A.C. V2 **mix** (8 inputs / 10 outputs) or **special function** (1 input / 1 output) out of a closed set of integer blocks, simulates it in the browser **bit-identically to the MCU**, and exports the `.c`/`.h` pair plus a hand-registration guide for the firmware.

The whole premise is that a mix is a composition of ~30 known integer operations, not arbitrary C — integer means the browser and the Cortex-M0 agree exactly. Any change that introduces a float into the graph math, or a second copy of a primitive, destroys the tool's reason to exist.

`PROMPT_FAC_MIX_EDITOR.md.initial` is the original build brief: the full requirement spec, decision log and acceptance criteria. Read it before any non-trivial change — but see *Precedence* below.

### Work order V2 — DONE, both phases

`PROMPT_FAC_MIX_EDITOR_V2.md` is the second work order and **all ten items are shipped**. Phase 1: §1 symmetric channel domain, §3 collapsed-group drag, §4 marquee selection, §5 grid snap, §6 right-button pan, §2 resizable panes, §7 auto-layout, §9 the *Blocks* tab. Phase 2: §8, the live 2D robot view and the closed loop that drives the simulated IMU from the robot's motion. Everything those sections describe is in the file and is documented below — read this file, not the work order, for what the tool does today.

Where §8 landed differently from the brief, and why:

- The brief's skid-steer preset is `M1+M3 / M2+M4`. **There is no M4**: `fac_settings.c:103-107` has five mapper rows, three of them motors. The pair selectors are therefore over the rows that exist, the second device of a side may be *none*, and the panel says a real four-motor skid needs a fourth row in firmware.
- Max yaw rate is displayed as the brief's `maxSpeed / track`, which is one side at full stick. Spinning on the spot is twice that; it is in the field's tooltip rather than being a second number to keep in step.

Two corrections the work order itself records, so they do not get re-proposed: scaling the motor mapper by `MOTOR_SPEED_RESOLUTION - 1` recovers nothing and truncates the whole scale, and the `FAC_servo_SET_position` clamp is load-bearing (it bounds the pulse formula), not defensive.

## Running and testing

There is no build, no package manager, no test runner.

- **Run**: open `FAC_mix_editor.html` in Chrome/Edge/Firefox (`file://` works, offline).
- **Tests**: the `SELFTEST` section is the test suite — **210 assertions** covering the `fac_math.h` port, the guarded returns, unsigned `sqrt`, the receiver-deadzone port swept over `value × deadzone × channel`, the mapper mirrors at the device end, a `simple_tank` parity grid against a hand transcription of `fac_simple_tank_mix.c`, the robot→IMU int16 boundary, and the schema migration. `runSelfTest()` runs **automatically at boot**; the *Self-test* bottom tab shows pass/fail and re-runs on demand. **Export is blocked while the self-test has not run or has any failure** — keep it that way.
- Nothing in the self-test may mutate the live project or the live `SIM`. Where it has to (the scripted robot run), it saves and restores `P`, `SIM.robot`, `SIM.outputs` and `SIM.tick` in a `finally`. That is why the schema assertions call **`migrateProject()`**, which is pure, instead of `loadProject()`, which resets the simulator and would wipe the user's scope traces on every manual re-run.
- Presets (`PRESETS`) double as regression fixtures; `simple_tank` is the acceptance test for the group-4 (plain-C) blocks, `headlessMix()` evaluates a project without the UI and is what the parity grid uses.
- Only if a browser refuses the Gamepad API on `file://`: `python -m http.server` in this folder and open `localhost`. That is a convenience, never a requirement — the tool must stay fully usable with sliders and no pad.

### Verifying a change without a browser

There is no test runner in the repo and there must not be one. When working from an agent that has no browser, build a **throwaway node harness in the scratchpad** and delete it afterwards — nothing test-shaped gets committed. What worked, and is worth rebuilding rather than reinventing:

- **The pure sections evaluate under `node`.** Slice the file from the `SECTION: FAC_MATH` banner to `SECTION: UI - palette`, run it in a `vm` context, and you get `NODES`, `PRESETS`, `analyse()`, `generate()`, `runSelfTest()` and the whole `FAC_MATH` port. Two gotchas: `const`/`class` bindings are script-scoped and never land on the context, so append an epilogue that copies the names you want onto a global; and `P` is a `let`, so export a `setP`/`getP` pair to swap the active project.
- **UI functions can still be tested** by extracting a single function's text by brace matching and evaluating it with stubs — `marqueeHits`, `snapDelta`, `collectMoveTargets`, `autoLayout`, `blockDocList`, `loadUiPrefs` and `drawRobotView` were all checked that way. Extracting the *real* function instead of restating its logic is the point: a copied assertion proves nothing. Two traps that cost time: an extracted function whose free variables are missing (`loadUiPrefs` without `LAYOUT_KEY`/`GRID_SIZES`) throws into its own `catch (e) {}` and **silently does nothing**, so every "rejected a bad value" case passes and every "kept a good value" case fails — if a whole class of assertions passes trivially, suspect the harness before the code. And a `vm` context whose `globalThis` you overwrite stops sharing bindings between scripts.
- **A canvas renderer is worth smoke-testing** against a `Proxy` standing in for the 2D context that counts calls and throws on any non-finite coordinate. Run it over the awkward shapes — 0×0, tiny, huge, nothing mapped, no drive side selected, a 1 cm chassis, a long trail — which is how a renderer's real bugs (a division by a zero dimension, an undefined field) surface without a browser.
- **A whole-script parse check** (`new vm.Script(...)` over the `<script>` body) catches what the pure-section harness cannot, since most of the UI lives outside it.
- **Two standing regression gates**: dump the generated `.c`/`.h` of every preset before and after a change and diff them, and check that every `$('#id')` in the JS matches an id that exists in the markup.

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
| `PROJECT FILES` | `.facmix.json` save/load, autosave, round-trip import from a generated `.c`, `migrateProject()` / `mergeSim()` |
| `SIM` | fixed 1 ms loop driving a simulated `HAL_GetTick()`, plus `servoPosition()` / `motorSpeed()` mirroring `fac_mapper.c`, and the robot loop: `integrateRobot()`, `robotImuCounts()`, `writeRobotImu()` |
| `GAMEPAD` | Gamepad API → the same stage-0 values the sliders write |
| `SELFTEST`, `PRESETS` | |
| `UI - palette, canvas editor, …` | also the UI prefs (`UI`, `LAYOUT_KEY`, `applyLayout()`), `snapPos()`/`snapDelta()`, the drag and marquee machinery |
| `AUTO LAYOUT` | `layoutUnits()`, `autoLayout()` — left to right by rank, over layout units |
| `UI - bottom tabs`, `UI - export, help, keyboard, autosave, boot` | also the robot view panel: `rbHost()`, `applyRobotPanel()`, `robotViewChanged()`, `rbDrag()`, `drawRobotView()` |
| `UI - block documentation` | `blockEmitDemo()`, `blockRange()`, `blockDocList()`, `uiBlocks()` — the *Blocks* tab, derived entirely from `NODES` |

### The one invariant: no second implementation

A catalogue entry declares `sim(ctx, args, params, state)` **and** `emit(ctx, args, params, node)` side by side, plus `range()`, `divs`, `out` kind, `ins` ports, `params` and a `help` string that states the division count and range constraint. The simulator calls `sim`; the generator calls `emit` and the emitted C maps one-to-one onto the same primitive. When adding a block, add all of these in one `def({...})` — never a simulation path that the generator does not mirror, or vice versa.

### Integer exactness rules (non-negotiable)

- `idiv(a,b) = Math.trunc(a/b)` — C truncates toward zero. **Never `Math.floor`, never bare `/`** on graph values.
- Group 1 clamps arguments *then* result, in the C's order (`m_mul(2000,2000) === 1000`). Group 2 does not clamp — every product goes through `mulChecked` (BigInt) and raises `FacOverflow`, surfaced on the node. **Never wrap silently**: `int32_t` overflow is UB in C.
- `m_sqrt` is the unsigned binary restoring method — `>>>` and `>>> 0`, not signed shifts.
- **There are exactly two floats in the program, and both die at a named boundary.** (1) a gamepad axis, in `PAD.padToStage0()`'s `Math.round(((axis+1)/2)*RECEIVER_CHANNEL_RESOLUTION)`. (2) the simulated robot's physical state — cm, cm/s, rad/s are floating by nature — in **`SIM.robotImuCounts()`**, which rounds *and* clamps to `int16_t` because that is what the driver hands over (`LSM6DS3.h:137-138`). Nothing downstream of either may see a non-integer, and the self-test sweeps both. `robotImuCounts()` is kept **pure** for exactly that reason; noise is added *before* the rounding so it cannot slip a fraction past it.
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

The mix drives the mapper, the mapper drives a kinematic robot, and the robot drives the simulated IMU the graph reads back. Unicycle model over two sides, `v = (vL+vR)/2`, `ω = (vR−vL)/track`, integrated at the existing 1 ms step from the mapper values scaled by the user's top speed.

- **`integrateRobot()` runs AFTER `runGraph()`**, so a pass reads the counts the previous millisecond produced. A sensor measures the past; integrating first would let a mix react to its own command with no delay at all.
- **It is a kinematic sketch, not a dynamics simulation** — no mass, no traction limit, no slip, so a step on the stick is a step in ground speed. The panel says so and that caveat stays visible. The one concession: the longitudinal acceleration is a difference quotient over a 1 ms step, i.e. hundreds of g on any stick movement, so it goes through a first-order lag of `ROBOT_ACC_TAU` (20 ms). That leaves a ramp exactly right and only rounds the corners of a step.
- **Axes: X forward, Y left, Z up**, right-handed, so a left turn is a positive gyro Z. That is a statement about how the board is *mounted* — the firmware only passes the sensor's own X/Y/Z through (`enum AXIS`, `LSM6DS3.h:143-147`).
- Gyro Z from `ω` at `GYRO_MDPS_PER_COUNT` (70, `LSM6DS3.h:130`); gyro X/Y are zero *by construction*, the sketch being planar. Accel X from the filtered `dv/dt`, Y from `v·ω`, Z holds 1 g, all at `ACCEL_UG_PER_COUNT` (488, `LSM6DS3.h:129`).
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
- **The line between the two stores runs through the robot.** Where the view sits is the person's (`UI.rb*`); *the machine* — track width, top speed, chassis size, which output drives which side, the IMU source — describes what the mix is for and is the project's (`P.sim.robot`, and the reason `SCHEMA_VERSION` is **2**).
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

The theory — the receiver chain, the group 1/2/3/4 clamp and overflow semantics, the cost model — lives **only** in that tab. The *simulator's* own theory — the kinematic model, the axis convention and its firmware citation, the two sensitivities, the one-pass sensor lag, the three-motor-row limit — lives **only** in the Robot tab, next to the controls it explains. The help modal keeps the shortcuts and the workflow and links to both. One copy of each explanation.

## Registration guide

The tool emits **no patch and no diff** and must never claim to have edited firmware — it cannot know the line numbers of a tree it has never seen. The guide is text only, states the two ID counts it *asked the user for* (`FAC_MIXES_ID` and `FAC_SPECIAL_FUNCTIONS_ID`, pre-filled 2 and 11), and every index it derives is labelled as derived from them. It must keep warning about: append-only enum edits (the active mix is stored in EEPROM as a number), refreshing the CubeIDE project or the new `.c` is silently not compiled, `FAC_SPECIAL_FUNCTION_LAST ≤ 20`, and the five mapper rows' hardcoded `200+10` ceiling — index 10 is already taken, so a special function added today registers and runs but cannot be linked to any device.

## Conventions

- Single file, offline, no external font/script/style, no telemetry, no network call. Any dependency is a regression.
- Keep the section banners and the section boundaries; do not let the file degenerate into one blob.
- Firmware API misspellings are API — `SPECIAL_FUNCITONS_NUMBER`, `special_functions_outouts`, `FAC_std_reciever_init`. Match them, do not "fix" them.
- Tabs for indentation; comments and UI text in English.
- Do not invent primitives. If it is not in `fac_math.h` or plain C already allowed by the contract, it is a firmware change, not something the generator may emit.
- Do not "simplify" the `simple_tank` preset's `diff` term into a saturated sum — a gimbal moves in a square gate; the explanation ships with the preset and with the firmware source.
