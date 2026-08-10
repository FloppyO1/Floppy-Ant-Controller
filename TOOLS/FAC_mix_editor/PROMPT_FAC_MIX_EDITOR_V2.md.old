# F.A.C. Mix Editor — work order V2

Ten changes to `TOOLS/FAC_mix_editor/FAC_mix_editor.html`, plus one coordinated
firmware fix in the `FIRMWARE V2/FAC-Firmware-V2` submodule.

Read `CLAUDE.md` in this folder first, and `PROMPT_FAC_MIX_EDITOR.md.initial` for
anything this document does not cover. Every invariant in `CLAUDE.md` still holds
unless this document overrides it explicitly, and it overrides exactly one thing:
the channel domain (§1).

**This document has been revised against the firmware sources.** Every `file:line`
below was opened and read, not inferred. Where the first draft of this work order
was wrong about the firmware, the text now states the verified behaviour instead —
see in particular §1 *Output side*, where the originally proposed motor fix
recovered nothing and the servo claim was inaccurate. Appendix A records the
defects found on the way that are deliberately **not** being fixed, and Appendix B
is the complete audit of the old-domain constants that §1 must eliminate.

## Ground rules for all ten items

- **Still one file.** No build step, no npm, no CDN, no font, no network call, no
  telemetry. It must keep working from `file://` with the devtools console clean.
- **No second implementation.** Every block stays a single `def({...})` declaring
  `sim` and `emit` side by side. The new documentation tab (§9) must *derive* its
  content from `NODES` — if it contains one hand-written sentence about a block
  that is already in that block's `help`, it is wrong.
- **No float in the graph math.** There are currently exactly two float boundaries:
  the gamepad axis conversion (`PAD.padToStage0`, ~line 2560) and the illustrative
  robot integrator (`SIM.integrateRobot`, ~line 2459). §8 grows the second one — it
  must still terminate in `Math.round(...)` before any value reaches `SIM.gyro` /
  `SIM.accel`, and a self-test assertion must prove it.
- **The self-test is the contract.** `runSelfTest()` runs at boot, export stays
  blocked while it has not run or has a failure. Every item below that changes
  arithmetic must add assertions, not just avoid breaking the existing ones.
- **Keep the section banners.** New code goes inside the right `SECTION:`, or gets
  its own banner. Do not let the file turn into one blob.
- Tabs for indentation, English identifiers and UI text, comments explaining *why*
  and citing `file:line` of the firmware when they encode firmware behaviour.

## Phases and order of work

**Phase 1 — mandatory:** §1 → §3 → §4 → §5 → §6 → §2 → §7 → §9.
§1 first because it changes the model everything else is validated against; the
canvas interaction fixes (§3-§6) next because they are small and independent; the
two big UI features (§2, §7, §9) after them.

**Phase 2 — deferred:** §8, the live robot view and the IMU closed loop. It is
fully specified at the end of this document so that it does not need to be
redesigned when it is picked up, but it is not part of the phase 1 delivery.

Work in stages: stop after each § for a browser check before starting the next
one. One commit per §.

---

## §0 — Verification protocol

There is no browser in the implementation environment and there must be no test
runner in the repository, so verification happens through a **throwaway node
harness** built in the session scratchpad and never committed.

- Node 24 is available. The harness extracts the pure sections of the HTML
  (`FAC_MATH`, `RECEIVER`, `INTERVALS`, `NODE_CATALOG`, `GRAPH`, `VALIDATE`,
  `CODEGEN`, `SELFTEST`, `PRESETS`) by their `SECTION:` banners, evaluates them
  with minimal stubs for the handful of DOM symbols they touch, and calls
  `runSelfTest()` and `headlessMix()`.
- The harness lives in the scratchpad only. It is a verification aid for the
  implementer, not an artefact: `CLAUDE.md` says no test runner, and that stands.
- Every § below carries a **Done when** block. Those are the acceptance gates; a
  § is not finished until every line of its block has actually been executed, not
  reasoned about.
- The one thing the harness cannot cover is the pointer interaction of §2-§7.
  Those sections' gates are explicitly manual and must be checked in a browser by
  the user before the next § starts.

---

## §1 — Channel domain becomes symmetric: −1000 … +1000

### The problem, precisely

The channel domain is *inconsistent between producers and consumers*, and the
asymmetric mix input is the visible symptom of that.

| stage | file | domain produced/assumed |
|---|---|---|
| capture (PWM) | `fac_pwm_receiver.c:82-84` | `map_uint32(..., 0, RESOLUTION-1)` then clamp to `RESOLUTION-1` → **0…999** |
| capture (PPM) | `fac_ppm_receiver.c:62` | same → **0…999** |
| deadzone | `fac_std_receiver.c:86-127` | maps in from `0…RESOLUTION` and back out to `0…RESOLUTION` → **already written for 0…1000** |
| store | `fac_std_receiver.c:60-67` | clamps to `RESOLUTION-1` → **0…999** |
| mix input | `fac_mixes.c:111-115`, `fac_functions.c:79-83` | `FAC_math_from_range(rx, 0, RESOLUTION)` → **assumes 0…1000** |

Two of the three stages already speak `0…1000`. Only the capture and the store
use `RESOLUTION-1`. That mismatch is what makes a mix input reach `-1000` but
only `+998`.

### Chosen fix — widen the input domain to `0…1000`

Rationale, so it does not get re-litigated: `0…1000` is 1001 values and therefore
has an **exact centre** — `rx = 500` gives `from_range(500,0,1000) = 0` with no
rounding, and the documented identity `mix_input = 2*rx - 1000` holds exactly.
Narrowing the consumer to `0…999` instead would make the stick centre land on
`+1` with the deadzone off, because 1000 values have no exact middle.

**Firmware — input side:**

1. `Core/Src/FAC_Code/fac_std_receiver.c:60-67` — `FAC_std_receiver_SET_channel()`
   clamps to `RECEIVER_CHANNEL_RESOLUTION`, not `- 1`. Replace the `// -1 because
   number 0 is considered` comment with one that states the domain is `0…1000`,
   1001 values, centre 500.
2. `Core/Src/FAC_Code/fac_pwm_receiver.c:82-84` — map to `0, RECEIVER_CHANNEL_RESOLUTION`
   and clamp to `RECEIVER_CHANNEL_RESOLUTION`.
3. `Core/Src/FAC_Code/fac_ppm_receiver.c:62` — same map change. The clamp there is
   commented out already; leave it commented, `map_uint32` bounds the result.
4. **Do not touch the deadzone.** It is already correct for this domain. Two of its
   guards are unreachable dead code — see Appendix A — and they stay as they are.

### Output side — what the first draft got wrong

The first draft claimed the same off-by-one existed mirrored on the output side and
that "full positive travel is unreachable at *both* ends of the chain". That is
**not what the sources say**, and the two fixes it proposed are one unnecessary and
one actively harmful. The verified situation:

**Servo.** `fac_mapper.c:102` produces `FAC_math_to_range(v, 0, SERVO_POSITION_RESOLUTION)`,
i.e. `p = (v + 1000) / 2` over `0…1000`. `FAC_servo_SET_position()`
(`fac_servo.c:30-37`) clamps that to `MAX_SERVO_VALUE = 999`.

- The clamp is **load-bearing, not defensive**. The pulse is
  `p_us = span * position / MAX_SERVO_VALUE` (`fac_servo.c:98`), so a position of
  1000 would produce a pulse *beyond* the configured `max_us`.
  `docs/README_API.md:537` states this explicitly. Do not describe this clamp as
  defensive and do not remove it.
- Full travel is therefore **already reachable**: `v = 998` gives position 999,
  which is exactly `max_us`. The real defect is smaller and different — a
  three-value flat spot at the top, since `v = 998`, `999` and `1000` all land on
  position 999, and a map that is not uniform across the travel.

**Motor.** `fac_mapper.c:75-78` produces `speed = abs(v) * MOTOR_SPEED_RESOLUTION / FAC_VALUE_MAX`,
which is exactly `abs(v)` because both constants are 1000. `FAC_motor_SET_speed()`
(`fac_motors.c:63-68`) clamps it to `MAX_DMA_PWM_VALUE = PWM_STEPS - 1 = 999`
(`DMApwm.h:32`).

- The originally proposed `abs(v) * (MOTOR_SPEED_RESOLUTION - 1) / FAC_VALUE_MAX`
  yields **the same value at full scale** (1000 → 999, exactly as the clamp already
  did) and truncates everywhere below it (500 → 499). It costs the exactness that
  `fac_math.h:28` and `docs/README_MIX_EDITOR_API.md:96` describe as the reason the
  ±1000 scale was chosen, and buys nothing. **Rejected.**
- The actual ceiling is the clamp, and it is a conservative choice in
  `fac_motors.c`, not a hardware limit: `setSoftPWM()` (`DMApwm.c:73-76`) accepts
  `duty == PWM_STEPS` and its full-buffer loop `i < duty` then sets all
  `PWM_STEPS` entries, driving the pin high for the whole period — a true 100 %
  duty. The hardware has been confirmed safe at 100 %.

### Firmware — output side

5. `Core/Src/FAC_Code/fac_mapper.c:102` — servo position becomes
   `FAC_math_to_range(outputLinkedValue, 0, MAX_SERVO_VALUE)`. Write
   `MAX_SERVO_VALUE`, not `SERVO_POSITION_RESOLUTION - 1`: it is the same number
   and it ties the expression to the divisor in `fac_servo.c:98` that makes it
   correct. Comment the two consequences: the map becomes uniform and the top flat
   spot disappears; the centre `v = 0` now lands on position 499 instead of 500,
   which is half a step below mechanical centre — 0,05 % of travel, and unavoidable
   because a 1000-step device has no exact middle. The `FAC_servo_SET_position()`
   clamp stays, and its comment must say it bounds the pulse formula.
6. `Core/Src/FAC_Code/fac_mapper.c:75-78` — **unchanged**. Keep
   `abs(v) * MOTOR_SPEED_RESOLUTION / FAC_VALUE_MAX` and keep the
   `// exact, the two resolutions are equal` comment, which is true and is the
   point. Add one line saying the full-scale ceiling now lives in the clamp below.
7. `Core/Src/FAC_Code/fac_motors.c` — this is the real motor fix, and it is
   **two coupled edits that must land together**:
   - `:63-68` — `FAC_motor_SET_speed()` clamps to `MOTOR_SPEED_RESOLUTION`
     (identical to `PWM_STEPS`, `DMApwm.h:25` defines one as the other), so speed
     1000 survives.
   - `:96-103` — the brake-enabled branch drives the pin pair as
     `MAX_DMA_PWM_VALUE - speed` / `MAX_DMA_PWM_VALUE`. Both references become
     `PWM_STEPS`.

   > **Do not raise the clamp without this second edit.** With the clamp at 1000 and
   > the pair still at 999, `999 - 1000` underflows `uint16_t` to 65535, `setSoftPWM`
   > clamps that to `PWM_STEPS`, and the complementary pin goes fully high alongside
   > the always-on pin: a full-throttle command would brake the motor. With
   > `PWM_STEPS` on both, speed 1000 gives `pinB = 0` / `pinF = PWM_STEPS` (true
   > 100 %) and speed 0 gives both pins high, which is the brake state the comment
   > table at `:85-95` already documents.

   The coast branch (`:105-110`) needs no change — it writes `speed` and `0`
   directly and has no complement to underflow.
   Rewrite the `:85-95` comment block: the "use -1 on every duty … ALREADY
   CONSIDERED ON MAX_DMA_PWM_VALUE" reasoning is exactly what is being reversed.
8. `Core/Inc/Libraries/DMApwm.h` — **untouched**. `MAX_DMA_PWM_VALUE` is the
   library's own last-addressable-index notion and stays defined even though
   `fac_motors.c` no longer uses it; the library boundary is not ours to redraw.
   Note in the commit message that the macro now has no user outside the header.

### Firmware — documentation, in scope

The firmware docs state the old domain in ten places and are the sources of truth
the tool cites. If they stay at `0…999` the next agent rebuilds the asymmetry.

9. `docs/README_API.md` lines 148 (the chain diagram), 391
   (`FAC_std_receiver_GET_channel` return range), 448
   (`FAC_mixes_update_mix_inputs` normalisation), and 537 (servo pulse — restate
   full deflection with the new mapper expression).
10. `docs/README_MIX_EDITOR_API.md` lines 114 (the code snippet comment), 317, 511
    and 518 (the `0 … 999` return contract). Line 96's "conversions at both ends
    are exact" survives: the input side becomes exact and the motor side stays
    exact; only the servo now trades half a step for uniformity, and that has to be
    said there.
11. The firmware's own `CLAUDE.md` lines 41 (chain diagram) and 55 (the
    `[0, RECEIVER_CHANNEL_RESOLUTION-1]` clamp claim), and line 99 if the servo
    wording needs it.

### Firmware — check and report on, do not change blindly

- `fac_app.c:116-118` and `:160-162` arming compare against
  `RECEIVER_CHANNEL_RESOLUTION / 100 * threshold` — unaffected, confirm.
- `fac_settings.c:216` `% RECEIVER_CHANNEL_RESOLUTION` is inside
  `#ifdef IM_TESTING_FAC_TOOL`, a fake stick generator — harmless, mention it.
- USB telemetry ships the raw channel as `uint16`; the PC-side tool now sees a
  `0…1000` scale instead of `0…999`. Note it in the commit message.
- `FAC_motor_make_noise()` (`fac_motors.c:175`) sets speed 50 — unaffected.
- The surviving `RECEIVER_CHANNEL_RESOLUTION - 1` after this work must be exactly
  one: `MAX_SERVO_VALUE` in `fac_servo.h:15`, which is correct because the servo
  position really does have 1000 steps. Grep and justify any other survivor.

### Tool — `FAC_mix_editor.html`

12. Rewrite the `SECTION: RECEIVER` header comment (~line 410). The four-stage
    table becomes `0: 0…1000`, `1a: 0…1000`, `1b: clamp to RESOLUTION (now a no-op
    in practice)`, `2: -1000…+1000`. Keep the stages separate — the reason they
    exist has not changed. Cite the firmware `file:line` for each stage as it does
    today, and record that the asymmetry was a firmware bug that has been fixed,
    with the commit reference. The paragraph currently headed "Stage 1b is the one
    the mix editor brief got wrong" is now historical and must be rewritten, not
    deleted: it explains why the stages are modelled separately at all.
13. `rxStoreClamp()` (~line 465) clamps to `RECEIVER_CHANNEL_RESOLUTION`.
14. Kill every hard-coded remnant of the old domain. **Appendix B is the complete
    list** — the first draft of this work order named five sites and there are
    seventeen. Work through the appendix, then re-grep `998`, `999` and
    `RECEIVER_CHANNEL_RESOLUTION - 1` and account for every remaining hit.
15. Model the output side too: wherever the tool converts a mix output to a device
    value for display (`SIM.deviceValue` consumers, `drawRobot`, the Robot tab
    readouts), mirror the mapper exactly — servo `m_to_range(v, 0, MAX_SERVO_VALUE)`
    → `0…999`, motor `abs(v)` → `0…1000` with the sign carried as direction. Add
    the two constants next to `RECEIVER_CHANNEL_RESOLUTION` at line 275 with their
    `config.h` / `fac_servo.h` provenance.

### Tool — self-test additions, all in `SECTION: SELFTEST`

16. New assertions:
    - `rxStage1(1000, 0, 1) === 1000` and `rxStage2(1000, false) === 1000` — full
      positive travel is now reachable.
    - `rxStage2(0, false) === -1000`, `rxStage2(500, false) === 0` — exact centre.
    - `rxStage2(rx, false) === 2*rx - 1000` for every `rx` in `0…1000`. Note that
      line 2748 already proves this identity for `m_from_range`; what is new is
      that the *chain* now preserves it, because stage 1b no longer truncates it.
    - the full deadzone sweep (line 2753) re-baselined to `0…1000`, and extended:
      for every percentage 0…20 and every channel 1…8 the result stays inside
      `0…1000`, is an integer, and is monotonic non-decreasing in the input.
    - the gamepad boundary: `PAD.padToStage0(defaults()[0], 1) === 1000`, `-1` gives
      `0`, `0` gives `500`, and the result is always an integer in `0…1000`
      (line 2829's range check moves with it).
    - servo and motor mapper mirrors: `m_to_range(1000, 0, MAX_SERVO_VALUE) === 999`,
      `m_to_range(-1000, 0, MAX_SERVO_VALUE) === 0`, `m_to_range(0, 0, MAX_SERVO_VALUE) === 499`,
      and the motor path is the identity on `abs(v)` over the whole normalized range.
    - the `simple_tank` parity grid (line 2840) gets `1000` added to its sample set
      and keeps `998` (still a legal value, just no longer the maximum).

### Tool — documentation

17. Update this folder's `CLAUDE.md`: the "channel chain" section, the `+998` claim
    in the *Precedence* paragraph, and the statement that the brief's `0…1000` was
    wrong — the brief was describing the intent, the source has now caught up with
    it. Add the output-side row: servo `0…999` via `MAX_SERVO_VALUE`, motor
    `0…1000` with a true 100 % duty.
18. No version-skew warning ships in the tool. A new tool on old firmware simulates
    `+1000` where the hardware gives `+998`; that belongs in the firmware changelog
    and the release notes, not in the editor UI and certainly not in the generated
    `.c`, which must stay byte-identical.

**Done when:**

- [ ] harness: `runSelfTest()` green, including every assertion in item 16
- [ ] harness: `headlessMix()` on `simple_tank` matches the hand transcription over
      the grid extended with `1000`
- [ ] harness: the generated `.c` and `.h` for every preset are byte-identical to
      the pre-§1 output — §1 changes no emitted code
- [ ] `grep -n "998\|999" FAC_mix_editor.html` returns only hits justified in
      Appendix B as intentional survivors
- [ ] `grep -rn "RECEIVER_CHANNEL_RESOLUTION *- *1" Core/` in the firmware returns
      only `fac_servo.h:15`
- [ ] browser: a stick slider at maximum shows `+1000` on every mix input, and the
      Sim & sticks readout agrees with the harness

---

## §2 — Resizable GUI and global UI scale

The four splitters already exist in the markup and CSS — `#splitPalette` (line
205), `#splitRight` (219), `#splitInsp` (224), `#splitBottom` (231), styled by
`.vsplit` / `.hsplit` (lines 61-66) — **and no JavaScript ever moves them.** They
are decoration.

Required:

- Wire all four. Pointer capture on `pointerdown`, `body.resizing-v` /
  `resizing-h` classes while dragging (the CSS is already there), min and max
  sizes per pane so nothing can be dragged to zero or push the canvas out of
  existence. Double click on a splitter resets that pane to its default.
- A collapse toggle on the palette, the right column and the bottom panel — the
  bottom one already has `#btBottomToggle`, make the other two match.
- **Global UI scale**: a control in the top bar, 70 %…150 %, driving a
  `--ui-scale` custom property.
  - It applies to **the chrome only**: top bar, palette, inspector, right column,
    bottom panel, their font sizes and paddings.
  - It does **not** apply to the nodes on the canvas. Node metrics and port hit
    areas stay fixed in world coordinates, and `P.view.z` remains the only thing
    that changes their apparent size. This is deliberate: nodes that grow in world
    coordinates would make a saved `x`/`y` layout occupy different space at
    different scales and would turn the §5 grid into a relative unit.
  - It must not touch `P.view.z`. Canvas zoom and UI scale are different things and
    must stay independent.
  - Add a compact/normal density switch if it falls out for free.
- **Persistence in `localStorage`, not in the project file.** Key
  `facmix.layout.v1`. Panel sizes are a property of the person, not of the mix; a
  `.facmix.json` handed to someone else must not resize their editor. Do not bump
  `SCHEMA_VERSION` for this. `localStorage` already holds the autosaved project
  under `AUTOSAVE_KEY` (line 4298) — the layout key is separate and the autosave
  restore path must not read or write it.
- The key also carries the §5 grid size and, in phase 2, the §8 overlay state.
- A *Reset layout* entry that clears the key and restores the defaults.

**Done when:**

- [ ] browser: every pane resizes, double click resets it, and all four survive a
      reload
- [ ] browser: the canvas is usable at 70 % and at 150 %, and node size on screen
      is unchanged by the scale control
- [ ] browser: export a `.facmix.json` and confirm no layout key is inside it
- [ ] browser: *Reset layout* restores the defaults without touching the project

---

## §3 — Dragging a collapsed group is not visible until release

**Cause, confirmed.** In the `mousemove` handler, the `move` branch (line ~3454)
updates the DOM with `$('#nodes .node[data-id="' + id + '"]')`. The members of a
collapsed group are not rendered — `renderNodes()` skips them via `isHidden(n)`
and draws one box carrying `data-grp` instead. So the selector matches nothing,
nothing moves on screen, and the jump only happens on the `refreshAll()` at
mouseup. The mousedown handler for a group box (line ~3413) also calls
`refreshAll()`, a full re-render, before the drag even starts.

Required:

- On mousedown, resolve the elements to move **once** into `dragState.doms` —
  node elements *and* the collapsed group boxes whose members are in the
  selection, each with its starting `x`/`y`. Per-frame `querySelector` goes away.
- The group box moves with its members and stays in sync with the box origin
  (`min(x)`, `min(y)` of the members).
- Coalesce the visual update into one `requestAnimationFrame`; `renderWires()`
  runs at most once per frame. Multi-selection drags of thirty nodes must stay
  smooth.
- Drop the `refreshAll()` from both mousedown paths — toggle the `sel` class
  directly and refresh the inspector, do not re-render the whole graph to start
  a drag.

**Done when:**

- [ ] browser: dragging a collapsed group tracks the cursor continuously, with the
      wires following, and the box lands where the cursor released it
- [ ] browser: a 20-node selection drags without stutter
- [ ] browser: expanding the group afterwards shows the members at the offsets they
      had before the drag
- [ ] harness: `simple_tank` still exports byte-identically — grouping is visual only

---

## §4 — Marquee selection does not stick

**Cause, confirmed.** The marquee itself works. The `click` listener on
`#viewport` (line ~3504) fires *after* the `mouseup` that completed the marquee,
sees `e.target` as `#viewport` or `#world`, and wipes the selection it just
built. Release the drag over a node or a wire and the target is different, so the
selection survives — exactly the reported symptom.

Required:

- Remove that `click` listener. Clearing the selection on an empty-canvas click is
  already handled by the mousedown path (line ~3443); if a guard is still wanted,
  gate it on a `moved` flag set by `mousemove`, with a few pixels of dead zone so
  a shaky click still counts as a click.
- **Intersection test**, as today — a block enters the selection if the marquee
  touches it at all, not only when it is fully contained.
- **Live feedback**: blocks that would be selected get the highlight *while* the
  marquee is being dragged, not on release. Use a lightweight class toggle, not a
  re-render.
- Include collapsed group boxes in the hit test — selecting a box selects its
  members, matching the existing click behaviour.
- Make sure `NODE_GEOM` is populated for every visible node and group box before
  the first marquee; the current `{w:120,h:60}` fallback silently misjudges wide
  nodes.
- Shift keeps its current meaning (add / toggle). Wires stay non-selectable.

**Done when:**

- [ ] browser: drag a marquee over empty canvas across several blocks and release
      over empty canvas — the blocks stay selected and the inspector shows the count
- [ ] browser: the highlight appears during the drag, not on release
- [ ] browser: a marquee over a collapsed group box selects its members
- [ ] browser: shift-marquee adds to an existing selection

---

## §5 — Grid snap is unreliable

Three separate causes, fix all three:

1. **The multi-node drag snaps each node's absolute position independently**
   (line ~3453: `n.x = Math.round((s.x + dx) / grid) * grid`). Nodes whose
   starting coordinates are not multiples of the grid drift *relative to each
   other* during the drag. Snap the **delta** instead: compute it from the node
   under the cursor, so that node lands exactly on the grid and every other node
   in the selection moves by the same amount, preserving the internal spacing
   exactly.
2. **The modifier state goes stale.** The snap bypass reads `keyState.ctrl`
   (line ~3450), which is only refreshed by `keydown` / `keyup` on `window`
   (lines 4304-4320) — and the `keydown` handler returns early when the focus is in
   an input, so the state is not even updated consistently. Press Ctrl+C, alt-tab,
   come back, and it can stay `true` — snap silently off. Read `e.ctrlKey` from the
   live mouse event instead, and add a `blur` handler on `window` that resets
   `keyState` regardless.
3. **Only the drag path snaps at all.** Palette drop (`addNode(t, w.x-50, w.y-20)`),
   double-click insert, paste (`pasteSel`, `c.x += 30`) and the §7 auto-layout all
   produce unsnapped coordinates, so the grid looks like it works sometimes.
   Introduce one `snapPos(x, y)` helper and route every position-producing path
   through it.

Also:

- Grid size becomes a user choice — off / 5 / 10 / 20 / 25 — in the top bar,
  persisted with the layout (§2), default 10 to match today.
- Draw the grid as a CSS background on `#viewport` aligned to `P.view` and the
  chosen size, so snapping is *visible*. It must not intercept pointer events.
- Arrow keys nudge the selection by one grid step, Shift+arrow by five. With the
  grid off, one step is 1 px. The nudge goes through `snapPos()` too, and shares
  one `pushUndo()` per burst rather than one per key repeat.

**Done when:**

- [ ] browser: with snap at 10, a block created by each of palette drop, double
      click, paste and auto-layout sits on a multiple of 10
- [ ] browser: a multi-selection drag preserves the relative offsets exactly —
      check two nodes that started 37 px apart
- [ ] browser: Ctrl+C, alt-tab away and back, then drag — it still snaps
- [ ] browser: the drawn grid stays aligned to the blocks while panning and zooming
- [ ] browser: the grid size choice survives a reload

---

## §6 — Right button drag pans

- `mousedown` with `e.button === 2` starts the same `pan` drag state that middle
  button, Alt and Space already start (line ~3437).
- Suppress the browser context menu on `#viewport` (and only there) with
  `preventDefault()`.
- **No context menu of our own** — a stationary right click does nothing.
- The right-button `mouseup` must not clear the selection or start a marquee.
- Update `#editHint` (line 215) and the shortcut table in the help modal
  (line ~4253) to list the right button.
- While the keyboard handler is open: `F` (line 4316) fires on any modifier
  combination, so `Ctrl+F` frames the graph instead of opening the browser's find.
  Gate it on no modifier being held. §7 adds `L` next to it and must be gated the
  same way from the start.

**Done when:**

- [ ] browser: right-button drag pans, and releasing does not disturb the selection
- [ ] browser: a stationary right click on the canvas opens nothing
- [ ] browser: the context menu still works everywhere outside `#viewport`
- [ ] browser: `Ctrl+F` no longer frames the graph; bare `F` still does

---

## §7 — Auto-layout button

A button next to *Frame* in the top bar, shortcut `L` (verified free — the only
current bindings are Delete/Backspace, Ctrl+Z/Y/C/V/A and bare `F`), that arranges
the graph left to right by topological rank.

- Reuse `topoOrder()`. Rank = longest path from the sources, so every block sits
  strictly to the right of everything feeding it. Sinks (mix outputs) are pinned
  to the last column.
- Order within a column by the barycentre of the predecessors' rows, two or three
  sweeps forward and back — enough to remove most crossings without importing a
  layout library.
- Column width from the measured node widths in that column plus a gutter, row
  spacing from the measured heights. Positions go through `snapPos()` (§5).
- A **collapsed group is laid out as a single unit** at its box size, and its
  members are repositioned to keep their relative arrangement inside that
  footprint, so expanding it afterwards does not produce a pile.
- Nodes that no edge reaches — orphans and freshly dropped blocks — go into a
  trailing column of their own rather than being scattered through rank 0.
- One `pushUndo()` for the whole operation, then frame all.
- Cyclic graphs (legal when a cycle passes through a stateful block) must not
  hang the ranking — break cycles at the stateful node, the same one that makes
  them legal.

**Done when:**

- [ ] browser: the `simple_tank` preset laid out automatically is readable with no
      overlapping blocks
- [ ] browser: Ctrl+Z restores the previous positions in one step
- [ ] browser: a preset containing a stateful cycle lays out without hanging
- [ ] browser: a collapsed group survives the layout and expands without a pile
- [ ] harness: the exported `.c` is byte-identical before and after a layout —
      positions are not semantics

---

## §9 — Block documentation section

A new bottom tab, *Blocks*, between *Generated code* and *Registration*,
**generated entirely from `NODES`**.

Per block, all derived, none re-typed:

- label, catalogue id, category, whether it is advanced
- input ports with their kinds, output kind
- parameters with type, range and default
- division count (`divs` / `divsOf`) and the sqrt step note where it applies
- the proven output interval from `range()`
- the `help` string, which already states the division count and the range
  constraint
- **the real emitted C**, obtained by calling the block's own `emit()` with
  placeholder arguments. Use a throwaway context — it must not touch the real
  `makeCtx()` state, must not register statics or resets, and must fall back to a
  short note if a block's `emit` needs context it cannot get. This is what
  guarantees the documentation cannot drift from the generator.

Around it:

- search and category filter, mirroring the palette
- a header section covering the receiver chain (§1, in its post-fix form), the
  group-1 versus group-2 versus group-4 clamp and overflow semantics, and the cost
  model — **moved** out of the help modal, not duplicated into it
- the help modal keeps the keyboard shortcuts and the workflow, loses the theory,
  and gains a link that opens the *Blocks* tab. One copy of each explanation
- a link from the inspector and from the palette entry to that block's card
- a *copy* button per card

**Done when:**

- [ ] harness: every id in `NODES` produces a card, and the count matches
      `Object.keys(NODES).length`
- [ ] harness: adding a throwaway `def({...})` makes a card appear with no other edit
- [ ] harness: the C shown for each block used by `simple_tank` appears verbatim in
      the generator's output for that preset
- [ ] harness: building every card does not mutate the real generator state —
      export before and after and compare
- [ ] browser: the help modal no longer contains the receiver table or the group
      semantics, and its link opens the tab

---

## §8 — Live 2D robot view, closed loop with the IMU — **PHASE 2**

Deferred out of the phase 1 delivery. Specified in full so it does not need to be
redesigned when it is picked up.

Today `drawRobot()` returns early when the Robot tab is hidden (line ~3931) and
`integrateRobot()` is a fixed 120 px/s differential drive hard-wired to
`P.sim.mapper.M1` / `M2` (line ~2459).

### Placement

- A **floating overlay** anchored bottom-right inside `#editor`: always visible,
  independent of which bottom tab is open. Draggable by its header, resizable
  from a corner, collapsible to a title bar, with an opacity control so it never
  hides the graph permanently.
- A **dock button** that moves it into the right column or into the Robot tab and
  back. Position, size, dock state and opacity persist with the layout (§2), in
  `facmix.layout.v1` — they are a property of the person, not of the mix.
- Split rendering from the tab check so the overlay draws whenever the simulator
  runs, and the Robot tab reuses the same renderer rather than a copy.

### Drive sources and kinematics preset

The current hard-wiring to M1/M2 is false for any robot that does not happen to
use those two outputs for traction — a weapon on M1 makes the view nonsense.

- Two selectors, *left wheel* and *right wheel*, choosing any mapped device
  (`M1…M4`, `S1…S2`), defaulting to `M1` and `M2`.
- A kinematics preset next to them: **2-wheel differential** (default) or
  **4-motor skid-steer**, where the selectors become left pair and right pair
  (`M1+M3` / `M2+M4` by default) and each side's speed is the average of its pair.
- Both live in `P.sim`, alongside the physical units below.

### Physical units

Replace the arbitrary constants with values the user sets, stored in `P.sim`
(these describe the robot being built, so they *do* belong to the project — bump
`SCHEMA_VERSION` to 2 and default them for older files on load):

- robot length and width, cm
- track width, cm
- maximum linear speed at full stick, cm/s
- derived and displayed, not entered: maximum yaw rate = `maxSpeed / trackWidth`
  in °/s, so the model stays self-consistent

Kinematics: `vL`, `vR` in cm/s from the mapper values for the selected sides scaled
by the maximum speed; `v = (vL+vR)/2`; `ω = (vR-vL)/trackWidth`. Integrate at the
existing 1 ms step. The view scale is cm per pixel, auto-fit with a manual
override, plus a follow/centre toggle and a fading trail of past positions.

### Closed loop: the robot drives the IMU

Add a selector *IMU source: simulated robot / manual sliders*, **defaulting to
the simulated robot**. When it is on the robot:

- gyro Z from `ω`, converted to mdps and then to raw counts at 70 mdps/count
  (`LSM6DS3_GYRO_SENSITIVITY_MDPS`, `LSM6DS3.h:130`, ±2000 dps full scale)
- gyro X, Y from the (zero) roll and pitch rates, unless a tilt model is added
- accel X from `dv/dt` (longitudinal), accel Y from `v·ω` (centripetal), both in
  mg then raw at 0.488 mg/count (`LSM6DS3_ACCEL_SENSITIVITY_UG`, `LSM6DS3.h:129`,
  ±16 g full scale)
- accel Z holds gravity, 1000 mg
- optional noise amplitude, off by default
- everything clamped to `int16` and **rounded to an integer before it is written
  to `SIM.gyro` / `SIM.accel`** — this is the float boundary rule, and it needs a
  self-test assertion proving no non-integer ever reaches those arrays

When the selector is on *manual*, the sensor sliders behave exactly as they do
today. Document in the panel which mounting convention the axes assume, and cite
the firmware file that fixes it.

### What is drawn (this covers item 10)

On top of the existing chassis, motor bars and servo horns:

- the **heading** as an arrow from the robot centre
- the **body-fixed axis triad**: X forward, Y left, Z out of the plane drawn as a
  circle with a rotation arc whose length and direction track `ω`, each axis
  colour-coded with a legend
- the **acceleration vector** as a second arrow, distinguishable from heading
- a small **world axis indicator** in a corner, so body versus world frame is
  never ambiguous
- numeric readouts: speed cm/s, yaw rate °/s, and the raw gyro/accel counts the
  graph is actually reading
- reset pose, pause, and clear trail

Keep the existing "illustrative, not a physical model" caveat visible — it is a
kinematic sketch, not a dynamics simulation, and the panel must say so.

**Done when:**

- [ ] harness: a scripted run of the simulator with the robot IMU source active
      asserts `Number.isInteger` on every value ever written to `SIM.gyro` and
      `SIM.accel`, over a sweep of stick inputs including both extremes
- [ ] harness: a `.facmix.json` saved at `SCHEMA_VERSION` 1 loads with the new
      `P.sim` fields defaulted, and re-saves at 2
- [ ] harness: with the IMU source on manual, the sensor path is bit-identical to
      the pre-§8 behaviour
- [ ] browser: the overlay draws with any bottom tab open, docks and undocks, and
      its geometry survives a reload
- [ ] browser: a preset using an IMU block responds to the simulated robot's yaw

---

## Appendix A — known, deliberately not fixed

Found while verifying this work order. Neither changes observable behaviour, and
fixing either is out of scope.

1. **`fac_std_receiver.c:98-101`, two unreachable guards in the deadzone.**
   `if (value > maxValue) temp = maxValue;` tests the raw `uint16_t` argument
   against `maxValue`, not the already-remapped `temp`, and after §1 the argument
   can never exceed `RECEIVER_CHANNEL_RESOLUTION` anyway; `if (value < minValue)`
   can never be true at all, since `value` is unsigned and `minValue` is negative.
   §1 item 4 says do not touch the deadzone and that stands — it is arithmetically
   correct for the new domain and every change there is a change to feel. Leave
   both, and leave the tool's port mirroring them line for line.
2. **`FAC_mix_editor.html:4316`, `F` ignores modifiers.** `if (e.key === 'f' || e.key === 'F') frameAll();`
   has no modifier guard, so `Ctrl+F` frames the graph instead of opening the
   browser's find. This one *is* picked up, but by §6 rather than as its own item,
   because §6 already opens that handler.

Also worth recording, since it looks like a bug and is not: channel 3 takes a
different arithmetic path through the deadzone (`fac_std_receiver.c:94`, `:120`) —
it is never remapped into ±RESOLUTION, so the centre-deadzone branch acts on the
raw `0…1000` throttle and effectively becomes a bottom-extremes deadzone. That is
intentional: throttle has no return spring. The tool's port must keep mirroring it.

---

## Appendix B — the old-domain audit

The complete list of sites that encode `0…999`. §1 item 14 must account for every
one of them. The first draft of this work order named five; there are seventeen in
the tool and ten in the firmware docs.

### Tool — `FAC_mix_editor.html`

| line | what |
|---|---|
| 415-425 | `SECTION: RECEIVER` header table and the "brief got wrong" paragraph |
| 465 | `rxStoreClamp()`, `RECEIVER_CHANNEL_RESOLUTION - 1` twice |
| 516-518 | `INTERVALS` header comment, "the real chain only reaches [-1000, +998]" |
| 583 | `rx_channel` `range: () => IV.mk(-1000, 998)` |
| 2499, 2559 | `SECTION: GAMEPAD` header comments, "stage-0 values 0..999" |
| 2569-2570 | `PAD.padToStage0`, `(RECEIVER_CHANNEL_RESOLUTION - 1)` in the conversion and in the clamp |
| 2745 | `T('from_range(999,0,1000)', …, 998)` — still true, but no longer the top of travel |
| 2753-2758 | deadzone sweep asserting the result is inside `[0, 999]` |
| 2763-2782 | eight `rxStage1` / `rxStage2` assertions built on 999 and 998 |
| 2782-2783 | `'mix input at the top of travel is +998, NOT +1000'` — inverts |
| 2829 | gamepad range check, `v > RECEIVER_CHANNEL_RESOLUTION - 1` |
| 2834 | `'a pad axis at +1 becomes stage 0 = 999'` |
| 2840 | `simple_tank` parity grid sample set |
| 3743, 3747 | Sim & sticks: the `stage 0 (0..999)` label and the sliders' `max="999"` |
| 3755-3756 | Sim & sticks explanatory paragraph |
| 4038 | gamepad tab explanatory paragraph |
| 4227-4232 | help modal channel-chain table and the paragraph under it |

Intentional survivors after the work: none. Every `998`/`999` in the file should be
gone or restated; if one remains, it must carry a comment saying why.

### Firmware documentation

| file:line | what |
|---|---|
| `docs/README_API.md:148` | chain diagram, `channels[8], integers 0 … 999` |
| `docs/README_API.md:391` | `FAC_std_receiver_GET_channel` return contract |
| `docs/README_API.md:448` | `FAC_mixes_update_mix_inputs`, `0…999` → `-1000…+1000` |
| `docs/README_API.md:537` | servo pulse paragraph — restate against the new mapper expression |
| `docs/README_MIX_EDITOR_API.md:96` | "conversions at both ends are exact" — qualify for the servo |
| `docs/README_MIX_EDITOR_API.md:114` | snippet comment, `0 .. 999` |
| `docs/README_MIX_EDITOR_API.md:317` | `FAC_mixes_update_mix_inputs` row |
| `docs/README_MIX_EDITOR_API.md:511` | `/* 1-BASED, returns 0 .. 999 */` |
| `docs/README_MIX_EDITOR_API.md:518` | "the returned value is `0 … 999`" |
| firmware `CLAUDE.md:41`, `:55`, `:99` | chain diagram, the clamp claim, the servo paragraph |

---

## Global regression checklist

Before calling any of this done:

- [ ] boot self-test green, and it still blocks export when red
- [ ] `simple_tank` still matches `fac_simple_tank_mix.c` over the parity grid
- [ ] every preset loads, simulates and exports
- [ ] the generated `.c` / `.h` for an unchanged graph is byte-identical to before
      this work — including after §1, which changes no emitted code
- [ ] round-trip: export a `.c`, re-import it, get the same graph
- [ ] `file://` open, console clean, no network request in the network tab
- [ ] no `float` and no `Math.floor` anywhere in the graph math; the only
      `Math.round` calls are the two documented float boundaries
- [ ] this folder's `CLAUDE.md` updated for §1 and for the new UI affordances
- [ ] the node harness deleted from the scratchpad, nothing test-shaped committed
- [ ] firmware changes left in the submodule working tree, uncommitted, for review

## Suggested commit messages

The firmware changes are **not** committed by the implementer — they are left in
the submodule working tree for review. These are the messages to use when they are
committed by hand.

**Firmware, one commit:**

```
receiver/mapper: make the value chain reach full scale at both ends

The channel domain was inconsistent between producers and consumers: the
deadzone and FAC_math_from_range already spoke 0..RECEIVER_CHANNEL_RESOLUTION
while the capture and the store clamped to RESOLUTION-1, so a mix input reached
-1000 but only +998. Widen the input side to 0..1000: 1001 values, exact centre
500, and the identity mix_input = 2*rx - 1000 now holds exactly.

  fac_std_receiver.c  SET_channel clamps to RESOLUTION, not RESOLUTION-1
  fac_pwm_receiver.c  map and clamp to RESOLUTION
  fac_ppm_receiver.c  map to RESOLUTION
  fac_std_receiver.c  deadzone untouched, it was already written for this domain

On the output side the servo mapper now targets MAX_SERVO_VALUE, so the map is
uniform over the travel instead of flattening the top three input values onto
position 999. The centre lands on 499 rather than 500, half a step below
mechanical centre, which is unavoidable for a 1000-step device. The clamp in
FAC_servo_SET_position stays: it bounds the pulse formula at fac_servo.c:98 and
is load-bearing, not defensive.

The motor scale is left exact - abs(v) * MOTOR_SPEED_RESOLUTION / FAC_VALUE_MAX
is the identity and multiplying by RESOLUTION-1 instead would have truncated the
whole scale to recover a ceiling the clamp already imposed. The ceiling is lifted
in fac_motors.c instead: SET_speed clamps to MOTOR_SPEED_RESOLUTION and the
brake-mode pin pair references PWM_STEPS. Both edits are required together -
with the clamp raised and the pair left at MAX_DMA_PWM_VALUE, PWM_STEPS-1-speed
underflows uint16 at full throttle and brakes the motor. setSoftPWM already
accepts duty == PWM_STEPS and drives the pin high for the whole period, so full
throttle is now a true 100% duty. MAX_DMA_PWM_VALUE stays defined in DMApwm.h
and now has no user outside that header.

Audit of RECEIVER_CHANNEL_RESOLUTION - 1: the only survivor is MAX_SERVO_VALUE
in fac_servo.h, which is correct - the servo position really does have 1000
steps. fac_app.c arming thresholds are percentage-based and unaffected.
fac_settings.c:216 is a fake stick generator behind IM_TESTING_FAC_TOOL.

USB telemetry ships the raw channel as uint16: the PC-side tool now sees a
0..1000 scale instead of 0..999.

Docs updated to match: README_API.md, README_MIX_EDITOR_API.md, CLAUDE.md.
```

**Tool, one commit per §.** For §1:

```
mix editor: channel domain becomes symmetric, -1000 .. +1000

Mirrors the firmware fix. The store clamp is now a no-op in practice, so a mix
input reaches +1000 and the stick centre is exact. The four stages stay modelled
separately - collapsing them would either double-apply the deadzone or invent
travel the hardware does not have.

Self-test: full positive travel, exact centre, the 2*rx-1000 identity across the
whole chain, the deadzone sweep re-baselined to 0..1000, the gamepad boundary,
the two mapper mirrors, and 1000 added to the simple_tank parity grid.

No generated code changes.
```
