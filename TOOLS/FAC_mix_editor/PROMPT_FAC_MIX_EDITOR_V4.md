# F.A.C. Mix Editor — work order V4

Seven items, **in the priority order the user gave them**. They are not one
theme: one is a firmware fact the tool had gone stale on, one is a bug, two are
about the tool explaining itself, one is a new documentation surface and two are
new blocks — one designed, one still to be.

Everything is in `TOOLS/FAC_mix_editor/FAC_mix_editor.html`. Read `CLAUDE.md` in
this folder first — every invariant in it still holds, and nothing here overrides
one. `PROMPT_FAC_MIX_EDITOR_V3.md` is the previous work order, shipped.

Status: **§1, §3, §4, §7 DONE.** §2 diagnosed, waiting on the user's
reproduction. §5 specified, not started. **§6 design settled, ready to build.**

---

## Ground rules (unchanged from V3, restated because they still bind)

- **Still one file.** No build step, no npm, no CDN, no font, no network call. It
  must keep working from `file://` with the console clean. This bites §3 and §5
  hardest: a markdown renderer and a documentation surface are exactly the places
  someone reaches for a library.
- **No second implementation.** A block declares `sim()` and `emit()` side by
  side, and any documentation of a block is **derived from `NODES`**, never
  written twice. §5 is a large amount of new prose and it must not become a
  second source of truth about what a block does.
- **No float in the graph math**, and no new primitive that is not already in
  `fac_math.h` or in the plain C the contract allows. This bites §6.
- **The self-test is the contract.** Export stays blocked while it has not run or
  has a failure. Every § below that touches behaviour adds assertions, and each
  new assertion is mutation-tested: break the thing on purpose, confirm red, put
  it back.
- **Firmware source > firmware docs > this document.** Where they disagree,
  follow the source and leave a comment naming the line that settles it.

---

## §1 — 20 special functions, one ceiling — **DONE**

### What the firmware changed

The user raised the limits in `FIRMWARE V2/FAC-Firmware-V2` (working tree, patch
version now **2.1.1**):

| File | Change |
|---|---|
| `Core/Src/FAC_Code/fac_settings.c:103-107` | the five mapper rows' `max` went from `200+10` to **`200+19`** |
| `Core/Inc/FAC_Code/fac_settings.h` | the settings codes are now **0-based**: `SPECIAL_FUNCTION0_INPUT_CHANNEL … 19`, not `1 … 20` |
| `Core/Inc/FAC_Code/mixes_functions/fac_functions.h` | `DC_SERVO_2ND`/`_3RD` retired, `SELF_RIGHTING_SYSTEM` added — `FAC_SPECIAL_FUNCTION_LAST` is now **10** |
| `Core/Inc/FAC_Code/config.h` | `FIRMWARE_VERSION_PATCH` 0 → 1 |

`SPECIAL_FUNCITONS_NUMBER` is still 20 and was already right in the tool.

**The point of the change is that there is now ONE ceiling instead of two.**
Before it, the framework arrays held 20 slots while a device link could only name
index 10 — so IDs 11…19 registered, compiled, ran, and could not be linked to
anything, with nothing warning about it. That whole second limit is gone, and
every place the tool used to explain it now says so once.

### What shipped in the editor

- **Two new firmware constants**, next to `SPECIAL_FUNCITONS_NUMBER`, each with
  the line of the file that settles it:
  - `MAPPER_FN_LINK_MAX = SPECIAL_FUNCITONS_NUMBER - 1` — the highest function
    index a mapper row can name (`fac_settings.c:103-107`). It is written as a
    *derivation* and not as the literal 19 on purpose: the property that matters
    is "every slot the arrays hold is linkable", and that is what the self-test
    pins.
  - `FW_MIX_COUNT = 2`, `FW_FN_COUNT = 10` — how full the two enums already are.
    These are only the **pre-filled value** of the two registration fields; the
    user edits them, and every index the guide derives is labelled as derived
    from them. `blankProject()` reads them instead of carrying its own literals.
- `registrationGuide()` step 5 rewritten: it states the `200+19` ceiling, says in
  one line what it used to be and what that cost, and no longer offers the "raise
  the five `max` values yourself" recipe — there is no `max` that rescues an index
  past the array end, so the only remaining advice is *reduce the instance count*,
  which step 2 has already said.
- The conformance checklist line and the Export modal's free-ID box collapsed from
  two limits to one, and the box now compares the free slots against the
  **instances this project actually needs**, so it goes red when it should.
- The Robot tab's mapper-link dropdown offered functions `0 … 10`. It now offers
  `0 … MAPPER_FN_LINK_MAX`, because offering fewer hides links the hardware
  accepts.

### Assertions added (4, all mutation-tested)

- every special function slot the arrays hold can be named by a mapper row —
  `MAPPER_FN_LINK_MAX === SPECIAL_FUNCITONS_NUMBER - 1` as a **property**, so
  putting the 10 back turns it red;
- the highest linkable row resolves to a function output, driven through
  `SIM.deviceValue()` with a planted value and restored after;
- one past it is not a link at all;
- a new project starts from the firmware counts, and the function default leaves
  room for at least one new function. Comparing `mixCount` against `FW_MIX_COUNT`
  alone would be the tautology `CLAUDE.md` warns about — both sides move together
  — so the assertion is on `blankProject()` *using* the constants, and the second
  half is the range property.

**Self-test 529 → 534, zero failures. Every preset's `.c` and `.h` is
byte-identical to `HEAD`; only the registration guide's text moved, which is the
whole point of the item.**

### Left deliberately alone

`FAC_SETTINGS_CODE_SPECIAL_FUNCTION<n>_INPUT_CHANNEL` becoming 0-based makes step
7 of the guide *more* true than it was (the `<n>` is now the enum index itself,
with no off-by-one to explain) and no wording change was needed. It is worth
saying out loud in §4, where the numbering is the subject.

---

## §2 — `reset state` freezes the simulation

**Reported:** pressing *reset state* in the Sim tab stops everything and the
simulation never resumes.

### What was verified headlessly

Under the node harness (whole script in a `vm` with DOM stubs, plus the real
`loop()` driven over a stub canvas):

- `SIM.reset()` does not throw, and does not touch `SIM.running`;
- `loop()` runs clean before and after a reset, and `tick` advances again;
- **every one of the nine presets produces byte-identical outputs after
  `reset()` + 3000 steps as it does before** — with the sticks off-centre, so the
  graph is actually doing something.

So there is no unconditional freeze. Whatever the user is hitting is
state-dependent, and one such state is real and reproducible:

### The one confirmed freeze — a failed arming calibration has no way back

`SIM.reset()` ends with `armCalibrate()`, which is right: a reset is a boot
followed by an arming, and arming is where `fac_imu.c` calibrates. But the
calibration can **fail**, and on failure nothing changes — which is the correct
model of the firmware (a mid-match re-arm must not destroy a good frame) and is
also a trap here, because in the simulator the only things that re-arm are
another reset and the end of a disarm.

If the graph reads any levelled block, `runGraph()` then sits in the
`usesLevel && !imu.cal` fallback branch **forever**: outputs held or zeroed,
nothing moving, no error, and the only explanation is a line of text on the
*Robot* tab, which is not the tab the user is on.

Reproduced exactly:

```
imuSource = 'sliders', accel = [0, 0, 1000]   (a slider dragged off rest)
SIM.reset()  ->  cal 0, why "the accelerometer read 0.49 g, outside the
                 0.8 .. 1.2 g a still robot must read"
500 steps    ->  outputs 0,0,0,0,0,0,0,0,0,0 and they stay there
```

`P.sim.imuFail` set at the moment of the reset does the same thing, and so does a
reset taken while the robot source is fed a chassis attitude the still check
rejects.

### What to do

1. **Say it where it happens.** The Sim tab must show the uncalibrated state and
   its `why` string, next to the run/pause row — the Robot tab's explanation is
   correct and in the wrong place. A *calibrate now* control already exists on the
   Robot tab (`#rbCalNow` → `SIM.armCalibrate()`); the Sim tab needs the same
   affordance, not a second implementation of it.
2. **Do not make the calibration always succeed.** That would be a lie about the
   firmware and would delete the reason the `is_calibrated()` guard exists at all.
   The fix is visibility plus a reachable retry, not a softer check.
3. `$('#sReset').onclick` calls `SIM.reset()` and nothing else, so the Sim tab is
   not re-rendered after a reset. Whatever indicator §2.1 adds has to be driven
   from `uiSimLive()` — the per-frame path — and not from `uiSim()`.

### Open question — ask before assuming this is the whole item

The harness does not exercise pointer interaction, and a JS exception thrown
inside `loop()` after `requestAnimationFrame(loop)` has already been scheduled
would look exactly like "everything froze" while leaving the loop alive. **Get
the reproduction from the user: which tab, was the robot driving, was the sim
paused or running, and what the browser console says.** A console error settles it
in one line; without one, §2.1–§2.3 are worth shipping on their own but must not
be reported as "the freeze is fixed".

---

## §3 — render the registration guide as markdown — **DONE**

`uiReg()` put `registrationGuide()` into a `<pre class="code">`. The guide *is*
markdown and it is meant to be read, not decoded by eye.

**`SECTION: MARKDOWN`** now holds `mdInline()`, `mdCells()` and `mdToHtml()` —
~120 lines, no library, pure `string → string`, no DOM. It covers exactly what
this tool emits (headings, fenced blocks, `|` tables, blockquotes, `- [x]`
checklists, `---`, bold/italic/code) and renders anything else as an escaped
paragraph, which is what the old `<pre>` did for that line anyway.

What the shipped version settled beyond the spec:

- **Escape first, mark up second — and the inline pass *builds* from `esc()`'d
  pieces** rather than running replacements over raw source and escaping after.
  The guide interpolates the project's title and author, which are user text; the
  mutation that swaps the order turns a title of `<img onerror=…>` into a real tag
  and the suite catches it.
- **Code spans are cut out before emphasis**, so `` `int **p` `` is a pointer and
  not bold. An unbalanced backtick degrades to plain text instead of swallowing
  the rest of the line.
- **A run of `|` lines is a table only if the second line is the `|---|`
  separator.** Guessing would mangle an ordinary line that starts with a pipe.
- **A blockquote's body goes back through `mdToHtml()`** — the guide puts fenced
  code and headings inside them. It cannot recurse forever: the marker is stripped
  each time.
- **`- [x]` is not an `<input>`.** It is a statement about a build that already
  happened, and a checkbox invites a click that would mean nothing.
- **Rendered by default, raw one click away**, and `UI.regRaw` remembers which —
  in `LAYOUT_KEY`, because it is a property of the person. *copy* and *download
  .md* hand over the exact guide string byte for byte whichever view is open, and
  the label above them says so. The Export modal's *Bundle* box stays raw
  deliberately: it is a copy-paste blob of all three files, not a document.

**21 assertions added**, including the whole guide of **both modes** rendered end
to end — every fenced block accounted for, no heading lost, no raw `<` through.
Seven mutations (escape order, unescaped fences, unprotected code spans, the
table separator rule, a clickable checklist, flattened blockquotes, one paragraph
per line) all go red.

`mdToHtml()` is deliberately standalone, because §5 wants it for a block's worked
example.

---

## §4 — the two enum-count inputs — **DONE**

The old wording — *"entries in `enum FAC_MIXES_ID` before `FAC_MIX_LAST`"* — was
precise and unanswerable: it asked for a count when what the user has in front of
them is a file, and never said what the number was *for*.

**`regCountsHtml(pfx)` / `regCountsWire(pfx, redraw)`** replaced both strips. One
control, two homes — the Registration tab and the Export modal — because they had
already drifted (one capped the function count at 20, the other did not), and this
item doubles the amount of text that could.

What it renders:

- **The current mode's enum first and loud**; the other count sits below, dimmed,
  labelled *"the guide reports it, nothing here is derived from it"*.
- **The header itself, as a picture, annotated with the enum's own VALUES.** The
  first attempt marked entries with an ordinal — `<-- 1` beside `FAC_MIX_NONE`,
  meaning "the first one you are counting". `FAC_MIX_NONE` is **0**, and a reader
  takes a number beside an enum entry as its value, correctly. Writing the real
  values makes the question answer itself: a C enum starts at 0 and the marker is
  last, so **`_LAST`'s own value already is the count**. The field labels name the
  marker (*"your `FAC_MIX_LAST` today"*) and the guide says it in one sentence.
  The sample is the stock 2.1.1 tree, with the marker's annotation derived from
  `FW_MIX_COUNT` / `FW_FN_COUNT` so it cannot drift.
- **The consequence beside the field, live**: for a mix, the enum name it takes,
  its index and `FAC_MIX_LAST`'s new value. For a function, the index or the index
  *range* for N instances, `_LAST`'s new value, **the `200+index` a device links
  to**, and its `FAC_SETTINGS_CODE_SPECIAL_FUNCTION<n>_INPUT_CHANNEL` — with the
  note that the codes are **0-based** since 2.1.1, so the number in the code *is*
  the index, and that `0` disables the slot entirely.
- **A green box when it fits, a red one when it does not**, comparing the count
  against the instances this project actually needs rather than against a constant.

No model change: `P.reg` keeps its two fields and their meaning.

**15 assertions**, driven off the panel's own rendered HTML rather than off a
restatement of its arithmetic: the index that is the count, `_LAST`'s new value,
three instances taking three consecutive slots, the `200+index` link, the 0-based
settings code, the exact-fit boundary, and the sample enum's annotations running
0, 1, 2 … up to a marker whose value equals the number before it. Eight
mutations, all red — including the ordinal one, which is **why** the last group
exists: the picture is prose, nothing derived it, and the mutation that put
`<-- 1` back survived every other assertion in the file. Assert the picture, not
only the arithmetic behind it.

---

## §5 — an internal guide with a worked example per block

**Reported:** the description a block carries today is not enough to know how to
use it.

The *Blocks* tab already exists and is **generated entirely from `NODES`** — ports,
params, division count, proven range, the `help` string, and C obtained by calling
the block's own `emit()` on a throwaway context. That derivation is the reason the
documentation cannot drift, and **§5 must not break it**.

What is missing is not more description. It is **an example**: what you wire into
it, what comes out, and what it is for.

### The shape to build

Add **one optional field to the catalogue entry**, alongside `help`:

```js
usage: {
    what:    'one sentence: the job, not the arithmetic',
    example: 'a concrete wiring, in words',
    inputs:  [ …literal argument values… ],   /* fed to the block's own sim() */
    params:  { … },
    note:    'the trap - what surprises people about this one'
}
```

and have the Blocks tab **run it**: call the block's real `sim()` with those
inputs and print the number that comes back, next to the C that `emit()` produced
for the same call. An example that is executed cannot be wrong about the block,
which is the same reason the tab already calls `emit()` instead of quoting C.

For a **stateful** block one row of inputs is not an example — the whole content
of a ramp, a filter, a latch or a timer is what it does over time. Those get a
short **input sequence** and the tab shows the output sequence beside it, driven
through the block's own `sim()` with a real state object over a handful of
simulated ticks. A sparkline of that sequence is worth more than any sentence.

### Rules

- **Every block gets one.** A block with no `usage` renders exactly as it does
  today, so this can land incrementally, but the item is not done until the
  catalogue is complete — a half-documented catalogue is worse than a uniform one
  because the gaps look like the block is trivial.
- **Do not restate `help`.** If a sentence in `usage.what` is already in `help`,
  one of the two is wrong. `help` states the division count and the range
  constraint; `usage` states the job and the trap.
- **The theory does not move.** The receiver chain, the group 1/2/3/4 semantics
  and the cost model stay in the Blocks tab's own preamble; the simulator's theory
  stays in the Robot tab. One copy of each explanation, as today.
- **Searchable.** The tab's filter must match `usage` text too, and it must keep
  building its shell once and re-rendering only `#bdList` — rebuilding the panel
  on a keystroke destroys the input being typed into.
- Assertions: every `usage.inputs` runs its block's `sim()` without throwing, and
  every block that declares `usage` produces a card. That is cheap and it is what
  stops a stale example from surviving a signature change.

---

## §6 — a block that delays a signal — **DESIGN SETTLED, READY TO BUILD**

**Reported, verbatim:** *"sarebbe comodo avere una funzione (blocco) che permetta
di aggiungere un ritardo nel segnale, ovvero dopo tot millisecondi il segnale
passa … proviamo quando è ora a capire insieme come fare."*

Three readings were on the table — **A** turn-on/turn-off delay on a BOOL, **B** a
ring-buffer transport delay, **C** a slew/rate limit (already covered by
`slew_ms` and the `soft_start` preset). The user picked **A**, as a **symmetric
gate**: the trigger is a BOOL, and the output only follows it after it has held
**both on the rising and on the falling edge** for N ms — a debounce in both
directions, not a one-way TON.

### The block

`id: 'bool_gate_delay'` (working name), `cat: 'state'`, `out: 'BOOL'`,
`stateful: true`, `nowNeeded: true` (needs `now`, like `timer`/`slew_ms`).

- `ins: [{ n: 'v', k: 'BOOL' }]`
- `params: [{ n: 'ms', label: 'delay ms', t: 'int', min: 0, max: 600000, def: 200 }, onResume]`
- **State**: one `uint32_t` timestamp (`t0`, last time the input changed) and one
  `uint8_t` (`out`, the gated output — this is what the block returns, not the raw
  input echoed back).
- **Logic**: if the input differs from what is currently *pending*, the timer
  restarts from now; once the input has held for `ms` without changing, `out`
  takes that value. Same rule for 0→1 and for 1→0, so no separate rising/falling
  params — matching what was asked.
- **First pass**: `t0` initialises to `now - ms` and `out` to the input's current
  value, so a graph does not spend its first `ms` milliseconds reporting a false
  "still low" before it has seen anything change. This mirrors how `timer` and
  `sample_hold` treat their first, `undefined`-state pass.
- **`onResume`**: the existing `resume` param (`reset` / `hold`), unchanged from
  every other stateful block — no new policy invented for this one.
- **Cost**: ~8 bytes of state, one instance. **Resolution**: one loop pass (same
  ceiling as `timer`).

`sim()` and `emit()` are a direct mirror of `timer`'s shape (`(uint32_t)(now - t0)
>= ms`, never `now > t0 + ms`, for the same wrap reason already documented there).
`range()` is `IV_BOOL`.

One `def({...})` with `sim`, `emit`, `range`, `divs: 0`, `ins`, `params`, `help`,
plus a `usage` entry for §5. Nothing else changes; it is additive, so every
preset's `.c`/`.h` stays byte-identical until a preset is built to use it.

---

## §7 — a ready-made PID block — **DONE**

**Requested, verbatim:** *"Aggiungi un nuovo punto dove va implementato un
blocco PID così è comodo da usare già pronto."*

Before building anything, the two readings on the table were weighed against
each other: a PID is entirely **composable today** from blocks already in the
catalogue (`raw_sub` for the error, `mul_scaled` for a fixed-point P gain,
`integrator` for I — which already carries the mandatory anti-windup clamp and
a `reset` port — `prev`+`raw_sub`+`mul_scaled` for D, `raw_add` to sum and
`clamp_to` on the output), so no new primitive was strictly required. The user
chose a **dedicated block** anyway, for the reason in the request itself — "già
pronto" — one node with the four design decisions baked in once, instead of
five or six nodes repeating the same wiring, and the safety net (the mandatory
anti-windup, the output clamp) guaranteed rather than left to whoever builds
the mix. `bool_gate_delay` (§6) set the precedent: composable is not a reason
to skip a convenience block when the user asks for one.

### The six questions, answered

1. **Gains as integers**: the existing `×1000` fixed-point convention of
   `mul_scaled`/`div_scaled` (`FAC_math_mul_scaled(a, b, 1000)` = `a·b/1000`,
   1000 is unity gain) — no new divisor scheme invented, `kp`/`ki`/`kd` all
   share it.
2. **Sample time**: (b), real elapsed `now - last`, `nowNeeded: true` — the same
   choice `slew_ms`/`timer` already made, and the honest one given the loop is
   not guaranteed 1 kHz.
3. **Integral windup**: a clamp on the accumulator (`ilo`/`ihi`, mandatory,
   defaults `∓1000000`) — exactly the `integrator` block's own scheme, not true
   anti-windup. Consistent with the rest of the tool and one comparison
   cheaper; true anti-windup would need the block to know about a clamp that,
   for every other block in the catalogue, is a separate downstream node.
4. **Derivative kick**: derivative-on-error, one state variable (previous
   error) instead of two. The kick on a setpoint step is accepted as a known
   property, stated in `help`.
5. **Ports and reset**: `sp`, `pv` (both `RAW`, arbitrary range — a PID has no
   reason to assume `NORM`), and an optional `reset` port (`{ opt: 1 }`, the
   `imu_heading` pattern) that zeroes only the integral term, same as
   `integrator`'s own `reset` input. Output clamps to `olo`/`ohi`, default
   `∓1000` like `clamp_to`'s own default.
6. **`onResume`**: `reset` zeroes the accumulator **and** the previous error
   **and** restarts the elapsed-time clock (`t = now`) — not just the raw
   struct — so the derivative does not see a stale error, and the integrator
   does not see a stale timestamp, across a disarm gap.

### What shipped

One `def({ id: 'pid', ... })` next to `integrator` in `SECTION: NODE_CATALOG`.
`err = sp - pv`; `P = FAC_math_mul_scaled(err, kp, 1000)`; `I` accumulates
`FAC_math_mul_scaled(FAC_math_mul_scaled(err, ki, 1000), dt, 1)` every pass
inside the mandatory `FAC_math_clamp_to(ilo, ihi)`; `D =
FAC_math_div_scaled(err − previous err, dt, kd)`, where `dt = 0` (two passes on
the same tick) reads as `D = 0` through `div_scaled`'s own `b == 0` guard rather
than a special case in the block. The sum is always
`FAC_math_clamp_to(P + I + D, olo, ohi)`. **No new primitive**: every emitted
line is `FAC_math_mul_scaled` / `FAC_math_div_scaled` / `FAC_math_clamp_to` or
plain C, the same three calls the catalogue already had. 4 divisions, 3 statics
(`int32_t acc`, `fac_value_t perr`, `uint32_t last`). `sim()` and `emit()` are a
line-by-line mirror, as the invariant requires.

`range()` does not try to prove the internal chain fits `int32_t` — like
`clamp_to` and `integrator`, it reports the block's own guaranteed post-clamp
bound (`[min(olo,ohi), max(olo,ohi)]`) and leaves overflow inside the chain to
the runtime `FacOverflow` that `mul_scaled`/`div_scaled` already raise. That is
the established contract for a group-2-built block, not a gap: `CLAUDE.md`
itself draws the line at "group 2 does not clamp — every product goes through
`mulChecked` … and raises `FacOverflow`, surfaced on the node."

**No `usage` field** (§5's mechanism does not exist in the file yet, so there
was nothing to attach one to); the caveat about open-loop tuning against a
kinematic sketch with no mass or motor curve is folded into `help` instead,
worth moving into a `usage.note` once §5 lands.

### Assertions added (18, mutation-tested), all green — self-test 570 → 588

Direct `sim()` calls (own `{tick}` stub, no full graph needed): pure-P unity
and half gain; the anti-windup bound is never escaped and is actually reached
given enough saturated passes; a wired reset drops the *old* integral before
the same pass's own increment is added (order matters, mirrors `integrator`);
the derivative term matches `FAC_math_div_scaled(delta, dt, kd)` to the bit; a
zero `dt` reads `D = 0` instead of throwing; the output is clamped to
`olo`/`ohi`; an overflowing gain raises `FacOverflow` instead of wrapping.
Through a real graph (`PB`/`analyse`/`generate`): an unwired `reset` raises no
unconnected-port error and emits no conditional branch at all; a wired one
emits exactly one guarded zero of the accumulator, reading the wired condition
and not a literal; the emitted body (comments stripped, boilerplate excluded,
same extraction the `PRESETS` sweep uses) contains no `float`/`double`/`math.h`/
shift and only the three `FAC_math_*` calls already checked elsewhere in the
suite.

**Every preset's `.c`/`.h` stays byte-identical to `HEAD`** — the block is
additive and no preset uses it yet, checked by diffing `generate()` output
before and after for all eight presets.

### Follow-up — the three gains are also PORTS

**Requested:** *"fai in modo che il blocco del pid abbia come ingressi i 3
fattori kp ki e kd, così possono essere associati a dei canali del
radiocomando."*

`kp`/`ki`/`kd` are now **both a parameter and an optional input port**. Unwired,
the parameter is emitted as the same literal it always was — every preset's
`.c`/`.h` is still byte-identical to `HEAD`, and so is a PID whose ports are
left alone. Wired, the parameter is ignored and the gain is read live from the
graph, so a spare channel tunes the loop from the transmitter without a rebuild.

Four decisions worth not re-deriving:

- **An unwired optional port is handed `null`, not `0`.** The emitter already
  had a sentinel for this (`'FAC_VALUE_ZERO'`); the simulator did not, and a
  channel can legitimately ask for a gain of **zero**. `unwired(ip)` — one
  helper, used by the three sim argument builders — gives an optional port
  `null` and a required one `0`. The existing `if (a[i])` reset tests in
  `imu_heading` and `pid` are unaffected, `null` being falsy.
- **The gains are appended after `reset`**, not put in their natural reading
  place next to `sp`/`pv`: port indices are positional and are never
  renumbered, the same rule `mix_output` states about its own index.
- **`byPort: '<port>'` on the parameter is the whole UI change.** The inspector
  greys and disables the field while the wire is there and `paramSummary()`
  prints `Kp x1000=port`, so the node never shows a number the generated C does
  not contain. No per-block UI code.
- **`range()` is unchanged.** A wired gain is a runtime number, so the internal
  chain was never provable and still is not — overflow there is the runtime
  `FacOverflow` that `mul_scaled`/`div_scaled` already raise.

**9 assertions added, self-test 588 → 597, zero failures.** The pair that
carries the item is *"a wired kp of ZERO disables the P term"* against *"an
unwired kp port falls back to the Kp parameter"* — a value-based fallback
(`a[3] || p.kp`) turns one of them red, and so does `unwired()` returning 0 for
everything. At the emitter: unwired emits the literal, wired emits the
expression and the literal is gone, and the `ki` left unwired still emits its
own. All four mutations confirmed red, then reverted.

---

## Acceptance

- The self-test is green and grew for §2–§6; every new assertion was
  mutation-tested.
- Every preset's `.c` and `.h` is byte-identical, **except** where §6 adds a block
  and a preset chooses to use it. §1–§5 are documentation, UI and one bug fix;
  none of them may move a generated byte.
- The `$('#id')` sweep still matches the markup (`selbox` is the known false
  positive).
- Opened in a browser and driven by hand — the harness does not exercise pointer
  interaction, and §3, §4 and §5 are all things you click.
